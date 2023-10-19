#include "hcal_cluster_hls.h"

ap_uint<8> disc_cluster(cluster_t ac, ap_uint<16> cluster_threshold)
{
  ap_uint<8> result = 0;

  // discriminate and report correct 4ns time bin that threshold crossing happened
  if(ac.e >= cluster_threshold)
    result[ac.t] = 1;

  return result;
}

// hcal_cluster_hls:
// - hit_dt: maximum time difference (in +/-4ns ticks) from seed hit required to accept adjacent spacial hit into cluster 
// - seed_threshold: minimum hit energy required for central hit of cluster position to allow a cluster to be formed
// - cluster_threshold: mimimum cluster energy required to generate a trigger
// - s_fadc_hits: FADC hit stream input (from VXS and fiber) of all fadc hits that can be used to perform cluster finding from the current frame
// - s_fiberout: cluster position stream output to SSP
// - s_cluster_all: cluster stream output
void hcal_cluster_hls(
    ap_uint<3> hit_dt,
    ap_uint<13> seed_threshold,
    ap_uint<16> cluster_threshold,
    hls::stream<fadc_hits_t> &s_fadc_hits,
    hls::stream<fiber_bins_t &s_fiberout,
    hls::stream<cluster_all_t> &s_cluster_all
  )
{
  fadc_hits_t fadc_hits = s_fadc_hits.read();

#ifndef __SYNTHESIS__  
  // Initialize for simulation only (creates a problem for synthesis scheduling)
  static hit_t all_fadc_hits_pre_pre = {{{0,0}},{{0,0}}};
  static hit_t all_fadc_hits_pre = {{{0,0}},{{0,0}}};
#else
  static hit_t all_fadc_hits_pre_pre;
  static hit_t all_fadc_hits_pre;
#endif

  hit_t all_fadc_hits[288];
  for(int ch=0; ch<256; ch++)
      all_fadc_hits[ch] = fadc_hits.vxs_ch[ch];
   
  for(int ch=0; ch<32; ch++)
      all_fadc_hits[ch+256] = fadc_hits.fiber_ch[ch];

  ap_uint<8> ac_disc[N_CHAN_SEC];
  trigger_t trigger = {0};
  cluster_all_t allc;
  
  for(int ch=0; ch<288;ch++){
      hit_t nearby_hit_pre[9];
      hit_t nearby_hit_cur[9];
      hit_t nearby_hit_aft[9];
  
      nearby_hit_pre[0] = all_fadc_hits_pre_pre[ch]
      nearby_hit_cur[0] = all_fadc_hits_pre[ch]
      nearby_hit_aft[0] = all_fadc_hits[ch]
   
      for(int ipos=0; ipos<8; ipos++){
         int nearby_ch = Find_nearby(ch, ipos);
         if(nearby_ch<0) continue;

         nearby_hit_pre[ipos+1]=all_fadc_hits_pre_pre[nearby_ch]
         nearby_hit_cur[ipos+1]=all_fadc_hits_pre[nearby_ch]
         nearby_hit_aft[ipos+1]=all_fadc_hits[nearby_ch]
       }
      

      allc.c[ch] = Find_cluster(nearby_hit_pre, nearby_hit_cur, nearby_hit_aft, hit_dt, seed_threshold, Find_block(ch,0), Find_block(ch,1));

  }
     
  // save the previous fadc_hits
  all_fadc_hits_pre_pre = all_fadc_hits_pre;
  all_fadc_hits_pre = all_fadc_hits;

#ifndef __SYNTHESIS__
  int nclust = 0;
  for(int ch=0; ch<N_CHAN_SEC;ch++){
    if(allc.c[ch].nhits>1)
      nclust++;
      //printf("nclust %d at (%d, %d), e=%d, t=%d\n",nclust,allc.c[ch].x.to_uint(),allc.c[ch].y.to_uint(),allc.c[ch].e.to_uint(),allc.c[ch].t.to_uint());
  }
  printf("nclust: %d\n",nclust);
#endif

  s_cluster_all.write(allc);

  for(int ii=0; ii<N_CHAN_SEC;ii++){
     ac_disc[ii]=disc_cluster(allc.c[ii],cluster_threshold); 
  }
  
  // 'or' together result from all channels, for each possible 4ns time bin
  for(int ii=0;ii<N_CHAN_SEC;ii++)
    trigger.trig |= ac_disc[ii];
  
  // write trigger result
  s_trigger.write(trigger);

  return;
}

// for a given channel number, return the nx and ny of the channel map 
ap_uint<5> Find_block(ap_uint<9> ch, ap_uint<1> dim){    
  ap_uint<5> nx=0; 
  ap_uint<4> ny=0;

  nx=block_map[ch].nx;
  ny=block_map[ch].ny;

  switch(dim){
    case 0: return nx;
    case 1: return ny;
  }

}

// for a given (x,y), return the channel number
int Find_channel(ap_uint<5> nx, ap_uint<4> ny){
  int ch = -1;

  if(nx<1 || nx>12 || ny<1 || ny>24)
    return ch;

  for(int ich=0;ich<288;ich++){
      if( (block_map[ich].nx==nx) && (block_map[ich].ny==ny) )
	   ch = ich;
           return ch
  }

  return ch;
}
   
int Find_nearby(ap_uint<9> ch, ap_uint<3> pos){
     ap_uint<5> nx=0;
     ap_uint<4> ny=0;
     nx = Find_block(ch,0);
     ny = Find_block(ch,1);

     switch(pos){
       case 0: return Find_channel(nx-1, ny-1); // up left
       case 1: return Find_channel(nx, ny-1); // up middle
       case 2: return Find_channel(nx+1, ny-1); // up right
       case 3: return Find_channel(nx-1, ny); // left
       case 4: return Find_channel(nx+1, ny); // right
       case 5: return Find_channel(nx-1, ny+1); // bottom left
       case 6: return Find_channel(nx, ny+1); // bottom middle
       case 7: return Find_channel(nx+1, ny+1); // bottom right
     }

}

ap_uint<1> hit_coin_t(ap_uint<4> t1, ap_uint<4> t2, ap_uint<3> dt) {
  ap_uint<4> diff = (t1<t2) ? (t2-t1) : (t1-t2);
  return (diff<=dt) ? 1 : 0;
}

cluster_t Find_cluster(
    hit_t prehits[9], hit_t curhits[9], hit_t afthits[9]
    ap_uint<3> hit_dt, ap_uint<13> seed_threshold,
    ap_uint<5> x, ap_uint<4> y
  ){
    ap_uint<4> t0 = 0;
    ap_uint<13> e_array[7];
    int hits[8] = 0;

    if( curhits[0].e<seed_threshold ) return NULL;
    t0 = curhits[0].t;
    e0 = curhits[0].e

   
    for(int ii=1; ii<9; ii++){
        if( curhits[ii].e>0 && hit_coin_t(t0, curhits[ii].t, hit_dt) ){
            if(curhits[ii].e > e0) return NULL;
            nhits[ii] = nhits[ii]+1;
        }

        if( prehits[ii].e>0 && hit_coin_t(t0+8, prehits[ii].t, hit_dt) ){
            if(prehits[ii].e > e0) return NULL;
            nhits[ii] = nhits[ii]+1;
        }

        if( afthits[ii].e && hit_coin_t(t0, afthits[ii].t+8, hit_dt) ){
            if(afthits[ii].e > e0) return NULL;
            nhits[ii] = nhits[ii]+1
        }

        if(nhits[ii]>0)
          hits_array[ii]=1
    }  

    return hits_array;

}

