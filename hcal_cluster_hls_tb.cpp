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
       a_cluster_pre.vxs_ch[hit_ch].e = ee;
       a_cluster_pre.vxs_ch[hit_ch].t = tt;
     }
     else{
       a_cluster_pre.fiber_ch[hit_ch-256].e = ee;
       a_cluster_pre.fiber_ch[hit_ch-256].t = tt;
     }
   }
   else if( (seed_t+ dt)>7 ){
     ap_uint<3> tt = dt + seed_t-8;
     if( hit_ch<256){
       a_cluster_aft.vxs_ch[hit_ch].e = ee;
       a_cluster_aft.vxs_ch[hit_ch].t = tt;
     }
     else{
       a_cluster_aft.fiber_ch[hit_ch-256].e = ee;
       a_cluster_aft.fiber_ch[hit_ch-256].t = tt;
     }
   }
   else{
     ap_uint<3> tt = dt + seed_t;
     if( hit_ch<256){
       a_cluster_cur.vxs_ch[hit_ch].e = ee;
       a_cluster_cur.vxs_ch[hit_ch].t = tt;
     }
     else{
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

// generate a random non cluster hits around some of the seed
// record each cluster and the fiber output

int main(int argc, char *argv[])
{
  ap_uint<3> hit_dt = 2;
  ap_uint<13> seed_threshold = 5000;
  ap_uint<16> cluster_threshold = 10000;
  hls::stream<fadc_hits_t> s_fadc_hits;
  hls::stream<cluster_all_t> s_cluster_all;

  srand(10);


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

      }

  }
  

  s_fadc_hits.write(fadc_hits);

/*
  for(int frame=0;frame<4;frame++)
  {
    trigger.trig = 0;
 
    for(int ch=0;ch<N_CHAN_SEC;ch++)
    {
      if(ch<32)
      {
        fadc_hits.fiber_ch_l[ch].e = 0;
        fadc_hits.fiber_ch_l[ch].t = 0;
        fadc_hits.fiber_ch_r[ch].e = 0;
        fadc_hits.fiber_ch_r[ch].t = 0;

        if((rand() % 100)<10 && ch<15)  // 1% hit chance
         {
           fadc_hits.fiber_ch_l[ch].e = rand() % 8192; // random hit energy
           fadc_hits.fiber_ch_l[ch].t = rand() % 8;    // random hit time (4ns)

#ifndef __SYNTHESIS__
        printf("fadc hit: fiber_l ch=%d, e=%d, t=%d\n",
            ch,
            fadc_hits.fiber_ch_l[ch].e.to_uint(),
            fadc_hits.fiber_ch_l[ch].t.to_uint()*4+frame*32
          );
#endif
	 }
      }
      if((rand() % 100)<10)  // 1% hit chance
      {
        fadc_hits.vxs_ch[ch].e = rand() % 8192; // random hit energy
        fadc_hits.vxs_ch[ch].t = rand() % 8;    // random hit time (4ns)

#ifndef __SYNTHESIS__
        printf("fadc hit: ch=%d, e=%d, t=%d\n",
            ch,
            fadc_hits.vxs_ch[ch].e.to_uint(),
            fadc_hits.vxs_ch[ch].t.to_uint()*4+frame*32
          );
#endif

        if(fadc_hits.vxs_ch[ch].e >= cluster_threshold)
          trigger.trig[fadc_hits.vxs_ch[ch].t] = 1;
      }
      else
      {
        fadc_hits.vxs_ch[ch].e = 0;
        fadc_hits.vxs_ch[ch].t = 0;
      }
    }
    s_fadc_hits.write(fadc_hits);
    //s_trigger_verify.write(trigger);
  }
*/
  while(!s_fadc_hits.empty())
  {
    ecal_cluster_hls(
        hit_dt,
        seed_threshold,
        cluster_threshold,
        s_fadc_hits,
        s_trigger,
        s_cluster_all
      );
  }

  int t32ns=0;
  while(!s_cluster_all.empty()){
    cluster_all_t C = s_cluster_all.read();

#ifndef __SYNTHESIS__
    for(int nc=0; nc<N_CHAN_SEC; nc++){
      cluster_t cl=C.c[nc];
      if(cl.nhits>0){
	 int tmpx=cl.x.to_uint();
	 int tmpy=cl.y.to_uint();
	 int tmpe=cl.e.to_uint();
	 int tmpt=0;
         tmpt=cl.t.to_uint()*4+32*t32ns-16;
	 int tmpn=cl.nhits.to_uint();
         printf("cluster at block(%d,%d): e=%d, t=%d, nhits=%d\n",tmpx, tmpy, tmpe, tmpt, tmpn); 
      }
   }
#endif
   t32ns++;
  }

  t32ns = 0;
  while(!s_trigger.empty())
  {
    trigger_t trigger;

    trigger = s_trigger.read();

#ifndef __SYNTHESIS__
    for(int i=0;i<8;i++)
    {
      if(trigger.trig[i])
        printf("Trigger found at T=%dns\n", t32ns*32+i*4-16);
    }
#endif
    t32ns++;
  }

/*
  t32ns = 0;
  int nfails = 0;
  int ntrigger_empty = 0;
  int ntrigger_verify_empty = 0;
  while(!s_trigger.empty())
  {
    trigger_t trigger, trigger_verify;

    trigger = s_trigger.read();

    if(!s_trigger_verify.empty())
      trigger_verify = s_trigger_verify.read();
    else
    {
      printf("Error: s_trigger_verify()\n");
      ntrigger_verify_empty++;
      continue;
    }

    if(trigger.trig != trigger_verify.trig)
    {
      printf("trigger != trigger_verify: %s != %s, t32ns=%d\n",
          trigger.trig.to_string(8,false).c_str(),
          trigger_verify.trig.to_string(8,false).c_str(),
          t32ns
        );
      nfails++;
    }

    for(int i=0;i<8;i++)
    {
      if(trigger.trig[i])
        printf("Trigger found at T=%dns\n", t32ns*32+i*4);
    }
    t32ns++;
  }

  return nfails+ntrigger_empty+ntrigger_verify_empty;
*/
  return 0;
}
