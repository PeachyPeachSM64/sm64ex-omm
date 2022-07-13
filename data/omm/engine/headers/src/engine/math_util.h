#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "types.h"

#define sins(x)     gSineTable[(u16) (x) >> 4]
#define coss(x)     (gSineTable + 0x400)[(u16) (x) >> 4]
#define min(a, b)   ((a) <= (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))
#define sqr(x)      ((x) * (x))
#define absx(x)     ((x) < 0 ? -(x) : (x))

extern f32 gSineTable[];
extern Vec2f gVec2fZero;
extern Vec2f gVec2fOne;
extern Vec3f gVec3fZero;
extern Vec3f gVec3fOne;
extern Vec3s gVec3sZero;
extern Vec3s gVec3sOne;
extern Vec4f gVec4fZero;
extern Vec4f gVec4fOne;
extern Mat4 gMat4Identity;

static inline s32 min_s         (s32 a, s32 b)                      { return (a < b ? a : b); }
static inline f32 min_f         (f32 a, f32 b)                      { return (a < b ? a : b); }
static inline s32 max_s         (s32 a, s32 b)                      { return (a > b ? a : b); }
static inline f32 max_f         (f32 a, f32 b)                      { return (a > b ? a : b); }
static inline s32 near_s        (s32 x, s32 a, s32 b)               { return (x - a < b - x ? a : b); }
static inline f32 near_f        (f32 x, f32 a, f32 b)               { return (x - a < b - x ? a : b); }
static inline s32 far_s         (s32 x, s32 a, s32 b)               { return (x - a > b - x ? a : b); }
static inline f32 far_f         (f32 x, f32 a, f32 b)               { return (x - a > b - x ? a : b); }
static inline s32 min_3_s       (s32 a, s32 b, s32 c)               { return min_s(min_s(a, b), c); }
static inline f32 min_3_f       (f32 a, f32 b, f32 c)               { return min_f(min_f(a, b), c); }
static inline s32 max_3_s       (s32 a, s32 b, s32 c)               { return max_s(max_s(a, b), c); }
static inline f32 max_3_f       (f32 a, f32 b, f32 c)               { return max_f(max_f(a, b), c); }
static inline s32 clamp_s       (s32 x, s32 a, s32 b)               { return (x < a ? a : (x > b ? b : x)); }
static inline f32 clamp_f       (f32 x, f32 a, f32 b)               { return (x < a ? a : (x > b ? b : x)); }
static inline s32 clamp_0_1_s   (s32 x)                             { return clamp_s(x, 0, 1); }
static inline f32 clamp_0_1_f   (f32 x)                             { return clamp_f(x, 0.f, 1.f); }
static inline s32 lerp_s        (f32 t, s32 a, s32 b)               { return (a + (s32) (((f32) (b - a)) * t)); }
static inline f32 lerp_f        (f32 t, f32 a, f32 b)               { return (a + (b - a) * t); }
static inline f32 invlerp_s     (s32 x, s32 a, s32 b)               { return (f32) (x - a) / (f32) (b - a); }
static inline f32 invlerp_f     (f32 x, f32 a, f32 b)               { return (x - a) / (b - a); }
static inline f32 invlerp_0_1_s (s32 x, s32 a, s32 b)               { return clamp_f(invlerp_s(x, a, b), 0.f, 1.f); }
static inline f32 invlerp_0_1_f (f32 x, f32 a, f32 b)               { return clamp_f(invlerp_f(x, a, b), 0.f, 1.f); }
static inline s32 relerp_s      (s32 x, s32 a, s32 b, s32 c, s32 d) { return lerp_s(invlerp_s(x, a, b), c, d); }
static inline f32 relerp_f      (f32 x, f32 a, f32 b, f32 c, f32 d) { return lerp_f(invlerp_f(x, a, b), c, d); }
static inline s32 relerp_0_1_s  (s32 x, s32 a, s32 b, s32 c, s32 d) { return lerp_s(invlerp_0_1_s(x, a, b), c, d); }
static inline f32 relerp_0_1_f  (f32 x, f32 a, f32 b, f32 c, f32 d) { return lerp_f(invlerp_0_1_f(x, a, b), c, d); }
static inline s32 abs_s         (s32 x)                             { return (x < 0 ? -x : x); }
static inline f32 abs_f         (f32 x)                             { return (x < 0.f ? -x : x); }
static inline s32 sign_s        (s32 x)                             { return (x < 0 ? -1 : +1); }
static inline f32 sign_f        (f32 x)                             { return (x < 0.f ? -1.f : +1.f); }
static inline s32 sign_0_s      (s32 x)                             { return (x == 0 ? 0 : sign_s(x)); }
static inline f32 sign_0_f      (f32 x)                             { return (x == 0.f ? 0.f : sign_f(x)); }
static inline s32 sqr_s         (s32 x)                             { return x * x; }
static inline f32 sqr_f         (f32 x)                             { return x * x; }

