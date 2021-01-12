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
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

/* ----- INTERNAL TYPES ----- */

typedef struct Objects {
  int32_t balloonBase;
  int32_t balloonInflating;
  int32_t balloonInflated;
  int32_t balloonInflatedHitMask;
  int32_t balloonDying;
  int32_t balloonCarcass;
} Objects;

/* ----- INTERNAL GLOBALS ------ */

extern Objects objects;

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterObjects(void);

void RegisterObjectListeners(void);

#endif /* OBJECT_H */
