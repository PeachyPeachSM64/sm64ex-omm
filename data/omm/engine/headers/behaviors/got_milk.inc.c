#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define o gCurrentObject
#endif
#if OMM_GAME_IS_R96X
#include "behavior_commands.h"

//
// Vertices
//

const Gfx wdw_area_1_start_gfx[] = { gsDPSetDepthImage(wdw_area_1_start_gfx), gsSPEndDisplayList(), };
const Gfx wdw_area_2_start_gfx[] = { gsDPSetDepthImage(wdw_area_2_start_gfx), gsSPEndDisplayList(), };
const Gfx wdw_end_gfx[] = { gsDPSetDepthImage(wdw_end_gfx), gsSPEndDisplayList(), };

static OmmHMap_(Vtx *) sWdwVtxBuffers[2] = { omm_hmap_zero, omm_hmap_zero };

const Vtx *wdw_swap_vtx(const Vtx *vtx, u32 count, s32 areaIndex) {

    // Check buffer
    s32 i = omm_hmap_find(sWdwVtxBuffers[areaIndex - 1], (uintptr_t) vtx);
    if (i != -1) {
        return omm_hmap_get(sWdwVtxBuffers[areaIndex - 1], const Vtx *, i) + 1;
    }

    // Create new buffer
    Vtx *buffer = mem_new(Vtx, count + 1);
    buffer[0].v.ob[0] = count;
    mem_cpy(buffer + 1, vtx, sizeof(Vtx) * count);
    omm_hmap_insert(sWdwVtxBuffers[areaIndex - 1], (uintptr_t) vtx, buffer);
    return buffer;
}

OMM_ROUTINE_LEVEL_ENTRY(wdw_reset_vtx) {
    for_each_in_(s32, areaIndex, { 1, 2 }) {
        omm_hmap_for_each(sWdwVtxBuffers[*areaIndex - 1], k, v) {
            Vtx *buffer = (Vtx *) *v;
            mem_del(buffer);
        }
        omm_hmap_delete(sWdwVtxBuffers[*areaIndex - 1]);
    }
}

//
// Surfaces
//

static OmmArray_(struct Surface *) sWdwSurfaces[2] = { omm_array_zero, omm_array_zero };
extern OmmArray sOmmLoadedSurfaces;

static void wdw_update_surfaces(s32 areaIndex) {
    for (s32 i = 0; i != gNumStaticSurfaces; ++i) {
        struct Surface *loaded = omm_array_get(sOmmLoadedSurfaces, ptr, i);
        struct Surface *surf = omm_array_get(sWdwSurfaces[areaIndex - 1], ptr, i);
        mem_cpy(loaded, surf, sizeof(struct Surface));
    }
}

OMM_ROUTINE_LEVEL_ENTRY(wdw_reset_surfaces) {
    for_each_in_(s32, areaIndex, { 1, 2 }) {
        omm_array_for_each(sWdwSurfaces[*areaIndex - 1], p_surf) {
            struct Surface *surf = (struct Surface *) p_surf->as_ptr;
            mem_del(surf);
        }
        omm_array_delete(sWdwSurfaces[*areaIndex - 1]);
    }
}

OMM_ROUTINE_UPDATE(wdw_init_surfaces) {
    static s32 sPrevAreaIndex = 0;
    if (gCurrLevelNum == LEVEL_WDW) {
        s32 initialized = omm_array_count(sWdwSurfaces[gCurrAreaIndex - 1]);
        if (initialized < gNumStaticSurfaces) {
            omm_array_grow(sWdwSurfaces[gCurrAreaIndex - 1], ptr, mem_new(struct Surface, 1), gNumStaticSurfaces);
            for (s32 i = initialized; i != gNumStaticSurfaces; ++i) {
                struct Surface *loaded = omm_array_get(sOmmLoadedSurfaces, ptr, i);
                struct Surface *surf = omm_array_get(sWdwSurfaces[gCurrAreaIndex - 1], ptr, i);
                mem_cpy(surf, loaded, sizeof(struct Surface));
            }
        }
        if (sPrevAreaIndex != gCurrAreaIndex) {
            wdw_update_surfaces(gCurrAreaIndex);
            sPrevAreaIndex = gCurrAreaIndex;
        }
    } else {
        sPrevAreaIndex = 0;
    }
}

//
// Punch, kick, ground-pound surfaces
//