#define vec2f_zero(dest)        memset(dest, 0, sizeof(Vec2f))
#define vec2f_one(dest)         memcpy(dest, gVec2fOne, sizeof(Vec2f))
#define vec2f_copy(dest, src)   memcpy(dest, src, sizeof(Vec2f))

#define vec3f_zero(dest)        memset(dest, 0, sizeof(Vec3f))
#define vec3f_one(dest)         memcpy(dest, gVec3fOne, sizeof(Vec3f))
#define vec3f_copy(dest, src)   memcpy(dest, src, sizeof(Vec3f))

#define vec3s_zero(dest)        memset(dest, 0, sizeof(Vec3s))
#define vec3s_one(dest)         memcpy(dest, gVec3sOne, sizeof(Vec3s))
#define vec3s_copy(dest, src)   memcpy(dest, src, sizeof(Vec3s))

#define vec4f_zero(dest)        memset(dest, 0, sizeof(Vec4f))
#define vec4f_one(dest)         memcpy(dest, gVec4fOne, sizeof(Vec4f))
#define vec4f_copy(dest, src)   memcpy(dest, src, sizeof(Vec4f))

#define mtxf_zero(dest)         memset(dest, 0, sizeof(Mat4))
#define mtxf_identity(dest)     memcpy(dest, gMat4Identity, sizeof(Mat4))
#define mtxf_copy(dest, src)    memcpy(dest, src, sizeof(Mat4))
#define mtxf_to_mtx(dest, src)  memcpy(dest, src, sizeof(Mat4))

u16 random_u16(void);
f32 random_float(void);
s32 random_sign(void);

void *vec2f_and_dist_to_3d(Vec3f dest3d, Vec2f src2d, f32 dist2d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2);
void *vec2f_to_3d_plane(Vec3f dest, Vec2f src, Vec3f o, Vec3f e1, Vec3f e1Scale, Vec3f e2, Vec3f e2Scale);
void *vec2f_get_projected_point_on_line(Vec2f dest, Vec2f p, Vec2f a, Vec2f b);

