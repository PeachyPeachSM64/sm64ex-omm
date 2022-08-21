#undef BEGIN
#undef DELAY
#undef CALL
#undef RETURN
#undef GOTO
#undef BEGIN_REPEAT
#undef END_REPEAT
#undef END_REPEAT_CONTINUE
#undef BEGIN_LOOP
#undef END_LOOP
#undef BREAK
#undef BREAK_UNUSED
#undef CALL_NATIVE
#undef ADD_FLOAT
#undef SET_FLOAT
#undef ADD_INT
#undef SET_INT
#undef OR_INT
#undef BIT_CLEAR
#undef SET_INT_RAND_RSHIFT
#undef SET_RANDOM_FLOAT
#undef SET_RANDOM_INT
#undef ADD_RANDOM_FLOAT
#undef ADD_INT_RAND_RSHIFT
#undef CMD_NOP_1
#undef CMD_NOP_2
#undef CMD_NOP_3
#undef SET_MODEL
#undef SPAWN_CHILD
#undef DEACTIVATE
#undef DROP_TO_FLOOR
#undef SUM_FLOAT
#undef SUM_INT
#undef BILLBOARD
#undef CYLBOARD
#undef HIDE
#undef SET_HITBOX
#undef CMD_NOP_4
#undef DELAY_VAR
#undef BEGIN_REPEAT_UNUSED
#undef LOAD_ANIMATIONS
#undef ANIMATE
#undef SPAWN_CHILD_WITH_PARAM
#undef LOAD_COLLISION_DATA
#undef SET_HITBOX_WITH_OFFSET
#undef SPAWN_OBJ
#undef SET_HOME
#undef SET_HURTBOX
#undef SET_INTERACT_TYPE
#undef SET_OBJ_PHYSICS
#undef SET_INTERACT_SUBTYPE
#undef SCALE
#undef PARENT_BIT_CLEAR
#undef ANIMATE_TEXTURE
#undef DISABLE_RENDERING
#undef SET_INT_UNUSED
#undef SPAWN_WATER_DROPLET

// Bit shifts
#define _B000(a)            (_SHIFTL(a, 24,  8))
#define _BB00(a, b)         (_SHIFTL(a, 24,  8) | _SHIFTL(b, 16,  8))
#define _BBBB(a, b, c, d)   (_SHIFTL(a, 24,  8) | _SHIFTL(b, 16,  8) | _SHIFTL(c, 8,  8) | _SHIFTL(d, 0, 8))
#define _BBHH(a, b, c)      (_SHIFTL(a, 24,  8) | _SHIFTL(b, 16,  8) | _SHIFTL(c, 0, 16))
#define _B0HH(a, b)         (_SHIFTL(a, 24,  8) | _SHIFTL(b,  0, 16))
#define _HH00(a)            (_SHIFTL(a, 16, 16))
#define _HHHH(a, b)         (_SHIFTL(a, 16, 16) | _SHIFTL(b,  0, 16))
#define _WWWW(a)            ((uintptr_t) (u32) (a))
#define _UPTR(a)            ((uintptr_t)       (a))

