#include "hcal_cluster_hls.h"
using namespace std;

block_coords block_map[288]={
{7,1},{7,7},{7,2},{7,8},{7,3},{7,9},{7,4},{7,10},{7,5},{7,11},{7,6},{7,12},{10,1},{10,7},{10,2},{10,8},{8,1},{8,7},{8,2},{8,8},{8,3},{8,9},{8,4},{8,10},{8,5},{8,11},{8,6},{8,12},{10,3},{10,9},{10,4},{10,10},{9,1},{9,7},{9,2},{9,8},{9,3},{9,9},{9,4},{9,10},{9,5},{9,11},{9,6},{9,12},{10,5},{10,11},{10,6},{10,12},{11,1},{11,7},{11,2},{11,8},{11,3},{11,9},{11,4},{11,10},{11,5},{11,11},{11,6},{11,12},{2,1},{2,7},{2,2},{2,8},{12,1},{12,7},{12,2},{12,8},{12,3},{12,9},{12,4},{12,10},{12,5},{12,11},{12,6},{12,12},{2,3},{2,9},{2,4},{2,10},{1,1},{1,7},{1,2},{1,8},{1,3},{1,9},{1,4},{1,10},{1,5},{1,11},{1,6},{1,12},{2,5},{2,11},{2,6},{2,12},{3,1},{3,7},{3,2},{3,8},{3,3},{3,9},{3,4},{3,10},{3,5},{3,11},{3,6},{3,12},{6,1},{6,7},{6,2},{6,8},{4,1},{4,7},{4,2},{4,8},{4,3},{4,9},{4,4},{4,10},{4,5},{4,11},{4,6},{4,12},{6,3},{6,9},{6,4},{6,10},{5,1},{5,7},{5,2},{5,8},{5,3},{5,9},{5,4},{5,10},{5,5},{5,11},{5,6},{5,12},{6,5},{6,11},{6,6},{6,12},{13,1},{13,7},{13,2},{13,8},{13,3},{13,9},{13,4},{13,10},{13,5},{13,11},{13,6},{13,12},{16,1},{16,7},{16,2},{16,8},{14,1},{14,7},{14,2},{14,8},{14,3},{14,9},{14,4},{14,10},{14,5},{14,11},{14,6},{14,12},{16,3},{16,9},{16,4},{16,10},{15,1},{15,7},{15,2},{15,8},{15,3},{15,9},{15,4},{15,10},{15,5},{15,11},{15,6},{15,12},{16,5},{16,11},{16,6},{16,12},{17,1},{17,7},{17,2},{17,8},{17,3},{17,9},{17,4},{17,10},{17,5},{17,11},{17,6},{17,12},{20,1},{20,7},{20,2},{20,8},{18,1},{18,7},{18,2},{18,8},{18,3},{18,9},{18,4},{18,10},{18,5},{18,11},{18,6},{18,12},{20,3},{20,9},{20,4},{20,10},{19,1},{19,7},{19,2},{19,8},{19,3},{19,9},{19,4},{19,10},{19,5},{19,11},{19,6},{19,12},{20,5},{20,11},{20,6},{20,12},{21,1},{21,7},{21,2},{21,8},{21,3},{21,9},{21,4},{21,10},{21,5},{21,11},{21,6},{21,12},{24,1},{24,7},{24,2},{24,8},{22,1},{22,7},{22,2},{22,8},{22,3},{22,9},{22,4},{22,10},{22,5},{22,11},{22,6},{22,12},{24,3},{24,9},{24,4},{24,10},{23,1},{23,7},{23,2},{23,8},{23,3},{23,9},{23,4},{23,10},{23,5},{23,11},{23,6},{23,12},{24,5},{24,11},{24,6},{24,12}
};

