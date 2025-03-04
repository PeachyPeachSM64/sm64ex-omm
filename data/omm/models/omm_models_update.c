#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

//
// Update animations
//

static s32 get_mario_current_anim_index() {
    struct_MarioAnimDmaTable *anim_dma_table = gMarioAnimDmaTable;
    for (s32 i = 0; i != (s32) anim_dma_table->count; ++i) {
        void *anim_addr = anim_dma_table->srcAddr + anim_dma_table->anim[i].offset;
        if (anim_addr == gMarioCurrAnimAddr) {
            return i;
        }
    }
    return -1;
}

// As we don't know the length of the table, let's hope that we'll always find the animation...
static s32 get_obj_current_anim_index(struct Object *obj) {
    if (!obj->oAnimations || !obj->oCurrAnim) {
        return -1;
    }
    for (s32 i = 0; obj->oAnimations[i] != NULL; ++i) {
        if (obj->oAnimations[i] == obj->oCurrAnim ||
            obj->oAnimations[i] == obj->oCurrAnimRef) {
            return i;
        }
    }
    return -1;
}

void omm_models_update_current_animation(void *ptr) {

    // Does the object have a model?
    struct Object *obj = (struct Object *) ptr;
    if (!obj->oGraphNode) {
        return;
    }

    // Actor index
    s32 actor_index = omm_models_get_actor_index(obj->oGraphNode->georef);
    if (actor_index == -1) {
        return;
    }

    // Gfx data
    OmmActorGfx *actor_gfx = (OmmActorGfx *) omm_array_get(gOmmActorList, ptr, actor_index);
    OmmGfxData *gfx_data = actor_gfx->gfx_data;
    if (!gfx_data) {
        return;
    }

    // Animation index
    s32 anim_index = (obj == gMarioObject ? get_mario_current_anim_index() : get_obj_current_anim_index(obj));
    if (anim_index == -1) {
        return;
    }

    // Animation data
    Animation *anim_data = NULL;

    // Retrieve the animation from the CS pack (Mario only)
    if (gOmmCsAnimations && obj == gMarioObject) {
        u32 cs_index = ((const OmmPackData *) omm_array_get(gOmmPackList, ptr, actor_gfx->pack_index))->cs_index;
        if (cs_index) {
            anim_data = omm_models_cs_get_animation(cs_index, anim_index);
        }
    }

    // Retrieve the animation from the model
    if (!anim_data && anim_index < omm_array_count(gfx_data->animation_table)) {
        anim_data = (Animation *) omm_array_get(gfx_data->animation_table, ptr, anim_index);
    }

    // Use the model custom animation
    if (anim_data) {
        obj->oCurrAnim = anim_data;
    }
}

//
// Update models
//

void omm_models_update_object(struct Object *obj) {
    if (obj->activeFlags && obj->oGraphNode) {

        // Actor index
        s32 actor_index = omm_models_get_actor_index(obj->oGraphNode->georef);
        if (actor_index != -1) {
            bool changed = false;

            // Replace the object's model and animations
            OmmActorGfx *actor_gfx = (OmmActorGfx *) omm_array_get(gOmmActorList, ptr, actor_index);
            omm_array_for_each(gOmmPackList, p_pack) {
                const OmmPackData *pack = (const OmmPackData *) p_pack->as_ptr;
                if (!pack || !pack->exists || pack->caching == OMM_MODELS_CACHING_ENABLED) continue;

                // If pack is enabled
                // load the pack's model
                if (pack->enabled) {
                    OmmGfxData *gfx_data = (
                        pack->cs_index ?
                        omm_models_cs_load(pack->cs_index, *omm_models_get_actor_names(actor_index), NULL) :
                        omm_models_load_from_binary(pack->path, omm_models_get_actor_names(actor_index), NULL)
                    );

                    // If no pack is selected for that actor or the loaded model has higher priority
                    // replace the actor's current model
                    if (gfx_data && (actor_gfx->pack_index == -1 || gfx_data->priority > actor_gfx->gfx_data->priority)) {
                        OmmDataNode_GeoLayout *geo_layout_node = (OmmDataNode_GeoLayout *) omm_array_get(gfx_data->geo_layouts, ptr, omm_array_count(gfx_data->geo_layouts) - 1);
                        actor_gfx->pack_index = i_p_pack;
                        actor_gfx->gfx_data = gfx_data;
                        actor_gfx->graph_node = (struct GraphNode *) geo_layout_to_graph_node(NULL, geo_layout_node->data);
                        actor_gfx->graph_node->georef = omm_models_get_actor_layout(actor_index);
                        actor_gfx->graph_node->noBillboard = gfx_data->disable_billboard;
                        changed = true;
                        break;
                    }
                }

                // If disabled and this pack is the one selected for that actor
                // replace the actor's model by the default one
                else if (actor_gfx->pack_index == i_p_pack) {
                    actor_gfx->pack_index = -1;
                    actor_gfx->gfx_data = NULL;
                    actor_gfx->graph_node = (struct GraphNode *) geo_layout_to_graph_node(NULL, (const GeoLayout *) omm_models_get_actor_layout(actor_index));
                    actor_gfx->graph_node->noBillboard = false;
                    changed = true;
                }
            }

            // Update object
            obj->oGraphNode = actor_gfx->graph_node;
            obj->oNodeFlags &= ~((GRAPH_RENDER_BILLBOARD | GRAPH_RENDER_CYLBOARD) * actor_gfx->graph_node->noBillboard);
            if (changed && obj->oCurrAnim && obj->oCurrAnimRef) {
                obj->oCurrAnim = (void *) obj->oCurrAnimRef;
            }
        }
    }
}

void omm_models_update() {
    if (gObjectLists && omm_array_count(gOmmPackList) != 0) {
        omm_models_load_all(NULL);
        omm_models_cs_update_current();
        for_each_object_in_all_lists(obj) {
            omm_models_update_object(obj);
        }
    }
}

void omm_models_disable_all() {
    omm_array_for_each(gOmmPackList, p_pack) {
        OmmPackData *pack = (OmmPackData *) p_pack->as_ptr;
        if (pack) {
            pack->enabled = false;
        }
    }
}
