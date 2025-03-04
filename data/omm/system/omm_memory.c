#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define DOUBLE_SIZE(s) max_s(2, s << 1)

OMM_INLINE bool __omm_nop_eq(__OmmNoP x, __OmmNoP y) {
    return x.as_u64 == y.as_u64;
}

OMM_INLINE void *__omm_realloc(void *p, s32 s, s32 c, s32 sot) {
    void *q = calloc(s, sot);
    if (OMM_LIKELY(p)) {
        mem_cpy(q, p, c * sot);
        mem_del(p);
    }
    return q;
}

//
// Strings
//

u32 __str_hash(const char *str) {
    u32 hash = 0;
    if (str) {
        for (; *str; ++str) {
            hash = (hash * 31) + *str;
        }
    }
    return hash;
}

char *__str_dup(const char *str) {
    if (str) {
        u32 len = (u32) strlen(str);
        char *dup = mem_new(char, len + 1);
        if (dup) {
            mem_cpy(dup, str, len);
            return dup;
        }
    }
    return NULL;
}

void __str_cpy(char *dst, s32 dst_siz, const char *src) {
    if (src && dst && dst_siz) {
        mem_cpy(dst, src, min_s(strlen(src) + 1, dst_siz - 1));
        dst[dst_siz - 1] = 0;
    }
}

void __str_rep(char *dst, s32 dst_siz, const char *src, char c0, char c1) {
    if (src && dst && dst_siz) {
        for (; *src && --dst_siz; ++src, ++dst) {
            *dst = (*src == c0 ? c1 : *src);
        }
        *dst = 0;
    }
}

void __str_lwr(char *dst, s32 dst_siz, const char *src) {
    if (src && dst && dst_siz) {
        for (; *src && --dst_siz; ++src, ++dst) {
            *dst = tolower(*src);
        }
        *dst = 0;
    }
}

void __str_upr(char *dst, s32 dst_siz, const char *src) {
    if (src && dst && dst_siz) {
        for (; *src && --dst_siz; ++src, ++dst) {
            *dst = toupper(*src);
        }
        *dst = 0;
    }
}

void __str_cat(char *dst, s32 dst_siz, const char **src) {
    if (src && dst && dst_siz) {
        for (; *src && dst_siz; ++src) {
            __str_cpy(dst, dst_siz, *src);
            s32 src_len = (s32) strlen(*src);
            dst += src_len;
            dst_siz = max_s(0, dst_siz - src_len);
        }
    }
}

//
// OmmArray: a growing array with dynamic memory allocation.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// ALLOWED TYPES ARE ONLY NUMERIC AND POINTERS.
//

s32 __omm_array_find(OmmArray *parr, __OmmNoP item) {
    for (s32 i = 0; i != parr->c; ++i) {
        if (__omm_nop_eq(parr->p[i], item)) {
            return i;
        }
    }
    return -1;
}

void __omm_array_add(OmmArray *parr, __OmmNoP item) {
    if (parr->c == parr->s) {
        parr->s = DOUBLE_SIZE(parr->s);
        parr->p = __omm_realloc(parr->p, parr->s, parr->c, sizeof(__OmmNoP));
    }
    parr->p[parr->c] = item;
    parr->c++;
}

void __omm_array_remove(OmmArray *parr, s32 index) {
    if (index == -1) {
        parr->c = 0;
    } else if (index < parr->c) {
        mem_mov(parr->p + index, parr->p + index + 1, sizeof(__OmmNoP) * (parr->c - index - 1));
        parr->c--;
    }
}

//
// OmmMap: a growing array of key/value pairs with dynamic memory allocation.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// ALLOWED TYPES ARE ONLY NUMERIC AND POINTERS.
//

s32 __omm_map_find_key(OmmMap *pmap, __OmmNoP key) {
    for (s32 i = 0; i != pmap->c; ++i) {
        if (__omm_nop_eq(pmap->k[i], key)) {
            return i;
        }
    }
    return -1;
}