#define OMM_MARIO_MILK_DISPLACEMENT 200

static void bhv_milk_particle_update() {
    if (o->oTimer > o->oAction) {
        obj_mark_for_deletion(o);
    }
}

static const BehaviorScript bhvMilkParticle[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(cur_obj_rotate_face_angle_using_vel),
        BHV_CALL_NATIVE(cur_obj_move_using_vel_and_gravity),
        BHV_CALL_NATIVE(bhv_milk_particle_update),
    BHV_END_LOOP(),
};

static bool are_surfaces_adjacent(struct Surface *surf1, struct Surface *surf2) {
    Vec3f verts[2][3] = {{
        { surf1->vertex1[0], surf1->vertex1[1], surf1->vertex1[2] },
        { surf1->vertex2[0], surf1->vertex2[1], surf1->vertex2[2] },
        { surf1->vertex3[0], surf1->vertex3[1], surf1->vertex3[2] },
    },{
        { surf2->vertex1[0], surf2->vertex1[1], surf2->vertex1[2] },
        { surf2->vertex2[0], surf2->vertex2[1], surf2->vertex2[2] },
        { surf2->vertex3[0], surf2->vertex3[1], surf2->vertex3[2] },
    }};
    for (s32 k = 0; k != 2; ++k)
    for (s32 i = 0; i != 3; ++i)
    for (s32 j = 0; j != 3; ++j) {
        Vec3f proj; f32 t;
        vec3f_get_projected_point_on_line(proj, &t, verts[k][i], verts[1 - k][j], verts[1 - k][(j + 1) % 3]);
        if (-0.01f <= t && t <= 1.01f && vec3f_dist(verts[k][i], proj) < 4.f) {
            return true;
        }
    }
    return false;
}

static bool omm_mario_milk_move_vertex(OmmArray *moved, void *src, Vec3f v, Vec3f tri[3], Vec3f dir, Vec3f n, Vec3f bbmin, Vec3f bbmax) {

    // Cannot move the same vertex twice during the same frame
    if (omm_array_find(*moved, ptr, src) != -1) {
        return false;
    }

    // Bounding box check
    if (bbmin[0] - 5 <= v[0] && v[0] <= bbmax[0] + 5 &&
        bbmin[1] - 5 <= v[1] && v[1] <= bbmax[1] + 5 &&
        bbmin[2] - 5 <= v[2] && v[2] <= bbmax[2] + 5) {

        // Projection check
        Vec3f proj, dif;
        vec3f_project_point_dir(proj, v, dir, tri[0], n);
        vec3f_dif(dif, proj, v);
        f32 dist = vec3f_dot(dif, dir);
        if (-5.f <= dist && dist <= OMM_MARIO_MILK_DISPLACEMENT + 5.f) {

            // Triangle check
            Vec3f coords;
            vec3f_get_barycentric_coords(coords, proj, tri[0], tri[1], tri[2]);
            if (-0.01f <= coords[0] && coords[0] <= 1.01f &&
                -0.01f <= coords[1] && coords[1] <= 1.01f &&
                -0.01f <= coords[2] && coords[2] <= 1.01f) {

                // Apply displacement
                v[0] += dir[0] * OMM_MARIO_MILK_DISPLACEMENT;
                v[1] += dir[1] * OMM_MARIO_MILK_DISPLACEMENT;
                v[2] += dir[2] * OMM_MARIO_MILK_DISPLACEMENT;

                omm_array_add(*moved, ptr, src);
                return true;
            }
        }
    }

    return false;
}

