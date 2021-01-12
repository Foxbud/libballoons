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

#include "obj/mod/ballooninflated.h"
#include "object.h"
#include "sprite.h"

/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.0375f;

static const float FRICTION = 0.1f;

static const float MAX_MOVE_SPEED = 0.2f;

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent *event, AERInstance *self,
                           AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  /* Set instance attributes. */
  AERInstanceSetFriction(self, FRICTION);
  AERInstanceSetSpriteSpeed(self, SPRITE_SPEED);

  /* Create hit mask. */
  AERInstance *mask = AERInstanceCreate(objects.balloonInflatedHitMask, 0, 0);
  AERInstanceGetModLocal(mask, "hitMaskTarget", true)->i =
      AERInstanceGetId(self);

  return true;
}

static bool DestroyListener(AEREvent *event, AERInstance *self,
                            AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  /* Spawn balloon dying instance. */
  float x, y;
  AERInstanceGetPosition(self, &x, &y);
  AERInstance *new = AERInstanceCreate(objects.balloonDying, x, y);

  /* Set new sprite. */
  int32_t spriteIdx = AERInstanceGetSprite(self);
  if (spriteIdx == sprites.balloonInflatedBlue)
    AERInstanceSetSprite(new, sprites.balloonDyingBlue);
  else
    AERInstanceSetSprite(new, sprites.balloonDyingRed);

  return true;
}

static bool StepListener(AEREvent *event, AERInstance *self,
                         AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  /* Synchronize draw depth. */
  AERInstanceSyncDepth(self);

  /* Limit speed. */
  float x, y;
  AERInstanceGetMotion(self, &x, &y);
  float speed = sqrtf(x * x + y * y);
  if (speed > MAX_MOVE_SPEED) {
    float coef = MAX_MOVE_SPEED / speed;
    AERInstanceSetMotion(self, x * coef, y * coef);
  }

  return true;
}

static bool SolidCollisionListener(AEREvent *event, AERInstance *self,
                                   AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  /* Move balloon away from center of other object. */
  float xt, yt, xo, yo;
  AERInstanceGetPosition(self, &xt, &yt);
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
  AERInstanceAddMotion(self, dx * coef, dy * coef);

  return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

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
  AERObjectAttachCollisionListener(objects.balloonInflated,
                                   AER_OBJECT_PATHFINDOBSTACLE,
                                   SolidCollisionListener);

  return;
}
