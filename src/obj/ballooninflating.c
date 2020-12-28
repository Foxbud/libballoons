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
#include "aer/instance.h"
#include "aer/object.h"
#include "aer/rand.h"

#include "objects.h"
#include "sprites.h"
#include "obj/ballooninflating.h"



/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.075f;



/* ----- PRIVATE FUNCTIONS ----- */

static bool DSCreateListener(AERInstance * balloon) {
	int32_t spriteIdx;
	switch (AERRandUIntRange(0, 2)) {
		case 1:
			spriteIdx = sprites.balloonInflatingBlue;
			break;

		default:
			spriteIdx = sprites.balloonInflatingRed;
	}
	AERInstanceSetSprite(balloon, spriteIdx);
	AERInstanceSetSpriteSpeed(balloon, SPRITE_SPEED);

	return true;
}

static bool DSDestroyListener(AERInstance * balloon) {
	/* Spawn balloon inflated instance. */
	float x, y;
	AERInstanceGetPosition(balloon, &x, &y);
	AERInstance * new = AERInstanceCreate(
			objects.balloonInflated,
			x,
			y
	);

	/* Set new sprite. */
	int32_t spriteIdx = AERInstanceGetSprite(balloon);
	if (spriteIdx == sprites.balloonInflatingBlue)
		AERInstanceSetSprite(new, sprites.balloonInflatedBlue);
	else
		AERInstanceSetSprite(new, sprites.balloonInflatedRed);

	return true;
}

static bool DSAnimationEndListener(AERInstance * balloon) {
	AERInstanceDestroy(balloon);

	return true;
}



/* ----- PUBLIC FUNCTIONS ----- */

void RegisterBalloonInflatingObject(void) {
	objects.balloonInflating = AERObjectRegister(
			"BalloonInflating",
			objects.balloonBase,
			-1,
			-1,
			0,
			true,
			false,
			false
	);

	return;
}

void RegisterBalloonInflatingListeners(void) {
	AERObjectAttachCreateListener(
			objects.balloonInflating,
			DSCreateListener,
			true
	);
	AERObjectAttachDestroyListener(
			objects.balloonInflating,
			DSDestroyListener,
			true
	);
	AERObjectAttachAnimationEndListener(
			objects.balloonInflating,
			DSAnimationEndListener,
			true
	);

	return;
}
