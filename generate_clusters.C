#include "generate_clusters.h"

const int seed_threshold = 4000;
const int cluster_threshold = 6000;
const int hit_dt = 3;  // in 4 ns

cluster_t GenRandomClusters(fadc_hits_t &a_cluster_pre, fadc_hits_t &a_cluster_cur, fadc_hits_t &a_cluster_aft){

  cluster_t cc;

  // generate a seed
  int seed_ch = rand()%288;
  int seed_e = rand()%(8192-seed_threshold)+ seed_threshold;
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
  int hit_pos_all=0;

  for(int ii=0; ii<nhits; ii++){
   int nblock = rand()%8;
   if( (1<<nblock) &hit_pos_all)continue;  // this hit position is already included in the cluster

   switch(nblock){
    case 0:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny-1;   
       break;
    case 1:
       hit_pos_x = seed_pos.nx;
       hit_pos_y = seed_pos.ny-1;   
       break;
    case 2:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny-1;   
       break;
    case 3:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny;   
       break;
    case 4:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny;   
       break;
    case 5:
       hit_pos_x = seed_pos.nx-1;
       hit_pos_y = seed_pos.ny+1;   
       break;
    case 6:
       hit_pos_x = seed_pos.nx;
       hit_pos_y = seed_pos.ny+1;   
       break;
    case 7:
       hit_pos_x = seed_pos.nx+1;
       hit_pos_y = seed_pos.ny+1;   
       break;
   }
   if(hit_pos_x==0 || hit_pos_x>24 || hit_pos_y==0 || hit_pos_y>12)
     continue;
 
   int hit_ch = (hit_pos_x-1)*12 + hit_pos_y-1 ;

   int ee = rand()%seed_threshold;
   int dt = rand()%(2*hit_dt)-hit_dt;

   int newee=ee;

   if(hit_ch<256){
      if( (seed_t+dt)<0 ){
        int tt = seed_t+dt+8;
        if(a_cluster_pre.vxs_ch[hit_ch].e>0){
            int new_dt = seed_t + 8 - a_cluster_pre.vxs_ch[hit_ch].t;
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_pre.vxs_ch[hit_ch].e;
        }
        else{
            a_cluster_pre.vxs_ch[hit_ch].e = ee;
            a_cluster_pre.vxs_ch[hit_ch].t = tt;
        }
      }
      else if( (seed_t+ dt)>7 ){
        int tt = dt + seed_t-8;
          if(a_cluster_aft.vxs_ch[hit_ch].e>0){
            int new_dt = 8 - seed_t + a_cluster_aft.vxs_ch[hit_ch].t;
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_aft.vxs_ch[hit_ch].e;
          }
          else{
             a_cluster_aft.vxs_ch[hit_ch].e = ee;
             a_cluster_aft.vxs_ch[hit_ch].t = tt;
          }
      }
      else{
        int tt = dt + seed_t;
          if(a_cluster_cur.vxs_ch[hit_ch].e>0){
            int new_dt = abs(seed_t - a_cluster_cur.vxs_ch[hit_ch].t);
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_cur.vxs_ch[hit_ch].e;
          }
          else{
             a_cluster_cur.vxs_ch[hit_ch].e = ee;
             a_cluster_cur.vxs_ch[hit_ch].t = tt;
          }
      }
   }
   else{
      if( (seed_t+dt)<0 ){
        int tt = seed_t+dt+8;
        if(a_cluster_pre.fiber_ch[hit_ch-256].e>0){
            int new_dt = seed_t + 8 - a_cluster_pre.fiber_ch[hit_ch-256].t;
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_pre.fiber_ch[hit_ch-256].e;
        }
        else{
            a_cluster_pre.fiber_ch[hit_ch-256].e = ee;
            a_cluster_pre.fiber_ch[hit_ch-256].t = tt;
        }
      }
      else if( (seed_t+ dt)>7 ){
        int tt = dt + seed_t-8;
          if(a_cluster_aft.fiber_ch[hit_ch-256].e>0){
            int new_dt = 8 - seed_t + a_cluster_aft.fiber_ch[hit_ch-256].t;
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_aft.fiber_ch[hit_ch-256].e;
          }
          else{
             a_cluster_aft.fiber_ch[hit_ch-256].e = ee;
             a_cluster_aft.fiber_ch[hit_ch-256].t = tt;
          }
      }
      else{
        int tt = dt + seed_t;
          if(a_cluster_cur.fiber_ch[hit_ch-256].e>0){
            int new_dt = abs(seed_t - a_cluster_cur.fiber_ch[hit_ch-256].t);
            if(new_dt>hit_dt)
              continue;
            else
              newee = a_cluster_cur.fiber_ch[hit_ch-256].e;
          }
          else{
             a_cluster_cur.fiber_ch[hit_ch-256].e = ee;
             a_cluster_cur.fiber_ch[hit_ch-256].t = tt;
          }
      }


   }
//cout<<ii<<"  "<<nblock<<"  "<<hit_ch<<"  "<<newee<<endl;
   hit_pos_all = hit_pos_all | (1<<nblock);

   tot = tot+1;   
   tot_e = tot_e + newee;
  } 

  cc.e = tot_e;
  cc.nhits = tot;

  return cc;  
}


