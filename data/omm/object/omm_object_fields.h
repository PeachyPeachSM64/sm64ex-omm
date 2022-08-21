#ifndef OMM_OBJECT_FIELDS_H
#define OMM_OBJECT_FIELDS_H

#include "types.h"
#include "object_fields.h"

struct OmmData {
    void (*reset)(void);
    void (*resetMario)(void);
    void (*resetObject)(void);
    void (*update)(void);
    void (*updateMario)(void);
    void (*updateObject)(void);

    // Mario data
    struct {

        // State data
        struct {
            s32 ticks;
            s32 coins;
            s32 o2;
            s32 airCombo;
            f32 peakHeight;
            void *poleObject;
            struct {
                s32 dmg;
                s32 gfx;
                s32 sfx;
            } freeze;
            struct {
                Vec3f pos;
                Vec4f vel;
                Vec3s angle;
            } previous;
        } state;

        // Grab
        struct {
            struct Object *obj;
        } grab;

        // Peach-only data
        struct {
            bool floated;
            s32 floatTimer;
            s32 vibeType;
            s32 vibeGauge;
            s32 vibeTimer;
            s32 joySpinYaw;
            s32 perryCharge;
            bool perryBlast;
            struct Object *perry;
        } peach;

        // Wall-slide moves data
        struct {
            bool jumped;
            s16 angle;
            f32 height;
        } wallSlide;

        // Spin moves data
        struct {
            s16 yaw;
            s32 timer;
            s32 decel;
        } spin;

        // Midair spin move data
        struct {
            s32 timer;
            s32 counter;
        } midairSpin;

        // Metal-water moves data
        struct {
            s32 punchType;
            u32 jumpNext;
            s32 jumpTimer;
        } metalWater;

        // Cappy data
        struct {
            bool bounced;
            struct Object *cappy;
        } cappy;
    
        // Capture data
        struct {
            const void *data;
            struct Object *obj;
            struct Object *prev;
            Vec3f animPos[3];
            s32 timer;
            s32 lockTimer;
            f32 stickX;   // [-1, +1], positive is right
            f32 stickY;   // [-1, +1], positive is up
            f32 stickMag; // [0, 1]
            s16 stickYaw; // intended yaw
            u16 buttonPressed;
            u16 buttonDown;
#if OMM_CODE_DEBUG
            f32 hitboxRadius;
            f32 hitboxHeight;
            f32 hitboxOffset;
            f32 hitboxWall;
#endif
        } capture;

        // Sparkly stars data
        struct {
            s32 mode;
            s32 ending;
            s32 cheats[4];
            bool starBlock;
            bool grandStar;
            bool gamePaused;
            bool timeStop;
            bool transition;
            bool marioUpdated;
            bool cheatDetected;
        } sparkly;
    } mario[1];

    // Object data
    struct {

        // State data
        struct {
            u32 properties;
            bool actionFlag;
            s32 actionState;
            s32 actionTimer;
            s32 squishTimer;
            s32 bullyTimer;
            s32 invincTimer;
            f32 walkDistance;
            Vec3f initialPos;
            bool camBehindMario;
        } state;

        // Cappy data
        struct {
            bool copyGfx;
            Vec3f offset;
            Vec3s angle;
            f32 scale;
        } cappy;

        // Per object data
        union {

            // Goomba
            struct {
                struct Object *stackObj[16];
                s32 stackCount;
            } goomba;

            // Koopa
            struct {
                f32 scale;
            } koopa;

            // Chain chomp
            struct {
                bool isFreed;
                bool isBiting;
            } chain_chomp;

            // Bullet bill
            struct {
                f32 scale;
            } bullet_bill;

            // Snowman's body
            struct {
                f32 peakHeight;
                bool headFound;
            } snowmans_body;

            // Boo
            struct {
                f32 gfxOffsetY;
            } boo;

            // Toad
            struct {
                s16 dialogId;
                s32 spawnStar;
            } toad;

            // Flaming bob-omb
            struct {
                s32 interactedTimer;
                bool interactedFire;
                bool captureDuringAscent;
            } flaming_bobomb;

            // Swoop
            struct {
                s32 ceilingType;
                union {
                    struct {
                        struct Surface *s;
                        f32 height;
                    } surface;
                    struct {
                        struct Object *o;
                        Vec3f pos;
                        Vec3f angle;
                        Vec3f scale;
                    } object;
                } ceiling;
            } swoop;

            // Motos
            struct {
                struct Object *heldObj;
            } motos;
        };
    } object[1];
};
extern struct OmmData *gOmmData;
#define gOmmMario      gOmmData->mario
#define gOmmPeach    (&gOmmData->mario->peach)
#define gOmmCappy      gOmmData->mario->cappy.cappy
#define gOmmPerry      gOmmData->mario->peach.perry
#define gOmmCapture    gOmmData->mario->capture.obj
#define gOmmObject     gOmmData->object

#define oFields             OBJECT_FIELD_OBJ(0x48)
#define oGfxInited          oFields->OBJECT_FIELD_U32(0x00)
#define oGeoData            oFields->OBJECT_FIELD_VPTR(0x01)
#define oBhvPointer         oFields->OBJECT_FIELD_CVPTR(0x02)
#define oBhvCommand         oFields->OBJECT_FIELD_CVPTR(0x03)
#define oBhvStackIndex      oFields->OBJECT_FIELD_U32(0x04)
#define oBhvTypes           oFields->OBJECT_FIELD_U32(0x05)
#define oSafeStepInited     oFields->OBJECT_FIELD_U32(0x06)
#define oSafeStepIgnore     oFields->OBJECT_FIELD_U32(0x07)
#define oSafeStepHeight     oFields->OBJECT_FIELD_F32(0x08)
#define oSafeStepIndex      oFields->OBJECT_FIELD_S32(0x09)
#define oSafeStepCoordA     oFields->OBJECT_FIELD_F32(0x0A)
#define oSafeStepCoordB     oFields->OBJECT_FIELD_F32(0x0B)
#define oSafeStepCoordC     oFields->OBJECT_FIELD_F32(0x0C)
#define oSafeStepCoords     oSafeStepCoordA

#ifndef OBJECT_FIELDS_INDEX_DIRECTLY
extern struct Object gOmmObjFields[OBJECT_POOL_CAPACITY];
OMM_INLINE bool obj_alloc_fields(struct Object *o) {
    u32 objSlot = (u32) (o - gObjectPool);
    return (objSlot < OBJECT_POOL_CAPACITY) && (o->oFields || (o->oFields = (struct Object *) omm_zero(gOmmObjFields + objSlot, sizeof(struct Object))) != NULL);
}
#endif

#endif // OMM_OBJECT_FIELDS_H