static void omm_mario_milk_move_vertices(struct Surface *surf, Vec3f dir) {
    Vec3f surfn = { surf->normal.x, surf->normal.y, surf->normal.z };

    // Get all adjacent surfaces that have the same normal
    OmmArray_(struct Surface *) asurfaces = omm_array_zero;
    omm_array_add(asurfaces, ptr, surf);
    for (s32 i = 0, count = 1; i < count; ++i) {
        struct Surface *asurf = omm_array_get(asurfaces, ptr, i);
        for (s32 j = 0; j != gNumStaticSurfaces; ++j) {
            struct Surface *s = omm_array_get(sWdwSurfaces[gCurrAreaIndex - 1], ptr, j);
            if (omm_array_find(asurfaces, ptr, s) != -1) {
                continue;
            }

            // Normal check
            Vec3f sn = { s->normal.x, s->normal.y, s->normal.z };
            if (vec3f_dot(surfn, sn) < 0.9f) {
                continue;
            }

            // Adjacency check
            if (!are_surfaces_adjacent(asurf, s)) {
                continue;
            }

            omm_array_add(asurfaces, ptr, s);
            count++;
        }
    }

    // For each surface
    OmmArray_(void *) moved = omm_array_zero;
    omm_array_for_each(asurfaces, p_asurf) {
        struct Surface *asurf = (struct Surface *) p_asurf->as_ptr;
        Vec3f n = {
            asurf->normal.x,
            asurf->normal.y,
            asurf->normal.z,
        };
        Vec3f before[3] = {
            { asurf->vertex1[0], asurf->vertex1[1], asurf->vertex1[2] },
            { asurf->vertex2[0], asurf->vertex2[1], asurf->vertex2[2] },
            { asurf->vertex3[0], asurf->vertex3[1], asurf->vertex3[2] },
        };
        Vec3f after[3] = {
            { asurf->vertex1[0] + dir[0] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex1[1] + dir[1] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex1[2] + dir[2] * OMM_MARIO_MILK_DISPLACEMENT },
            { asurf->vertex2[0] + dir[0] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex2[1] + dir[1] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex2[2] + dir[2] * OMM_MARIO_MILK_DISPLACEMENT },
            { asurf->vertex3[0] + dir[0] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex3[1] + dir[1] * OMM_MARIO_MILK_DISPLACEMENT, asurf->vertex3[2] + dir[2] * OMM_MARIO_MILK_DISPLACEMENT },
        };

        // Compute bounding box
        Vec3f bbmin = {
            min_f(min_3_f(before[0][0], before[1][0], before[2][0]), min_3_f(after[0][0], after[1][0], after[2][0])),
            min_f(min_3_f(before[0][1], before[1][1], before[2][1]), min_3_f(after[0][1], after[1][1], after[2][1])),
            min_f(min_3_f(before[0][2], before[1][2], before[2][2]), min_3_f(after[0][2], after[1][2], after[2][2])),
        };
        Vec3f bbmax = {
            max_f(max_3_f(before[0][0], before[1][0], before[2][0]), max_3_f(after[0][0], after[1][0], after[2][0])),
            max_f(max_3_f(before[0][1], before[1][1], before[2][1]), max_3_f(after[0][1], after[1][1], after[2][1])),
            max_f(max_3_f(before[0][2], before[1][2], before[2][2]), max_3_f(after[0][2], after[1][2], after[2][2])),
        };

        // Move surface vertices
        vec3f_to_vec3s(asurf->vertex1, after[0]);
        vec3f_to_vec3s(asurf->vertex2, after[1]);
        vec3f_to_vec3s(asurf->vertex3, after[2]);
        recompute_surface_parameters(asurf);

        // Surface vertices
        for (s32 i = 0; i != gNumStaticSurfaces; ++i) {
            struct Surface *s = omm_array_get(sWdwSurfaces[gCurrAreaIndex - 1], ptr, i);
            if (omm_array_find(asurfaces, ptr, s) == -1) {
                Vec3f v1 = { s->vertex1[0], s->vertex1[1], s->vertex1[2] };
                Vec3f v2 = { s->vertex2[0], s->vertex2[1], s->vertex2[2] };
                Vec3f v3 = { s->vertex3[0], s->vertex3[1], s->vertex3[2] };
                bool movedv1 = omm_mario_milk_move_vertex(&moved, s->vertex1, v1, after, dir, n, bbmin, bbmax);
                bool movedv2 = omm_mario_milk_move_vertex(&moved, s->vertex2, v2, after, dir, n, bbmin, bbmax);
                bool movedv3 = omm_mario_milk_move_vertex(&moved, s->vertex3, v3, after, dir, n, bbmin, bbmax);

                // Recompute surface parameters
                if (movedv1 || movedv2 || movedv3) {
                    vec3f_to_vec3s(s->vertex1, v1);
                    vec3f_to_vec3s(s->vertex2, v2);
                    vec3f_to_vec3s(s->vertex3, v3);
                    recompute_surface_parameters(s);
                }
            }
        }

        // Gfx vertices
        omm_hmap_for_each(sWdwVtxBuffers[gCurrAreaIndex - 1], k, v) {
            Vtx *vtx = (Vtx *) *v;
            u32 count = vtx[0].v.ob[0];
            for (u32 i = 0; i != count; ++i) {
                omm_mario_milk_move_vertex(&moved, vtx + 1 + i, (vtx + 1 + i)->v.ob, after, dir, n, bbmin, bbmax);
            }
        }
    }

    // Update collisions
    wdw_update_surfaces(gCurrAreaIndex);
    extern void omm_surface_add_all_surfaces_again();
    omm_surface_add_all_surfaces_again();

    // Cleanup
    omm_array_delete(moved);
    omm_array_delete(asurfaces);
}

