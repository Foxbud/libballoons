/*
 * Copyright 2021 the libballoons authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "aer/object.h"
#include "aer/rand.h"
#include "aer/sprite.h"

#include "obj/mod/ballooninflating.h"
#include "object.h"
#include "sprite.h"

/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.075f;

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent* event,
                           AERInstance* target,
                           AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    /* Randomly pick sprite. */
    int32_t spriteIdx;
    switch (AERRandUIntRange(0, 2)) {
        case 1:
            spriteIdx = sprites.balloonInflatingBlue;
            break;

        default:
            spriteIdx = sprites.balloonInflatingRed;
    }
    AERInstanceSetSprite(target, spriteIdx);
    AERInstanceSetSpriteSpeed(target, SPRITE_SPEED);

    return true;
}

static bool DestroyListener(AEREvent* event,
                            AERInstance* target,
                            AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    /* Spawn balloon inflated instance. */
    float x, y;
    AERInstanceGetPosition(target, &x, &y);
    AERInstance* new = AERInstanceCreate(objects.balloonInflated, x, y);

    /* Set new sprite. */
    int32_t spriteIdx = AERInstanceGetSprite(target);
    if (spriteIdx == sprites.balloonInflatingBlue)
        AERInstanceSetSprite(new, sprites.balloonInflatedBlue);
    else
        AERInstanceSetSprite(new, sprites.balloonInflatedRed);

    return true;
}

static bool AnimationEndListener(AEREvent* event,
                                 AERInstance* target,
                                 AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    AERInstanceDestroy(target);

    return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void BalloonInflatingSetPaused(AERInstance* target, bool paused) {
    AERInstanceSetSpriteSpeed(target, SPRITE_SPEED * !paused);

    return;
}

void RegisterBalloonInflatingObject(void) {
    objects.balloonInflating = AERObjectRegister(
        "BalloonInflating", objects.balloonBase, AER_SPRITE_NULL,
        AER_SPRITE_NULL, 0, true, false, false);

    return;
}

void RegisterBalloonInflatingListeners(void) {
    AERObjectAttachCreateListener(objects.balloonInflating, CreateListener);
    AERObjectAttachDestroyListener(objects.balloonInflating, DestroyListener);
    AERObjectAttachAnimationEndListener(objects.balloonInflating,
                                        AnimationEndListener);

    return;
}
