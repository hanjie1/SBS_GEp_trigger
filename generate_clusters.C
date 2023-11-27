#include "generate_clusters.h"

const int seed_threshold = 5000;
const int cluster_threshold = 10000;
const int hit_dt = 3;  // in 4 ns

cluster_t GenRandomClusters(fadc_hits_t &a_cluster_pre, fadc_hits_t &a_cluster_cur, fadc_hits_t &a_cluster_aft){

  cluster_t cc;

  // generate a seed
  int seed_ch = rand()%288;
  int seed_e = rand()%8192 + seed_threshold;
  int seed_t = rand()%8;

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
  int tot = 1;
  int tot_e = seed_e;

  int hit_pos_x=0;
  int hit_pos_y=0;

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
   
   int hit_ch = (hit_pos_x-1)*12 + hit_pos_y-1 ;

   int ee = rand()%seed_threshold;
   int dt = rand()%(2*hit_dt)-hit_dt;

   if( (seed_t+dt)<0 ){
     int tt = seed_t+dt+8;
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
     int tt = dt + seed_t-8;
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
     int tt = dt + seed_t;
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


void generate_clusters(){

    fadc_hits_t pre_pre_hits;
    fadc_hits_t pre_hits;
    for(int frame=0; frame<6; frame++){
        fadc_hits_t new_hits;

        for(int ii=0; ii<256; ii++){
           new_hits.vxs_ch[ii].e=0;
           new_hits.vxs_ch[ii].t=0;
        }

        for(int ii=0; ii<32; ii++){
           new_hits.fiber_ch[ii].e=0;
           new_hits.fiber_ch[ii].t=0;
        }

      // generate random hits below seed_threshold
      int nhits = rand()%10;
      for(int ii=0; ii<nhits; ii++){
          int rd_ch = rand()%288;
          int ee = rand()%seed_threshold;
          int tt = rand()%8;

          if(rd_ch<256){
             new_hits.vxs_ch[rd_ch].e = ee;
             new_hits.vxs_ch[rd_ch].t = tt;
          }
          else{
             new_hits.fiber_ch[rd_ch-256].e = ee;
             new_hits.fiber_ch[rd_ch-256].t = tt;
          }
      }

      

      if(frame>=2){
         // generate one cluster 
         cluster_t newcc;
         newcc=GenRandomClusters(pre_pre_hits, pre_hits, new_hits);
         if(newcc.e>cluster_threshold){
           int cc_ch = (newcc.x-1)*12 + newcc.y-1 ;
           int cc_fbin = fiber_map[cc_ch];

         // record each cluster and the fiber output
           int newt = newcc.t + (frame-1)*8;
           if(fbin_verify[frame].bins[cc_fbin].valid==1){
              if( newt<fbin_verify[frame].bins[cc_fbin].t )
                  fbin_verify[frame].bins[cc_fbin].t = newt;
           }
           else
              fbin_verify[frame].bins[cc_fbin].t = newt;
        
           fbin_verify[frame].bins[cc_fbin].valid = 1;           
           c_verify[frame].c[cc_ch] = newcc;

           printf("Cluster: chan %d, frame %d, (x,y,e,t,n)=(%d, %d, %d, %d, %d)\n ",cc_ch,frame,newcc.x,newcc.y,newcc.e,newcc.t,newcc.nhits);
         }
      }
      
    }

}
