#include <stdio.h>
#include <stdlib.h>
#include "hcal_cluster_hls.h"

cluster_t GenRandomClusters(ap_uint<13> seed_threshold, ap_uint<3> hit_dt, fadc_hits_t &a_cluster_pre, fadc_hits_t &a_cluster_cur, fadc_hits_t &a_cluster_aft){

  cluster_t cc;

  // generate a seed
  int seed_ch = rand()%288;
  ap_uint<13> seed_e = rand()%8192 + seed_threshold;
  ap_uint<3> seed_t = rand()%8;

  if(seed_ch<256){
     a_cluster_cur.vxs_ch[seed_ch].e = seed_e;
     a_cluster_cur.vxs_ch[seed_ch].t = seed_t;
  }
  else{
     a_cluster_cur.fiber_ch[seed_ch-256].e = seed_e;
     a_cluster_cur.fiber_ch[seed_ch-256].t = seed_t;
  }
  
  block_coords seed_pos;
  seed_pos = block_map[seed_ch];

  cc.x = seed_pos.nx;
  cc.y = seed_pos.ny;
  cc.t = seed_t;
  
  // generate a random cluster around each seed 

// 3*3 clusters
/*
   |---|---|---|
   | 0 | 1 | 2 |
   |---|---|---|
   | 3 | S | 4 |
   |---|---|---|
   | 5 | 6 | 7 |
   |---|---|---|
*/
  int nhits = rand()%8;
  ap_uint<4> tot = 1;
  ap_uint<13> tot_e = seed_e;

  for(int ii=0; ii<nhits; ii++){
   int nblock = rand()%8;
   switch(nblock){
    case 0:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny-1;   
    case 1:
       hit_pos_x = seed_pos.nx;
       hit_pos_y = seed_pos.ny-1;   
    case 2:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny-1;   
    case 3:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny;   
    case 4:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny;   
    case 5:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny+1;   
    case 6:
       hit_pos_x = seed_pos.nx;
       hit_pos_y = seed_pos.ny+1;   
    case 7:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny+1;   
   }
   if(hit_pos_x==0 || hit_pos_x>24 || hit_pos_y==0 || hit_pos_y>12)
     continue;
   
   hit_ch = (hit_pos_x-1)*12 + hit_pos_y-1 ;

   ap_uint<13> ee = rand()%seed_threshold;
   int dt = rand()%(2*hit_dt)-hit_dt;

   if( (seed_t+dt)<0 ){
     ap_uint<3> tt = seed_t+dt+8;
     if( hit_ch<256){
       if(a_cluster_pre.vxs_ch[hit_ch].e>0) continue;
       a_cluster_pre.vxs_ch[hit_ch].e = ee;
       a_cluster_pre.vxs_ch[hit_ch].t = tt;
     }
     else{
       if(a_cluster_pre.fiber_ch[hit_ch-256].e>0) continue;
       a_cluster_pre.fiber_ch[hit_ch-256].e = ee;
       a_cluster_pre.fiber_ch[hit_ch-256].t = tt;
     }
   }
   else if( (seed_t+ dt)>7 ){
     ap_uint<3> tt = dt + seed_t-8;
     if( hit_ch<256){
       if(a_cluster_aft.vxs_ch[hit_ch].e>0) continue;
       a_cluster_aft.vxs_ch[hit_ch].e = ee;
       a_cluster_aft.vxs_ch[hit_ch].t = tt;
     }
     else{
       if(a_cluster_aft.fiber_ch[hit_ch-256].e>0) continue;
       a_cluster_aft.fiber_ch[hit_ch-256].e = ee;
       a_cluster_aft.fiber_ch[hit_ch-256].t = tt;
     }
   }
   else{
     ap_uint<3> tt = dt + seed_t;
     if( hit_ch<256){
       if(a_cluster_cur.vxs_ch[hit_ch].e>0) continue;
       a_cluster_cur.vxs_ch[hit_ch].e = ee;
       a_cluster_cur.vxs_ch[hit_ch].t = tt;
     }
     else{
       if(a_cluster_cur.fiber_ch[hit_ch-256].e>0) continue;
       a_cluster_cur.fiber_ch[hit_ch-256].e = ee;
       a_cluster_cur.fiber_ch[hit_ch-256].t = tt;
     }
   }

   tot = tot+1;   
   tot_e = tot_e + ee;
  } 

  cc.e = tot_e;
  cc.nhits = tot;

  return cc;  
}

