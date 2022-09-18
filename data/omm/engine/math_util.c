#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "trig_tables.inc.c"

//
// Data
//

u16 gRandomSeed = 0;
Vec2f gVec2fZero = { 0, 0 };
Vec2f gVec2fOne = { 1, 1 };
Vec3f gVec3fZero = { 0, 0, 0 };
Vec3f gVec3fOne = { 1, 1, 1 };
Vec3f gVec3fX = { 1, 0, 0 };
Vec3f gVec3fY = { 0, 1, 0 };
Vec3f gVec3fZ = { 0, 0, 1 };
Vec3s gVec3sZero = { 0, 0, 0 };
Vec3s gVec3sOne = { 1, 1, 1 };
Vec4f gVec4fZero = { 0, 0, 0, 0 };
Vec4f gVec4fOne = { 1, 1, 1, 1 };
Vec4s gVec4sZero = { 0, 0, 0, 0 };
Vec4s gVec4sOne = { 1, 1, 1, 1 };
Mat4 gMat4Identity = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };

//
// Random
//

u16 random_u16(void) {
    u16 gRandomNext;
    gRandomNext = ((gRandomSeed & 0x00FF) << 8) ^ gRandomSeed;
    gRandomSeed = ((gRandomNext & 0x00FF) << 8) + ((gRandomNext & 0xFF00) >> 8);
    gRandomNext = ((gRandomNext & 0x00FF) << 1) ^ gRandomSeed;
    gRandomSeed = ((gRandomNext >> 1) ^ 0xFF80) ^ ((gRandomNext & 1) ? 0x8180 : 0x1FF4);
    gRandomSeed = ((gRandomSeed != 0x560A) * gRandomSeed);
    return gRandomSeed;
}

f32 random_float(void) {
    return random_u16() / 65536.f;
}

s32 random_sign(void) {
    return (random_u16() & 1) ? +1 : -1;
}

//
// Vec2f
//

void *vec2f_and_dist_to_3d(Vec3f dest3d, Vec2f src2d, f32 dist2d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2) {
    dest3d[0] = o[0] + src2d[0] * e1[0] + src2d[1] * e2[0] + dist2d * n[0];
    dest3d[1] = o[1] + src2d[0] * e1[1] + src2d[1] * e2[1] + dist2d * n[1];
    dest3d[2] = o[2] + src2d[0] * e1[2] + src2d[1] * e2[2] + dist2d * n[2];
    return dest3d;
}

void *vec2f_to_3d_plane(Vec3f dest, Vec2f src, Vec3f o, Vec3f e1, Vec3f e1Scale, Vec3f e2, Vec3f e2Scale) {
    f32 x = src[0];
    f32 y = src[1];
    dest[0] = o[0] + x * e1[0] * e1Scale[0] + y * e2[0] * e2Scale[0];
    dest[1] = o[1] + x * e1[1] * e1Scale[1] + y * e2[1] * e2Scale[1];
    dest[2] = o[2] + x * e1[2] * e1Scale[2] + y * e2[2] * e2Scale[2];
    return dest;
}

void *vec2f_get_projected_point_on_line(Vec2f dest, f32 *t, Vec2f p, Vec2f a, Vec2f b) {
    Vec2f ab = { b[0] - a[0], b[1] - a[1] };
    Vec2f ap = { p[0] - a[0], p[1] - a[1] };
    f32 dota = (ab[0] * ap[0]) + (ab[1] * ap[1]);
    f32 iab2 = 1.f / ((ab[0] * ab[0]) + (ab[1] * ab[1]));
    f32 abt = dota * iab2;
    dest[0] = a[0] + ab[0] * abt;
    dest[1] = a[1] + ab[1] * abt;
    if (t) *t = abt;
    return dest;
}

//
// Vec3f
//

f32 vec3f_length(Vec3f v) {
	return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

f32 vec3f_dot(Vec3f a, Vec3f b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

f32 vec3f_dist(Vec3f a, Vec3f b) {
    return sqrtf(sqr_f(b[0] - a[0]) + sqr_f(b[1] - a[1]) + sqr_f(b[2] - a[2]));
}

f32 vec3f_hdist(Vec3f a, Vec3f b) {
    return sqrtf(sqr_f(b[0] - a[0]) + sqr_f(b[2] - a[2]));
}

void *vec3f_set(Vec3f dest, f32 x, f32 y, f32 z) {
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return dest;
}

void *vec3f_add(Vec3f dest, Vec3f a) {
    dest[0] += a[0];
    dest[1] += a[1];
    dest[2] += a[2];
    return dest;
}

void *vec3f_sum(Vec3f dest, Vec3f a, Vec3f b) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    return dest;
}

#if OMM_GAME_IS_XALO
void vec3f_sub(Vec3f dest, Vec3f a) {
    dest[0] -= a[0];
    dest[1] -= a[1];
    dest[2] -= a[2];
}
#endif

void *vec3f_dif(Vec3f dest, Vec3f a, Vec3f b) {
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
    return dest;
}

void *vec3f_mul(Vec3f dest, f32 a) {
    dest[0] *= a;
    dest[1] *= a;
    dest[2] *= a;
    return dest;
}

void *vec3f_mult(Vec3f dest, Vec3f a, Vec3f b) {
    dest[0] = a[0] * b[0];
    dest[1] = a[1] * b[1];
    dest[2] = a[2] * b[2];
    return dest;
}

void *vec3f_mult_mtx(Vec3f dest, Vec3f a, Mat4 b) {
    dest[0] = vec3f_dot(a, b[0]);
    dest[1] = vec3f_dot(a, b[1]);
    dest[2] = vec3f_dot(a, b[2]);
    return dest;
}

void *vec3f_to_vec3s(Vec3s dest, Vec3f a) {
    s16 x = roundf(((f32 *) a)[0]);
    s16 y = roundf(((f32 *) a)[1]);
    s16 z = roundf(((f32 *) a)[2]);
    ((s16 *) dest)[0] = x;
    ((s16 *) dest)[1] = y;
    ((s16 *) dest)[2] = z;
    return dest;
}

void *vec3f_cross(Vec3f dest, Vec3f a, Vec3f b) {
    dest[0] = a[1] * b[2] - b[1] * a[2];
    dest[1] = a[2] * b[0] - b[2] * a[0];
    dest[2] = a[0] * b[1] - b[0] * a[1];
    return dest;
}

void *vec3f_normalize(Vec3f dest) {
    f32 mag2 = dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2];
    if (OMM_LIKELY(mag2 != 0.f)) {
        f32 invsqrt = 1.f / sqrtf(mag2);
        vec3f_mul(dest, invsqrt);
    }
    return dest;
}

void vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw) {
    f32 x = to[0] - from[0];
    f32 y = to[1] - from[1];
    f32 z = to[2] - from[2];
    *dist = sqrtf(x * x + y * y + z * z);
    *pitch = atan2s(sqrtf(x * x + z * z), y);
    *yaw = atan2s(z, x);
}

void vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32 dist, s16 pitch, s16 yaw) {
    f32 dcos = dist * coss(pitch);
    to[0] = from[0] + dcos * sins(yaw);
    to[1] = from[1] + dist * sins(pitch);
    to[2] = from[2] + dcos * coss(yaw);
}

void *vec3f_set_mag(Vec3f v, f32 mag) {
    vec3f_normalize(v);
    vec3f_mul(v, mag);
    return v;
}

void vec3f_to_polar_coords(Vec3f v, f32 *dist, s16 *pitch, s16 *yaw) {
    if (v[0] == 0.f && v[1] == 0.f && v[2] == 0.f) {
        if (dist) { *dist = 0.f; }
        if (pitch) { *pitch = 0; }
        if (yaw) { *yaw = 0; }
    } else {
        if (dist) { *dist = sqrtf(sqr_f(v[0]) + sqr_f(v[1]) + sqr_f(v[2])); }
        if (pitch) { *pitch = atan2s(sqrtf(sqr_f(v[0]) + sqr_f(v[2])), v[1]); }
        if (yaw) { *yaw = atan2s(v[2], v[0]); }
    }
}

void vec3f_get_nullspace(Vec3f destAxisN, Vec3f destAxisE1, Vec3f destAxisE2, Vec3f n) {
    vec3f_set(destAxisN, n[0], n[1], n[2]);
    if (destAxisN[0] == 0.f && destAxisN[1] == 0.f) {
        vec3f_set(destAxisE1, 1.f, 0.f, 0.f);    
        vec3f_set(destAxisE2, 0.f, 1.f, 0.f);
    } else {
        vec3f_set(destAxisE1, -destAxisN[1], destAxisN[0], 0.f);
        vec3f_set(destAxisE2, -destAxisN[0] * destAxisN[2], -destAxisN[1] * destAxisN[2], sqr_f(destAxisN[0]) + sqr_f(destAxisN[1]));
    }
    vec3f_normalize(destAxisN);
    vec3f_normalize(destAxisE1);
    vec3f_normalize(destAxisE2);
}

void *vec3f_project_point(Vec3f dest, Vec3f v, Vec3f o, Vec3f n) {
    f32 dot = (v[0] - o[0]) * n[0] + (v[1] - o[1]) * n[1] + (v[2] - o[2]) * n[2];
    dest[0] = v[0] - dot * n[0];
    dest[1] = v[1] - dot * n[1];
    dest[2] = v[2] - dot * n[2];
    return dest;
}

void *vec3f_project_vector(Vec3f dest, Vec3f v, Vec3f n) {
    f32 dot = vec3f_dot(v, n);
    dest[0] = v[0] - dot * n[0];
    dest[1] = v[1] - dot * n[1];
    dest[2] = v[2] - dot * n[2];
    return dest;
}

void *vec3f_get_projected_point_on_line(Vec3f dest, f32 *t, Vec3f p, Vec3f a, Vec3f b) {
    Vec3f ab = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
    Vec3f ap = { p[0] - a[0], p[1] - a[1], p[2] - a[2] };
    f32 dota = (ab[0] * ap[0]) + (ab[1] * ap[1]) + (ab[2] * ap[2]);
    f32 iab2 = 1.f / ((ab[0] * ab[0]) + (ab[1] * ab[1]) + (ab[2] * ab[2]));
    f32 abt = dota * iab2;
    dest[0] = a[0] + ab[0] * abt;
    dest[1] = a[1] + ab[1] * abt;
    dest[2] = a[2] + ab[2] * abt;
    if (t) *t = abt;
    return dest;
}

void vec3f_to_2d_plane(Vec2f dest2d, f32 *dist2d, Vec3f src3d, Vec3f o, Vec3f n, Vec3f e1, Vec3f e2) {
    Vec3f op = { src3d[0] - o[0], src3d[1] - o[1], src3d[2] - o[2] };
    dest2d[0] = vec3f_dot(e1, op);
    dest2d[1] = vec3f_dot(e2, op);
    *dist2d = vec3f_dot(n, op);
}

void *vec3f_rotate_zxy(Vec3f dest, Vec3f v, s16 pitch, s16 yaw, s16 roll) {
    Vec3s r = { pitch, yaw, roll };
    Mat4 mtx;
    mtxf_rotate_zxy_and_translate(mtx, gVec3fZero, r);
    f32 x = v[0];
    f32 y = v[1];
    f32 z = v[2];
    dest[0] = x * mtx[0][0] + y * mtx[1][0] + z * mtx[2][0] + mtx[3][0];
    dest[1] = x * mtx[0][1] + y * mtx[1][1] + z * mtx[2][1] + mtx[3][1];
    dest[2] = x * mtx[0][2] + y * mtx[1][2] + z * mtx[2][2] + mtx[3][2];
    return dest;
}

// Rodrigues' formula
// dest = v * cos(r) + (n x v) * sin(r) + n * (n . v) * (1 - cos(r))
void *vec3f_rotate_around_n(Vec3f dest, Vec3f v, Vec3f n, s16 r) {
    Vec3f nvCross;
    vec3f_cross(nvCross, n, v);
    f32 nvDot = vec3f_dot(n, v);
    f32 cosr = coss(r);
    f32 sinr = sins(r);
    dest[0] = v[0] * cosr + nvCross[0] * sinr + n[0] * nvDot * (1.f - cosr);
    dest[1] = v[1] * cosr + nvCross[1] * sinr + n[1] * nvDot * (1.f - cosr);
    dest[2] = v[2] * cosr + nvCross[2] * sinr + n[2] * nvDot * (1.f - cosr);
    return dest;
}

// Scale -> Rotate (ZXY) -> Translate
void *vec3f_transform(Vec3f dest, Vec3f v, Vec3f translation, Vec3s rotation, Vec3f scale) {
    vec3f_copy(dest, v);
    vec3f_mult(dest, dest, scale);
    vec3f_rotate_zxy(dest, dest, rotation[0], rotation[1], rotation[2]);
    vec3f_add(dest, translation);
    return dest;
}

