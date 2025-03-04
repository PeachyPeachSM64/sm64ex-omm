#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#include "behavior_commands.h"

//
// Behavior
//

static void bhv_omm_afterimage_update() {
    struct Object *o = gCurrentObject;
    if (o->oTimer >= o->oShockwaveDuration) {
        obj_mark_for_deletion(o);
        return;
    }
    f32 t = (f32) o->oTimer / (f32) o->oShockwaveDuration;
    f32 scale = lerp_f(t, o->oShockwaveScaleMin, o->oShockwaveScaleMax);
    u8 opacity = (u8) lerp_f(t, o->oShockwaveAlphaMax, o->oShockwaveAlphaMin);
    vec3f_set(o->oGfxScale, o->oVelX * scale, o->oVelY * scale, o->oVelZ * scale);
    o->oTransparency = 0xFF - opacity;
    o->oOpacity = opacity;
}

const BehaviorScript bhvOmmAfterimage[] = {
    OBJ_TYPE_UNIMPORTANT,
    BHV_OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BHV_BEGIN_LOOP(),
        BHV_CALL_NATIVE(bhv_omm_afterimage_update),
    BHV_END_LOOP()
};

//
// Spawner
//

struct Object *omm_obj_spawn_afterimage(struct Object *o, f32 scaleMin, f32 scaleMax, u8 alphaMin, u8 alphaMax, s32 duration) {
    struct Object *image = spawn_object(o, MODEL_NONE, bhvOmmAfterimage);
    image->oGraphNode = o->oGraphNode;
    image->oAnimations = o->oAnimations;
    image->oAnimInfo = o->oAnimInfo;
    image->oGraphYOffset = 0;
    obj_anim_set_speed(image, 0.f);
    obj_set_pos_vec3f(image, o->oGfxPos);
    obj_set_angle_vec3s(image, o->oGfxAngle);
    obj_set_scale_vec3f(image, o->oGfxScale);
    vec3f_mul(image->oGfxScale, scaleMin);
    vec3f_copy(&image->oVelX, o->oGfxScale);
    image->oShockwaveScaleMin = scaleMin;
    image->oShockwaveScaleMax = scaleMax;
    image->oShockwaveAlphaMin = alphaMin;
    image->oShockwaveAlphaMax = alphaMax;
    image->oShockwaveDuration = duration;
    image->oTransparency = 0xFF - alphaMax;
    image->oOpacity = alphaMax;
    image->oFlags |= (OBJ_FLAG_NO_SHADOW | OBJ_FLAG_DONT_RENDER_ON_INIT);
    return image;
}