int main(int argc, char *argv[])
{
  ap_uint<3> hit_dt = 2;
  ap_uint<13> seed_threshold = 5000;
  ap_uint<16> cluster_threshold = 10000;
  hls::stream<fadc_hits_t> s_fadc_hits;
  hls::stream<fiber_bins_t> s_fiberout;
  hls::stream<cluster_all_t> s_cluster_all;

  srand(10);

  cluster_all_t c_verify[6];
  fiber_bins_t fbin_verify[6];

  fadc_hits_t pre_pre_hits;
  fadc_hits_t pre_hits;
  for(int frame=0; frame<6; frame++){
      fadc_hits_t new_hits;
      
      for(int ii=0; ii<256; ii++){
         new_hits.vxs_ch[ii].e=0
         new_hits.vxs_ch[ii].t=0
      }

      for(int ii=0; ii<32; ii++){
         new_hits.fiber_ch[ii].e=0
         new_hits.fiber_ch[ii].t=0
      }

      for(int ii=0; ii<128; ii++){
         fbin_verify[frame].bins[ii].t=0;
         fbin_verify[frame].bins[ii].valid=0;
      }           


      // generate random hits below seed_threshold
      int nhits = rand()%10;
      for(int ii=0; ii<nhits; ii++){
          int rd_ch = rand()%288;
          ap_uint<13> ee = rand()%seed_threshold;
          ap_uint<3> tt = rand()%8;

          if(seed_ch<256){
             new_hits.vxs_ch[rd_ch].e = ee;
             new_hits.vxs_ch[rd_ch].t = tt;
          }
          else{
             new_hits.vxs_ch[rd_ch-256].e = ee;
             new_hits.vxs_ch[rd_ch-256].t = tt;
          }
      }

      if(frame>=2){
         // generate one cluster 
         cluster_t newcc;
         newcc=GenRandomClusters(seed_threshold, hit_dt, fadc_hits_t &pre_pre_hits, fadc_hits_t &pre_hits, fadc_hits_t &new_hits);
         if(newcc.e>cluster_threshold){
           cc_ch = (newcc.x-1)*12 + newcc.y-1 ;
           ap_uint<7> cc_fbin = fiber_map[cc_ch];

         // record each cluster and the fiber output
           newt = newcc.t + (frame-1)*8;
           if(fbin_verify[frame].bins[cc_fbin].valid==1){
              if( newt<fbin_verify[frame].bins[cc_fbin].t )
                  fbin_verify[frame].bins[cc_fbin].t = newt;
           }
           else
              fbin_verify[frame].bins[cc_fbin].t = newt;
        
           fbin_verify[frame].bins[cc_fbin].valid = 1;           
           c_verify[frame].c[cc_ch] = newcc;

           printf("Cluster: chan %d, frame %d, (x,y,e,t,n)=(%d, %d, %d, %d, %d)\n ",cc_ch,nframe,newcc.x,newcc.y,newcc.e,newcc.t,newcc.nhits);
         }
      }
      
      if(frame>0){
         pre_pre_hits = pre_hits;
      }
      pre_hits = new_hits;

      s_fadc_hits.write(new_hits);
  }
  
  while(!s_fadc_hits.empty())
  {
    ecal_cluster_hls(
        hit_dt,
        seed_threshold,
        cluster_threshold,
        s_fadc_hits,
        s_fiberout,
        s_cluster_all
      );
  }

  // verify the fiber out
  int nframe=0;
  while(!s_fiberout.empty()){
    fiber_bins_t fout = s_fiberout.read();
    for(int ii=0; ii<128; ii++){
        ft_s = fbin_verify[nframe].bins[ii].t;
        fv_s = fbin_verify[nframe].bins[ii].valid;
        ft_r = fout.bins[ii].t + (nframe-1)*8;
        fv_r = fout.bins[ii].valid;

        if(fv_r != fv_s)
           printf("Fiber Fail: bin %d, frame %d, valid, (set, return)=(%d, %d)\n ",ii,nframe,fv_s,fv_r);

        if(ft_r != ft_s)
           printf("Fiber Fail: bin %d, frame %d,  time, (set, return)=(%d, %d)\n ",ii,nframe,ft_s,ft_r);
    }
    nframe++;
  }    

  // verify the cluster out
  nframe=0;
  while(!s_cluster_all.empty()){
    cluster_all_t cout = s_cluster_all.read();
    c_s = c_verify[nframe];
    for(int ii=0; ii<288; ii++){
       if( cout.c[ii].x != c_s.c[ii].x )
           printf("Cluster Fail: chan %d, frame %d, x, (set, return)=(%d, %d)\n ",ii,nframe,c_s.c[ii].x,cout.c[ii].x);
       if( cout.c[ii].y != c_s.c[ii].y )
           printf("Cluster Fail: chan %d, frame %d, y, (set, return)=(%d, %d)\n ",ii,nframe,c_s.c[ii].y,cout.c[ii].y);
       if( cout.c[ii].e != c_s.c[ii].e )
           printf("Cluster Fail: chan %d, frame %d, e, (set, return)=(%d, %d)\n ",ii,nframe,c_s.c[ii].e,cout.c[ii].e);
       if( cout.c[ii].t != c_s.c[ii].t )
           printf("Cluster Fail: chan %d, frame %d, t, (set, return)=(%d, %d)\n ",ii,nframe,c_s.c[ii].t,cout.c[ii].t);
       if( cout.c[ii].nhits != c_s.c[ii].nhits )
           printf("Cluster Fail: chan %d, frame %d, n, (set, return)=(%d, %d)\n ",ii,nframe,c_s.c[ii].nhits,cout.c[ii].nhits);
    } 
    nframe++;
  }

  return 0;
}
