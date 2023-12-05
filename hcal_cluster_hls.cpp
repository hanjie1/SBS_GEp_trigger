#include "hcal_cluster_hls.h"
using namespace std;

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

  ap_uint<9> ch=0;
  for(ch=0; ch<256; ch++)
      all_fadc_hits[ch] = fadc_hits.vxs_ch[ch];
   
  for(ch=0; ch<32; ch++)
      all_fadc_hits[ch+256] = fadc_hits.fiber_ch[ch];

/*
 for(ch=0; ch<288; ch++){
   cout<<ch<<"  "<<all_fadc_hits_pre_pre[ch].e<<"  "<<all_fadc_hits_pre_pre[ch].t<<endl;
 }

 for(ch=0; ch<288; ch++){
   cout<<ch<<"  "<<all_fadc_hits_pre[ch].e<<"  "<<all_fadc_hits_pre[ch].t<<endl;
 }

 for(ch=0; ch<288; ch++){
   cout<<ch<<"  "<<all_fadc_hits[ch].e<<"  "<<all_fadc_hits[ch].t<<endl;
 }
*/

  cluster_all_t allc;
  
  for(ch=0; ch<288;ch++){
      hit_t nearby_hit_pre[9];
      hit_t nearby_hit_cur[9];
      hit_t nearby_hit_aft[9];
  
      nearby_hit_pre[0] = all_fadc_hits_pre_pre[ch];
      nearby_hit_cur[0] = all_fadc_hits_pre[ch];
      nearby_hit_aft[0] = all_fadc_hits[ch];
  
      ap_uint<4> ipos=0;
 
      for(ipos=0; ipos<8; ipos++){
         ap_uint<9> nearby_ch = Find_nearby(ch, ipos);
         if(nearby_ch>=288) continue;

         nearby_hit_pre[ipos+1]=all_fadc_hits_pre_pre[nearby_ch];
         nearby_hit_cur[ipos+1]=all_fadc_hits_pre[nearby_ch];
         nearby_hit_aft[ipos+1]=all_fadc_hits[nearby_ch];
       }
      

      allc.c[ch] = Find_cluster(nearby_hit_pre, nearby_hit_cur, nearby_hit_aft, hit_dt, seed_threshold, Find_block(ch,0), Find_block(ch,1));

  }
     
  // save the previous fadc_hits

  for(ch=0; ch<288; ch++){
     all_fadc_hits_pre_pre[ch] = all_fadc_hits_pre[ch];
     all_fadc_hits_pre[ch] = all_fadc_hits[ch];
  }
#ifndef __SYNTHESIS__
  int nclust = 0;
  for(ch=0; ch<288;ch++){
    if(allc.c[ch].nhits>1)
      nclust++;
      //printf("nclust %d at (%d, %d), e=%d, t=%d\n",nclust,allc.c[ch].x.to_uint(),allc.c[ch].y.to_uint(),allc.c[ch].e.to_uint(),allc.c[ch].t.to_uint());
  }
  printf("nclust: %d\n",nclust);
#endif

  s_cluster_all.write(allc);
  fiber_bins_t fiberout;
  fiberout = FiberOut(allc, cluster_threshold);
  s_fiberout.write(fiberout);
  
  return;
}