f32   vec3f_length(Vec3f v);
f32   vec3f_dot(Vec3f a, Vec3f b);
f32   vec3f_dist(Vec3f a, Vec3f b);
f32   vec3f_hdist(Vec3f a, Vec3f b);
void *vec3f_set(Vec3f dest, f32 x, f32 y, f32 z);
void *vec3f_add(Vec3f dest, Vec3f a);
void *vec3f_sum(Vec3f dest, Vec3f a, Vec3f b);
void  vec3f_sub(Vec3f dest, Vec3f a);
void *vec3f_dif(Vec3f dest, Vec3f a, Vec3f b);
void *vec3f_mul(Vec3f dest, f32 a);
void *vec3f_mult(Vec3f dest, Vec3f a, Vec3f b);
void *vec3f_mult_mtx(Vec3f dest, Vec3f a, Mat4 b);
void *vec3f_to_vec3s(Vec3s dest, Vec3f a);
void *vec3f_cross(Vec3f dest, Vec3f a, Vec3f b);
void *vec3f_normalize(Vec3f dest);
void  vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw);
void  vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32  dist, s16  pitch, s16  yaw);
void *vec3f_set_mag(Vec3f v, f32 mag);
void  vec3f_to_polar_coords(Vec3f v, f32 *dist, s16 *pitch, s16 *yaw);
void  vec3f_get_nullspace(Vec3f destAxisN, Vec3f destAxisE1, Vec3f destAxisE2, Vec3f n);
void *vec3f_project_point(Vec3f dest, Vec3f v, Vec3f o, Vec3f n);
void *vec3f_project_vector(Vec3f dest, Vec3f v, Vec3f n);
void  vec3f_to_2d_plane(Vec2f dest2d, f32 *dist2d, Vec3f src3d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2);
void *vec3f_rotate_zxy(Vec3f dest, Vec3f v, s16 pitch, s16 yaw, s16 roll);
void *vec3f_rotate_around_n(Vec3f dest, Vec3f v, Vec3f n, s16 r);
void *vec3f_transform(Vec3f dest, Vec3f v, Vec3f translation, Vec3s rotation, Vec3f scale);
void *vec3f_interpolate(Vec3f dest, Vec3f from, Vec3f to, f32 t);
void *vec3f_interpolate3(Vec3f dest, f32 t, Vec3f p0, f32 t0, Vec3f p1, f32 t1, Vec3f p2, f32 t2);
bool  vec3f_is_inside_cylinder(Vec3f v, Vec3f pos, f32 radius, f32 height, f32 downOffset);
void *find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c);

void *vec3s_set(Vec3s dest, s16 x, s16 y, s16 z);
void *vec3s_add(Vec3s dest, Vec3s a);
void *vec3s_sum(Vec3s dest, Vec3s a, Vec3s b);
void *vec3s_sub(Vec3s dest, Vec3s a);
void *vec3s_dif(Vec3s dest, Vec3s a, Vec3s b);
void *vec3s_to_vec3f(Vec3f dest, Vec3s a);
void *vec3s_interpolate(Vec3s dest, Vec3s from, Vec3s to, f32 t);
void *vec3s_interpolate_angles(Vec3s dest, Vec3s from, Vec3s to, f32 t);

