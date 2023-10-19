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
    hls::stream<fadc_hits_t> &s_fadc_hits,
    hls::stream<fiber_bins_t &s_fiberout,
    hls::stream<cluster_all_t> &s_cluster_all
);


ap_uint<5> Find_block(ap_uint<8> ch, ap_uint<2> dim);
int Find_channel(ap_uint<5> nx, ap_uint<5> ny);
int Find_nearby(ap_uint<8> ch, ap_uint<3> ii);
cluster_t Find_cluster(hit_t prehits[7], hit_t curhits[7], ap_uint<3> hit_dt, ap_uint<13> seed_threshold, ap_uint<5> x, ap_uint<4> y);


typedef struct
{
  hit_t prehits[8];
  hit_t curhits[8];
  hit_t afthits[8];
} coin_hits;


typedef struct{
   ap_uint<5> nx;
   ap_uint<4> ny;
}block_coords;

block_coords block_map[288]={
{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},{1,11},{1,12},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8},{2,9},{2,10},{2,11},{2,12},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8},{3,9},{3,10},{3,11},{3,12},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8},{4,9},{4,10},{4,11},{4,12},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},{5,11},{5,12},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{6,12},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},{7,8},{7,9},{7,10},{7,11},{7,12},{8,1},{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{8,8},{8,9},{8,10},{8,11},{8,12},{9,1},{9,2},{9,3},{9,4},{9,5},{9,6},{9,7},{9,8},{9,9},{9,10},{9,11},{9,12},{10,1},{10,2},{10,3},{10,4},{10,5},{10,6},{10,7},{10,8},{10,9},{10,10},{10,11},{10,12},{11,1},{11,2},{11,3},{11,4},{11,5},{11,6},{11,7},{11,8},{11,9},{11,10},{11,11},{11,12},{12,1},{12,2},{12,3},{12,4},{12,5},{12,6},{12,7},{12,8},{12,9},{12,10},{12,11},{12,12},{13,1},{13,2},{13,3},{13,4},{13,5},{13,6},{13,7},{13,8},{13,9},{13,10},{13,11},{13,12},{14,1},{14,2},{14,3},{14,4},{14,5},{14,6},{14,7},{14,8},{14,9},{14,10},{14,11},{14,12},{15,1},{15,2},{15,3},{15,4},{15,5},{15,6},{15,7},{15,8},{15,9},{15,10},{15,11},{15,12},{16,1},{16,2},{16,3},{16,4},{16,5},{16,6},{16,7},{16,8},{16,9},{16,10},{16,11},{16,12},{17,1},{17,2},{17,3},{17,4},{17,5},{17,6},{17,7},{17,8},{17,9},{17,10},{17,11},{17,12},{18,1},{18,2},{18,3},{18,4},{18,5},{18,6},{18,7},{18,8},{18,9},{18,10},{18,11},{18,12},{19,1},{19,2},{19,3},{19,4},{19,5},{19,6},{19,7},{19,8},{19,9},{19,10},{19,11},{19,12},{20,1},{20,2},{20,3},{20,4},{20,5},{20,6},{20,7},{20,8},{20,9},{20,10},{20,11},{20,12},{21,1},{21,2},{21,3},{21,4},{21,5},{21,6},{21,7},{21,8},{21,9},{21,10},{21,11},{21,12},{22,1},{22,2},{22,3},{22,4},{22,5},{22,6},{22,7},{22,8},{22,9},{22,10},{22,11},{22,12},{23,1},{23,2},{23,3},{23,4},{23,5},{23,6},{23,7},{23,8},{23,9},{23,10},{23,11},{23,12},{24,1},{24,2},{24,3},{24,4},{24,5},{24,6},{24,7},{24,8},{24,9},{24,10},{24,11},{24,12}
};

#endif
