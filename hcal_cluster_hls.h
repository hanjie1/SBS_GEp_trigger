#ifndef hcal_cluster_h
#define hcal_cluster_h

#include <ap_int.h>
#include <hls_stream.h>

// hit_t:
// - every 32ns each fadc channel reports 13 bit energy, and 3 bit hit time (time offset in current 32ns clock: 0=0ns, 1=4ns, 2=8ns, ..., 7=28ns)
// - if the channel has no hit, then the energy, e, will be reported as 0
// - energy, e, will saturate at 8191 (e.g. if the FADC integral (after pedestal subtraction and gain) is greater than 8191, the FADC report 8191
typedef struct
{
  ap_uint<13> e;
  ap_uint<3> t;
} hit_t;


// fadc_hits_t:
// - contains 256 VXS channels worth + 32 fiber of hit_t reported each 32ns
// - vxs_ch[  0] to vxs_ch[ 15]: VME slot 3, ch 0 to 15 FADC channels
//   vxs_ch[ 16] to vxs_ch[ 31]: VME slot 4, ch 0 to 15 FADC channels
//   ...
//   vxs_ch[112] to vxs_ch[127]: VME slot 10, ch 0 to 15 FADC channels
//   (VXS switch A & B are at VME slot positions 11,12, so the FADC never can be installed here)
//   vxs_ch[128] to vxs_ch[143]: VME slot 13, ch 0 to 15 FADC channels
//   vxs_ch[144] to vxs_ch[159]: VME slot 14, ch 0 to 15 FADC channels
//   ...
//   vxs_ch[240] to vxs_ch[255]: VME slot 20, ch 0 to 15 FADC channels
//
//   fiber_ch[0] to fiber_ch[31]: these come from the other crate
typedef struct
{
  hit_t vxs_ch[256];
  hit_t fiber_ch[32];
} fadc_hits_t;

typedef struct
{
  ap_uint<3> t;
  ap_uint<1> valid;
} bin_t;
typedef struct
{
  bin_t bins[128];
} fiber_bins_t;


// cluster_t:
// - recommended cluster structure to be reported for each possible cluster position.
// - idx: cluster index (map based - alternatively a 2d coordinate would be fine is easy to define
// - e: cluster measured energy (sum of up to 7 fadc_hit_t.e)
// - t: cluster time, timestamped from cluster central hit time appended to coarse 32ns frame counter: this timestamp spans 0 to 8188ns (0 to 2047 *4ns)
// - nhits: number of hit channels in cluster. can be useful to reject single channel noise hits
typedef struct
{
  ap_uint<5> x;
  ap_uint<4> y;
  ap_uint<16> e;
  ap_uint<3> t;
  ap_uint<3>  nhits;
} cluster_t;


typedef struct
{
  cluster_t c[N_CHAN_SEC];
} cluster_all_t;

// trigger_t:
// - code works with 32ns of data at a time. hits & trigger have 4ns resolution, so 8 trigger decisions per iteration are computed.
// - trig: [0]=>0ns, [1]=>4ns, [2]=>8ns, ..., [7]=28ns, when bit=0 no trigger, when bit=1 trigger
typedef struct
{
  ap_uint<8> trig;
} trigger_t;

void hcal_cluster_hls(
    ap_uint<3> hit_dt,
    ap_uint<13> seed_threshold,
    ap_uint<16> cluster_threshold,
    hls::stream<fadc_hits_t> &s_fadc_hits_pre,
    hls::stream<fadc_hits_t> &s_fadc_hits_cur,
    hls::stream<fadc_hits_t> &s_fadc_hits_aft,
    hls::stream<fiber_bins_t &s_fiberout,
    hls::stream<cluster_all_t> &s_cluster_all
);


ap_uint<5> Find_block(ap_uint<8> ch, ap_uint<2> dim);
int Find_channel(ap_uint<5> nx, ap_uint<5> ny);
int Find_nearby(ap_uint<8> ch, ap_uint<3> ii);
cluster_t Find_cluster(hit_t prehits[7], hit_t curhits[7], ap_uint<3> hit_dt, ap_uint<13> seed_threshold, ap_uint<5> x, ap_uint<4> y);


#endif
