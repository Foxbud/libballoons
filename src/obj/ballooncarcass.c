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

#include "confvars.h"
#include "objects.h"
#include "sprites.h"
#include "obj/ballooncarcass.h"



/* ----- PRIVATE CONSTANTS ----- */

static const int32_t FADE_DELAY = (int32_t)(30.0f * 60.0f); /* 60 seconds. */

static const float MAX_SPRITE_ANGLE = 20.0f;



/* ----- PRIVATE FUNCTIONS ----- */

static bool CreateListener(
		AEREventTrapIter * ctx,
		AERInstance * target,
		AERInstance * other
) {
	if (!ctx->next(ctx, target, other)) return false;

	AERInstanceSetAlarm(
			target,
			conf.alarmBalloonCarcassFade,
			FADE_DELAY
	);
	AERInstanceSetSpriteAngle(
			target,
			AERRandFloatRange(-MAX_SPRITE_ANGLE, MAX_SPRITE_ANGLE)
	);

	return true;
}

static bool FadeAlarmListener(
		AEREventTrapIter * ctx,
		AERInstance * target,
		AERInstance * other
) {
	if (!ctx->next(ctx, target, other)) return false;

	float alpha = AERInstanceGetSpriteAlpha(target);
	alpha -= 5.0f / (30.0f * 5.0f);
	if (alpha > 0.0f) {
		AERInstanceSetSpriteAlpha(target, alpha);
		AERInstanceSetAlarm(
				target,
				conf.alarmBalloonCarcassFade,
				5
		);
	} else {
		AERInstanceDestroy(target);
	}

	return true;
}



/* ----- PUBLIC FUNCTIONS ----- */

void RegisterBalloonCarcassObject(void) {
	objects.balloonCarcass = AERObjectRegister(
			"BalloonCarcass",
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

void RegisterBalloonCarcassListeners(void) {
	AERObjectAttachCreateListener(
			objects.balloonCarcass,
			CreateListener
	);
	AERObjectAttachAlarmListener(
			objects.balloonCarcass,
			conf.alarmBalloonCarcassFade,
			FadeAlarmListener
	);

	return;
}
