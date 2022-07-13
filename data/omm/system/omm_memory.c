#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

#define DOUBLE_SIZE(s) max_s(8, s << 1)

OMM_INLINE bool __omm_nop_eq(__OmmNoP x, __OmmNoP y) {
    return x.as_u64 == y.as_u64;
}

OMM_INLINE void *__omm_realloc(void *p, s32 s, s32 c, s32 sot) {
    void *q = calloc(s, sot);
    if (OMM_LIKELY(p)) {
        OMM_MEMCPY(q, p, c * sot);
        OMM_MEMDEL(p);
    }
    return q;
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
        OMM_MEMMOV(parr->p + index, parr->p + index + 1, sizeof(__OmmNoP) * (parr->c - index - 1));
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
        OMM_MEMMOV(pmap->k + index, pmap->k + index + 1, sizeof(__OmmNoP) * (pmap->c - index - 1));
        OMM_MEMMOV(pmap->v + index, pmap->v + index + 1, sizeof(__OmmNoP) * (pmap->c - index - 1));
        pmap->c--;
    }
}

//
// OmmHMap: a sorted array of key/pointer pairs with dynamic memory allocation; its key is a u32 and its size is always a PoT.
// DO NOT USE THE FUNCTIONS OR STRUCTURES STARTING WITH UNDERSCORES.
// DO NOT USE THE KEY 0, AS IT IS USED AS AN EMPTY SLOT.
// ZERO CHECKS ARE OMITTED FOR FASTER PROCESSING.
//

s32 __omm_hmap_find(OmmHMap *phmap, u32 key) {
    if (OMM_LIKELY(phmap->s)) {
        s32 i = 0, c = phmap->s;
        const u32 *k = phmap->k;
        while (c >>= 1) {
            i += ((k[i + c] >= key) ? c : 0);
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
            OMM_MEMMOV(phmap->k + i + 1, phmap->k + i, sizeof(u32)    * (phmap->s - i - 1));
            OMM_MEMMOV(phmap->v + i + 1, phmap->v + i, sizeof(void *) * (phmap->s - i - 1));
            phmap->k[i] = key;
            phmap->v[i] = val;
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

void *gOmmMemoryPoolStrings;
void *gOmmMemoryPoolGeoData;

static OmmMemoryObject *omm_memory_get_free_slot(OmmMemoryPool *omp) {
    OmmMemoryObject *obj = NULL;
    while (!obj) {
        obj = &omp->objects[omp->current];

        // Check if the owner's oGeoData points to the block of memory
        // If that's the case, it means the block is in use, so skip it
        if (omp == gOmmMemoryPoolGeoData) {
            struct Object *o = (struct Object *) obj->owner;
            if (o && o->oGeoData && o->oGeoData == obj->data) {
                obj = NULL;
            }
        }
        omp->current = (omp->current + 1) % (omp->capacity);
    }
    return obj;
}

void *omm_memory_new(void *pool, s32 size, void *caller) {
    void *p = NULL;
    if (pool) {
        OmmMemoryPool *omp = (OmmMemoryPool *) pool;
        OmmMemoryObject *obj = omm_memory_get_free_slot(omp);
        if (obj->size < size) {
            OMM_MEMDEL(obj->data);
            obj->data = OMM_MEMNEW(u8, size);
            obj->size = size;
        } else {
            OMM_MEMSET(obj->data, 0, size);
        }
        obj->owner = caller;
        p = obj->data;
    } else {
        p = OMM_MEMNEW(u8, size);
    }
    return p;
}

static void omm_memory_init_pool(void **pool, s32 capacity) {
    *pool = OMM_MEMNEW(OmmMemoryPool, 1);
    OmmMemoryPool *omp = (OmmMemoryPool *) *pool;
    omp->objects = (OmmMemoryObject *) OMM_MEMNEW(OmmMemoryObject, capacity);
    omp->capacity = capacity;
    omp->current = 0;
}

OMM_AT_STARTUP static void omm_memory_init_pools() {
    omm_memory_init_pool(&gOmmMemoryPoolStrings, 32);
    omm_memory_init_pool(&gOmmMemoryPoolGeoData, 256);
}

// DEBUG DISPLAY FOR COUNTING THE USED SLOTS OF GEO DATA POOL
/*OMM_ROUTINE_UPDATE(aaa) {
    s32 usedCount = 0;
    OmmMemoryPool *omp = (OmmMemoryPool *) gOmmMemoryPoolGeoData;
    for (s32 i = 0; i != omp->capacity; ++i) {
        OmmMemoryObject *obj = &omp->objects[i];
        if (omp == gOmmMemoryPoolGeoData) {
            struct Object *o = (struct Object *) obj->owner;
            if (o && o->oGeoData && o->oGeoData == obj->data) {
                usedCount++;
            }
        }
    }
    OMM_PRINT_TEXT(-60, 24, "%d", omp->current);
    OMM_PRINT_TEXT(-60, 4, "%d I %d", usedCount, omp->capacity);
}*/