#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <math.h>
#include "types.h"
#include "libc/math.h"

#define sins(x)     gSineTable[(u16) (x) >> 4]
#define coss(x)     (gSineTable + 0x400)[(u16) (x) >> 4]
#define min(a, b)   ((a) <= (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))
#define sqr(x)      ((x) * (x))
#define absx(x)     ((x) < 0 ? -(x) : (x))

enum AngleOrder { XYZ, YXZ, ZXY, ZYX, YZX, XZY };

extern f32 gSineTable[];
extern Vec2f gVec2fZero;
extern Vec2f gVec2fOne;
extern Vec3f gVec3fZero;
extern Vec3f gVec3fOne;
extern Vec3f gVec3fX;
extern Vec3f gVec3fY;
extern Vec3f gVec3fZ;
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
#define vec2f_eq(a, b)          (memcmp(a, b, sizeof(Vec2f)) == 0)

#define vec3f_zero(dest)        memset(dest, 0, sizeof(Vec3f))
#define vec3f_one(dest)         memcpy(dest, gVec3fOne, sizeof(Vec3f))
#define vec3f_copy(dest, src)   memcpy(dest, src, sizeof(Vec3f))
#define vec3f_eq(a, b)          (memcmp(a, b, sizeof(Vec3f)) == 0)

#define vec3s_zero(dest)        memset(dest, 0, sizeof(Vec3s))
#define vec3s_one(dest)         memcpy(dest, gVec3sOne, sizeof(Vec3s))
#define vec3s_copy(dest, src)   memcpy(dest, src, sizeof(Vec3s))
#define vec3s_eq(a, b)          (memcmp(a, b, sizeof(Vec3s)) == 0)

#define vec4f_zero(dest)        memset(dest, 0, sizeof(Vec4f))
#define vec4f_one(dest)         memcpy(dest, gVec4fOne, sizeof(Vec4f))
#define vec4f_copy(dest, src)   memcpy(dest, src, sizeof(Vec4f))
#define vec4f_eq(a, b)          (memcmp(a, b, sizeof(Vec4f)) == 0)

#define mtxf_zero(dest)         memset(dest, 0, sizeof(Mat4))
#define mtxf_identity(dest)     memcpy(dest, gMat4Identity, sizeof(Mat4))
#define mtxf_copy(dest, src)    memcpy(dest, src, sizeof(Mat4))
#define mtxf_to_mtx(dest, src)  memcpy(dest, src, sizeof(Mat4))

u16 random_u16(void);
f32 random_float(void);
f32 random_float_n1_p1(void);
s32 random_sign(void);

u16 srandom_u16(u16 seed);
f32 srandom_float(u16 seed);
f32 srandom_float_n1_p1(u16 seed);
s32 srandom_sign(u16 seed);