ap_uint<7> fiber_map[288]={
0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71
};

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
    hls::stream<fadc_hits_vxs> &s_fadc_hits_vxs,
    hls::stream<fadc_hits_fiber> &s_fadc_hits_fiber,
    hls::stream<fiber_bins_t> &s_fiberout,
    hls::stream<cluster_all_t> (&s_cluster_all)[4]
  )
{
  fadc_hits_vxs vxs_hits = s_fadc_hits_vxs.read();
#pragma HLS ARRAY_PARTITION variable=vxs_hits.vxs_ch dim=1 type=complete

  fadc_hits_fiber fiber_hits = s_fadc_hits_fiber.read();
#pragma HLS ARRAY_PARTITION variable=fiber_hits.fiber_ch dim=1 type=complete

  static hit_t all_fadc_hits_pre_pre[NCHAN_TOT];
#pragma HLS ARRAY_PARTITION variable=all_fadc_hits_pre_pre dim=1 type=complete

  static hit_t all_fadc_hits_pre[NCHAN_TOT];
#pragma HLS ARRAY_PARTITION variable=all_fadc_hits_pre dim=1 type=complete

  hit_t all_fadc_hits[NCHAN_TOT];
#pragma HLS ARRAY_PARTITION variable=all_fadc_hits dim=1 type=complete

  ap_uint<9> ch=0;
  for(ch=0; ch<NCHAN_CRATE; ch++)
      all_fadc_hits[ch] = vxs_hits.vxs_ch[ch];
   
  for(ch=0; ch<NCHAN_FIBER; ch++)
      all_fadc_hits[ch+NCHAN_CRATE] = fiber_hits.fiber_ch[ch];
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

// get all the nearby blocks for each block
  static ap_uint<9> nearby_ch[NCHAN_TOT][9];
  for(ch=0; ch<NCHAN_TOT; ch++){
      nearby_ch[ch][0] = ch;

      ap_uint<4> ipos;
      for(ipos=0; ipos<8; ipos++)
         nearby_ch[ch][ipos+1] = Find_nearby(ch, ipos);

  }

  cluster_all_t allc[4];
#pragma HLS ARRAY_PARTITION variable=allc dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[0].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[1].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[2].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[3].c dim=1 type=complete

  for(ch=0; ch<NCHAN_TOT;ch++){
//  for(ch=0; ch<10;ch++){
      hit_t nearby_hit_pre[9];
#pragma HLS ARRAY_PARTITION variable=nearby_hit_pre dim=1 type=complete

      hit_t nearby_hit_cur[9];
#pragma HLS ARRAY_PARTITION variable=nearby_hit_cur dim=1 type=complete

      hit_t nearby_hit_aft[9];
#pragma HLS ARRAY_PARTITION variable=nearby_hit_aft dim=1 type=complete

      hit_t hit_t_init = {0,0};
  
      ap_uint<4> ipos=0;
      for(ipos=0; ipos<9; ipos++){

       nearby_hit_pre[ipos] = (nearby_ch[ch][ipos]<NCHAN_TOT) ? all_fadc_hits_pre_pre[nearby_ch[ch][ipos]] : hit_t_init;
       nearby_hit_cur[ipos] = (nearby_ch[ch][ipos]<NCHAN_TOT) ? all_fadc_hits_pre[nearby_ch[ch][ipos]] : hit_t_init;
       nearby_hit_aft[ipos] = (nearby_ch[ch][ipos]<NCHAN_TOT) ? all_fadc_hits[nearby_ch[ch][ipos]] : hit_t_init;
      }
     
      cluster_t tmpc;
      tmpc = Find_cluster(nearby_hit_pre, nearby_hit_cur, nearby_hit_aft, hit_dt, seed_threshold, Find_block(ch,0), Find_block(ch,1));

     
      ap_uint<2> sec = ch/(NCHAN_TOT/4);
      switch(sec){
         case 0: allc[0].c[ch] = tmpc; break;
         case 1: allc[1].c[ch-NCHAN_TOT/4] = tmpc; break;
         case 2: allc[2].c[ch-2*NCHAN_TOT/4] = tmpc; break;
         case 3: allc[3].c[ch-3*NCHAN_TOT/4] = tmpc; break;
      }

  }

  // save the previous fadc_hits

  for(ch=0; ch<NCHAN_TOT; ch++){
     all_fadc_hits_pre_pre[ch] = all_fadc_hits_pre[ch];
  }

  for(ch=0; ch<NCHAN_TOT; ch++){
     all_fadc_hits_pre[ch] = all_fadc_hits[ch];
  }


#ifndef __SYNTHESIS__
  int nclust = 0;
  for(ch=0; ch<NCHAN_TOT;ch++){
    int sec = ch/(NCHAN_TOT/4);

    cluster_t tmpc; 
    switch(sec){
      case 0: tmpc = allc[0].c[ch]; break;
      case 1: tmpc = allc[1].c[ch-NCHAN_TOT/4]; break;
      case 2: tmpc = allc[2].c[ch-2*NCHAN_TOT/4]; break;
      case 3: tmpc = allc[3].c[ch-3*NCHAN_TOT/4]; break;
    }

    if(tmpc.nhits>0){
      nclust++;
      printf("nclust %d at (%d, %d), e=%d, t=%d\n",nclust,tmpc.x.to_uint(),tmpc.y.to_uint(),tmpc.e.to_uint(),tmpc.t.to_uint());
    }
  }
  printf("nclust: %d\n",nclust);
#endif

  s_cluster_all[0].write(allc[0]);
  s_cluster_all[1].write(allc[1]);
  s_cluster_all[2].write(allc[2]);
  s_cluster_all[3].write(allc[3]);

  fiber_bins_t fiberout;
  fiberout = FiberOut(allc, cluster_threshold);
  s_fiberout.write(fiberout);
  
  return;
}