void mtxf_translate(Mat4 dest, Vec3f b);
void mtxf_lookat(Mat4 mtx, Vec3f from, Vec3f to, s16 roll);
void mtxf_rotate_zxy_and_translate(Mat4 dest, Vec3f translation, Vec3s rotation);
void mtxf_rotate_xyz_and_translate(Mat4 dest, Vec3f translation, Vec3s rotation);
void mtxf_billboard(Mat4 dest, Mat4 mtx, Vec3f position, s16 angle);
void mtxf_cylboard(Mat4 dest, Mat4 mtx, Vec3f position, s16 angle);
void mtxf_align_terrain_normal(Mat4 dest, Vec3f upDir, Vec3f pos, s16 yaw);
void mtxf_align_terrain_triangle(Mat4 mtx, Vec3f pos, s16 yaw, f32 radius);
void mtxf_mul(Mat4 dest, Mat4 a, Mat4 b);
void mtxf_scale_vec3f(Mat4 dest, Mat4 mtx, Vec3f s);
void mtxf_interpolate(Mat4 dest, Mat4 from, Mat4 to, f32 t);
bool mtxf_invert(Mat4 dest, Mat4 m);
void mtxf_rotate_xy(Mtx *mtx, s16 angle);
void mtxf_ortho(Mtx *mtx, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale);
u16  mtxf_perspective(Mtx *mtx, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void get_pos_from_transform_mtx(Vec3f dest, Mat4 objMtx, Mat4 camMtx);

s16  atan2s(f32 y, f32 x);
s32  approach_s32(s32 current, s32 target, s32 inc, s32 dec);
f32  approach_f32(f32 current, f32 target, f32 inc, f32 dec);
void spline_get_weights(Vec4f result, f32 t, UNUSED s32 c);
void anim_spline_init(Vec4s *keyFrames);
s32  anim_spline_poll(Vec3f result);

//
// Frame interpolation
//

extern s32 gNumInterpolatedFrames;
#define MAX_INTERPOLATED_FRAMES 12

#define is_frame_interpolation_enabled()    (gNumInterpolatedFrames > 1)
#define get_subframe_t(k)                   (((f32) ((k) + 1)) / ((f32) gNumInterpolatedFrames))
#define interpolate                         for (s32 k = 0; k < gNumInterpolatedFrames; ++k)
#define interpolate1                        for (s32 k = 1; k < gNumInterpolatedFrames; ++k)

#define check_timestamp(ts)                 (*((u32 *) &(ts)) == (gGlobalTimer - 1))
#define reset_timestamp(ts)                 {*((u32 *) &(ts)) = 0;}
#define update_timestamp_s16(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (s16) (x);}
#define update_timestamp_s32(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (s32) (x);}
#define update_timestamp_u16(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (u16) (x);}
#define update_timestamp_u32(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (u32) (x);}
#define update_timestamp_f32(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (f32) (x);}
#define update_timestamp_vec3f(ts, x)       {*((u32 *) &(ts)) = gGlobalTimer; vec3f_copy((ts).v, x);}
#define update_timestamp_vec3s(ts, x)       {*((u32 *) &(ts)) = gGlobalTimer; vec3s_copy((ts).v, x);}
#define update_timestamp_mtxf(ts, x)        {*((u32 *) &(ts)) = gGlobalTimer; mtxf_copy((ts).v, x);}
#define update_timestamp_ptr(ts, x)         {*((u32 *) &(ts)) = gGlobalTimer; (ts).v = (void *) (x);}

#define interpolate_s16(dest, from, to)     { if (shouldInterpolate) { dest = (from) + (s16) (((to) - (from)) * get_subframe_t(k)); } else { dest = to; } }
#define interpolate_s32(dest, from, to)     { if (shouldInterpolate) { dest = (from) + (s32) (((to) - (from)) * get_subframe_t(k)); } else { dest = to; } }
#define interpolate_u16(dest, from, to)     { if (shouldInterpolate) { dest = (from) + (u16) (((to) - (from)) * get_subframe_t(k)); } else { dest = to; } }
#define interpolate_u32(dest, from, to)     { if (shouldInterpolate) { dest = (from) + (u32) (((to) - (from)) * get_subframe_t(k)); } else { dest = to; } }
#define interpolate_f32(dest, from, to)     { if (shouldInterpolate) { dest = (from) + (f32) (((to) - (from)) * get_subframe_t(k)); } else { dest = to; } }
#define interpolate_vec3f(dest, from, to)   { if (shouldInterpolate) { vec3f_interpolate(dest, from, to, get_subframe_t(k)); } else { vec3f_copy(dest, to); } }
#define interpolate_vec3s(dest, from, to)   { if (shouldInterpolate) { vec3s_interpolate(dest, from, to, get_subframe_t(k)); } else { vec3s_copy(dest, to); } }
#define interpolate_angles(dest, from, to)  { if (shouldInterpolate) { vec3s_interpolate_angles(dest, from, to, get_subframe_t(k)); } else { vec3s_copy(dest, to); } }
#define interpolate_mtxf(dest, from, to)    { if (shouldInterpolate) { mtxf_interpolate(dest, from, to, get_subframe_t(k)); } else { mtxf_copy(dest, to); } }

// backwards compatibility
#define interpolate_vectors(dest, from, to)     vec3f_interpolate(dest, from, to, 1.f)
#define interpolate_vectors_s16(dest, from, to) vec3s_interpolate(dest, from, to, 1.f)

#endif