void *vec3f_interpolate(Vec3f dest, Vec3f from, Vec3f to, f32 t) {
    dest[0] = lerp_f(t, from[0], to[0]);
    dest[1] = lerp_f(t, from[1], to[1]);
    dest[2] = lerp_f(t, from[2], to[2]);
}

// Lagrange's polynoms
//                t - t1      t - t2           t - t0      t - t2           t - t0      t - t1
// L3(t) = p0 * --------- * --------- + p1 * --------- * --------- + p2 * --------- * ---------
//               t0 - t1     t0 - t2          t1 - t0     t1 - t2          t2 - t0     t2 - t1
void *vec3f_interpolate3(Vec3f dest, f32 t, Vec3f p0, f32 t0, Vec3f p1, f32 t1, Vec3f p2, f32 t2) {
    f32 tt0 = t  - t0;
    f32 tt1 = t  - t1;
    f32 tt2 = t  - t2;
    f32 t10 = t1 - t0;
    f32 t20 = t2 - t0;
    f32 t21 = t2 - t1;
    f32 m0 = (tt1 / -t10) * (tt2 / -t20);
    f32 m1 = (tt0 /  t10) * (tt2 / -t21);
    f32 m2 = (tt0 /  t20) * (tt1 /  t21);
    dest[0] = p0[0] * m0 + p1[0] * m1 + p2[0] * m2;
    dest[1] = p0[1] * m0 + p1[1] * m1 + p2[1] * m2;
    dest[2] = p0[2] * m0 + p1[2] * m1 + p2[2] * m2;
    return dest;
}

bool vec3f_is_inside_cylinder(Vec3f v, Vec3f pos, f32 radius, f32 height, f32 downOffset) {
    f32 vDist = v[1] - pos[1] + downOffset;
    if (vDist < 0.f || vDist >= height) return false;
    f32 hDist = vec3f_hdist(v, pos);
    return hDist < radius;
}

void *vec3f_get_barycentric_coords(Vec3f coords, Vec3f p, Vec3f a, Vec3f b, Vec3f c) {
    Vec3f v0; vec3f_dif(v0, b, a);
    Vec3f v1; vec3f_dif(v1, c, a);
    Vec3f v2; vec3f_dif(v2, p, a);
    f32 d00 = vec3f_dot(v0, v0);
    f32 d01 = vec3f_dot(v0, v1);
    f32 d11 = vec3f_dot(v1, v1);
    f32 d20 = vec3f_dot(v2, v0);
    f32 d21 = vec3f_dot(v2, v1);
    f32 mul = 1.f / (d00 * d11 - d01 * d01);
    coords[1] = (d11 * d20 - d01 * d21) * mul;
    coords[2] = (d00 * d21 - d01 * d20) * mul;
    coords[0] = 1.f - coords[1] - coords[2];
}

void *vec3f_from_barycentric_coords(Vec3f dest, Vec3f coords, Vec3f a, Vec3f b, Vec3f c) {
    dest[0] = coords[0] * a[0] + coords[1] * b[0] + coords[2] * c[0];
    dest[1] = coords[0] * a[1] + coords[1] * b[1] + coords[2] * c[1];
    dest[2] = coords[0] * a[2] + coords[1] * b[2] + coords[2] * c[2];
}

void *find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c) {
    f32 ba0 = b[0] - a[0];
    f32 ba1 = b[1] - a[1];
    f32 ba2 = b[2] - a[2];
    f32 cb0 = c[0] - b[0];
    f32 cb1 = c[1] - b[1];
    f32 cb2 = c[2] - b[2];
    dest[0] = ba1 * cb2 - cb1 * ba2;
    dest[1] = ba2 * cb0 - cb2 * ba0;
    dest[2] = ba0 * cb1 - cb0 * ba1;
    return dest;
}

//
// Vec3s
//

void *vec3s_set(Vec3s dest, s16 x, s16 y, s16 z) {
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return dest;
}

void *vec3s_add(Vec3s dest, Vec3s a) {
    dest[0] += a[0];
    dest[1] += a[1];
    dest[2] += a[2];
    return dest;
}

void *vec3s_sum(Vec3s dest, Vec3s a, Vec3s b) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    return dest;
}

void *vec3s_sub(Vec3s dest, Vec3s a) {
    dest[0] -= a[0];
    dest[1] -= a[1];
    dest[2] -= a[2];
    return dest;
}

void *vec3s_dif(Vec3s dest, Vec3s a, Vec3s b) {
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
    return dest;
}

void *vec3s_to_vec3f(Vec3f dest, Vec3s a) {
    f32 x = a[0];
    f32 y = a[1];
    f32 z = a[2];
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return dest;
}

void *vec3s_interpolate(Vec3s dest, Vec3s from, Vec3s to, f32 t) {
    dest[0] = lerp_s(t, from[0], to[0]);
    dest[1] = lerp_s(t, from[1], to[1]);
    dest[2] = lerp_s(t, from[2], to[2]);
}

static s16 interpolate_angle_s16(s16 from, s16 to, f32 t) {
    f32 c = lerp_f(t, coss(from), coss(to));
    f32 s = lerp_f(t, sins(from), sins(to));
    return atan2s(c, s);
}

void *vec3s_interpolate_angles(Vec3s dest, Vec3s from, Vec3s to, f32 t) {
    dest[0] = interpolate_angle_s16(from[0], to[0], t);
    dest[1] = interpolate_angle_s16(from[1], to[1], t);
    dest[2] = interpolate_angle_s16(from[2], to[2], t);
}

//
// Mat4/Mtx
//

#define IS_ZERO(x)      ((x) > -0.0001f && (x) < +0.0001f)
#define MTXF_ONE(m, k)  ((u32 *) (m))[k] = 0x3F800000
#define MTXF_33_1(m)    MTXF_ONE(m, 15)

void mtxf_translate(Mat4 dest, Vec3f b) {
    mtxf_identity(dest);
    vec3f_copy(dest[3], b);
}