static void omm_mario_milk_process_surface(struct Surface *surf, Vec3f dir) {
    if (surf->object) {
        obj_destroy(surf->object);
    } else {
        omm_mario_milk_move_vertices(surf, dir);
    }
}

static void omm_mario_milk_spawn_particles_and_sound(struct MarioState *m, Vec3f pos, Vec3f dir) {

    // Spawn triangles
    for (s32 i = 0; i != 64; ++i) {
        struct Object *particle = spawn_object(m->marioObj, MODEL_DIRT_ANIMATION, bhvMilkParticle);
        f32 mag = lerp_f(random_float(), 25.f, 50.f);
        Vec3f e1, e2, n = { dir[0], dir[1], dir[2] };
        vec3f_get_nullspace(n, e1, e2, n);
        vec3f_rotate_around_n(e1, e1, n, random_u16());
        vec3f_rotate_around_n(n, n, e1, random_float_n1_p1() * 0x2000);
        vec3f_set_mag(n, mag);
        particle->oPosX = pos[0] + (e1[0] * mag * 4.f - dir[0] * OMM_MARIO_MILK_DISPLACEMENT);
        particle->oPosY = pos[1] + (e1[1] * mag * 4.f - dir[1] * OMM_MARIO_MILK_DISPLACEMENT);
        particle->oPosZ = pos[2] + (e1[2] * mag * 4.f - dir[2] * OMM_MARIO_MILK_DISPLACEMENT);
        particle->oVelX = n[0];
        particle->oVelY = n[1];
        particle->oVelZ = n[2];
        particle->oAnimState = 4;
        particle->oFaceAngleYaw = random_u16();
        particle->oFaceAnglePitch = random_u16();
        particle->oAngleVelPitch = 0x1000;
        particle->oAngleVelYaw = 0x500;
        particle->oAction = lerp_f(random_float(), 15, 30);
        obj_scale(particle, lerp_f(random_float(), 3.f, 5.f));
    }

    // Spawn shockwave
    {
        f32 dist; s16 pitch, yaw;
        vec3f_get_dist_and_angle(gVec3fZero, dir, &dist, &pitch, &yaw);
        struct Object *shockwave = obj_spawn_from_geo(m->marioObj, omm_geo_shockwave, bhvOmmShockwave);
        obj_set_pos_vec3f(shockwave, pos);
        obj_set_angle(shockwave, pitch - 0x4000, yaw, 0);
        obj_scale(shockwave, 20.f);
        shockwave->oOpacity = 0xFF;
        shockwave->oShockwaveScaleMin = 20.f;
        shockwave->oShockwaveScaleMax = 200.f;
        shockwave->oShockwaveAlphaMin = 0x00;
        shockwave->oShockwaveAlphaMax = 0x1FF;
        shockwave->oShockwaveDuration = 20;
    }

    // Sfx and camera shake
    play_sound(SOUND_GENERAL_WALL_EXPLOSION | 0xFFFF, gGlobalSoundArgs);
    set_camera_pitch_shake(0x100, 0x8, 0x4000);
}

