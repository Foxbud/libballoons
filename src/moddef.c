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
#include <stddef.h>

#include "aer/log.h"

#include "confvars.h"
#include "export.h"
#include "moddef.h"
#include "objects.h"
#include "pseudoevents.h"
#include "sprites.h"

/* ----- PRIVATE FUNCTIONS ----- */

static void ModConstructor(void) {
  AERLogInfo("Initializing mod...");

  ConfVarsConstructor();

  AERLogInfo("Done initializing mod.");
  return;
}

static void ModDestructor(void) {
  AERLogInfo("Deinitializing mod...");

  ConfVarsDestructor();

  AERLogInfo("Done deinitializing mod.");
  return;
}

/* ----- PUBLIC FUNCTIONS ----- */

MOD_EXPORT AERModDef DefineMod(void) {
  return (AERModDef){.constructor = ModConstructor,
                     .destructor = ModDestructor,
                     .regSprites = RegisterSprites,
                     .regObjects = RegisterObjects,
                     .regObjListeners = RegisterObjectListeners,
                     .roomStepListener = RoomStepListener,
                     .roomChangeListener = NULL};
}
