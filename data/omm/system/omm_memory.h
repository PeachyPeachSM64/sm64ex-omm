#ifndef OMM_MEMORY_H
#define OMM_MEMORY_H

#include "types.h"

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

typedef struct { s32 c; s32 s; __OmmNoP *p; } OmmArray;

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
#define omm_array_delete(arr)                       { OMM_MEMDEL(arr.p); OMM_MEMSET(&(arr), 0, sizeof(OmmArray)); }
#define omm_array_grow(arr, type, item, count)      { while(omm_array_count(arr) < (s32) (count)) { omm_array_add(arr, type, item); } }
#define omm_array_for_each(arr, item)               __OmmNoP *item = (arr).p; for (s32 i_##item = 0, c_##item = omm_array_count(arr); i_##item != c_##item; ++i_##item, item++)

//
// OmmMap: a growing array of key/value pairs with dynamic memory allocation.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// ALLOWED TYPES ARE ONLY NUMERIC AND POINTERS.
//

typedef struct { s32 c; s32 s; __OmmNoP *k; __OmmNoP *v; } OmmMap;

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
#define omm_map_delete(map)                         { OMM_MEMDEL(map.k); OMM_MEMDEL(map.v); OMM_MEMSET(&(map), 0, sizeof(OmmMap)); }
#define omm_map_for_each(map, key, val)             __OmmNoP *key = (map).k, *val = (map).v; for (s32 i_##key = 0, c_##key = omm_map_count(map); i_##key != c_##key; ++i_##key, key++, val++)

//
// OmmHMap: a sorted array of key/pointer pairs with dynamic memory allocation; its key is a u32 and its size is always a PoT.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// DO NOT USE THE KEY 0, AS IT IS USED AS AN EMPTY SLOT.
// ZERO CHECKS ARE OMITTED FOR FASTER PROCESSING.
//

typedef struct { s32 s; u32 *k; void **v; } OmmHMap;

s32  __omm_hmap_find  (OmmHMap *phmap, u32 key);
void __omm_hmap_insert(OmmHMap *phmap, u32 key, void *val);

#define omm_hmap_zero                               { 0, NULL, NULL }
#define omm_hmap_size(hmap)                         ((hmap).s)
#define omm_hmap_get(hmap, type, index)             ((type) (hmap).v[index])
#define omm_hmap_find(hmap, key)                    __omm_hmap_find(&(hmap), (u32) (key))
#define omm_hmap_insert(hmap, key, val)             __omm_hmap_insert(&(hmap), (u32) (key), (void *) (val))
#define omm_hmap_delete(hmap)                       { OMM_MEMDEL(hmap.k); OMM_MEMDEL(hmap.v); OMM_MEMSET(&(hmap), 0, sizeof(OmmHMap)); }
#define omm_hmap_for_each(hmap, key, val)           u32 *key = (hmap).k; void **val = (hmap).v; for (s32 i_##key = 0, s_##key = omm_hmap_size(hmap); i_##key != s_##key; ++i_##key, key++, val++)

//
// Memory pools
//

extern void *gOmmMemoryPoolStrings;
extern void *gOmmMemoryPoolGeoData;
void *omm_memory_new(void *pool, s32 size, void *caller);

#endif // OMM_MEMORY_H