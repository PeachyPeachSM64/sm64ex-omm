#ifndef OMM_MEMORY_H
#define OMM_MEMORY_H

#include "types.h"

//
// C types
//

#define array_of(type)                      (type *) (type[])
#define array_length(arr)                   (sizeof(arr) / sizeof(arr[0]))
#define array_for_each_(type, item, arr)    u32 i_##item = 0; for (type *item = arr; i_##item != array_length(arr); ++i_##item, item++)

//
// Memory
//

#define mem_new(typ, cnt)                   ((typ *) calloc(cnt, sizeof(typ)))
#define mem_del(dst)                        { if (dst) { free((void *) dst); dst = NULL; } }
#define mem_set(dst, val, siz)              memset(dst, val, siz)
#define mem_zero(dst, siz)                  memset(dst, 0, siz)
#define mem_cpy(dst, src, siz)              memcpy(dst, src, siz)
#define mem_mov(dst, src, siz)              memmove(dst, src, siz)
#define mem_dup(src, siz)                   memcpy(calloc(siz, sizeof(u8)), src, siz)
#define mem_eq(pt1, pt2, siz)               (memcmp(pt1, pt2, siz) == 0)
#define mem_swap(pt1, pt2, siz)             { unsigned char _temp_[siz]; memcpy(&_temp_, &(pt1), siz); memcpy(&(pt1), &(pt2), siz); memcpy(&(pt2), &_temp_, siz); }
#define mem_new1(typ, var, ...)             static typ var; if (!var) { var = __VA_ARGS__; }

//
// Strings
//

typedef char str_t[0x100];
typedef u8 ustr_t[0x100];

u32  __str_hash(const char *str);
char *__str_dup(const char *str);
void __str_cpy(char *dst, s32 dst_siz, const char *src);
void __str_rep(char *dst, s32 dst_siz, const char *src, char c0, char c1);
void __str_lwr(char *dst, s32 dst_siz, const char *src);
void __str_upr(char *dst, s32 dst_siz, const char *src);
void __str_cat(char *dst, s32 dst_siz, const char **src);

#define str_hash(str)                           __str_hash(str)
#define str_dup(str)                            __str_dup(str)
#define str_cpy(dst, dst_siz, src)              __str_cpy(dst, dst_siz, src)
#define str_rep(dst, dst_siz, src, c0, c1)      __str_rep(dst, dst_siz, src, c0, c1)
#define str_lwr(dst, dst_siz, src)              __str_lwr(dst, dst_siz, src)
#define str_upr(dst, dst_siz, src)              __str_upr(dst, dst_siz, src)
#define str_cat(dst, dst_siz, ...)              __str_cat(dst, dst_siz, array_of(const char *) { __VA_ARGS__, NULL })
#define str_fmt(dst, dst_siz, fmt, ...)         snprintf(dst, dst_siz, fmt, __VA_ARGS__)

// Numeric or pointer union
// Used as data type in OmmArray and OmmMap
typedef void *ptr;
typedef union {
    u32 as_u32;
    u64 as_u64;
    s32 as_s32;
    s64 as_s64;
    f32 as_f32;
    f64 as_f64;
    ptr as_ptr;
} __OmmNoP;
#define omm_as_nop(type, x) ((__OmmNoP) { .as_##type = (type) (x) })

//
// OmmArray: a growing array with dynamic memory allocation.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// ALLOWED TYPES ARE ONLY NUMERIC AND POINTERS.
//

typedef struct OmmArray { s32 c; s32 s; __OmmNoP *p; } OmmArray;
#define OmmArray_(T) OmmArray // Does nothing, only for readability

s32  __omm_array_find  (OmmArray *parr, __OmmNoP item);
void __omm_array_add   (OmmArray *parr, __OmmNoP item);
void __omm_array_remove(OmmArray *parr, s32 index);

#define omm_array_zero                              { 0, 0, NULL }
#define omm_array_count(arr)                        ((arr).c)
#define omm_array_get(arr, type, index)             ((arr).p[index].as_##type)
#define omm_array_set(arr, type, item, index)       { (arr).p[index] = omm_as_nop(type, item); }
#define omm_array_find(arr, type, item)             __omm_array_find(&(arr), omm_as_nop(type, item))
#define omm_array_add(arr, type, item)              __omm_array_add(&(arr), omm_as_nop(type, item))
#define omm_array_remove(arr, index)                __omm_array_remove(&(arr), index)
#define omm_array_clear(arr)                        __omm_array_remove(&(arr), -1)
#define omm_array_delete(arr)                       { mem_del((arr).p); mem_zero(&(arr), sizeof(OmmArray)); }
#define omm_array_grow(arr, type, item, count)      { while(omm_array_count(arr) < (s32) (count)) { omm_array_add(arr, type, item); } }
#define omm_array_for_each(arr, item)               __OmmNoP *item = (arr).p; for (s32 i_##item = 0, c_##item = omm_array_count(arr); i_##item != c_##item; ++i_##item, item++)