void generate_clusters(){

    fadc_hits_t pre_pre_hits;
    fadc_hits_t pre_hits;

    ofstream outfile_c("data/gen_clusters.txt");
    ofstream outfile_f("data/gen_fibers.txt");

    int nframe=6;
    for(int frame=0; frame<nframe; frame++){
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
           int newt = (newcc.t + (frame-1)*8)*4;
           outfile_c<<frame-1<<" "<<cc_ch<<" "<<" "<<newcc.x<<" "<<newcc.y<<" "<<newcc.e<<" "<<newcc.t<<" "<<newcc.nhits<<endl;
           outfile_f<<frame-1<<" "<<cc_fbin<<" "<<" "<<newt<<" "<<endl;
         }

      }

      if(frame>=2){
         ofstream outfile(Form("data/frame%d.txt",frame-2));
         for(int ich=0; ich<256; ich++)
            outfile<<pre_pre_hits.vxs_ch[ich].e<<"  "<<pre_pre_hits.vxs_ch[ich].t<<endl;
         for(int ich=0; ich<32; ich++)
            outfile<<pre_pre_hits.fiber_ch[ich].e<<"  "<<pre_pre_hits.fiber_ch[ich].t<<endl;
         
         outfile.close();
      }

      if(frame>=(nframe-2)){
         ofstream outfile1(Form("data/frame%d.txt",nframe-2));
         for(int ich=0; ich<256; ich++)
            outfile1<<pre_hits.vxs_ch[ich].e<<"  "<<pre_hits.vxs_ch[ich].t<<endl;
         for(int ich=0; ich<32; ich++)
            outfile1<<pre_hits.fiber_ch[ich].e<<"  "<<pre_hits.fiber_ch[ich].t<<endl;
         outfile1.close();

         ofstream outfile2(Form("data/frame%d.txt",nframe-1));
         for(int ich=0; ich<256; ich++)
            outfile2<<new_hits.vxs_ch[ich].e<<"  "<<new_hits.vxs_ch[ich].t<<endl;
         for(int ich=0; ich<32; ich++)
            outfile2<<new_hits.fiber_ch[ich].e<<"  "<<new_hits.fiber_ch[ich].t<<endl;
         outfile2.close();
      }

      if(frame>0){
         pre_pre_hits = pre_hits;
/*
for(int ich=0; ich<256; ich++){
if(pre_hits.vxs_ch[ich].e>0)
  cout<<ich<<"  "<<pre_hits.vxs_ch[ich].e<<"  "<<pre_hits.vxs_ch[ich].t<<endl;
}

for(int ich=0; ich<32; ich++){
if(pre_hits.fiber_ch[ich].e>0)
  cout<<ich<<"  "<<pre_hits.fiber_ch[ich].e<<"  "<<pre_hits.fiber_ch[ich].t<<endl;
}
cout<<"================= 3 ================"<<endl;
*/
      }
      pre_hits = new_hits;

    }

}