fiber_bins_t FiberOut(cluster_all_t allc[4], ap_uint<16> cluster_threshold){
#pragma HLS ARRAY_PARTITION variable=allc dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[0].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[1].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[2].c dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=allc[3].c dim=1 type=complete

  fiber_bins_t allf;
#pragma HLS ARRAY_PARTITION variable=allf.bins dim=1 type=complete

  ap_uint<8> fiber_ch=0; 
  for(fiber_ch=0; fiber_ch<128; fiber_ch++){
      allf.bins[fiber_ch].t=0;
      allf.bins[fiber_ch].valid=0;
  }

  ap_uint<9> ch=0;
  for(ch=0; ch<NCHAN_TOT;ch++){

      ap_uint<2> sec = ch/(NCHAN_TOT/4);

      cluster_t tmpc; 
      switch(sec){
        case 0: tmpc = allc[0].c[ch]; break;
        case 1: tmpc = allc[1].c[ch-NCHAN_TOT/4]; break;
        case 2: tmpc = allc[2].c[ch-2*NCHAN_TOT/4]; break;
        case 3: tmpc = allc[3].c[ch-3*NCHAN_TOT/4]; break;
      }

     if( tmpc.e>cluster_threshold ){
         ap_uint<7> fiber_bin = fiber_map[ch];
       
         ap_uint<3> newt=0; 
         if(allf.bins[fiber_bin].valid==1)
            newt = (allf.bins[fiber_bin].t>tmpc.t)?tmpc.t:allf.bins[fiber_bin].t;
         else
            newt = tmpc.t;
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

//  if(nx<1 || nx>24 || ny<1 || ny>12)
//    return ch;

  ap_uint<9> ich=0;
  for(ich=0;ich<NCHAN_TOT;ich++){
      if( (block_map[ich].nx==nx) && (block_map[ich].ny==ny) ){
	   ch = ich;
      }
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

#pragma HLS ARRAY_PARTITION variable=prehits dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=curhits dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=afthits dim=1 type=complete

    cluster_t cc;
    cc.e=0;
    cc.t=0;
    cc.nhits=0;
    cc.x=x;
    cc.y=y;
   
    if( curhits[0].e<seed_threshold ) return cc;

    ap_uint<4> t0 = 0;
    ap_uint<13> e0=0; 
    ap_uint<4> nhits[9] = {1,0,0,0,0,0,0,0,0};

    t0 = curhits[0].t;
    e0 = curhits[0].e;
    ap_uint<16> total_e=e0;

    ap_uint<4> nblock;


    for(nblock=1; nblock<9; nblock++){
        if( prehits[nblock].e>0 && hit_coin_t(t0+8, prehits[nblock].t, hit_dt) ){
            if(prehits[nblock].e > e0)
               nhits[0]=0;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + prehits[nblock].e; 
        }
        else if( curhits[nblock].e>0 && hit_coin_t(t0, curhits[nblock].t, hit_dt) ){
            if(curhits[nblock].e > e0)
               nhits[0]=0;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + curhits[nblock].e; 
        }
        else if( afthits[nblock].e && hit_coin_t(t0, afthits[nblock].t+8, hit_dt) ){
            if(afthits[nblock].e > e0) 
               nhits[0]=0;
            nhits[nblock] = nhits[nblock]+1;
            total_e = total_e + afthits[nblock].e; 
        }
    }  

    if(nhits[0]>0){
       cc.e = total_e;
       cc.t = t0;
       cc.nhits = nhits[0]+nhits[1]+nhits[2]+nhits[3]+nhits[4]+nhits[5]+nhits[6]+nhits[7]+nhits[8];
       cc.x = x;
       cc.y = y;
    } 

    return cc;

}

