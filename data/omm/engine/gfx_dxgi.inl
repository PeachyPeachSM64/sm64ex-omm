extern "C" {
#undef FRAME_INTERVAL_US_DENOMINATOR
#define FRAME_INTERVAL_US_DENOMINATOR (3 * gNumInterpolatedFrames)
extern int gNumInterpolatedFrames;
uint64_t *frame_timestamp = &dxgi.frame_timestamp;
}
