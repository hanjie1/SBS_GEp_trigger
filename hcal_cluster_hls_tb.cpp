#include <stdio.h>
#include <stdlib.h>
#include "hcal_cluster_hls.h"

int main(int argc, char *argv[])
{
  ap_uint<3> hit_dt = 3;
  ap_uint<13> seed_threshold = 5000;
  ap_uint<16> cluster_threshold = 10000;
  hls::stream<fadc_hits_t> s_fadc_hits;
  hls::stream<fiber_bins_t> s_fiberout;
  hls::stream<cluster_all_t> s_cluster_all;

  int nframe=6;
  for(int ii=0; ii<nframe; ii++){

      fadc_hits_t new_hits;

      char filename[100];
      sprintf(filename, "%s%d.txt","data/frame",ii);
      ifstream infile(filename);

      string line;
      int nn=0;
      while( getline( infile, line ) )
      {
         int ee,tt;
         sscanf(line, "%d  %d",ee,tt);
         if(nn<256){
            new_hits.vxs_ch[nn].e = (ap_uint<14>)ee;
            new_hits.vxs_ch[nn].t = (ap_uint<3>)tt;
         }
         else{
            new_hits.fiber_ch[nn-256].e = (ap_uint<14>)ee;
            new_hits.fiber_ch[nn-256].t = (ap_uint<3>)tt;
         }
         nn++;
      }

      s_fadc_hits.write(new_hits);
  }
  
  while(!s_fadc_hits.empty())
  {
    hcal_cluster_hls(
        hit_dt,
        seed_threshold,
        cluster_threshold,
        s_fadc_hits,
        s_fiberout,
        s_cluster_all
      );
  }

  int nn=0;
  while(!s_cluster_all.empty()){
     cluster_all_t cc = s_cluster_all.read();
     for(int ii=0; ii<288; ii++){
         if(cc[ii].e>0){
            tmpe = cc[ii].e.to_uint();
            tmpt = cc[ii].t.to_uint()*4+nn*8*4;
            tmpx = cc[ii].x.to_uint();
            tmpy = cc[ii].y.to_uint();
            tmpn = cc[ii].nhits.to_uint();

            printf("Find cluster at frame %d: ch=%d, e=%d, t=%d, x=%d, y=%d, nhits=%d\n",nn,ii,tmpe,tmpt,tmpx,tmpy,tmpn);
         }
     }
     nn++;
  }

  nn=0;
  while(!s_fiberout.empty()){
     fiber_bins_t ff = s_fiberout.read();
     for(int ii=0; ii<128; ii++){
         if(ff[ii].valid==1){
            tmpt = cc[ii].t.to_uint()*4+nn*8*4;

            printf("Fiber out at frame %d: ch=%d, t=%d\n",nn,ii,tmpt);
         }
     }
     nn++;
  }


  return 0;
}