void mtxf_lookat(Mat4 mtx, Vec3f from, Vec3f to, s16 roll) {
    f32 dx = to[0] - from[0];
    f32 dz = to[2] - from[2];
    f32 invLength = -1.f / sqrtf(dx * dx + dz * dz);
    dx *= invLength;
    dz *= invLength;
    Vec3f colX, colY, colZ;
    f32 sr  = sins(roll);
    colY[1] = coss(roll);
    colY[0] = dz * sr;
    colY[2] = dx * -sr;
    vec3f_dif(colZ, from, to);
    vec3f_normalize(colZ);
    vec3f_cross(colX, colY, colZ);
    vec3f_normalize(colX);
    vec3f_cross(colY, colZ, colX);
    vec3f_normalize(colY);
    mtx[0][0] = colX[0];
    mtx[0][1] = colY[0];
    mtx[0][2] = colZ[0];
    mtx[0][3] = 0;
    mtx[1][0] = colX[1];
    mtx[1][1] = colY[1];
    mtx[1][2] = colZ[1];
    mtx[1][3] = 0;
    mtx[2][0] = colX[2];
    mtx[2][1] = colY[2];
    mtx[2][2] = colZ[2];
    mtx[2][3] = 0;
    mtx[3][0] = -vec3f_dot(from, colX);
    mtx[3][1] = -vec3f_dot(from, colY);
    mtx[3][2] = -vec3f_dot(from, colZ);
    MTXF_33_1(mtx);
}

void mtxf_rotate_zxy_and_translate(Mat4 dest, Vec3f translation, Vec3s rotation) {
    f32 sx     = sins(rotation[0]);
    f32 cx     = coss(rotation[0]);
    f32 sy     = sins(rotation[1]);
    f32 cy     = coss(rotation[1]);
    f32 sz     = sins(rotation[2]);
    f32 cz     = coss(rotation[2]);
    f32 sysz   = (sy * sz);
    f32 cycz   = (cy * cz);
    f32 cysz   = (cy * sz);
    f32 sycz   = (sy * cz);
    dest[0][0] = ((sysz * sx) + cycz);
    dest[0][1] = (cx * sz);
    dest[0][2] = ((cysz * sx) - sycz);
    dest[0][3] = 0;
    dest[1][0] = ((sycz * sx) - cysz);
    dest[1][1] = (cx * cz);
    dest[1][2] = ((cycz * sx) + sysz);
    dest[1][3] = 0;
    dest[2][0] = (cx * sy);
    dest[2][1] = -sx;
    dest[2][2] = (cx * cy);
    dest[2][3] = 0;
    vec3f_copy(dest[3], translation);
    MTXF_33_1(dest);
}

void mtxf_rotate_xyz_and_translate(Mat4 dest, Vec3f translation, Vec3s rotation) {
    f32 sx     = sins(rotation[0]);
    f32 cx     = coss(rotation[0]);
    f32 sy     = sins(rotation[1]);
    f32 cy     = coss(rotation[1]);
    f32 sz     = sins(rotation[2]);
    f32 cz     = coss(rotation[2]);
    f32 sxcz   = (sx * cz);
    f32 cxsz   = (cx * sz);
    f32 sxsz   = (sx * sz);
    f32 cxcz   = (cx * cz);
    dest[0][0] = (cy * cz);
    dest[0][1] = (cy * sz);
    dest[0][2] = -sy;
    dest[0][3] = 0;
    dest[1][0] = ((sxcz * sy) - cxsz);
    dest[1][1] = ((sxsz * sy) + cxcz);
    dest[1][2] = (sx * cy);
    dest[1][3] = 0;
    dest[2][0] = ((cxcz * sy) + sxsz);
    dest[2][1] = ((cxsz * sy) - sxcz);
    dest[2][2] = (cx * cy);
    dest[2][3] = 0;
    vec3f_copy(dest[3], translation);
    MTXF_33_1(dest);
}

void mtxf_billboard(Mat4 dest, Mat4 mtx, Vec3f position, s16 angle) {
    mtxf_identity(dest);
    dest[0][0] = coss(angle);
    dest[0][1] = sins(angle);
    dest[1][0] = -dest[0][1];
    dest[1][1] = dest[0][0];
    __m128 p0 = _mm_set1_ps(position[0]);
    __m128 p1 = _mm_set1_ps(position[1]);
    __m128 p2 = _mm_set1_ps(position[2]);
    __m128 p3 = _mm_set1_ps(1.f);
    __m128 m0 = _mm_loadu_ps(mtx[0]);
    __m128 m1 = _mm_loadu_ps(mtx[1]);
    __m128 m2 = _mm_loadu_ps(mtx[2]);
    __m128 m3 = _mm_loadu_ps(mtx[3]);
    __m128 d3 = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(m0, p0), _mm_mul_ps(m1, p1)),
        _mm_add_ps(_mm_mul_ps(m2, p2), _mm_mul_ps(m3, p3))
    );
    _mm_storeu_ps(dest[3], d3);
    MTXF_33_1(dest);
}

void mtxf_cylboard(Mat4 dest, Mat4 mtx, Vec3f position, s16 angle) {
    mtxf_identity(dest);
    dest[0][0] = coss(angle);
    dest[0][1] = sins(angle);
    vec3f_copy(dest[1], mtx[1]);
    __m128 p0 = _mm_set1_ps(position[0]);
    __m128 p1 = _mm_set1_ps(position[1]);
    __m128 p2 = _mm_set1_ps(position[2]);
    __m128 p3 = _mm_set1_ps(1.f);
    __m128 m0 = _mm_loadu_ps(mtx[0]);
    __m128 m1 = _mm_loadu_ps(mtx[1]);
    __m128 m2 = _mm_loadu_ps(mtx[2]);
    __m128 m3 = _mm_loadu_ps(mtx[3]);
    __m128 d3 = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(m0, p0), _mm_mul_ps(m1, p1)),
        _mm_add_ps(_mm_mul_ps(m2, p2), _mm_mul_ps(m3, p3))
    );
    _mm_storeu_ps(dest[3], d3);
    MTXF_33_1(dest);
}

void mtxf_align_terrain_normal(Mat4 dest, Vec3f upDir, Vec3f pos, s16 yaw) {
    Vec3f lateralDir;
    Vec3f leftDir;
    Vec3f forwardDir;
    vec3f_set(lateralDir, sins(yaw), 0.f, coss(yaw));
    vec3f_normalize(upDir);
    vec3f_cross(leftDir, upDir, lateralDir);
    vec3f_normalize(leftDir);
    vec3f_cross(forwardDir, leftDir, upDir);
    vec3f_normalize(forwardDir);
    vec3f_copy(dest[0], leftDir);
    vec3f_copy(dest[1], upDir);
    vec3f_copy(dest[2], forwardDir);
    vec3f_copy(dest[3], pos);
    dest[0][3] = 0;
    dest[1][3] = 0;
    dest[2][3] = 0;
    MTXF_33_1(dest);
}

