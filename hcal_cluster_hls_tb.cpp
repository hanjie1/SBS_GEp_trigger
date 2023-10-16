#include <stdio.h>
#include <stdlib.h>
#include "ecal_cluster_hls.h"

int main(int argc, char *argv[])
{
  ap_uint<3> hit_dt = 2;
  ap_uint<13> seed_threshold = 5000;
  ap_uint<16> cluster_threshold = 10000;
  hls::stream<fadc_hits_t> s_fadc_hits;
  hls::stream<trigger_t> s_trigger, s_trigger_verify;
  hls::stream<cluster_all_t> s_cluster_all;

  srand(10);

  // generate some random FADC hits
  fadc_hits_t fadc_hits;
  trigger_t trigger;

  for(int ch=0; ch<N_CHAN_SEC; ch++){
      fadc_hits.vxs_ch[ch].e = 0;
      fadc_hits.vxs_ch[ch].t = 0;

      if(ch<32){
        fadc_hits.fiber_ch_l[ch].e = 0;
        fadc_hits.fiber_ch_l[ch].t = 0;
        fadc_hits.fiber_ch_r[ch].e = 0;
        fadc_hits.fiber_ch_r[ch].t = 0;
      }
  }

  fadc_hits.vxs_ch[5].e=3000;
  fadc_hits.vxs_ch[5].t=2;
  fadc_hits.vxs_ch[10].e=6600;
  fadc_hits.vxs_ch[10].t=1;
  fadc_hits.vxs_ch[11].e=3500;
  fadc_hits.vxs_ch[11].t=3;
  fadc_hits.fiber_ch_l[1].e=2400;
  fadc_hits.fiber_ch_l[1].t=3;
  fadc_hits.fiber_ch_l[2].e=3500;
  fadc_hits.fiber_ch_l[2].t=2;
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