f32   vec2f_length(Vec2f v);
f32   vec2f_length2(Vec2f v);
f32   vec2f_dot(Vec2f a, Vec2f b);
f32   vec2f_dotperp(Vec2f a, Vec2f b);
f32   vec2f_dist(Vec2f a, Vec2f b);
void *vec2f_set(Vec2f dest, f32 x, f32 y);
void *vec2f_add(Vec2f dest, Vec2f a);
void *vec2f_sum(Vec2f dest, Vec2f a, Vec2f b);
void *vec2f_sub(Vec2f dest, Vec2f a);
void *vec2f_dif(Vec2f dest, Vec2f a, Vec2f b);
void *vec2f_mul(Vec2f dest, f32 a);
void *vec2f_mult(Vec2f dest, Vec2f a, Vec2f b);
void *vec2f_perp(Vec2f dest, Vec2f v);
void *vec2f_normalize(Vec2f dest);
void *vec2f_set_mag(Vec2f v, f32 mag);
void *vec2f_and_dist_to_3d(Vec3f dest3d, Vec2f src2d, f32 dist2d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2);
void *vec2f_to_3d_plane(Vec3f dest, Vec2f src, Vec3f o, Vec3f e1, Vec3f e1Scale, Vec3f e2, Vec3f e2Scale);
void *vec2f_get_projected_point_on_line(Vec2f dest, f32 *t, Vec2f p, Vec2f a, Vec2f b);
bool  vec2f_get_intersect_point(Vec2f dest, Vec2f p0, Vec2f v0, Vec2f p1, Vec2f v1);

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
void *vec3f_get_normal(Vec3f dest, Vec3f a, Vec3f b, Vec3f c);
void  vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw);
void  vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32  dist, s16  pitch, s16  yaw);
void *vec3f_set_mag(Vec3f v, f32 mag);
void  vec3f_to_polar_coords(Vec3f v, f32 *dist, s16 *pitch, s16 *yaw);
void  vec3f_get_nullspace(Vec3f destAxisN, Vec3f destAxisE1, Vec3f destAxisE2, Vec3f n);
void *vec3f_project_point(Vec3f dest, Vec3f p, Vec3f o, Vec3f n);
void *vec3f_project_point_dir(Vec3f dest, Vec3f p, Vec3f d, Vec3f o, Vec3f n);
void *vec3f_project_vector(Vec3f dest, Vec3f v, Vec3f n);
void *vec3f_get_projected_point_on_line(Vec3f dest, f32 *t, Vec3f p, Vec3f a, Vec3f b);
void  vec3f_to_2d_plane(Vec2f dest2d, f32 *dist2d, Vec3f src3d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2);
void *vec3f_rotate_zxy(Vec3f dest, Vec3f v, s16 pitch, s16 yaw, s16 roll);
void *vec3f_rotate_around_n(Vec3f dest, Vec3f v, Vec3f n, s16 r);
void *vec3f_transform(Vec3f dest, Vec3f v, Vec3f translation, Vec3s rotation, Vec3f scale);
void *vec3f_interpolate(Vec3f dest, Vec3f from, Vec3f to, f32 t);
void *vec3f_interpolate3(Vec3f dest, f32 t, Vec3f p0, f32 t0, Vec3f p1, f32 t1, Vec3f p2, f32 t2);
bool  vec3f_is_inside_cylinder(Vec3f v, Vec3f pos, f32 radius, f32 height, f32 downOffset);
bool  vec3f_is_inside_box(Vec3f v, Vec3f boxMin, Vec3f boxMax);
bool  vec3f_check_cylinder_overlap(Vec3f pos1, f32 radius1, f32 height1, f32 offset1, Vec3f pos2, f32 radius2, f32 height2, f32 offset2);
void *vec3f_get_barycentric_coords(Vec3f coords, Vec3f p, Vec3f a, Vec3f b, Vec3f c);
void *vec3f_from_barycentric_coords(Vec3f dest, Vec3f coords, Vec3f a, Vec3f b, Vec3f c);
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
bool mtxf_invert(Mat4 dest, Mat4 m);
void mtxf_rotate_xy(Mtx *mtx, s16 angle);
void mtxf_ortho(Mtx *mtx, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale);
u16  mtxf_perspective(Mtx *mtx, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void get_pos_from_transform_mtx(Vec3f dest, Mat4 objMtx, Mat4 camMtx);
void mtxf_get_rotation(Mat4 m, Vec3s rotation, enum AngleOrder order);
void mtxf_get_components(Mat4 m, Vec3f translation, Vec3s rotation, Vec3f shear, Vec3f scale);
void mtxf_transform(Mat4 dest, Vec3f translation, Vec3s rotation, Vec3f shear, Vec3f scale);
void mtxf_interpolate(Mat4 dest, Mat4 from, Mat4 to, f32 t);
void mtxf_interpolate_fast(Mat4 dest, Mat4 from, Mat4 to, f32 t);
void vtxv_interpolate(Vtx_t *dest, Vtx_t *from, Vtx_t *to, f32 t);
void vtxn_interpolate(Vtx_tn *dest, Vtx_tn *from, Vtx_tn *to, f32 t);

s16  atan2s(f32 y, f32 x);
s32  approach_s32(s32 current, s32 target, s32 inc, s32 dec);
f32  approach_f32(f32 current, f32 target, f32 inc, f32 dec);
void spline_get_weights(Vec4f result, f32 t, UNUSED s32 c);
void anim_spline_init(Vec4s *keyFrames);
s32  anim_spline_poll(Vec3f result);

Gfx *gfx_create_identity_matrix(Gfx *gfx);
Gfx *gfx_create_translation_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 x, f32 y, f32 z);
Gfx *gfx_create_rotation_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 a, f32 x, f32 y, f32 z);
Gfx *gfx_create_scale_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 x, f32 y, f32 z);
Gfx *gfx_create_ortho_matrix(Gfx *gfx);
Gfx *gfx_create_billboard(Gfx *gfx, Vtx *vtx, f32 left, f32 top, f32 width, f32 height, s32 texX, s32 texY, s32 texW, s32 texH, Vec3f billboardPos, Vec3f objPos, f32 cameraOffset, s16 roll, u8 r, u8 g, u8 b, u8 a);

Gfx *gfx_font_init(Gfx *gfx, const char *font, s32 cols, s32 texw, s32 texh, s32 glyphw, s32 glyphh);
Gfx *gfx_font_render_char(Gfx *gfx, char c, f32 x, f32 y, f32 w, f32 h, s32 du, s32 dv, u8 r, u8 g, u8 b, u8 a);
Gfx *gfx_font_end(Gfx *gfx);

//
// Frame interpolation
//

extern bool gFrameInterpolation;

typedef struct {
    Gfx *pos;
    Gfx *gfx;
    Vtx *vtx;
    f32 x, x0, x1;
    f32 y, y0, y1;
    f32 z, z0, z1;
    f32 a, a0, a1;
    f32 s, s0, s1;
    f32 t, t0, t1;
    bool inited;
} InterpData;

void interp_data_update(InterpData *data, bool shouldInterp, Gfx *pos, Gfx *gfx, Vtx *vtx, f32 x, f32 y, f32 z, f32 a, f32 s, f32 t);
void interp_data_lerp(InterpData *data, f32 t);
void interp_data_reset(InterpData *data);

#endif
