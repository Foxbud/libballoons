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

#include "aer/core.h"
#include "aer/object.h"
#include "aer/rand.h"
#include "aer/sprite.h"

#include "obj/mod/ballooninflated.h"
#include "object.h"
#include "sprite.h"

/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.0375f;

static const float FRICTION = 0.1f;

static const float MAX_MOVE_SPEED = 0.2f;

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent *event, AERInstance *target,
                           AERInstance *other) {
  if (!event->handle(event, target, other))
    return false;

  /* Set instance attributes. */
  AERInstanceSetFriction(target, FRICTION);
  AERInstanceSetSpriteSpeed(target, SPRITE_SPEED);

  /* Create hit mask. */
  AERInstance *mask = AERInstanceCreate(objects.balloonInflatedHitMask, 0, 0);
  AERInstanceGetModLocal(mask, "hitMaskTarget", true)->i =
      AERInstanceGetId(target);

  return true;
}

static bool DestroyListener(AEREvent *event, AERInstance *target,
                            AERInstance *other) {
  if (!event->handle(event, target, other))
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

  return true;
}

static bool StepListener(AEREvent *event, AERInstance *target,
                         AERInstance *other) {
  if (!event->handle(event, target, other))
    return false;

  /* Limit speed. */
  if (!AERGetPaused()) {
    float x, y;
    AERInstanceGetMotion(target, &x, &y);
    float speed = sqrtf(x * x + y * y);
    if (speed > MAX_MOVE_SPEED) {
      float coef = MAX_MOVE_SPEED / speed;
      AERInstanceSetMotion(target, x * coef, y * coef);
    }
  }

  return true;
}

static bool SolidCollisionListener(AEREvent *event, AERInstance *target,
                                   AERInstance *other) {
  if (!event->handle(event, target, other))
    return false;

  /* Move balloon away from center of other object. */
  if (!AERGetPaused()) {
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
  }

  return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void BalloonInflatedSetPaused(AERInstance *target, bool paused) {
  AERInstanceSetSpriteSpeed(target, SPRITE_SPEED * !paused);
  if (paused)
    AERInstanceSetMotion(target, 0.0f, 0.0f);

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
  AERObjectAttachStepListener(objects.balloonInflated, StepListener);

  /* Solid collisions. */
  AERObjectAttachCollisionListener(
      objects.balloonInflated, objects.balloonInflated, SolidCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflated, AER_OBJECT_CHAR,
                                   SolidCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflated, AER_OBJECT_ENEMY,
                                   SolidCollisionListener);
  AERObjectAttachCollisionListener(objects.balloonInflated,
                                   AER_OBJECT_PATHFINDOBSTACLE,
                                   SolidCollisionListener);

  return;
}
