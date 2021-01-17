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

#include "obj/hld/enemy.h"
#include "object.h"

/* ----- PRIVATE FUNCTIONS ----- */

static bool DestroyListener(AEREvent *event, AERInstance *target,
                            AERInstance *other) {
  if (!event->handle(event, target, other))
    return false;

  /* Get enemy position. */
  float x, y;
  AERInstanceGetPosition(target, &x, &y);

  /* Spawn balloon instance. */
  AERInstanceCreate(objects.balloonInflating, x, y);

  return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterEnemyListeners(void) {
  AERObjectAttachCreateListener(AER_OBJECT_ENEMY, DestroyListener);

  return;
}
