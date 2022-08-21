#ifndef MATH_UTIL_HACKERSM64_H
#define MATH_UTIL_HACKERSM64_H

#include "types.h"

#define F32_MAX                                             __FLT_MAX__
#define NEAR_ZERO                                           __FLT_EPSILON__
#define NEAR_ONE                                            (1.0f - __FLT_EPSILON__)
#define FLOAT_ONE                                           0x3F800000
#define FLT_IS_NONZERO(x)                                   (absf(x) > NEAR_ZERO)
#define DEGREES(x) (s16)                                    ((f32) (x) * (f32) (0x10000 / 360.f))
#define CLAMP(x, low, high)                                 (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define ABS(x)                                              absx(x)
#define absf(x)                                             abs_f(x)
#define absi(x)                                             abs_s(x)
#define abss(x)                                             abs_s(x)
#define round_float(in)                                     roundf(in)
#define signum_positive(x)                                  ((x < 0) ? -1 : 1)

#define vec3_zero(v)                                        memset(v, 0, sizeof(v))
#define vec3_sumsq(v)                                       (sqr_f((v)[0]) + sqr_f((v)[1]) + sqr_f((v)[2]))
#define vec3_copy(dst, src)                                 { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; }
#define vec3_copy_y_off(dst, src, y)                        { (dst)[0] = (src)[0]; (dst)[1] = ((src)[1] + (y)); (dst)[2] = (src)[2]; }
#define vec3_diff(dst, src1, src2)                          { (dst)[0] = ((src1)[0] - (src2)[0]); (dst)[1] = ((src1)[1] - (src2)[1]); (dst)[2] = ((src1)[2] - (src2)[2]); }

#define vec3f_compare_f32(vec, x, y, z)                     ((vec)[0] == (x) && (vec)[1] == (y) && (vec)[2] == (z))
#define vec3f_copy_y_off(dest, src, yOff)                   { vec3f_copy(dest, src); (dest)[1] += yOff; }
#define vec3f_diff(dest, a, b)                              vec3f_dif(dest, a, b)
#define vec3f_prod(dest, a, b)                              vec3f_mult(dest, a, b)
#define vec3f_quot(dest, a, b)                              { (dest)[0] = (a)[0] / (b)[0]; (dest)[1] = (a)[1] / (b)[1]; (dest)[2] = (a)[2] / (b)[2]; }
#define vec3f_get_lateral_dist(from, to, lateralDist)       { *(lateralDist) = vec3f_hdist(from, to); }
#define vec3f_get_dist(from, to, dist)                      { *(dist) = vec3f_dist(from, to); }
#define vec3f_get_dist_squared(from, to, dist)              { Vec3f _diff_; vec3_diff(_diff_, to, from); *(dist) = vec3_sumsq(_diff_); }
#define vec3f_get_pitch(from, to, pitch)                    { Vec3f _diff_; vec3_diff(_diff_, to, from); *(pitch) = atan2s(sqrtf(sqr_f(_diff_[0]) + sqr_f(_diff_[2])), _diff_[1]); }
#define vec3f_get_yaw(from, to, yaw)                        { Vec3f _diff_; vec3_diff(_diff_, to, from); *(yaw) = atan2s(_diff_[2], _diff_[0]); }
#define vec3f_get_angle(from, to, pitch, yaw)               { Vec3f _diff_; vec3_diff(_diff_, to, from); *(pitch) = atan2s(sqrtf(sqr_f(_diff_[0]) + sqr_f(_diff_[2])), _diff_[1]); *(yaw) = atan2s(_diff_[2], _diff_[0]); }

#define approach_s16_symmetric(current, target, inc)        approach_s16((current), (target), (inc), (inc))
#define approach_f32_symmetric(current, target, inc)        approach_f32((current), (target), (inc), (inc))
#define approach_s16_symmetric_bool(current, target, inc)   approach_s16_bool((current), (target), (inc), (inc))
#define approach_s32_symmetric_bool(current, target, inc)   approach_s32_bool((current), (target), (inc), (inc))
#define approach_f32_symmetric_bool(current, target, inc)   approach_f32_bool((current), (target), (inc), (inc))

//
// New functions, introduced in HackerSM64
//

static inline s32 approach_angle(s32 current, s32 target, s32 inc) {
    s32 dist = (s16)(target - current);
    if (dist < 0) {
        dist += inc;
        if (dist > 0) dist = 0;
    } else if (dist > 0) {
        dist -= inc;
        if (dist < 0) dist = 0;
    }
    return (target - dist);
}

static inline bool approach_angle_bool(s16 *current, s32 target, s32 inc) {
    *current = approach_angle(*current, target, inc);
    return (*current != target);
}

static inline s16 approach_s16(s32 current, s32 target, s32 inc, s32 dec) {
    s16 dist = (target - current);
    if (dist >= 0) {
        current = ((dist >  inc) ? (current + inc) : target);
    } else {
        current = ((dist < -dec) ? (current - dec) : target);
    }
    return current;
}

static inline bool approach_s16_bool(s16 *current, s32 target, s32 inc, s32 dec) {
    *current = approach_s16(*current, target, inc, dec);
    return (*current != target);
}

