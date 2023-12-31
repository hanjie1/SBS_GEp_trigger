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
  hls::stream<fadc_hits_t> s_fadc_hits;
  hls::stream<fiber_bins_t> s_fiberout;
  hls::stream<cluster_all_t> s_cluster_all;

  int nframe=6;
  int ii=0;
  for(ii=0; ii<nframe; ii++){

      fadc_hits_t new_hits;

      char filename[200];
      snprintf(filename, 200, "%s%d.txt","/home/hanjie/GEp/GEp_trigger/data/frame",ii);
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
            new_hits.vxs_ch[nn].e = (ap_uint<14>)ee;
            new_hits.vxs_ch[nn].t = (ap_uint<3>)tt;
         }
         else{
            new_hits.fiber_ch[nn-NCHAN_CRATE].e = (ap_uint<14>)ee;
            new_hits.fiber_ch[nn-NCHAN_CRATE].t = (ap_uint<3>)tt;
         }
         nn++;
      }
      printf("%d %d\n",ii,nn);
      infile.close();

      s_fadc_hits.write(new_hits);
  }

  int nn=0;
  while(!s_fadc_hits.empty())
  {
    cout<<"frame  "<<nn<<endl;
    hcal_cluster_hls(
        hit_dt,
        seed_threshold,
        cluster_threshold,
        s_fadc_hits,
        s_fiberout,
        s_cluster_all
      );
    nn++;
  }

  nn=0;
  while(!s_cluster_all.empty()){
     cluster_all_t cc = s_cluster_all.read();
     for(int ii=0; ii<NCHAN_TOT; ii++){
         if(cc.c[ii].e>0){
            int tmpe = cc.c[ii].e.to_uint();
            int tmpt = cc.c[ii].t.to_uint()*4+(nn-1)*8*4;
            int tmpx = cc.c[ii].x.to_uint();
            int tmpy = cc.c[ii].y.to_uint();
            int tmpn = cc.c[ii].nhits.to_uint();

            printf("Find cluster at frame %d: ch=%d, e=%d, t=%d, x=%d, y=%d, nhits=%d\n",nn,ii,tmpe,tmpt,tmpx,tmpy,tmpn);
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