block_coords block_map[288]={
{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},{1,11},{1,12},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8},{2,9},{2,10},{2,11},{2,12},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8},{3,9},{3,10},{3,11},{3,12},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8},{4,9},{4,10},{4,11},{4,12},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},{5,11},{5,12},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{6,12},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},{7,8},{7,9},{7,10},{7,11},{7,12},{8,1},{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{8,8},{8,9},{8,10},{8,11},{8,12},{9,1},{9,2},{9,3},{9,4},{9,5},{9,6},{9,7},{9,8},{9,9},{9,10},{9,11},{9,12},{10,1},{10,2},{10,3},{10,4},{10,5},{10,6},{10,7},{10,8},{10,9},{10,10},{10,11},{10,12},{11,1},{11,2},{11,3},{11,4},{11,5},{11,6},{11,7},{11,8},{11,9},{11,10},{11,11},{11,12},{12,1},{12,2},{12,3},{12,4},{12,5},{12,6},{12,7},{12,8},{12,9},{12,10},{12,11},{12,12},{13,1},{13,2},{13,3},{13,4},{13,5},{13,6},{13,7},{13,8},{13,9},{13,10},{13,11},{13,12},{14,1},{14,2},{14,3},{14,4},{14,5},{14,6},{14,7},{14,8},{14,9},{14,10},{14,11},{14,12},{15,1},{15,2},{15,3},{15,4},{15,5},{15,6},{15,7},{15,8},{15,9},{15,10},{15,11},{15,12},{16,1},{16,2},{16,3},{16,4},{16,5},{16,6},{16,7},{16,8},{16,9},{16,10},{16,11},{16,12},{17,1},{17,2},{17,3},{17,4},{17,5},{17,6},{17,7},{17,8},{17,9},{17,10},{17,11},{17,12},{18,1},{18,2},{18,3},{18,4},{18,5},{18,6},{18,7},{18,8},{18,9},{18,10},{18,11},{18,12},{19,1},{19,2},{19,3},{19,4},{19,5},{19,6},{19,7},{19,8},{19,9},{19,10},{19,11},{19,12},{20,1},{20,2},{20,3},{20,4},{20,5},{20,6},{20,7},{20,8},{20,9},{20,10},{20,11},{20,12},{21,1},{21,2},{21,3},{21,4},{21,5},{21,6},{21,7},{21,8},{21,9},{21,10},{21,11},{21,12},{22,1},{22,2},{22,3},{22,4},{22,5},{22,6},{22,7},{22,8},{22,9},{22,10},{22,11},{22,12},{23,1},{23,2},{23,3},{23,4},{23,5},{23,6},{23,7},{23,8},{23,9},{23,10},{23,11},{23,12},{24,1},{24,2},{24,3},{24,4},{24,5},{24,6},{24,7},{24,8},{24,9},{24,10},{24,11},{24,12}
};

ap_uint<7> fiber_map[288]={
0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71
};


fiber_bins_t FiberOut(cluster_all_t allc, ap_uint<16> cluster_threshold){

  fiber_bins_t allf;
  ap_uint<8> fiber_ch=0; 
  for(fiber_ch=0; fiber_ch<128; fiber_ch++){
      allf.bins[fiber_ch].t=0;
      allf.bins[fiber_ch].valid=0;
  }

  ap_uint<9> ch=0;
  for(ch=0; ch<288;ch++){
     if( allc.c[ch].e>cluster_threshold ){
         ap_uint<7> fiber_bin = fiber_map[ch];
       
         ap_uint<3> newt=0; 
         if(allf.bins[fiber_bin].valid==1)
            newt = (allf.bins[fiber_bin].t>allc.c[ch].t)?allc.c[ch].t:allf.bins[fiber_bin].t;
         else
            newt = allc.c[ch].t;
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
ap_uint<9> Find_channel(ap_uint<5> nx, ap_uint<4> ny){
  ap_uint<9> ch = 511;

  if(nx<1 || nx>12 || ny<1 || ny>24)
    return ch;

  ap_uint<9> ich=0;
  for(ich=0;ich<288;ich++){
      if( (block_map[ich].nx==nx) && (block_map[ich].ny==ny) )
	   ch = ich;
           return ch;
  }

  return ch;
}
   
ap_uint<9> Find_nearby(ap_uint<9> ch, ap_uint<3> pos){
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

     return 511;
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
cout<<x.to_uint()<<"  "<<y.to_uint()<<endl;

    ap_uint<4> t0 = 0;
    ap_uint<13> e0=0; 
    ap_uint<4> nhits[9] = {1,0,0,0,0,0,0,0,0};

    t0 = curhits[0].t;
    e0 = curhits[0].e;
    ap_uint<16> total_e=e0;

    ap_uint<4> nblock;
    for(nblock=1; nblock<9; nblock++){
        if( curhits[nblock].e>0 && hit_coin_t(t0, curhits[nblock].t, hit_dt) ){
            if(curhits[nblock].e > e0) return cc;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + curhits[nblock].e; 
        }

        if( prehits[nblock].e>0 && hit_coin_t(t0+8, prehits[nblock].t, hit_dt) ){
            if(prehits[nblock].e > e0) return cc;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + prehits[nblock].e; 
        }

        if( afthits[nblock].e && hit_coin_t(t0, afthits[nblock].t+8, hit_dt) ){
            if(afthits[nblock].e > e0) return cc;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + afthits[nblock].e; 
        }

        if(nhits[nblock]>0)
          nhits[0] = nhits[0]+nhits[nblock];
        
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