static inline bool approach_s32_bool(s32 *current, s32 target, s32 inc, s32 dec) {
    *current = approach_s32(*current, target, inc, dec);
    return (*current != target);
}

static inline bool approach_f32_bool(f32 *current, f32 target, f32 inc, f32 dec) {
    *current = approach_f32(*current, target, inc, dec);
    return !(*current == target);
}

static inline bool approach_f32_signed(f32 *current, f32 target, f32 inc) {
    *current += inc;
    if (inc >= 0.0f) {
        if (*current > target) {
            *current = target;
            return true;
        }
    } else {
        if (*current < target) {
            *current = target;
            return true;
        }
    }
    return false;
}

static inline bool approach_f32_asymptotic_bool(f32 *current, f32 target, f32 multiplier) {
    if (multiplier > (1.0f)) {
        multiplier = (1.0f);
    }
    *current = (*current + ((target - *current) * multiplier));
    return (*current != target);
}

static inline f32 approach_f32_asymptotic(f32 current, f32 target, f32 multiplier) {
    return (current + ((target - current) * multiplier));
}

static inline bool approach_s16_asymptotic_bool(s16 *current, s16 target, s16 divisor) {
    s16 temp = *current;
    if (divisor == 0) {
        *current = target;
    } else {
        temp -= target;
        temp -= (temp / divisor);
        temp += target;
        *current = temp;
    }
    return (*current != target);
}

static inline s16 approach_s16_asymptotic(s16 current, s16 target, s16 divisor) {
    s16 temp = current;
    if (divisor == 0) {
        current = target;
    } else {
        temp -= target;
        temp -= (temp / divisor);
        temp += target;
        current = temp;
    }
    return current;
}

static inline s16 abs_angle_diff(s16 a0, s16 a1) {
    s16 diff = (a1 - a0);
    if (diff == -0x8000) {
        return 0x7FFF;
    }
    return abss(diff);
}

static inline void evaluate_cubic_spline(f32 progress, Vec3f pos, Vec3f spline1, Vec3f spline2, Vec3f spline3, Vec3f spline4) {
    f32 B[4];
    if (progress > 1.0f) {
        progress = 1.0f;
    }
    f32 omp = (1.0f - progress);       // 1-p
    f32 sqp = sqr_f(progress);         // p^3
    f32 hcp = (sqp * progress) / 2.0f; // (p^3)/2
    B[0] = (omp * omp * omp) / 6.0f;                            // ((1-p)^3)/6
    B[1] =  hcp - sqp + (2.0f / 3.0f);                          //  (p^3)/2 - p^2 + 2/3
    B[2] = -hcp + sqp / 2.0f + progress / 2.0f + (1.0f / 6.0f); // -(p^3)/2 + (p^2)/2 + (p^1)/2 + 1/6
    B[3] =  hcp / 3.0f;                                         //  (p^3)/6
    pos[0] = (B[0] * spline1[0]) + (B[1] * spline2[0]) + (B[2] * spline3[0]) + (B[3] * spline4[0]);
    pos[1] = (B[0] * spline1[1]) + (B[1] * spline2[1]) + (B[2] * spline3[1]) + (B[3] * spline4[1]);
    pos[2] = (B[0] * spline1[2]) + (B[1] * spline2[2]) + (B[2] * spline3[2]) + (B[3] * spline4[2]);
}

static inline void create_transformation_from_matrices(Mat4 dst, Mat4 a1, Mat4 a2) {
    Vec3f medium;
    for (s32 i = 0; i < 3; i++) {
        medium[i] = (a2[3][0] * a2[i][0])
                  + (a2[3][1] * a2[i][1])
                  + (a2[3][2] * a2[i][2]);
        dst[i][3] = 0;
    }
    for (s32 i = 0; i < 4; i++)
    for (s32 j = 0; j < 3; j++) {
        dst[i][j] = (a1[i][0] * a2[j][0]) 
                  + (a1[i][1] * a2[j][1]) 
                  + (a1[i][2] * a2[j][2]);
    }
    dst[3][0] -= medium[0];
    dst[3][1] -= medium[1];
    dst[3][2] -= medium[2];
    *((u32 *) &dst[3][3]) = FLOAT_ONE;
}

static inline void linear_mtxf_mul_vec3f(Mat4 mtx, Vec3f dest, Vec3f src) {
    f32 x = src[0];
    f32 y = src[1];
    f32 z = src[2];
    f32 *mtxp = (f32 *) mtx;
    f32 *dstp = (f32 *) dest;
    for (s32 i = 0; i < 3; i++) {
        *dstp++ = ((mtxp[0] * x)
                 + (mtxp[4] * y)
                 + (mtxp[8] * z));
        mtxp++;
    }
}

static inline void linear_mtxf_transpose_mul_vec3f(Mat4 mtx, Vec3f dest, Vec3f src) {
    f32 x = src[0];
    f32 y = src[1];
    f32 z = src[2];
    f32 *dstp = (f32 *) dest;
    for (s32 i = 0; i < 3; i++) {
        *dstp++ = ((mtx[i][0] * x)
                 + (mtx[i][1] * y)
                 + (mtx[i][2] * z)); // dot(mtx[i], src);
    }
}

#endif // MATH_UTIL_HACKERSM64_H
