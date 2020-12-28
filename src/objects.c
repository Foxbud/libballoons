/*
 * Copyright 2020 the libballoons authors
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
#include "objects.h"
#include "obj/balloonbase.h"
#include "obj/ballooninflating.h"
#include "obj/ballooninflated.h"
#include "obj/balloondying.h"
#include "obj/ballooncarcass.h"



/* ----- PUBLIC GLOBALS ----- */

Objects objects = {0};



/* ----- PUBLIC FUNCTIONS ----- */

void RegisterObjects(void) {
	RegisterBalloonBaseObject();
	RegisterBalloonInflatingObject();
	RegisterBalloonInflatedObject();
	RegisterBalloonDyingObject();
	RegisterBalloonCarcassObject();

	return;
}

void RegisterObjectListeners(void) {
	RegisterBalloonBaseListeners();
	RegisterBalloonInflatingListeners();
	RegisterBalloonInflatedListeners();
	RegisterBalloonDyingListeners();
	RegisterBalloonCarcassListeners();

	return;
}