void mtxf_align_terrain_triangle(Mat4 mtx, Vec3f pos, s16 yaw, f32 radius) {
    Vec3f point0, point1, point2, forward, xColumn, yColumn, zColumn;
    f32 minY = -radius * 3;
    f32 height = pos[1] + 150;
    point0[0] = pos[0] + radius * sins(yaw + 0x2AAA);
    point0[2] = pos[2] + radius * coss(yaw + 0x2AAA);
    point0[1] = find_floor_height(point0[0], height, point0[2]);
    point1[0] = pos[0] + radius * sins(yaw + 0x8000);
    point1[2] = pos[2] + radius * coss(yaw + 0x8000);
    point1[1] = find_floor_height(point1[0], height, point1[2]);
    point2[0] = pos[0] + radius * sins(yaw + 0xD555);
    point2[2] = pos[2] + radius * coss(yaw + 0xD555);
    point2[1] = find_floor_height(point2[0], height, point2[2]);
    if ((point0[1] - pos[1]) < minY) point0[1] = pos[1];
    if ((point1[1] - pos[1]) < minY) point1[1] = pos[1];
    if ((point2[1] - pos[1]) < minY) point2[1] = pos[1];
    vec3f_set(forward, sins(yaw), 0.f, coss(yaw));
    find_vector_perpendicular_to_plane(yColumn, point0, point1, point2);
    vec3f_normalize(yColumn);
    vec3f_cross(xColumn, yColumn, forward);
    vec3f_normalize(xColumn);
    vec3f_cross(zColumn, xColumn, yColumn);
    vec3f_normalize(zColumn);
    vec3f_copy(mtx[0], xColumn);
    vec3f_copy(mtx[1], yColumn);
    vec3f_copy(mtx[2], zColumn);
    vec3f_copy(mtx[3], pos);
    mtx[0][3] = 0;
    mtx[1][3] = 0;
    mtx[2][3] = 0;
    MTXF_33_1(mtx);
}

void mtxf_mul(Mat4 dest, Mat4 a, Mat4 b) {
    Mat4 c;
    __m128 b0 = _mm_loadu_ps(b[0]);
    __m128 b1 = _mm_loadu_ps(b[1]);
    __m128 b2 = _mm_loadu_ps(b[2]);
    __m128 b3 = _mm_loadu_ps(b[3]);
    for (s32 i = 0; i != 4; ++i) {
        __m128 a0 = _mm_set1_ps(a[i][0]);
        __m128 a1 = _mm_set1_ps(a[i][1]);
        __m128 a2 = _mm_set1_ps(a[i][2]);
        __m128 a3 = _mm_set1_ps(a[i][3]);
        __m128 ci = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1)),
            _mm_add_ps(_mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3))
        );
        _mm_storeu_ps(c[i], ci);
    }
    mtxf_copy(dest, c);
}

void mtxf_scale_vec3f(Mat4 dest, Mat4 mtx, Vec3f s) {
    mtxf_copy(dest, mtx);
    vec3f_mul(dest[0], s[0]);
    vec3f_mul(dest[1], s[1]);
    vec3f_mul(dest[2], s[2]);
}

