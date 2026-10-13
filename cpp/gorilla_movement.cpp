#include "gorilla_movement.h"
#include <cmath>
#include <cstring>

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static float length(const Vec3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static Vec3 sub(const Vec3& a, const Vec3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static Vec3 scale(const Vec3& v, float s) {
    return {v.x * s, v.y * s, v.z * s};
}

static Vec3 add(const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

extern "C" {

EXPORT void gorilla_init(GorillaState* state) {
    if (!state) return;
    std::memset(state, 0, sizeof(GorillaState));
    state->mass = 1.0f;
    state->drag = 0.98f;
    state->climbForce = 18.0f;
    state->swingMultiplier = 2.4f;
    state->maxSpeed = 22.0f;
    state->gravity = -18.0f;
    state->position = {0.0f, 1.6f, 0.0f};
}

EXPORT void gorilla_set_hands(GorillaState* state, float lx, float ly, float lz, float rx, float ry, float rz) {
    if (!state) return;
    state->leftHandPrev = state->leftHand;
    state->rightHandPrev = state->rightHand;
    state->leftHand = {lx, ly, lz};
    state->rightHand = {rx, ry, rz};
}

EXPORT void gorilla_set_gripping(GorillaState* state, int left, int right) {
    if (!state) return;
    state->leftGripping = left != 0;
    state->rightGripping = right != 0;
}

EXPORT void gorilla_update(GorillaState* state, float dt) {
    if (!state || dt <= 0.0f) return;

    // Hand velocity contribution (core of Gorilla Tag style movement)
    Vec3 leftDelta = sub(state->leftHand, state->leftHandPrev);
    Vec3 rightDelta = sub(state->rightHand, state->rightHandPrev);

    Vec3 force = {0, 0, 0};

    if (state->leftGripping) {
        // When gripping, hand movement pulls the body the opposite way
        force = add(force, scale(leftDelta, -state->climbForce * state->swingMultiplier));
    }
    if (state->rightGripping) {
        force = add(force, scale(rightDelta, -state->climbForce * state->swingMultiplier));
    }

    // Apply force as acceleration
    state->velocity.x += force.x * dt;
    state->velocity.y += force.y * dt;
    state->velocity.z += force.z * dt;

    // Gravity only when not firmly gripping both hands (simplified)
    if (!(state->leftGripping && state->rightGripping)) {
        state->velocity.y += state->gravity * dt;
    }

    // Drag
    state->velocity.x *= state->drag;
    state->velocity.y *= state->drag;
    state->velocity.z *= state->drag;

    // Clamp speed
    float spd = length(state->velocity);
    if (spd > state->maxSpeed) {
        float s = state->maxSpeed / spd;
        state->velocity = scale(state->velocity, s);
    }

    // Integrate position
    state->position.x += state->velocity.x * dt;
    state->position.y += state->velocity.y * dt;
    state->position.z += state->velocity.z * dt;

    // Simple ground clamp
    if (state->position.y < 0.0f) {
        state->position.y = 0.0f;
        if (state->velocity.y < 0.0f) state->velocity.y = 0.0f;
    }
}

EXPORT void gorilla_get_position(GorillaState* state, float* x, float* y, float* z) {
    if (!state) return;
    if (x) *x = state->position.x;
    if (y) *y = state->position.y;
    if (z) *z = state->position.z;
}

EXPORT void gorilla_get_velocity(GorillaState* state, float* x, float* y, float* z) {
    if (!state) return;
    if (x) *x = state->velocity.x;
    if (y) *y = state->velocity.y;
    if (z) *z = state->velocity.z;
}

EXPORT void gorilla_reset(GorillaState* state, float x, float y, float z) {
    if (!state) return;
    state->position = {x, y, z};
    state->velocity = {0, 0, 0};
    state->leftHand = state->leftHandPrev = {0, 0, 0};
    state->rightHand = state->rightHandPrev = {0, 0, 0};
    state->leftGripping = state->rightGripping = false;
}

} // extern "C"