// Commands
#define BEGIN(objList)                                                      _BB00(0x00, objList)
#define DELAY(num)                                                          _B0HH(0x01, num)
#define CALL(addr)                                                          _B000(0x02), _UPTR(addr)
#define RETURN()                                                            _B000(0x03)
#define GOTO(addr)                                                          _B000(0x04), _UPTR(addr)
#define BEGIN_REPEAT(count)                                                 _B0HH(0x05, count)
#define END_REPEAT()                                                        _B000(0x06)
#define END_REPEAT_CONTINUE()                                               _B000(0x07)
#define BEGIN_LOOP()                                                        _B000(0x08)
#define END_LOOP()                                                          _B000(0x09)
#define BREAK()                                                             _B000(0x0A)
#define BREAK_UNUSED()                                                      _B000(0x0B)
#define CALL_NATIVE(func)                                                   _B000(0x0C), _UPTR(func)
#define ADD_FLOAT(field, value)                                             _BBHH(0x0D, field, value)
#define SET_FLOAT(field, value)                                             _BBHH(0x0E, field, value)
#define ADD_INT(field, value)                                               _BBHH(0x0F, field, value)
#define SET_INT(field, value)                                               _BBHH(0x10, field, value)
#define OR_INT(field, value)                                                _BBHH(0x11, field, value)
#define BIT_CLEAR(field, value)                                             _BBHH(0x12, field, value)
#define SET_INT_RAND_RSHIFT(field, min, rshift)                             _BBHH(0x13, field, min), _HH00(rshift)
#define SET_RANDOM_FLOAT(field, min, range)                                 _BBHH(0x14, field, min), _HH00(range)
#define SET_RANDOM_INT(field, min, range)                                   _BBHH(0x15, field, min), _HH00(range)
#define ADD_RANDOM_FLOAT(field, min, range)                                 _BBHH(0x16, field, min), _HH00(range)
#define ADD_INT_RAND_RSHIFT(field, min, rshift)                             _BBHH(0x17, field, min), _HH00(rshift)
#define CMD_NOP_1(field)                                                    _BB00(0x18, field)
#define CMD_NOP_2(field)                                                    _BB00(0x19, field)
#define CMD_NOP_3(field)                                                    _BB00(0x1A, field)
#define SET_MODEL(modelID)                                                  _B0HH(0x1B, modelID)
#define SPAWN_CHILD(modelID, behavior)                                      _B000(0x1C), _WWWW(modelID), _UPTR(behavior)
#define DEACTIVATE()                                                        _B000(0x1D)
#define DROP_TO_FLOOR()                                                     _B000(0x1E)
#define SUM_FLOAT(fieldDst, fieldSrc1, fieldSrc2)                           _BBBB(0x1F, fieldDst, fieldSrc1, fieldSrc2)
#define SUM_INT(fieldDst, fieldSrc1, fieldSrc2)                             _BBBB(0x20, fieldDst, fieldSrc1, fieldSrc2)
#define BILLBOARD()                                                         _B000(0x21)
#define CYLBOARD()                                                          _B000(0x38)
#define HIDE()                                                              _B000(0x22)
#define SET_HITBOX(radius, height)                                          _B000(0x23), _HHHH(radius, height)
#define CMD_NOP_4(field, value)                                             _BBHH(0x24, field, value)
#define DELAY_VAR(field)                                                    _BB00(0x25, field)
#define BEGIN_REPEAT_UNUSED(count)                                          _BB00(0x26, count)
#define LOAD_ANIMATIONS(field, anims)                                       _BB00(0x27, field), _UPTR(anims)
#define ANIMATE(animIndex)                                                  _BB00(0x28, animIndex)
#define SPAWN_CHILD_WITH_PARAM(bhvParam, modelID, behavior)                 _B0HH(0x29, bhvParam), _WWWW(modelID), _UPTR(behavior)
#define LOAD_COLLISION_DATA(collisionData)                                  _B000(0x2A), _UPTR(collisionData)
#define SET_HITBOX_WITH_OFFSET(radius, height, downOffset)                  _B000(0x2B), _HHHH(radius, height), _HH00(downOffset)
#define SPAWN_OBJ(modelID, behavior)                                        _B000(0x2C), _WWWW(modelID), _UPTR(behavior)
#define SET_HOME()                                                          _B000(0x2D)
#define SET_HURTBOX(radius, height)                                         _B000(0x2E), _HHHH(radius, height)
#define SET_INTERACT_TYPE(type)                                             _B000(0x2F), _WWWW(type)
#define SET_OBJ_PHYSICS(radius, grav, bounce, drag, frict, buoy, _6, _7)    _B000(0x30), _HHHH(radius, grav), _HHHH(bounce, drag), _HHHH(frict, buoy), _HHHH(_6, _7)
#define SET_INTERACT_SUBTYPE(subtype)                                       _B000(0x31), _WWWW(subtype)
#define SCALE(_0, percent)                                                  _BBHH(0x32, _0, percent)
#define PARENT_BIT_CLEAR(field, flags)                                      _BB00(0x33, field), _WWWW(flags)
#define ANIMATE_TEXTURE(field, rate)                                        _BBHH(0x34, field, rate)
#define DISABLE_RENDERING()                                                 _B000(0x35)
#define SET_INT_UNUSED(field, value)                                        _BB00(0x36, field), _HHHH(0, value)
#define SPAWN_WATER_DROPLET(params)                                         _B000(0x37), _UPTR(params)