static bool omm_mario_milk_process_wall_interactions(struct MarioState *m) {
    if ((m->flags & (MARIO_PUNCHING | MARIO_KICKING | MARIO_TRIPPING)) != 0 || m->action == ACT_WATER_PUNCH) {
        f32 attackRadius = 2.f * m->marioObj->hitboxRadius;
        for_each_in_(f32, dy, { 0.5f, 0.3f, 0.7f, 0.1f, 0.9f }) {
            Vec3f orig = { m->pos[0], m->pos[1] + m->marioObj->hitboxHeight * *dy, m->pos[2] };
            Vec3f dir = { attackRadius * sins(m->faceAngle[1]), 0, attackRadius * coss(m->faceAngle[1]) };
            RayCollisionData hits;
            if (find_collisions_on_ray(orig, dir, &hits, 1.f, RAYCAST_FLAGS_SURFACES) > 0) {
                for (s32 i = 0; i != hits.count; ++i) {
                    RayHit *hit = hits.hits + i;
                    if (abs_f(hit->surf->normal.y) < 0.7f) {
                        omm_mario_milk_process_surface(hit->surf, (Vec3f) { -hit->surf->normal.x, 0, -hit->surf->normal.z });
                        omm_mario_milk_spawn_particles_and_sound(m, hit->pos, (Vec3f) { hit->surf->normal.x, 0, hit->surf->normal.z });
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static bool omm_mario_milk_process_floor_interactions(struct MarioState *m) {
    static struct Surface *gpFloor = NULL;
    if (!omm_mario_is_ground_pound_landing(m)) {
        gpFloor = NULL;
        return false;
    }
    if (gpFloor != m->floor) {
        omm_mario_milk_process_surface(m->floor, (Vec3f) { 0, -1, 0 });
        omm_mario_milk_spawn_particles_and_sound(m, m->pos, (Vec3f) { 0, 1, 0 });
        m->pos[1] = m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
        gpFloor = m->floor;
        return true;
    }
    return false;
}

static bool omm_mario_milk_process_ceil_interactions(struct MarioState *m) {
    if (m->vel[1] > 0.f && ((m->action & ACT_FLAG_SWIMMING) == 0 || m->action == ACT_OMM_WATER_GROUND_POUND_JUMP)) {
        Vec3f orig = { m->pos[0], m->pos[1], m->pos[2] };
        Vec3f dir = { 0, m->marioObj->hitboxHeight, 0 };
        RayCollisionData hits;
        if (find_collisions_on_ray(orig, dir, &hits, 1.f, RAYCAST_FLAG_CEILS) > 0) {
            RayHit *hit = hits.hits;
            omm_mario_milk_process_surface(hit->surf, (Vec3f) { 0, 1, 0 });
            omm_mario_milk_spawn_particles_and_sound(m, hit->pos, (Vec3f) { 0, -1, 0 });
            m->vel[1] = 0.f;
            return true;
        }
    }
    return false;
}

bool omm_mario_milk_process_surface_interactions(struct MarioState *m) {
    return omm_mario_milk_process_floor_interactions(m) ||
           omm_mario_milk_process_wall_interactions(m) ||
           omm_mario_milk_process_ceil_interactions(m);
}

//
// Behavior
//

static struct ObjectHitbox sMilkHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 80,
    /* hurtboxHeight:     */ 80,
};

static void bhv_milk_disable() {
    cur_obj_become_intangible();
    cur_obj_hide();
}

static void bhv_milk_enable() {
    cur_obj_become_tangible();
    cur_obj_unhide();
}

void bhv_milk_init(void) {
    if (gMarioState->numStars >= 120 && !OMM_SPARKLY_MODE_IS_ENABLED && !gOmmGlobals->yoshiMode) {
        o->oHomeX = o->oPosX;
        o->oHomeY = o->oPosY;
        o->oHomeZ = o->oPosZ;
        obj_set_hitbox(o, &sMilkHitbox);
        cur_obj_scale(0.5f);
        if (omm_mario_is_milk(gMarioState)) {
            o->oAction = 1;
        }
    } else {
        obj_mark_for_deletion(o);
    }
}

void bhv_milk_loop(void) {
    if (o->oAction == 0) {
        bhv_milk_enable();
        o->oFaceAngleYaw += 0x400;
        koopa_shell_spawn_sparkles(10.f);
        if (obj_check_if_collided_with_object(o, gMarioObject)) {
            mario_stop_riding_and_holding(gMarioState);
            omm_mario_unpossess_object(gMarioState, OMM_MARIO_UNPOSSESS_ACT_NONE, 0);
            omm_mario_set_action(gMarioState, ACT_OMM_MILK_POWER_UP, 0, 0xFFFF);
            bhv_milk_disable();
            o->oAction = 1;
        }
    } else {
        bhv_milk_disable();
        if (!omm_mario_is_milk(gMarioState)) {
            cur_obj_set_pos_to_home();
            o->oAction = 0;
        }
    }
    o->oInteractStatus = 0;
}

#endif
