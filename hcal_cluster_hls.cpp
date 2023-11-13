#include "hcal_cluster_hls.h"

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
    hls::stream<fiber_bins_t> &s_fiberout,
    hls::stream<cluster_all_t> &s_cluster_all
  )
{
  fadc_hits_t fadc_hits = s_fadc_hits.read();

//#ifndef __SYNTHESIS__  
//  // Initialize for simulation only (creates a problem for synthesis scheduling)
//  static hit_t all_fadc_hits_pre_pre[288] = {{{0,0}},{{0,0}}};
//  static hit_t all_fadc_hits_pre = {{{0,0}},{{0,0}}};
//#else
  static hit_t all_fadc_hits_pre_pre[288];
  static hit_t all_fadc_hits_pre[288];
//#endif

  hit_t all_fadc_hits[288];
  for(int ch=0; ch<256; ch++)
      all_fadc_hits[ch] = fadc_hits.vxs_ch[ch];
   
  for(int ch=0; ch<32; ch++)
      all_fadc_hits[ch+256] = fadc_hits.fiber_ch[ch];

  cluster_all_t allc;
  
  for(int ch=0; ch<288;ch++){
      hit_t nearby_hit_pre[9];
      hit_t nearby_hit_cur[9];
      hit_t nearby_hit_aft[9];
  
      nearby_hit_pre[0] = all_fadc_hits_pre_pre[ch];
      nearby_hit_cur[0] = all_fadc_hits_pre[ch];
      nearby_hit_aft[0] = all_fadc_hits[ch];
   
      for(int ipos=0; ipos<8; ipos++){
         int nearby_ch = Find_nearby(ch, ipos);
         if(nearby_ch<0) continue;

         nearby_hit_pre[ipos+1]=all_fadc_hits_pre_pre[nearby_ch];
         nearby_hit_cur[ipos+1]=all_fadc_hits_pre[nearby_ch];
         nearby_hit_aft[ipos+1]=all_fadc_hits[nearby_ch];
       }
      

      allc.c[ch] = Find_cluster(nearby_hit_pre, nearby_hit_cur, nearby_hit_aft, hit_dt, seed_threshold, Find_block(ch,0), Find_block(ch,1));

  }
     
  // save the previous fadc_hits
  for(int ch=0; ch<288; ch++){
     all_fadc_hits_pre_pre[ch] = all_fadc_hits_pre[ch];
     all_fadc_hits_pre[ch] = all_fadc_hits[ch];
  }

#ifndef __SYNTHESIS__
  int nclust = 0;
  for(int ch=0; ch<288;ch++){
    if(allc.c[ch].nhits>1)
      nclust++;
      //printf("nclust %d at (%d, %d), e=%d, t=%d\n",nclust,allc.c[ch].x.to_uint(),allc.c[ch].y.to_uint(),allc.c[ch].e.to_uint(),allc.c[ch].t.to_uint());
  }
  printf("nclust: %d\n",nclust);
#endif

  s_cluster_all.write(allc);
  fiber_bins_t fiberout;
  //fiberout = FiberOut(allc, cluster_threshold);
  s_fiberout.write(fiberout);
  
  return;
}

fiber_bins_t FiberOut(cluster_all_t allc, ap_uint<16> cluster_threshold){

  fiber_bins_t allf;

  for(int ii=0; ii<128; ii++){
      allf.bins[ii].t=0;
      allf.bins[ii].valid=0;
  }

  for(int ii=0; ii<288;ii++){
     if( allc.c[ii].e>cluster_threshold ){
         ap_uint<7> fiber_bin = fiber_map[ii];
       
         ap_uint<3> newt=0; 
         if(allf.bins[fiber_bin].valid==1)
            newt = (allf.bins[fiber_bin].t>allc.c[ii].t)?allc.c[ii].t:allf.bins[fiber_bin].t;
         else
            newt = allc.c[ii].t;
         allf.bins[fiber_bin].t = newt;
         allf.bins[fiber_bin].valid = 1;

     }
  }

  return allf;

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

  return 0;

}

// for a given (x,y), return the channel number
int Find_channel(ap_uint<5> nx, ap_uint<4> ny){
  int ch = -1;

  if(nx<1 || nx>12 || ny<1 || ny>24)
    return ch;

  for(int ich=0;ich<288;ich++){
      if( (block_map[ich].nx==nx) && (block_map[ich].ny==ny) )
	   ch = ich;
           return ch;
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

     return -1;
}

ap_uint<1> hit_coin_t(ap_uint<4> t1, ap_uint<4> t2, ap_uint<3> dt) {
  ap_uint<4> diff = (t1<t2) ? (t2-t1) : (t1-t2);
  return (diff<=dt) ? 1 : 0;
}

cluster_t Find_cluster(
    hit_t prehits[9], hit_t curhits[9], hit_t afthits[9],
    ap_uint<3> hit_dt, ap_uint<13> seed_threshold,
    ap_uint<5> x, ap_uint<4> y
  ){
    cluster_t cc;
    cc.e=0;
    cc.t=0;
    cc.nhits=0;
    cc.x=x;
    cc.y=y;
   
    if( curhits[0].e<seed_threshold ) return cc;

    ap_uint<4> t0 = 0;
    ap_uint<13> e0=0; 
    int nhits[9] = {1,0,0,0,0,0,0,0,0};

    t0 = curhits[0].t;
    e0 = curhits[0].e;
    ap_uint<16> total_e=e0;

    for(int ii=1; ii<9; ii++){
        if( curhits[ii].e>0 && hit_coin_t(t0, curhits[ii].t, hit_dt) ){
            if(curhits[ii].e > e0) return cc;
            nhits[ii] = nhits[ii]+1;
            total_e = total_e + curhits[ii].e; 
        }

        if( prehits[ii].e>0 && hit_coin_t(t0+8, prehits[ii].t, hit_dt) ){
            if(prehits[ii].e > e0) return cc;
            nhits[ii] = nhits[ii]+1;
            total_e = total_e + prehits[ii].e; 
        }

        if( afthits[ii].e && hit_coin_t(t0, afthits[ii].t+8, hit_dt) ){
            if(afthits[ii].e > e0) return cc;
            nhits[ii] = nhits[ii]+1;
            total_e = total_e + afthits[ii].e; 
        }

        if(nhits[ii]>0)
          nhits[0] = nhits[0]+nhits[ii];
        
    }  

    if(nhits[0]>0){
       cc.e = total_e;
       cc.t = t0;
       cc.nhits = nhits[0];
       cc.x = x;
       cc.y = y;
    } 

    return cc;

}

