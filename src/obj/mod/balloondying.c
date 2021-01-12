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

#include "obj/mod/balloondying.h"
#include "object.h"
#include "sprite.h"

/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.25f;

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent *event, AERInstance *self,
                           AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  AERInstanceSetSpriteSpeed(self, SPRITE_SPEED);

  return true;
}

static bool DestroyListener(AEREvent *event, AERInstance *self,
                            AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  /* Spawn balloon carcass instance. */
  float x, y;
  AERInstanceGetPosition(self, &x, &y);
  AERInstance *new = AERInstanceCreate(objects.balloonCarcass, x, y);

  /* Set new sprite. */
  int32_t spriteIdx = AERInstanceGetSprite(self);
  if (spriteIdx == sprites.balloonDyingBlue)
    AERInstanceSetSprite(new, sprites.balloonCarcassBlue);
  else
    AERInstanceSetSprite(new, sprites.balloonCarcassRed);

  return true;
}

static bool AnimationEndListener(AEREvent *event, AERInstance *self,
                                 AERInstance *other) {
  if (!event->handle(event, self, other))
    return false;

  AERInstanceDestroy(self);

  return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterBalloonDyingObject(void) {
  objects.balloonDying =
      AERObjectRegister("BalloonDying", objects.balloonBase, AER_SPRITE_NULL,
                        AER_SPRITE_NULL, 0, true, false, false);

  return;
}

void RegisterBalloonDyingListeners(void) {
  AERObjectAttachCreateListener(objects.balloonDying, CreateListener);
  AERObjectAttachDestroyListener(objects.balloonDying, DestroyListener);
  AERObjectAttachAnimationEndListener(objects.balloonDying,
                                      AnimationEndListener);

  return;
}
