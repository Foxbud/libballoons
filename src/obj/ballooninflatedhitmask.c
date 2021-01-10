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
#include "aer/instance.h"
#include "aer/object.h"
#include "aer/sprite.h"

#include "obj/ballooninflated.h"
#include "obj/ballooninflatedhitmask.h"
#include "objects.h"
#include "sprites.h"

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREventTrapIter *ctx, AERInstance *target,
                           AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  AERInstanceCreateModLocal(target, "hitMaskTarget", true, NULL)->i = -1;

  return true;
}

static bool DestroyListener(AEREventTrapIter *ctx, AERInstance *target,
                            AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  AERInstanceDestroyModLocal(target, "hitMaskTarget", true);

  return true;
}

static bool PostStepListener(AEREventTrapIter *ctx, AERInstance *target,
                             AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Synchronize position with balloon inflated. */
  float x = 0.0f, y = 0.0f;
  AERInstanceGetPosition(
      AERInstanceGetById(
          AERInstanceGetModLocal(target, "hitMaskTarget", true)->i),
      &x, &y);
  AERInstanceSetPosition(target, x, y);

  return true;
}

static bool HitCollisionListener(AEREventTrapIter *ctx, AERInstance *target,
                                 AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Pop balloon inflated. */
  BalloonInflatedPop(AERInstanceGetById(
      AERInstanceGetModLocal(target, "hitMaskTarget", true)->i));

  return true;
}

/* ----- PUBLIC FUNCTIONS ----- */

void RegisterBalloonInflatedHitMaskObject(void) {
  objects.balloonInflatedHitMask = AERObjectRegister(
      "BalloonInflatedHitMask", objects.balloonBase, AER_SPRITE_NULL,
      sprites.balloonInflatedHitMask, 0, false, true, false);

  return;
}

void RegisterBalloonInflatedHitMaskListeners(void) {
  AERObjectAttachCreateListener(objects.balloonInflatedHitMask, CreateListener);
  AERObjectAttachDestroyListener(objects.balloonInflatedHitMask,
                                 DestroyListener);
  AERObjectAttachPostStepListener(objects.balloonInflatedHitMask,
                                  PostStepListener);

  /* Hit collisions. */
  AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                   AER_OBJECT_ATTACKCOL, HitCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflatedHitMask,
                                   AER_OBJECT_BULLET, HitCollisionListener);

  return;
}
