#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"
extern bool omm_obj_is_holdable(struct Object *o);
extern void pobj_hold_object(struct Object *obj);

#define OMM_YOSHI_TONGUE_LENGTH (400)

//
// Gfx data
//

static const Lights1 omm_yoshi_tongue_light = gdSPDefLights1(
    0x70, 0x00, 0x00,
    0xE0, 0x00, 0x00,
    0x28, 0x28, 0x28
);

static const Vtx omm_yoshi_tongue_vtx[] = {
    { { { 0, 0, 100 }, 0, { 0, 0 }, { 0, 0, 127, 0xFF } } },
    { { { 0, 35, 70 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { -42, 25, 70 }, 0, { 0, 0 }, { -90, 90, 0, 0xFF } } },
    { { { -60, 0, 70 }, 0, { 0, 0 }, { -127, 0, 0, 0xFF } } },
    { { { -42, -25, 70 }, 0, { 0, 0 }, { -90, -90, 0, 0xFF } } },
    { { { 0, -35, 70 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { 42, -25, 70 }, 0, { 0, 0 }, { 90, -90, 0, 0xFF } } },
    { { { 60, 0, 70 }, 0, { 0, 0 }, { 127, 0, 0, 0xFF } } },
    { { { 42, 25, 70 }, 0, { 0, 0 }, { 90, 90, 0, 0xFF } } },
    { { { 0, 50, 0 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { -57, 35, 0 }, 0, { 0, 0 }, { -90, 90, 0, 0xFF } } },
    { { { -80, 0, 0 }, 0, { 0, 0 }, { -127, 0, 0, 0xFF } } },
    { { { -57, -35, 0 }, 0, { 0, 0 }, { -90, -90, 0, 0xFF } } },
    { { { 0, -50, 0 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { 57, -35, 0 }, 0, { 0, 0 }, { 90, -90, 0, 0xFF } } },
    { { { 80, 0, 0 }, 0, { 0, 0 }, { 127, 0, 0, 0xFF } } },
    { { { 57, 35, 0 }, 0, { 0, 0 }, { 90, 90, 0, 0xFF } } },
    { { { 0, 35, -70 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { -42, 25, -70 }, 0, { 0, 0 }, { -90, 90, 0, 0xFF } } },
    { { { -60, 0, -70 }, 0, { 0, 0 }, { -127, 0, 0, 0xFF } } },
    { { { -42, -25, -70 }, 0, { 0, 0 }, { -90, -90, 0, 0xFF } } },
    { { { 0, -35, -70 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { 42, -25, -70 }, 0, { 0, 0 }, { 90, -90, 0, 0xFF } } },
    { { { 60, 0, -70 }, 0, { 0, 0 }, { 127, 0, 0, 0xFF } } },
    { { { 42, 25, -70 }, 0, { 0, 0 }, { 90, 90, 0, 0xFF } } },
    { { { 0, 25, -90 }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { -35, 18, -90 }, 0, { 0, 0 }, { -90, 90, 0, 0xFF } } },
    { { { -50, 0, -90 }, 0, { 0, 0 }, { -127, 0, 0, 0xFF } } },
    { { { -35, -18, -90 }, 0, { 0, 0 }, { -90, -90, 0, 0xFF } } },
    { { { 0, -25, -90 }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { 35, -18, -90 }, 0, { 0, 0 }, { 90, -90, 0, 0xFF } } },
    { { { 50, 0, -90 }, 0, { 0, 0 }, { 127, 0, 0, 0xFF } } },
    { { { 35, 18, -90 }, 0, { 0, 0 }, { 90, 90, 0, 0xFF } } },
    { { { 0, 25, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { 0, 127, 0, 0xFF } } },
    { { { -35, 18, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { -90, 90, 0, 0xFF } } },
    { { { -50, 0, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { -127, 0, 0, 0xFF } } },
    { { { -35, -18, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { -90, -90, 0, 0xFF } } },
    { { { 0, -25, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { 0, -127, 0, 0xFF } } },
    { { { 35, -18, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { 90, -90, 0, 0xFF } } },
    { { { 50, 0, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { 127, 0, 0, 0xFF } } },
    { { { 35, 18, -4 * OMM_YOSHI_TONGUE_LENGTH }, 0, { 0, 0 }, { 90, 90, 0, 0xFF } } },
};

static const Gfx omm_yoshi_tongue_tri[] = {
    gsSPVertex(omm_yoshi_tongue_vtx + 0, 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(0, 3, 4, 0, 0, 4, 5, 0),
    gsSP2Triangles(0, 5, 6, 0, 0, 6, 7, 0),
    gsSP2Triangles(0, 7, 8, 0, 0, 8, 1, 0),
    gsSPVertex(omm_yoshi_tongue_vtx + 1, 16, 0),
    gsSP2Triangles(0, 8, 1, 0, 8, 9, 1, 0),
    gsSP2Triangles(1, 9, 2, 0, 9, 10, 2, 0),
    gsSP2Triangles(2, 10, 3, 0, 10, 11, 3, 0),
    gsSP2Triangles(3, 11, 4, 0, 11, 12, 4, 0),
    gsSP2Triangles(4, 12, 5, 0, 12, 13, 5, 0),
    gsSP2Triangles(5, 13, 6, 0, 13, 14, 6, 0),
    gsSP2Triangles(6, 14, 7, 0, 14, 15, 7, 0),
    gsSP2Triangles(7, 15, 0, 0, 15, 8, 0, 0),
    gsSPVertex(omm_yoshi_tongue_vtx + 9, 16, 0),
    gsSP2Triangles(0, 8, 1, 0, 8, 9, 1, 0),
    gsSP2Triangles(1, 9, 2, 0, 9, 10, 2, 0),
    gsSP2Triangles(2, 10, 3, 0, 10, 11, 3, 0),
    gsSP2Triangles(3, 11, 4, 0, 11, 12, 4, 0),
    gsSP2Triangles(4, 12, 5, 0, 12, 13, 5, 0),
    gsSP2Triangles(5, 13, 6, 0, 13, 14, 6, 0),
    gsSP2Triangles(6, 14, 7, 0, 14, 15, 7, 0),
    gsSP2Triangles(7, 15, 0, 0, 15, 8, 0, 0),
    gsSPVertex(omm_yoshi_tongue_vtx + 17, 16, 0),
    gsSP2Triangles(0, 8, 1, 0, 8, 9, 1, 0),
    gsSP2Triangles(1, 9, 2, 0, 9, 10, 2, 0),
    gsSP2Triangles(2, 10, 3, 0, 10, 11, 3, 0),
    gsSP2Triangles(3, 11, 4, 0, 11, 12, 4, 0),
    gsSP2Triangles(4, 12, 5, 0, 12, 13, 5, 0),
    gsSP2Triangles(5, 13, 6, 0, 13, 14, 6, 0),
    gsSP2Triangles(6, 14, 7, 0, 14, 15, 7, 0),
    gsSP2Triangles(7, 15, 0, 0, 15, 8, 0, 0),
    gsSPVertex(omm_yoshi_tongue_vtx + 25, 16, 0),
    gsSP2Triangles(0, 8, 1, 0, 8, 9, 1, 0),
    gsSP2Triangles(1, 9, 2, 0, 9, 10, 2, 0),
    gsSP2Triangles(2, 10, 3, 0, 10, 11, 3, 0),
    gsSP2Triangles(3, 11, 4, 0, 11, 12, 4, 0),
    gsSP2Triangles(4, 12, 5, 0, 12, 13, 5, 0),
    gsSP2Triangles(5, 13, 6, 0, 13, 14, 6, 0),
    gsSP2Triangles(6, 14, 7, 0, 14, 15, 7, 0),
    gsSP2Triangles(7, 15, 0, 0, 15, 8, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx omm_yoshi_tongue_gfx[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPLight(&omm_yoshi_tongue_light.l, 1),
    gsSPLight(&omm_yoshi_tongue_light.a, 2),
    gsSPDisplayList(omm_yoshi_tongue_tri),
    gsSPEndDisplayList(),
};

//
// Geo layout
//

const GeoLayout omm_geo_yoshi_tongue[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SCALE(0, 0x4000),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, omm_yoshi_tongue_gfx),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

//
// Behavior
//

static bool omm_obj_is_yoshi_tongueable(struct Object *o, struct Object *obj, const OmmBhvDataYoshiTongue *yt) {

    // Check holdable
    if (yt->type <= YOSHI_TONGUE_TYPE_THROWABLE && !omm_obj_is_holdable(obj)) {
        return false;
    }

    // Check mushroom 1-up
    if (yt->type == YOSHI_TONGUE_TYPE_MUSHROOM_1UP && !omm_obj_is_mushroom_1up(obj)) {
        return false;
    }

    // If it's a surface object, check surface intersection
    if (obj_get_list_index(obj) == OBJ_LIST_SURFACE && obj->collisionData != NULL) {
        for (struct Surface *surf = obj->oSurfaces; surf; surf = get_next_surface(surf)) {
            if (surface_intersects_cylinder(surf, &o->oPosX, o->hitboxRadius, o->hitboxHeight, o->hitboxDownOffset)) {
                return true;
            }
        }
        return false;
    }

    // Otherwise, check hitbox overlap
    return obj_detect_hitbox_overlap(o, obj, OBJ_OVERLAP_FLAG_HITBOX, OBJ_OVERLAP_FLAG_HITBOX_HURTBOX_MAX | ((yt->type <= YOSHI_TONGUE_TYPE_THROWABLE) * OBJ_OVERLAP_FLAG_IGNORE_INTANGIBLE));
}

static OmmArray omm_obj_get_yoshi_tongue_behaviors() {
    static OmmArray sOmmYoshiTongueBehaviors = omm_array_zero;
    OMM_DO_ONCE {
        const OmmArray *behaviors = omm_get_behaviors_list();
        omm_array_for_each(*behaviors, p_bhv) {
            const BehaviorScript *bhv = p_bhv->as_ptr;
            if (omm_behavior_data_get_yoshi_tongue(bhv)) {
                omm_array_add(sOmmYoshiTongueBehaviors, ptr, bhv);
            }
        }
    }
    return sOmmYoshiTongueBehaviors;
}

static void bhv_omm_yoshi_tongue_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (!p || p != gOmmCapture || omm_capture_get_type(p) != OMM_CAPTURE_YOSHI || gOmmObject->yoshi.tongueTimer <= 0) {
        obj_mark_for_deletion(o);
        return;
    }

    // Hitbox
    // To be consistent between different Yoshi models, position for collision is computed from object values
    o->oPosX = p->oPosX + (omm_capture_get_hitbox_radius(p) + OMM_YOSHI_TONGUE_LENGTH * gOmmObject->yoshi.tongueSine) * sins(p->oFaceAngleYaw);
    o->oPosY = p->oPosY + (omm_capture_get_hitbox_height(p) / 2.f);
    o->oPosZ = p->oPosZ + (omm_capture_get_hitbox_radius(p) + OMM_YOSHI_TONGUE_LENGTH * gOmmObject->yoshi.tongueSine) * coss(p->oFaceAngleYaw);
    obj_set_angle(o, 0, p->oFaceAngleYaw, 0);
    obj_set_scale(o, p->oScaleX, p->oScaleY, p->oScaleZ);
    obj_set_params(o, 0, 0, 0, 0, true);
    obj_reset_hitbox(o, 80, 160, 0, 0, 0, 80);

    // Collisions
    if (!gOmmObject->yoshi.tongued) {
        omm_array_for_each(omm_obj_get_yoshi_tongue_behaviors(), p_bhv) {
            const BehaviorScript *bhv = (const BehaviorScript *) p_bhv->as_ptr;
            const OmmBhvDataYoshiTongue *yt = omm_behavior_data_get_yoshi_tongue(bhv);
            for_each_object_with_behavior(obj, bhv) {
                if (!obj_is_dormant(obj) && omm_obj_is_yoshi_tongueable(o, obj, yt)) {
                    switch (yt->type) {

                        // Hold object
                        case YOSHI_TONGUE_TYPE_DEFAULT:
                        case YOSHI_TONGUE_TYPE_EATABLE:
                        case YOSHI_TONGUE_TYPE_THROWABLE: if (!gOmmObject->yoshi.tongued) {
                            pobj_hold_object(obj);
                            gOmmObject->yoshi.tongued = obj;
                        } break;

                        // Assign dummy behavior to prevent the object from updating
                        case YOSHI_TONGUE_TYPE_FIRE:
                        case YOSHI_TONGUE_TYPE_DESTROY: if (!gOmmObject->yoshi.tongued) {
                            obj->curBhvCommand = bhvOmmDummy;
                            gOmmObject->yoshi.tongued = obj;
                        } break;

                        // Make the object intangible to not collect it when processing the capture object interactions
                        case YOSHI_TONGUE_TYPE_MUSHROOM_1UP:
                        case YOSHI_TONGUE_TYPE_STAR_OR_KEY:
                        case YOSHI_TONGUE_TYPE_CAP: if (!gOmmObject->yoshi.tongued) {
                            obj->oIntangibleTimer = -1;
                            gOmmObject->yoshi.tongued = obj;
                        } break;

                        // Collect object
                        case YOSHI_TONGUE_TYPE_COIN:
                        case YOSHI_TONGUE_TYPE_SECRET: {
                            obj->oPosX = p->oPosX;
                            obj->oPosY = p->oPosY + 60.f;
                            obj->oPosZ = p->oPosZ;
                        } break;

                        // Attack object
                        case YOSHI_TONGUE_TYPE_ATTACK: {
                            if (omm_obj_is_unagis_tail(obj)) {
                                if (obj->parentObj && obj->parentObj->oAnimState != 0) {
                                    obj->oDistanceToMario = 0;
                                    obj->oFlags &= ~OBJ_FLAG_COMPUTE_DIST_TO_MARIO;
                                }
                            } else if (omm_obj_is_water_diamond(obj)) {
                                obj->oAction = WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL;
                                gWDWWaterLevelChanging = 1;
                            } else {
                                obj->oInteractStatus = (ATTACK_GROUND_POUND_OR_TWIRL | INT_STATUS_INTERACTED | INT_STATUS_WAS_ATTACKED);
                            }
                        } break;
                    }
                }
            }
        }
    }

    // Gfx
    obj_set_scale(o, p->oScaleX, p->oScaleY, p->oScaleZ * gOmmObject->yoshi.tongueSine);
    Mat4 objTransform;
    geo_compute_capture_cappy_obj_transform(p, 4, objTransform);
    o->oPosX = objTransform[3][0] + (10.f * p->oScaleX + OMM_YOSHI_TONGUE_LENGTH * gOmmObject->yoshi.tongueSine) * sins(p->oFaceAngleYaw);
    o->oPosY = objTransform[3][1] + (15.f * p->oScaleY);
    o->oPosZ = objTransform[3][2] + (10.f * p->oScaleZ + OMM_YOSHI_TONGUE_LENGTH * gOmmObject->yoshi.tongueSine) * coss(p->oFaceAngleYaw);
    obj_set_angle(o, 0, p->oFaceAngleYaw, 0);
    obj_update_gfx(o);
    obj_set_always_rendered(o, true);
    obj_copy_visibility_and_transparency(o, p);
    o->activeFlags = (o->activeFlags & ~ACTIVE_FLAG_DITHERED_ALPHA) | (p->activeFlags & ACTIVE_FLAG_DITHERED_ALPHA);

    // Update "tongued" object
    if (gOmmObject->yoshi.tongued) {
        obj_copy_pos(gOmmObject->yoshi.tongued, o);
        obj_update_gfx(gOmmObject->yoshi.tongued);
    }
}

const BehaviorScript bhvOmmYoshiTongue[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_tongue_update),
    BHV_END_LOOP(),
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_tongue(struct Object *o) {
    struct Object *tongue = obj_get_first_with_behavior(bhvOmmYoshiTongue);
    if (!tongue) {
        tongue = obj_spawn_from_geo(o, omm_geo_yoshi_tongue, bhvOmmYoshiTongue);
        tongue->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        tongue->parentObj = o;
        obj_scale(tongue, 0.f);
        obj_set_interact_id(o, tongue);
    }
    return tongue;
}