//
// OmmMap: a growing array of key/value pairs with dynamic memory allocation.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// ALLOWED TYPES ARE ONLY NUMERIC AND POINTERS.
//

typedef struct OmmMap { s32 c; s32 s; __OmmNoP *k; __OmmNoP *v; } OmmMap;
#define OmmMap_(K, V) OmmMap // Does nothing, only for readability

s32  __omm_map_find_key(OmmMap *pmap, __OmmNoP key);
s32  __omm_map_find_val(OmmMap *pmap, __OmmNoP val);
void __omm_map_add     (OmmMap *pmap, __OmmNoP key, __OmmNoP val);
void __omm_map_remove  (OmmMap *pmap, s32 index);

#define omm_map_zero                                { 0, 0, NULL, NULL }
#define omm_map_count(map)                          ((map).c)
#define omm_map_get_key(map, ktype, index)          ((map).k[index].as_##ktype)
#define omm_map_get_val(map, vtype, index)          ((map).v[index].as_##vtype)
#define omm_map_set_key(map, ktype, key, index)     { (map).k[index] = omm_as_nop(ktype, key); }
#define omm_map_set_val(map, vtype, val, index)     { (map).v[index] = omm_as_nop(vtype, val); }
#define omm_map_find_key(map, ktype, key)           __omm_map_find_key(&(map), omm_as_nop(ktype, key))
#define omm_map_find_val(map, vtype, val)           __omm_map_find_val(&(map), omm_as_nop(vtype, val))
#define omm_map_add(map, ktype, key, vtype, val)    __omm_map_add(&(map), omm_as_nop(ktype, key), omm_as_nop(vtype, val))
#define omm_map_remove(map, index)                  __omm_map_remove(&(map), index)
#define omm_map_clear(map)                          __omm_map_remove(&(map), -1)
#define omm_map_delete(map)                         { mem_del((map).k); mem_del((map).v); mem_zero(&(map), sizeof(OmmMap)); }
#define omm_map_for_each(map, key, val)             __OmmNoP *key = (map).k, *val = (map).v; for (s32 i_##key = 0, c_##key = omm_map_count(map); i_##key != c_##key; ++i_##key, key++, val++)

//
// OmmHMap: a sorted array of key/pointer pairs with dynamic memory allocation; its key is a u32 and its size is always a PoT.
// Keys are stored in descending order. Empty slots are always located at the end of the buffers.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// DO NOT USE THE KEY 0, AS IT IS USED AS AN EMPTY SLOT.
// ZERO CHECKS ARE OMITTED FOR FASTER PROCESSING.
//

typedef struct OmmHMap { s32 c; s32 s; u32 *k; void **v; } OmmHMap;
#define OmmHMap_(T) OmmHMap // Does nothing, only for readability

s32  __omm_hmap_find  (OmmHMap *phmap, u32 key);
void __omm_hmap_insert(OmmHMap *phmap, u32 key, void *val);

#define omm_hmap_zero                               { 0, 0, NULL, NULL }
#define omm_hmap_count(hmap)                        ((hmap).c)
#define omm_hmap_get(hmap, type, index)             ((type) (hmap).v[index])
#define omm_hmap_find(hmap, key)                    __omm_hmap_find(&(hmap), (u32) (key))
#define omm_hmap_insert(hmap, key, val)             __omm_hmap_insert(&(hmap), (u32) (key), (void *) (val))
#define omm_hmap_delete(hmap)                       { mem_del((hmap).k); mem_del((hmap).v); mem_zero(&(hmap), sizeof(OmmHMap)); }
#define omm_hmap_for_each(hmap, key, val)           u32 *key = (hmap).k; void **val = (hmap).v; for (s32 i_##key = 0, c_##key = omm_hmap_count(hmap); i_##key != c_##key; ++i_##key, key++, val++)

//
// Memory pools
//

extern void *gOmmMemoryPoolStrings;
extern void *gOmmMemoryPoolGeoData;
void *omm_memory_new(void *pool, s32 size, void *caller);

#if OMM_CODE_DEV
Vtx *__omm_alloc_vtx(const char *func, s32 count);
Gfx *__omm_alloc_gfx(const char *func, s32 count);
#define omm_alloc_vtx(count) __omm_alloc_vtx(__FUNCTION__, count)
#define omm_alloc_gfx(count) __omm_alloc_gfx(__FUNCTION__, count)
#else
Vtx *omm_alloc_vtx(s32 count);
Gfx *omm_alloc_gfx(s32 count);
#endif

#endif // OMM_MEMORY_H