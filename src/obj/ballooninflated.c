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
#include <math.h>

#include "aer/instance.h"
#include "aer/object.h"
#include "aer/rand.h"

#include "confvars.h"
#include "objects.h"
#include "sprites.h"
#include "obj/ballooninflated.h"



/* ----- PRIVATE CONSTANTS ----- */

static const float SPRITE_SPEED = 0.0375f;

static const float FRICTION = 0.1f;

static const float MAX_MOVE_SPEED = 0.2f;

static const int32_t MAX_POP_DELAY = 3;



/* ----- PRIVATE FUNCTIONS ----- */

static bool DSCreateListener(AERInstance * balloon) {
	AERInstanceSetFriction(balloon, FRICTION);
	AERInstanceSetSpriteSpeed(balloon, SPRITE_SPEED);

	return true;
}

static bool DSDestroyListener(AERInstance * balloon) {
	/* Spawn balloon dying instance. */
	float x, y;
	AERInstanceGetPosition(balloon, &x, &y);
	AERInstance * new = AERInstanceCreate(
			objects.balloonDying,
			x,
			y
	);

	/* Set new sprite. */
	int32_t spriteIdx = AERInstanceGetSprite(balloon);
	if (spriteIdx == sprites.balloonInflatedBlue)
		AERInstanceSetSprite(new, sprites.balloonDyingBlue);
	else
		AERInstanceSetSprite(new, sprites.balloonDyingRed);

	return true;
}

static bool DSAlarmPopListener(AERInstance * balloon) {
	AERInstanceDestroy(balloon);

	return true;
}

static bool DSStepListener(AERInstance * balloon) {
	/* Synchronize draw depth. */
	AERInstanceSyncDepth(balloon);

	/* Limit speed. */
	float x, y;
	AERInstanceGetMotion(balloon, &x, &y);
	float speed = sqrtf(x * x + y * y);
	if (speed > MAX_MOVE_SPEED) {
		float coef = MAX_MOVE_SPEED / speed;
		AERInstanceSetMotion(
				balloon,
				x * coef,
				y * coef
		);
	}

	return true;
}

static bool DSMoveCollisionListener(
		AERInstance * balloon,
		AERInstance * other
) {
	/* Move balloon away from center of other object. */
	float xb, yb, xo, yo;
	AERInstanceGetPosition(balloon, &xb, &yb);
	AERInstanceGetPosition(other, &xo, &yo);
	float dx = xb - xo;
	float dy = yb - yo;
	/*
	 * If balloon and other object are at the exact same position,
	 * randomly generate motion.
	 */
	while (dx == 0.0f && dy == 0.0f) {
		dx = AERRandFloatRange(-0.5f, 0.5f);
		dy = AERRandFloatRange(-0.5f, 0.5f);
	}
	float coef = MAX_MOVE_SPEED / sqrtf(dx * dx + dy * dy);
	AERInstanceAddMotion(
			balloon,
			dx * coef,
			dy * coef
	);

	return true;
}

static bool DSPopCollisionListener(
		AERInstance * balloon,
		AERInstance * other
) {
	(void)other;

	AERInstanceSetAlarm(
			balloon,
			conf.alarmBalloonInflatedPop,
			AERRandIntRange(1, MAX_POP_DELAY + 1)
	);

	return true;
}



/* ----- PUBLIC FUNCTIONS ----- */

void RegisterBalloonInflatedObject(void) {
	objects.balloonInflated = AERObjectRegister(
			"BalloonInflated",
			objects.balloonBase,
			-1,
			sprites.balloonInflatedSolidMask,
			0,
			true,
			true,
			false
	);

	return;
}

void RegisterBalloonInflatedListeners(void) {
	AERObjectAttachCreateListener(
			objects.balloonInflated,
			DSCreateListener,
			true
	);
	AERObjectAttachDestroyListener(
			objects.balloonInflated,
			DSDestroyListener,
			true
	);
	AERObjectAttachAlarmListener(
			objects.balloonInflated,
			conf.alarmBalloonInflatedPop,
			DSAlarmPopListener,
			true
	);
	AERObjectAttachStepListener(
			objects.balloonInflated,
			DSStepListener,
			true
	);

	/* Move collisions. */
	AERObjectAttachCollisionListener(
			objects.balloonInflated,
			objects.balloonInflated,
			DSMoveCollisionListener,
			true
	);
	AERObjectAttachCollisionListener(
			objects.balloonInflated,
			AER_OBJECT_CHAR,
			DSMoveCollisionListener,
			true
	);
	AERObjectAttachCollisionListener(
			objects.balloonInflated,
			AER_OBJECT_PATHFINDOBSTACLE,
			DSMoveCollisionListener,
			true
	);

	/* Pop collisions. */
	AERObjectAttachCollisionListener(
			objects.balloonInflated,
			AER_OBJECT_ATTACKCOL,
			DSPopCollisionListener,
			true
	);

	return;
}
