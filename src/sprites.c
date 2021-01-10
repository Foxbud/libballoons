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
#include "aer/sprite.h"

#include "sprites.h"

/* ----- PUBLIC GLOBALS ----- */

Sprites sprites = {0};

/* ----- PUBLIC FUNCTIONS ----- */

void RegisterSprites(void) {
  sprites.balloonInflatingRed = AERSpriteRegister(
      "BalloonInflatingRed", "sprites/balloon/red_inflating.png", 6, 6, 40);
  sprites.balloonInflatingBlue = AERSpriteRegister(
      "BalloonInflatingBlue", "sprites/balloon/blue_inflating.png", 6, 6, 40);
  sprites.balloonInflatedRed = AERSpriteRegister(
      "BalloonInflatedRed", "sprites/balloon/red_inflated.png", 6, 6, 40);
  sprites.balloonInflatedBlue = AERSpriteRegister(
      "BalloonInflatedBlue", "sprites/balloon/blue_inflated.png", 6, 6, 40);
  sprites.balloonInflatedSolidMask =
      AERSpriteRegister("BalloonInflatedSolidMask",
                        "sprites/balloon/inflated_solid_mask.png", 1, 6, 2);
  sprites.balloonInflatedHitMask =
      AERSpriteRegister("BalloonInflatedHitMask",
                        "sprites/balloon/inflated_hit_mask.png", 1, 6, 40);
  sprites.balloonDyingRed = AERSpriteRegister(
      "BalloonDyingRed", "sprites/balloon/red_dying.png", 3, 6, 40);
  sprites.balloonDyingBlue = AERSpriteRegister(
      "BalloonDyingBlue", "sprites/balloon/blue_dying.png", 3, 6, 40);
  sprites.balloonCarcassRed = AERSpriteRegister(
      "BalloonCarcassRed", "sprites/balloon/red_carcass.png", 1, 6, 2);
  sprites.balloonCarcassBlue = AERSpriteRegister(
      "BalloonCarcassBlue", "sprites/balloon/blue_carcass.png", 1, 6, 2);

  return;
}
