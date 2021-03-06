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
#include "object.h"
#include "obj/hld/enemy.h"
#include "obj/mod/balloonbase.h"
#include "obj/mod/ballooncarcass.h"
#include "obj/mod/balloondying.h"
#include "obj/mod/ballooninflated.h"
#include "obj/mod/ballooninflatedhitmask.h"
#include "obj/mod/ballooninflating.h"
#include "option.h"

/* ----- INTERNAL GLOBALS ----- */

Objects objects = {0};

/* ----- INTERNAL FUNCTIONS ----- */

void RegisterObjects(void) {
    RegisterBalloonBaseObject();
    RegisterBalloonInflatingObject();
    RegisterBalloonInflatedObject();
    RegisterBalloonInflatedHitMaskObject();
    RegisterBalloonDyingObject();
    RegisterBalloonCarcassObject();

    return;
}

void RegisterObjectListeners(void) {
    /* Vanilla object listeners. */
    if (opts.enableEnemyBalloons)
        RegisterEnemyListeners();

    /* Mod object listeners. */
    RegisterBalloonBaseListeners();
    RegisterBalloonInflatingListeners();
    RegisterBalloonInflatedListeners();
    RegisterBalloonInflatedHitMaskListeners();
    RegisterBalloonDyingListeners();
    RegisterBalloonCarcassListeners();

    return;
}
