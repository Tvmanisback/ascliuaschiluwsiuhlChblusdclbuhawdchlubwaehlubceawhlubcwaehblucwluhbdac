#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT
#endif

struct Vec3 {
    float x, y, z;
};

struct GorillaState {
    Vec3 position;
    Vec3 velocity;
    Vec3 leftHand;
    Vec3 rightHand;
    Vec3 leftHandPrev;
    Vec3 rightHandPrev;
    bool leftGripping;
    bool rightGripping;
    float mass;
    float drag;
    float climbForce;
    float swingMultiplier;
    float maxSpeed;
    float gravity;
};

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void gorilla_init(GorillaState* state);
EXPORT void gorilla_set_hands(GorillaState* state, float lx, float ly, float lz, float rx, float ry, float rz);
EXPORT void gorilla_set_gripping(GorillaState* state, int left, int right);
EXPORT void gorilla_update(GorillaState* state, float dt);
EXPORT void gorilla_get_position(GorillaState* state, float* x, float* y, float* z);
EXPORT void gorilla_get_velocity(GorillaState* state, float* x, float* y, float* z);
EXPORT void gorilla_reset(GorillaState* state, float x, float y, float z);

#ifdef __cplusplus
}
#endif
