#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#define secret_ "secret_"
#define SECRET_ "SECRET_"

#define OMM_SECRET_DEFINE_(x) \
[OMM_SECRET_##x] = { \
    SECRET_ #x, \
    OMM_TEXT_SECRET_##x##_NAME, \
    OMM_TEXT_SECRET_##x##_COND, \
    OMM_TEXT_SECRET_##x##_TEXT, \
    false \
}

typedef struct {
    const char *id;
    const char *name;
    const char *cond;
    const char *text;
    bool unlocked;
} OmmSecret;

static OmmSecret sOmmSecrets[] = {
    OMM_SECRET_DEFINE_(BOWSER_THROW),
    OMM_SECRET_DEFINE_(DARK_BASEMENT),
    OMM_SECRET_DEFINE_(PEACHY_ROOM),
    OMM_SECRET_DEFINE_(FLYING_DORRIE),
    OMM_SECRET_DEFINE_(TOAD_CHEAT_CODE),
    OMM_SECRET_DEFINE_(CHEATER_FISH),
    OMM_SECRET_DEFINE_(PEACH_SECRET_1),
    OMM_SECRET_DEFINE_(PEACH_SECRET_2),
    OMM_SECRET_DEFINE_(YOSHI_SECRET),
    OMM_SECRET_DEFINE_(SMMS_SECRET),
    OMM_SECRET_DEFINE_(SM74_SECRET),
    OMM_SECRET_DEFINE_(SMSR_SECRET),
    OMM_SECRET_DEFINE_(SMGS_SECRET),
    OMM_SECRET_DEFINE_(R96_SECRET),
    // TODO: YOSHIMODE
    // OMM_SECRET_DEFINE_(YOSHI_ENDING),
    // OMM_SECRET_DEFINE_(SMSR_YOSHI_ENDING),
};

u32 omm_secrets_get_count() {
    return array_length(sOmmSecrets);
}

u32 omm_secrets_get_unlocked_count() {
    u32 count = 0;
    for (u32 secret = 0; secret != omm_secrets_get_count(); ++secret) {
        count += omm_secrets_is_unlocked(secret);
    }
    return count;
}

bool omm_secrets_is_unlocked(u32 secret) {
    if (secret < array_length(sOmmSecrets)) {
        return sOmmSecrets[secret].unlocked;
    }
    return false;
}

bool omm_secrets_get(u32 secret, const char **name, const char **cond, const char **text) {
    if (secret < array_length(sOmmSecrets)) {
        if (name) *name = sOmmSecrets[secret].name;
        if (cond) *cond = sOmmSecrets[secret].cond;
        if (text) *text = sOmmSecrets[secret].text;
        return true;
    }
    return false;
}

bool omm_secrets_read(const char *name, const char *value, UNUSED bool *invalid) {
    if (strstr(name, secret_) == name) {
        if (strcmp(value, "1") != 0) {
            return false;
        }
        str_t id;
        str_upr(id, sizeof(str_t), name);
        array_for_each_(OmmSecret, sec, sOmmSecrets) {
            if (strcmp(id, sec->id) == 0) {
                sec->unlocked = true;
                return true;
            }
        }
    }
    return false;
}

void omm_secrets_write() {
    omm_save_file_write_buffer(OMM_SAVE_FILE_SECTION_SECRETS "\n");
    array_for_each_(const OmmSecret, ach, sOmmSecrets) {
        if (ach->unlocked) {
            str_t id;
            str_lwr(id, sizeof(str_t), ach->id);
            omm_save_file_write_buffer("%s = 1\n", id);
        }
    }
    omm_save_file_write_buffer("\n");
}

void omm_secrets_reset() {
    array_for_each_(OmmSecret, sec, sOmmSecrets) {
        sec->unlocked = false;
    }
    omm_save_file_do_save();
}

bool omm_secrets_unlock(u32 secret) {
    if (secret < array_length(sOmmSecrets) && !sOmmSecrets[secret].unlocked) {
        sOmmSecrets[secret].unlocked = true;
        omm_save_file_do_save();
        omm_sound_play(OMM_SOUND_EVENT_SECRET, gGlobalSoundArgs);
    }
    return false;
}
