#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "hcal_cluster_hls.h"
using namespace std;

int main(int argc, char *argv[])
{

  ap_uint<3> hit_dt = 3;
  ap_uint<13> seed_threshold = 4000;
  ap_uint<16> cluster_threshold = 6000;
  hls::stream<fadc_hits_vxs> s_fadc_hits_vxs;
  hls::stream<fadc_hits_fiber> s_fadc_hits_fiber;
  hls::stream<fiber_bins_t> s_fiberout;
  hls::stream<cluster_all_t> s_cluster_all[4];

  int nframe=6;
  int ii=0;
  for(ii=0; ii<nframe; ii++){

      fadc_hits_vxs new_hits_vxs;
      fadc_hits_fiber new_hits_fiber;

      char filename[200];
      snprintf(filename, 200, "%s%d.txt","/daqfs/home/hanjie/Desktop/GEp/SBS_GEp_trigger/data/frame",ii);
      ifstream infile(filename);
      if(infile.is_open())
        printf("open file: %s\n",filename);
      else
        continue;

      int nn=0;
      while( !infile.eof() && nn<NCHAN_TOT) 
      {
         int ee,tt;
         infile>>ee;
         infile>>tt;
         if(nn<NCHAN_CRATE){
            new_hits_vxs.vxs_ch[nn].e = (ap_uint<13>)ee;
            new_hits_vxs.vxs_ch[nn].t = (ap_uint<3>)tt;
         }
         else{
            new_hits_fiber.fiber_ch[nn-NCHAN_CRATE].e = (ap_uint<13>)ee;
            new_hits_fiber.fiber_ch[nn-NCHAN_CRATE].t = (ap_uint<3>)tt;
         }
         nn++;
      }
      printf("%d %d\n",ii,nn);
      infile.close();

      s_fadc_hits_vxs.write(new_hits_vxs);
      s_fadc_hits_fiber.write(new_hits_fiber);
  }

  int nn=0;
  while(!s_fadc_hits_vxs.empty())
  {
    cout<<"frame  "<<nn<<endl;
    hcal_cluster_hls(
        hit_dt,
        seed_threshold,
        cluster_threshold,
        s_fadc_hits_vxs,
        s_fadc_hits_fiber,
        s_fiberout,
        s_cluster_all
      );
    nn++;
  }

  nn=0;
  while(!s_cluster_all[0].empty()){
     for(int ii=0; ii<4; ii++){
       cluster_all_t cc = s_cluster_all[ii].read();
       for(int jj=0; jj<(NCHAN_TOT/4); jj++){
           if(cc.c[jj].e>0){
              int tmpe = cc.c[jj].e.to_uint();
              int tmpt = cc.c[jj].t.to_uint()*4+(nn-1)*8*4;
              int tmpx = cc.c[jj].x.to_uint();
              int tmpy = cc.c[jj].y.to_uint();
              int tmpn = cc.c[jj].nhits.to_uint();
  
              printf("Find cluster at frame %d: ch=%d, e=%d, t=%d, x=%d, y=%d, nhits=%d\n",nn,jj+ii*NCHAN_TOT/4,tmpe,tmpt,tmpx,tmpy,tmpn);
           }
       }
     }
     nn++;
  }

  nn=0;
  while(!s_fiberout.empty()){
     fiber_bins_t ff = s_fiberout.read();
     for(int ii=0; ii<128; ii++){
         if(ff.bins[ii].valid==1){
            int tmpt = ff.bins[ii].t.to_uint()*4+(nn-1)*8*4;

            printf("Fiber out at frame %d: ch=%d, t=%d\n",nn,ii,tmpt);
         }
     }
     nn++;
  }


  return 0;
}