s32 __omm_map_find_val(OmmMap *pmap, __OmmNoP val) {
    for (s32 i = 0; i != pmap->c; ++i) {
        if (__omm_nop_eq(pmap->v[i], val)) {
            return i;
        }
    }
    return -1;
}

void __omm_map_add(OmmMap *pmap, __OmmNoP key, __OmmNoP val) {
    if (pmap->c == pmap->s) {
        pmap->s = DOUBLE_SIZE(pmap->s);
        pmap->k = __omm_realloc(pmap->k, pmap->s, pmap->c, sizeof(__OmmNoP));
        pmap->v = __omm_realloc(pmap->v, pmap->s, pmap->c, sizeof(__OmmNoP));
    }
    pmap->k[pmap->c] = key;
    pmap->v[pmap->c] = val;
    pmap->c++;
}

void __omm_map_remove(OmmMap *pmap, s32 index) {
    if (index == -1) {
        pmap->c = 0;
    } else if (index < pmap->c) {
        mem_mov(pmap->k + index, pmap->k + index + 1, sizeof(__OmmNoP) * (pmap->c - index - 1));
        mem_mov(pmap->v + index, pmap->v + index + 1, sizeof(__OmmNoP) * (pmap->c - index - 1));
        pmap->c--;
    }
}

//
// OmmHMap: a sorted array of key/pointer pairs with dynamic memory allocation; its key is a u32 and its size is always a PoT.
// Keys are stored in descending order. Empty slots are always located at the end of the buffers.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// DO NOT USE THE KEY 0, AS IT IS USED AS AN EMPTY SLOT.
// ZERO CHECKS ARE OMITTED FOR FASTER PROCESSING.
//

s32 __omm_hmap_find(OmmHMap *phmap, u32 key) {
    if (OMM_LIKELY(phmap->s)) {
        u32 i = 0, c = phmap->s;
        const u32 *k = phmap->k;
        while (c >>= 1) {
            i += (k[i + c] >= key) * c;
        }
        if (k[i] == key) {
            return i;
        }
    }
    return -1;
}

void __omm_hmap_insert(OmmHMap *phmap, u32 key, void *val) {
    if (!phmap->k || phmap->k[phmap->s - 1]) {
        phmap->s = DOUBLE_SIZE(phmap->s);
        phmap->k = __omm_realloc(phmap->k, phmap->s, phmap->s >> 1, sizeof(u32));
        phmap->v = __omm_realloc(phmap->v, phmap->s, phmap->s >> 1, sizeof(void *));
    }
    for (s32 i = 0; i != phmap->s; ++i) {
        if (key > phmap->k[i]) {
            mem_mov(phmap->k + i + 1, phmap->k + i, sizeof(u32)    * (phmap->s - i - 1));
            mem_mov(phmap->v + i + 1, phmap->v + i, sizeof(void *) * (phmap->s - i - 1));
            phmap->k[i] = key;
            phmap->v[i] = val;
            phmap->c++;
            return;
        }
    }
}

//
// Memory pools
// Circular buffers allocated on heap once and for all
// Objects can be resized if not enough space
//

typedef struct {
    void *owner;
    void *data;
    s32 size;
} OmmMemoryObject;

typedef struct {
    OmmMemoryObject *objects;
    s32 capacity;
    s32 current;
} OmmMemoryPool;

void *gOmmMemoryPoolStrings = NULL;
void *gOmmMemoryPoolGeoData = NULL;

static void omm_memory_pool_grow(OmmMemoryPool *omp, s32 capacity) {
    OmmMemoryObject *objects = mem_new(OmmMemoryObject, capacity);
    if (omp->objects) {
        mem_cpy(objects, omp->objects, sizeof(OmmMemoryObject) * omp->capacity);
        mem_del(omp->objects);
    }
    omp->objects = objects;
    omp->capacity = capacity;
}