// This assumes the last row is always [0, 0, 0, 1]
bool mtxf_invert(Mat4 dest, Mat4 m) {
    f32 det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) 
            - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) 
            + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
    if (det != 0.f) {
        det = 1.f / det;
        dest[0][0] = det * (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
        dest[0][1] = det * (m[2][1] * m[0][2] - m[0][1] * m[2][2]);
        dest[0][2] = det * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
        dest[0][3] = 0;
        dest[1][0] = det * (m[2][0] * m[1][2] - m[1][0] * m[2][2]);
        dest[1][1] = det * (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
        dest[1][2] = det * (m[1][0] * m[0][2] - m[0][0] * m[1][2]);
        dest[1][3] = 0;
        dest[2][0] = det * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
        dest[2][1] = det * (m[2][0] * m[0][1] - m[0][0] * m[2][1]);
        dest[2][2] = det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
        dest[2][3] = 0;
        dest[3][0] = -m[3][0] * dest[0][0] - m[3][1] * dest[1][0] - m[3][2] * dest[2][0];
        dest[3][1] = -m[3][0] * dest[0][1] - m[3][1] * dest[1][1] - m[3][2] * dest[2][1];
        dest[3][2] = -m[3][0] * dest[0][2] - m[3][1] * dest[1][2] - m[3][2] * dest[2][2];
        MTXF_33_1(dest);
        return true;
    }
    return false;
}

void mtxf_rotate_xy(Mtx *mtx, s16 angle) {
    mtxf_zero(mtx);
    mtx->m[0][0] = coss(angle);
    mtx->m[0][1] = sins(angle);
    mtx->m[1][0] = -mtx->m[0][1];
    mtx->m[1][1] = +mtx->m[0][0];
    MTXF_ONE(mtx->m, 10);
    MTXF_ONE(mtx->m, 15);
}

void mtxf_ortho(Mtx *mtx, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale) {
    mtxf_zero(mtx);
    f32 srl = scale / (right - left);
    f32 stb = scale / (top - bottom);
    f32 sfn = scale / (far - near);
    mtx->m[0][0] = srl * +2.f;
    mtx->m[1][1] = stb * +2.f;
    mtx->m[2][2] = sfn * -2.f;
    mtx->m[3][0] = srl * -(right + left);
    mtx->m[3][1] = stb * -(top + bottom);
    mtx->m[3][2] = sfn * -(far + near);
    mtx->m[3][3] = scale;
}

u16 mtxf_perspective(Mtx *mtx, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    mtxf_zero(mtx);
    s16 fov16 = (s16) ((0x8000 * fovy) / 360.f);
    f32 ssy = scale * (coss(fov16) / sins(fov16));
    f32 snf = scale / (near - far);
    mtx->m[0][0] = ssy / aspect;
    mtx->m[1][1] = ssy;
    mtx->m[2][2] = snf * (near + far);
    mtx->m[2][3] = -scale;
    mtx->m[3][2] = snf * near * far * 2.f;
    mtx->m[3][3] = 0;
    return (u16) ((f32) 0x20000 / clamp_f(near + far, 2, 0x20000));
}

void get_pos_from_transform_mtx(Vec3f dest, Mat4 objMtx, Mat4 camMtx) {
    Vec3f v;
    vec3f_dif(v, objMtx[3], camMtx[3]);
    dest[0] = vec3f_dot(v, camMtx[0]);
    dest[1] = vec3f_dot(v, camMtx[1]);
    dest[2] = vec3f_dot(v, camMtx[2]);
}

#define ANGLES(o, a0, a0x, a0m, a1, a1x, a1mY, a1mX, a2, a2x, a2mY, a2mX, a1e, a1ex, a1emY, a1emX, a2e) \
if (order == o) { \
    f32 ax, ay, az; \
    a0 = asinf(a0x * clamp_f(a0m, -1, 1)); \
    if (abs_f(a0m) < 0.999999f) { \
        a1 = atan2f(a1x * a1mY, a1mX); \
        a2 = atan2f(a2x * a2mY, a2mX); \
    } else { \
        a1e = atan2f(a1ex * a1emY, a1emX); \
        a2e = 0; \
    } \
    vec3s_set(rotation, ax * RAD_TO_S16, ay * RAD_TO_S16, az * RAD_TO_S16); \
    return; \
}

// Credits to https://github.com/mrdoob/three.js/blob/dev/src/math/Euler.js#L105
// This code assumes that 'm' is a pure rotation matrix (i.e. no scaling, no shearing)
void mtxf_get_rotation(Mat4 m, Vec3s rotation, enum AngleOrder order) {
    static const f32 RAD_TO_S16 = 0x8000 / M_PI;
    f32 m00 = m[0][0]; f32 m10 = m[1][0]; f32 m20 = m[2][0];
    f32 m01 = m[0][1]; f32 m11 = m[1][1]; f32 m21 = m[2][1];
    f32 m02 = m[0][2]; f32 m12 = m[1][2]; f32 m22 = m[2][2];
    ANGLES(XYZ, ay, +1, m20, ax, -1, m21, m22, az, -1, m10, m00, ax, +1, m12, m11, az);
    ANGLES(YXZ, ax, -1, m21, ay, +1, m20, m22, az, +1, m01, m11, ay, -1, m02, m00, az);
    ANGLES(ZXY, ax, +1, m12, ay, -1, m02, m22, az, -1, m10, m11, az, +1, m01, m00, ay);
    ANGLES(ZYX, ay, -1, m02, ax, +1, m12, m22, az, +1, m01, m00, az, -1, m10, m11, ax);
    ANGLES(YZX, az, +1, m01, ax, -1, m21, m11, ay, -1, m02, m00, ay, +1, m20, m22, ax);
    ANGLES(XZY, az, -1, m10, ax, +1, m12, m11, ay, +1, m20, m00, ax, -1, m21, m22, ay);
}

#define vec3f_ortho(v1, v2, shear)          { v1[0] -= v2[0] * (shear); v1[1] -= v2[1] * (shear); v1[2] -= v2[2] * (shear); }
#define vec3f_unscale(v, scale, default)    if (IS_ZERO(scale)) { vec3f_copy(v, default); } else { vec3f_mul(v, 1.f / (scale)); }
#define shear_unscale(shear, scale)         if (IS_ZERO(scale)) { shear = 0; } else { shear /= (scale); }

// Credits to https://webdocs.cs.ualberta.ca/~graphics/books/GraphicsGems/gemsii/unmatrix.c
void mtxf_get_components(Mat4 m, Vec3f translation, Vec3s rotation, Vec3f shear, Vec3f scale) {

    // Translation
    vec3f_copy(translation, m[3]);

    // Axes
    Vec3f xAxis, yAxis, zAxis;
    vec3f_copy(xAxis, m[0]);
    vec3f_copy(yAxis, m[1]);
    vec3f_copy(zAxis, m[2]);

    // X scale
    scale[0] = vec3f_length(xAxis);
    vec3f_unscale(xAxis, scale[0], gVec3fX);
    
    // XY shear
    shear[0] = vec3f_dot(xAxis, yAxis);
    vec3f_ortho(yAxis, xAxis, shear[0]);
    
    // Y scale
    scale[1] = vec3f_length(yAxis);
    vec3f_unscale(yAxis, scale[1], gVec3fY);
    shear_unscale(shear[0], scale[1]);

    // XZ shear
    shear[1] = vec3f_dot(xAxis, zAxis);
    vec3f_ortho(zAxis, xAxis, shear[1]);

    // YZ shear
    shear[2] = vec3f_dot(yAxis, zAxis);
    vec3f_ortho(zAxis, yAxis, shear[2]);

    // Z scale
    scale[2] = vec3f_length(zAxis);
    vec3f_unscale(zAxis, scale[2], gVec3fZ);
    shear_unscale(shear[1], scale[2]);
    shear_unscale(shear[2], scale[2]);

    // (xAxis, yAxis, zAxis) is now an orthonormal coordinate system.
    // Check for a system flip, and if that's the case, negate axes and scaling.
    Vec3f yzCross;
    if (vec3f_dot(xAxis, vec3f_cross(yzCross, yAxis, zAxis)) < 0.f) {
        vec3f_mul(scale, -1);
        vec3f_mul(xAxis, -1);
        vec3f_mul(yAxis, -1);
        vec3f_mul(zAxis, -1);
    }

    // Rotation
    Mat4 mRot;
    mtxf_identity(mRot);
    vec3f_copy(mRot[0], xAxis);
    vec3f_copy(mRot[1], yAxis);
    vec3f_copy(mRot[2], zAxis);
    mtxf_get_rotation(mRot, rotation, YXZ);
}

void mtxf_transform(Mat4 dest, Vec3f translation, Vec3s rotation, Vec3f shear, Vec3f scale) {
    mtxf_zero(dest);
    MTXF_33_1(dest);

    // Scale
    dest[0][0] = scale[0];
    dest[1][1] = scale[1];
    dest[2][2] = scale[2];

    // Shear
    dest[1][0] = dest[1][1] * shear[0];
    dest[2][0] = dest[2][2] * shear[1];
    dest[2][1] = dest[2][2] * shear[2];

    // Rotate
    Mat4 mRot;
    mtxf_rotate_zxy_and_translate(mRot, gVec3fZero, rotation);
    mtxf_mul(dest, dest, mRot);

    // Translate
    vec3f_copy(dest[3], translation);
}

void mtxf_interpolate(Mat4 dest, Mat4 from, Mat4 to, f32 t) {
    Vec3f translation0, translation1, translationT;
    Vec3s rotation0, rotation1, rotationT;
    Vec3f shear0, shear1, shearT;
    Vec3f scale0, scale1, scaleT;

    // Extract matrix components
    mtxf_get_components(from, translation0, rotation0, shear0, scale0);
    mtxf_get_components(to, translation1, rotation1, shear1, scale1);

    // Interpolate components
    vec3f_interpolate(translationT, translation0, translation1, t);
    vec3s_interpolate_angles(rotationT, rotation0, rotation1, t);
    vec3f_interpolate(shearT, shear0, shear1, t);
    vec3f_interpolate(scaleT, scale0, scale1, t);

    // Build interpolated matrix
    mtxf_transform(dest, translationT, rotationT, shearT, scaleT);
}

void mtxf_interpolate_fast(Mat4 dest, Mat4 from, Mat4 to, f32 t) {
    // Note: This is NOT the proper way to interpolate a
    // transform matrix, but for the sake of simplicity
    // and computation time, it's good enough.
    // f32 *a = (f32 *) from;
    // f32 *b = (f32 *) to;
    // f32 *c = (f32 *) dest;
    // for (s32 i = 0; i != 16; ++i, a++, b++, c++) {
    //     *c = lerp_f(t, *a, *b);
    // }
    __m128 t4 = _mm_set1_ps(t);
    for (s32 i = 0; i != 4; ++i) {
        __m128 a = _mm_loadu_ps(from[i]);
        __m128 b = _mm_loadu_ps(to[i]);
        __m128 c = _mm_add_ps(a, _mm_mul_ps(_mm_sub_ps(b, a), t4));
        _mm_storeu_ps(dest[i], c);
    }
}

void vtxv_interpolate(Vtx_t *dest, Vtx_t *from, Vtx_t *to, f32 t) {
    vec3f_interpolate(dest->ob, from->ob, to->ob, t);
    dest->tc[0] = (s16) lerp_f(t, from->tc[0], to->tc[0]);
    dest->tc[1] = (s16) lerp_f(t, from->tc[1], to->tc[1]);
    dest->cn[0] = (u8)  lerp_f(t, from->cn[0], to->cn[0]);
    dest->cn[1] = (u8)  lerp_f(t, from->cn[1], to->cn[1]);
    dest->cn[2] = (u8)  lerp_f(t, from->cn[2], to->cn[2]);
    dest->cn[3] = (u8)  lerp_f(t, from->cn[3], to->cn[3]);
}

void vtxn_interpolate(Vtx_tn *dest, Vtx_tn *from, Vtx_tn *to, f32 t) {
    vec3f_interpolate(dest->ob, from->ob, to->ob, t);
    dest->tc[0] = (s16) lerp_f(t, from->tc[0], to->tc[0]);
    dest->tc[1] = (s16) lerp_f(t, from->tc[1], to->tc[1]);
    dest->n[0]  = (s8)  lerp_f(t, from->n[0], to->n[0]);
    dest->n[1]  = (s8)  lerp_f(t, from->n[1], to->n[1]);
    dest->n[2]  = (s8)  lerp_f(t, from->n[2], to->n[2]);
    dest->a     = (u8)  lerp_f(t, from->a, to->a);
}

//
// Other stuff (declared in math_util.h)
//

static u16 atan2_lookup(f32 y, f32 x) {
    if (OMM_UNLIKELY(x == 0)) {
        return gArctanTable[0];
    }
    return gArctanTable[(s32) ((y / x) * 1024 + 0.5f)];
}

s16 atan2s(f32 y, f32 x) {
    if (x >= 0) {
        if (y >= 0) {
            if (y >= x) {
                return atan2_lookup(x, y);
            }
            return 0x4000 - atan2_lookup(y, x);
        }
        y = -y;
        if (y < x) {
            return 0x4000 + atan2_lookup(y, x);
        }
        return 0x8000 - atan2_lookup(x, y);
    }
    x = -x;
    if (y < 0) {
        y = -y;
        if (y >= x) {
            return 0x8000 + atan2_lookup(x, y);
        }
        return 0xC000 - atan2_lookup(y, x);
    }
    if (y < x) {
        return 0xC000 + atan2_lookup(y, x);
    }
    return -atan2_lookup(x, y);
}

s32 approach_s32(s32 current, s32 target, s32 inc, s32 dec) {
    if (current < target) {
        return min_s(current + inc, target);
    }
    return max_s(current - dec, target);
}

f32 approach_f32(f32 current, f32 target, f32 inc, f32 dec) {
    if (current < target) {
        return min_f(current + inc, target);
    }
    return max_f(current - dec, target);
}

Vec4s *gSplineKeyframe;
float gSplineKeyframeFraction;
int gSplineState;

#define CURVE_BEGIN_1 1
#define CURVE_BEGIN_2 2
#define CURVE_MIDDLE 3
#define CURVE_END_1 4
#define CURVE_END_2 5

/**
 * Set 'result' to a 4-vector with weights corresponding to interpolation
 * value t in [0, 1] and gSplineState. Given the current control point P, these
 * weights are for P[0], P[1], P[2] and P[3] to obtain an interpolated point.
 * The weights naturally sum to 1, and they are also always in range [0, 1] so
 * the interpolated point will never overshoot. The curve is guaranteed to go
 * through the first and last point, but not through intermediate points.
 *
 * gSplineState ensures that the curve is clamped: the first two points
 * and last two points have different weight formulas. These are the weights
 * just before gSplineState transitions:
 * 1: [1, 0, 0, 0]
 * 1->2: [0, 3/12, 7/12, 2/12]
 * 2->3: [0, 1/6, 4/6, 1/6]
 * 3->3: [0, 1/6, 4/6, 1/6] (repeats)
 * 3->4: [0, 1/6, 4/6, 1/6]
 * 4->5: [0, 2/12, 7/12, 3/12]
 * 5: [0, 0, 0, 1]
 *
 * I suspect that the weight formulas will give a 3rd degree B-spline with the
 * common uniform clamped knot vector, e.g. for n points:
 * [0, 0, 0, 0, 1, 2, ... n-1, n, n, n, n]
 * TODO: verify the classification of the spline / figure out how polynomials were computed
 */
void spline_get_weights(Vec4f result, f32 t, UNUSED s32 c) {
    f32 tinv = 1 - t;
    f32 tinv2 = tinv * tinv;
    f32 tinv3 = tinv2 * tinv;
    f32 t2 = t * t;
    f32 t3 = t2 * t;
    switch (gSplineState) {
        case CURVE_BEGIN_1:
            result[0] = tinv3;
            result[1] = t3 * 1.75f - t2 * 4.5f + t * 3.f;
            result[2] = -t3 * (11 / 12.f) + t2 * 1.5f;
            result[3] = t3 * (1 / 6.f);
            break;
        case CURVE_BEGIN_2:
            result[0] = tinv3 * 0.25f;
            result[1] = t3 * (7 / 12.f) - t2 * 1.25f + t * 0.25f + (7 / 12.f);
            result[2] = -t3 * 0.5f + t2 * 0.5f + t * 0.5f + (1 / 6.f);
            result[3] = t3 * (1 / 6.f);
            break;
        case CURVE_MIDDLE:
            result[0] = tinv3 * (1 / 6.f);
            result[1] = t3 * 0.5f - t2 + (4 / 6.f);
            result[2] = -t3 * 0.5f + t2 * 0.5f + t * 0.5f + (1 / 6.f);
            result[3] = t3 * (1 / 6.f);
            break;
        case CURVE_END_1:
            result[0] = tinv3 * (1 / 6.f);
            result[1] = -tinv3 * 0.5f + tinv2 * 0.5f + tinv * 0.5f + (1 / 6.f);
            result[2] = tinv3 * (7 / 12.f) - tinv2 * 1.25f + tinv * 0.25f + (7 / 12.f);
            result[3] = t3 * 0.25f;
            break;
        case CURVE_END_2:
            result[0] = tinv3 * (1 / 6.f);
            result[1] = -tinv3 * (11 / 12.f) + tinv2 * 1.5f;
            result[2] = tinv3 * 1.75f - tinv2 * 4.5f + tinv * 3.f;
            result[3] = t3;
            break;
    }
}

void anim_spline_init(Vec4s *keyFrames) {
    gSplineKeyframe = keyFrames;
    gSplineKeyframeFraction = 0;
    gSplineState = 1;
}

s32 anim_spline_poll(Vec3f result) {
    Vec4f weights;
    s32 i;
    s32 hasEnded = FALSE;
    vec3f_copy(result, gVec3fZero);
    spline_get_weights(weights, gSplineKeyframeFraction, gSplineState);
    for (i = 0; i < 4; i++) {
        result[0] += weights[i] * gSplineKeyframe[i][1];
        result[1] += weights[i] * gSplineKeyframe[i][2];
        result[2] += weights[i] * gSplineKeyframe[i][3];
    }
    if ((gSplineKeyframeFraction += gSplineKeyframe[0][0] / 1000.f) >= 1) {
        gSplineKeyframe++;
        gSplineKeyframeFraction--;
        switch (gSplineState) {
            case CURVE_END_2:
                hasEnded = TRUE;
                break;
            case CURVE_MIDDLE:
                if (gSplineKeyframe[2][0] == 0) {
                    gSplineState = CURVE_END_1;
                }
                break;
            default:
                gSplineState++;
                break;
        }
    }
    return hasEnded;
}

//
// Gfx stuff
//

Gfx *gfx_create_identity_matrix(Gfx *gfx) {
    static const Mtx sIdentityMatrix = { {
        { 1.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    } };
    gSPMatrix(gfx++, &sIdentityMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &sIdentityMatrix, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    return gfx;
}

Gfx *gfx_create_translation_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 x, f32 y, f32 z) {
    guTranslate(mtx, x, y, z);
    gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_MUL | (push ? G_MTX_PUSH : G_MTX_NOPUSH));
    return gfx;
}

Gfx *gfx_create_rotation_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 a, f32 x, f32 y, f32 z) {
    guRotate(mtx, a, x, y, z);
    gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_MUL | (push ? G_MTX_PUSH : G_MTX_NOPUSH));
    return gfx;
}

