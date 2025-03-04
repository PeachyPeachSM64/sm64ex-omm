#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Geo layouts
//

const GeoLayout omm_geo_yoshi_fire_red[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(red_flame_geo),
};

const GeoLayout omm_geo_yoshi_fire_blue[] = {
    OMM_GEO_BILLBOARD_AND_BRANCH(blue_flame_geo),
};

//
// Behavior
//

static void bhv_omm_yoshi_fire_update() {
    struct Object *o = gCurrentObject;
    struct Object *p = o->parentObj;
    if (p && p->oPosY + 90.f * p->oScaleY < find_water_level(p->oPosX, p->oPosZ)) {
        o->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_yoshi_fire_blue);
    } else {
        o->oGraphNode = geo_layout_to_graph_node(NULL, omm_geo_yoshi_fire_red);
    }
}

const BehaviorScript bhvOmmYoshiFire[] = {
    OBJ_TYPE_SPECIAL,
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_yoshi_fire_update),
        BHV_ANIMATE_TEXTURE(oAnimState, 1),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_yoshi_fire(struct Object *o) {
    struct Object *fire = spawn_object(o, MODEL_NONE, bhvOmmYoshiFire);
    fire->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    fire->parentObj = o;
    return fire;
}
