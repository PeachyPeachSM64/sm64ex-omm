#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

OMM_INLINE s32 obj_get_list_index_from_behavior(const BehaviorScript *behavior) {
    if (behavior && !(behavior[0] & 0xFF000000)) {
        return (s32) ((u32) ((behavior[0] >> 16) & 0xFF));
    }
    return OBJ_LIST_DEFAULT;
}

struct Object *obj_spawn_from_geo(struct Object *parent, const GeoLayout *geoLayout, const BehaviorScript *behavior) {
    if (OMM_LIKELY(gObjectLists)) {
        struct Object *obj = spawn_object(parent, 0, behavior);
        obj->oGraphNode = geo_layout_to_graph_node(NULL, geoLayout);
        return obj;
    }
    return NULL;
}

struct Object *obj_get_first(s32 list) {
    if (OMM_LIKELY(gObjectLists && list >= 0 && list < NUM_OBJ_LISTS)) {
        struct Object *head = (struct Object *) &gObjectLists[list];
        struct Object *obj = (struct Object *) head->header.next;
        if (obj != head) {
            return obj;
        }
    }
    return NULL;
}

struct Object *obj_get_next(struct Object *o, s32 list) {
    if (OMM_LIKELY(gObjectLists && o)) {
        struct Object *head = (struct Object *) &gObjectLists[list];
        struct Object *next = (struct Object *) o->header.next;
        if (next != head) {
            return next;
        }
    }
    return NULL;
}

struct Object *obj_get_first_with_behavior(const BehaviorScript *behavior) {
    if (OMM_LIKELY(behavior)) {
        for_each_object_in_list(obj, obj_get_list_index_from_behavior(behavior)) {
            if (obj->behavior == behavior && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) {
                return obj;
            }
        }
    }
    return NULL;
}

struct Object *obj_get_next_with_behavior(struct Object *o, const BehaviorScript *behavior) {
    if (OMM_LIKELY(o && behavior)) {
        s32 list = obj_get_list_index_from_behavior(behavior);
        for (struct Object *obj = obj_get_next(o, list); obj != NULL; obj = obj_get_next(obj, list)) {
            if (obj->behavior == behavior && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) {
                return obj;
            }
        }
    }
    return NULL;
}

struct Object *obj_get_first_with_behavior_and_field_s32(const BehaviorScript *behavior, s32 fieldIndex, s32 value) {
    if (OMM_LIKELY(behavior)) {
        for_each_object_in_list(obj, obj_get_list_index_from_behavior(behavior)) {
            if (obj->behavior == behavior && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED && obj->OBJECT_FIELD_S32(fieldIndex) == value) {
                return obj;
            }
        }
    }
    return NULL;
}

struct Object *obj_get_first_with_behavior_and_field_f32(const BehaviorScript *behavior, s32 fieldIndex, f32 value) {
    if (OMM_LIKELY(behavior)) {
        for_each_object_in_list(obj, obj_get_list_index_from_behavior(behavior)) {
            if (obj->behavior == behavior && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED && obj->OBJECT_FIELD_F32(fieldIndex) == value) {
                return obj;
            }
        }
    }
    return NULL;
}

struct Object *obj_get_nearest_with_behavior(struct Object *o, const BehaviorScript *behavior) {
    if (OMM_LIKELY(o && behavior)) {
        f32 distMin = 1e10f;
        struct Object *nearest = NULL;
        for_each_object_with_behavior(obj, behavior) {
            if (obj != o) {
                f32 dist = obj_get_distance(o, obj);
                if (dist < distMin) {
                    nearest = obj;
                    distMin = dist;
                }
            }
        }
        return nearest;
    }
    return NULL;
}

struct Object *obj_get_nearest_with_behavior_and_radius(struct Object *o, const BehaviorScript *behavior, f32 distMax) {
    if (OMM_LIKELY(o && behavior)) {
        f32 distMin = distMax;
        struct Object *nearest = NULL;
        for_each_object_with_behavior(obj, behavior) {
            if (obj != o) {
                f32 dist = obj_get_distance(o, obj);
                if (dist < distMin) {
                    nearest = obj;
                    distMin = dist;
                }
            }
        }
        return nearest;
    }
    return NULL;
}

s32 obj_get_list_index(struct Object *o) {
    return (OMM_LIKELY(o) ? obj_get_list_index_from_behavior(o->behavior) : -1);
}

s32 obj_get_count_with_behavior(const BehaviorScript *behavior) {
    if (OMM_LIKELY(behavior)) {
        s32 count = 0;
        for_each_object_with_behavior(obj, behavior) {
            count++;
        }
        return count;
    }
    return 0;
}

s32 obj_get_count_with_behavior_and_field_s32(const BehaviorScript *behavior, s32 fieldIndex, s32 value) {
    if (OMM_LIKELY(behavior)) {
        s32 count = 0;
        for_each_object_with_behavior(obj, behavior) {
            count += (obj->OBJECT_FIELD_S32(fieldIndex) == value);
        }
        return count;
    }
    return 0;
}

s32 obj_get_count_with_behavior_and_field_f32(const BehaviorScript *behavior, s32 fieldIndex, f32 value) {
    if (OMM_LIKELY(behavior)) {
        s32 count = 0;
        for_each_object_with_behavior(obj, behavior) {
            count += (obj->OBJECT_FIELD_F32(fieldIndex) == value);
        }
        return count;
    }
    return 0;
}

void obj_deactivate_all_with_behavior(const BehaviorScript *behavior) {
    struct Object *obj;
    while ((obj = obj_get_first_with_behavior(behavior)) != NULL) {
        obj_mark_for_deletion(obj);
    }
}

void obj_unload_all_with_behavior(const BehaviorScript *behavior) {
    struct Object *obj;
    while ((obj = obj_get_first_with_behavior(behavior)) != NULL) {
        unload_object(obj);
    }
}
