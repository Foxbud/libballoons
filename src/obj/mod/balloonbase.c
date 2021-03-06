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

#include "obj/mod/balloonbase.h"
#include "object.h"

/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(AEREvent* event,
                           AERInstance* target,
                           AERInstance* other) {
    if (!event->handle(event, target, other))
        return false;

    AERInstanceSyncDepth(target);

    return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterBalloonBaseObject(void) {
    objects.balloonBase = AERObjectRegister(
        "BalloonBase", AER_OBJECT_MASTERCLASS, AER_SPRITE_NULL, AER_SPRITE_NULL,
        0, false, false, false);

    return;
}

void RegisterBalloonBaseListeners(void) {
    AERObjectAttachCreateListener(objects.balloonBase, CreateListener);

    return;
}
