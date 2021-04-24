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
#include "aer/core.h"
#include "aer/err.h"
#include "aer/instance.h"
#include "aer/object.h"
#include "aer/sprite.h"

#include "obj/mod/ballooninflatedhitmask.h"
#include "object.h"
#include "sprite.h"

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent* event,
                           AERInstance* target,
                           AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    AERInstanceCreateModLocal(target, "hitMaskTarget", true, NULL)->i = -1;

    return true;
}

static bool DestroyListener(AEREvent* event,
                            AERInstance* target,
                            AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    AERInstanceDestroyModLocal(target, "hitMaskTarget", true);

    return true;
}

static bool PostStepListener(AEREvent* event,
                             AERInstance* target,
                             AERInstance* other) {
    /* Destroy self if balloon inflated is gone. */
    int32_t balloonId =
        AERInstanceGetModLocal(target, "hitMaskTarget", true)->i;
    aererr = AER_TRY;
    AERInstance* balloon = AERInstanceGetById(balloonId);
    if (aererr == AER_FAILED_LOOKUP) {
        AERInstanceDestroy(target);
        return false;
    }

    if (!event->handle(event, target, other))
        return false;

    /* Synchronize position with balloon inflated. */
    if (!AERGetPaused()) {
        float x = 0.0f, y = 0.0f;
        AERInstanceGetPosition(balloon, &x, &y);
        AERInstanceSetPosition(target, x, y);
    }

    return true;
}

static bool HitCollisionListener(AEREvent* event,
                                 AERInstance* target,
                                 AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    /* Pop balloon inflated. */
    aererr = AER_TRY;
    AERInstance* balloon = AERInstanceGetById(
        AERInstanceGetModLocal(target, "hitMaskTarget", true)->i);
    if (aererr == AER_OK)
        AERInstanceDestroy(balloon);

    /* Destroy self. */
    AERInstanceDestroy(target);

    return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterBalloonInflatedHitMaskObject(void) {
    objects.balloonInflatedHitMask = AERObjectRegister(
        "BalloonInflatedHitMask", AER_OBJECT_MASTERCLASS, AER_SPRITE_NULL,
        sprites.balloonInflatedHitMask, 0, false, true, false);

    return;
}

void RegisterBalloonInflatedHitMaskListeners(void) {
    AERObjectAttachCreateListener(objects.balloonInflatedHitMask,
                                  CreateListener);
    AERObjectAttachDestroyListener(objects.balloonInflatedHitMask,
                                   DestroyListener);
    AERObjectAttachPostStepListener(objects.balloonInflatedHitMask,
                                    PostStepListener);

    /* Hit collisions. */
    AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                     AER_OBJECT_ATTACKCOL,
                                     HitCollisionListener);
    AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                     AER_OBJECT_ENEMYWEAPONCOL,
                                     HitCollisionListener);
    AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                     AER_OBJECT_BULLET, HitCollisionListener);
    AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                     AER_OBJECT_ENEMYBULLET,
                                     HitCollisionListener);

    return;
}
