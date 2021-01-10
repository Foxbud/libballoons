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
#include <math.h>

#include "aer/object.h"
#include "aer/rand.h"
#include "aer/sprite.h"

#include "confvars.h"
#include "obj/ballooninflated.h"
#include "objects.h"
#include "sprites.h"

/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.0375f;

static const float FRICTION = 0.1f;

static const float MAX_MOVE_SPEED = 0.2f;

static const int32_t MAX_POP_DELAY = 3;

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREventTrapIter *ctx, AERInstance *target,
                           AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Set instance attributes. */
  AERInstanceSetFriction(target, FRICTION);
  AERInstanceSetSpriteSpeed(target, SPRITE_SPEED);

  /* Create hit mask. */
  AERInstance *mask = AERInstanceCreate(objects.balloonInflatedHitMask, 0, 0);
  AERInstanceGetModLocal(mask, "hitMaskTarget", true)->i =
      AERInstanceGetId(target);
  AERInstanceCreateModLocal(target, "hitMask", true, NULL)->i =
      AERInstanceGetId(mask);

  return true;
}

static bool DestroyListener(AEREventTrapIter *ctx, AERInstance *target,
                            AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Spawn balloon dying instance. */
  float x, y;
  AERInstanceGetPosition(target, &x, &y);
  AERInstance *new = AERInstanceCreate(objects.balloonDying, x, y);

  /* Set new sprite. */
  int32_t spriteIdx = AERInstanceGetSprite(target);
  if (spriteIdx == sprites.balloonInflatedBlue)
    AERInstanceSetSprite(new, sprites.balloonDyingBlue);
  else
    AERInstanceSetSprite(new, sprites.balloonDyingRed);

  /* Destroy hit mask. */
  AERInstanceDestroy(
      AERInstanceGetById(AERInstanceGetModLocal(target, "hitMask", true)->i));
  AERInstanceDestroyModLocal(target, "hitMask", true);

  return true;
}

static bool PopAlarmListener(AEREventTrapIter *ctx, AERInstance *target,
                             AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  AERInstanceDestroy(target);

  return true;
}

static bool StepListener(AEREventTrapIter *ctx, AERInstance *target,
                         AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Synchronize draw depth. */
  AERInstanceSyncDepth(target);

  /* Limit speed. */
  float x, y;
  AERInstanceGetMotion(target, &x, &y);
  float speed = sqrtf(x * x + y * y);
  if (speed > MAX_MOVE_SPEED) {
    float coef = MAX_MOVE_SPEED / speed;
    AERInstanceSetMotion(target, x * coef, y * coef);
  }

  return true;
}

static bool SolidCollisionListener(AEREventTrapIter *ctx, AERInstance *target,
                                   AERInstance *other) {
  if (!ctx->next(ctx, target, other))
    return false;

  /* Move balloon away from center of other object. */
  float xt, yt, xo, yo;
  AERInstanceGetPosition(target, &xt, &yt);
  AERInstanceGetPosition(other, &xo, &yo);
  float dx = xt - xo;
  float dy = yt - yo;
  /*
   * If balloon and other object are at the exact same position,
   * randomly generate motion.
   */
  while (dx == 0.0f && dy == 0.0f) {
    dx = AERRandFloatRange(-0.5f, 0.5f);
    dy = AERRandFloatRange(-0.5f, 0.5f);
  }
  float coef = MAX_MOVE_SPEED / sqrtf(dx * dx + dy * dy);
  AERInstanceAddMotion(target, dx * coef, dy * coef);

  return true;
}

/* ----- PUBLIC FUNCTIONS ----- */

void BalloonInflatedPop(AERInstance *balloon) {
  AERInstanceSetAlarm(balloon, conf.alarmBalloonInflatedPop,
                      AERRandIntRange(1, MAX_POP_DELAY + 1));

  return;
}

void RegisterBalloonInflatedObject(void) {
  objects.balloonInflated =
      AERObjectRegister("BalloonInflated", objects.balloonBase, AER_SPRITE_NULL,
                        sprites.balloonInflatedSolidMask, 0, true, true, false);

  return;
}

void RegisterBalloonInflatedListeners(void) {
  AERObjectAttachCreateListener(objects.balloonInflated, CreateListener);
  AERObjectAttachDestroyListener(objects.balloonInflated, DestroyListener);
  AERObjectAttachAlarmListener(objects.balloonInflated,
                               conf.alarmBalloonInflatedPop, PopAlarmListener);
  AERObjectAttachStepListener(objects.balloonInflated, StepListener);

  /* Solid collisions. */
  AERObjectAttachCollisionListener(
      objects.balloonInflated, objects.balloonInflated, SolidCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflated, AER_OBJECT_CHAR,
                                   SolidCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflated,
                                   AER_OBJECT_PATHFINDOBSTACLE,
                                   SolidCollisionListener);

  return;
}
