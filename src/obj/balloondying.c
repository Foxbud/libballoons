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

#include "objects.h"
#include "sprites.h"
#include "obj/balloondying.h"



/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.25f;



/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(
		AEREventTrapIter * event,
		AERInstance * target,
		AERInstance * other
) {
	if (!event->next(event, target, other)) return false;

	AERInstanceSetSpriteSpeed(target, SPRITE_SPEED);

	return true;
}

static bool DestroyListener(
		AEREventTrapIter * event,
		AERInstance * target,
		AERInstance * other
) {
	if (!event->next(event, target, other)) return false;

	/* Spawn balloon carcass instance. */
	float x, y;
	AERInstanceGetPosition(target, &x, &y);
	AERInstance * new = AERInstanceCreate(
			objects.balloonCarcass,
			x,
			y
	);

	/* Set new sprite. */
	int32_t spriteIdx = AERInstanceGetSprite(target);
	if (spriteIdx == sprites.balloonDyingBlue)
		AERInstanceSetSprite(new, sprites.balloonCarcassBlue);
	else
		AERInstanceSetSprite(new, sprites.balloonCarcassRed);

	return true;
}

static bool AnimationEndListener(
		AEREventTrapIter * event,
		AERInstance * target,
		AERInstance * other
) {
	if (!event->next(event, target, other)) return false;

	AERInstanceDestroy(target);

	return true;
}



/* ----- PUBLIC FUNCTIONS ----- */

void RegisterBalloonDyingObject(void) {
	objects.balloonDying = AERObjectRegister(
			"BalloonDying",
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

void RegisterBalloonDyingListeners(void) {
	AERObjectAttachCreateListener(
			objects.balloonDying,
			CreateListener
	);
	AERObjectAttachDestroyListener(
			objects.balloonDying,
			DestroyListener
	);
	AERObjectAttachAnimationEndListener(
			objects.balloonDying,
			AnimationEndListener
	);

	return;
}