static OmmMemoryPool *omm_memory_pool_create(s32 capacity) {
    OmmMemoryPool *pool = mem_new(OmmMemoryPool, 1);
    omm_memory_pool_grow(pool, capacity);
    pool->current = 0;
    return pool;
}

static OmmMemoryObject *omm_memory_pool_get_free_slot(OmmMemoryPool *omp) {
    for (s32 i = 0; i != omp->capacity; ++i) {
        OmmMemoryObject *obj = omp->objects + omp->current;
        omp->current = (omp->current + 1) % (omp->capacity);
        if (!obj->owner) {
            return obj;
        }
    }

    // No free slot found, increase pool capacity
    omm_memory_pool_grow(omp, omp->capacity * 2);
    omp->current = omp->capacity / 2;
    return omp->objects + omp->current;
}

OMM_AT_STARTUP static void omm_memory_init_pools() {
    gOmmMemoryPoolStrings = omm_memory_pool_create(32);
    gOmmMemoryPoolGeoData = omm_memory_pool_create(64);
}

void *omm_memory_new(void *pool, s32 size, void *caller) {
    void *p = NULL;
    if (pool) {
        OmmMemoryPool *omp = (OmmMemoryPool *) pool;
        OmmMemoryObject *obj = omm_memory_pool_get_free_slot(omp);
        if (obj->size < size) {
            mem_del(obj->data);
            obj->data = mem_new(u8, size);
            obj->size = size;
        } else {
            mem_zero(obj->data, size);
        }
        obj->owner = caller;
        p = obj->data;
    } else {
        p = mem_new(u8, size);
    }
    return p;
}

OMM_ROUTINE_UPDATE(omm_memory_pool_geo_data_gc) {
    OmmMemoryPool *omp = gOmmMemoryPoolGeoData;
    for (s32 i = 0; i != omp->capacity; ++i) {
        OmmMemoryObject *obj = omp->objects + i;
        struct Object *o = obj->owner;
        if (o && !o->activeFlags) {
            obj->owner = NULL;
        }
    }
}

// // DEBUG DISPLAY FOR COUNTING THE USED SLOTS OF GEO DATA POOL
// OMM_ROUTINE_PRE_RENDER(omm_memory_pool_geo_data_debug_display) {
//     s32 usedCount = 0;
//     OmmMemoryPool *omp = gOmmMemoryPoolGeoData;
//     for (s32 i = 0; i != omp->capacity; ++i) {
//         OmmMemoryObject *obj = omp->objects + i;
//         usedCount += (obj->owner != NULL);
//     }
//     omm_debug_text(-50, 24, "%d", omp->current);
//     omm_debug_text(-50, 4, "%d I %d", usedCount, omp->capacity);
// }

//
// Pools for VTX and GFX
//

#if !OMM_CODE_DEV

#define OMM_VTX_POOL_SIZE (0x10000)
#define OMM_GFX_POOL_SIZE (0x10000)

Vtx *omm_alloc_vtx(s32 count) {
    mem_new1(Vtx *, sVtxPool, mem_new(Vtx, OMM_VTX_POOL_SIZE));
    mem_new1(Vtx *, sVtxHead, sVtxPool);
    if (sVtxHead + count > sVtxPool + OMM_VTX_POOL_SIZE) {
        sVtxHead = sVtxPool;
    }
    Vtx *vtx = sVtxHead;
    sVtxHead += count;
    return vtx;
}

Gfx *omm_alloc_gfx(s32 count) {
    mem_new1(Gfx *, sGfxPool, mem_new(Gfx, OMM_GFX_POOL_SIZE));
    mem_new1(Gfx *, sGfxHead, sGfxPool);
    if (sGfxHead + count > sGfxPool + OMM_GFX_POOL_SIZE) {
        sGfxHead = sGfxPool;
    }
    Gfx *gfx = sGfxHead;
    sGfxHead += count;
    return gfx;
}

#endif