Gfx *gfx_create_scale_matrix(Gfx *gfx, Mtx *mtx, bool push, f32 x, f32 y, f32 z) {
    guScale(mtx, x, y, z);
    gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_MUL | (push ? G_MTX_PUSH : G_MTX_NOPUSH));
    return gfx;
}

Gfx *gfx_create_ortho_matrix(Gfx *gfx) {
    static const Mtx sOrthoMatrix = { {
        { 2.f / SCREEN_WIDTH, 0.f, 0.f, 0.f },
        { 0.f, 2.f / SCREEN_HEIGHT, 0.f, 0.f },
        { 0.f, 0.f, -0.1f, 0.f },
        { -1.f, -1.f, 0.f, 1.f },
    } };
    gSPPerspNormalize(gfx++, 0xFFFF);
    gfx = gfx_create_identity_matrix(gfx);
    gSPMatrix(gfx++, &sOrthoMatrix, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    return gfx;
}

//
// Frame interpolation
//

void interp_data_update(InterpData *data, bool shouldInterp, Gfx *pos, f32 x, f32 y, f32 z, f32 a, f32 s, f32 t) {

    // Previous frame
    if (gFrameInterpolation && shouldInterp && data->inited) {
        data->pos = pos;
        data->x0 = data->x1;
        data->y0 = data->y1;
        data->z0 = data->z1;
        data->a0 = data->a1;
        data->s0 = data->s1;
        data->t0 = data->t1;
    } else {
        data->pos = NULL;
        data->x0 = x;
        data->y0 = y;
        data->z0 = z;
        data->a0 = a;
        data->s0 = s;
        data->t0 = t;
    }

    // Current frame
    data->x1 = x;
    data->y1 = y;
    data->z1 = z;
    data->a1 = a;
    data->s1 = s;
    data->t1 = t;
    data->inited = true;
}

void interp_data_lerp(InterpData *data, f32 t) {
    data->x = lerp_f(t, data->x0, data->x1);
    data->y = lerp_f(t, data->y0, data->y1);
    data->z = lerp_f(t, data->z0, data->z1);
    data->a = lerp_f(t, data->a0, data->a1);
    data->s = lerp_f(t, data->s0, data->s1);
    data->t = lerp_f(t, data->t0, data->t1);
}

// Legacy
void interpolate_vectors(Vec3f res, UNUSED Vec3f a, Vec3f b) { vec3f_copy(res, b); }
void interpolate_vectors_s16(Vec3s res, UNUSED Vec3s a, Vec3s b) { vec3s_copy(res, b); }
