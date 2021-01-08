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
#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>

/* ----- PUBLIC TYPES ----- */

typedef struct Sprites {
  int32_t balloonInflatingRed;
  int32_t balloonInflatingBlue;
  int32_t balloonInflatedRed;
  int32_t balloonInflatedBlue;
  int32_t balloonInflatedSolidMask;
  int32_t balloonDyingRed;
  int32_t balloonDyingBlue;
  int32_t balloonCarcassRed;
  int32_t balloonCarcassBlue;
} Sprites;

/* ----- PUBLIC GLOBALS ------ */

extern Sprites sprites;

/* ----- PUBLIC FUNCTIONS ----- */

void RegisterSprites(void);

#endif /* SPRITES_H */
