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
#include <stdlib.h>

#include "aer/err.h"
#include "aer/instance.h"
#include "aer/log.h"
#include "aer/mre.h"
#include "aer/object.h"

#include "confvars.h"
#include "objects.h"
#include "pseudoevents.h"



/* ----- PRIVATE FUNCTIONS ----- */

void KeybindSpawnBalloonListener(void) {
	/* Get player instance. */
	AERInstance * player;
	size_t numPlayers = AERObjectGetInstances(
			AER_OBJECT_CHAR,
			1,
			&player
	);
	if (numPlayers < 1) {
		AERLogWarn("Could not locate player.");
		return;
	} else if (numPlayers > 1) {
		AERLogWarn("Cannot manually spawn balloon in co-op mode.");
		return;
	}

	/* Get player position. */
	float x, y;
	AERInstanceGetPosition(player, &x, &y);

	/* Spawn balloon inflating instance. */
	aererr = AER_OK;
	AERInstanceCreate(objects.balloonInflating, x, y);
	if (aererr == AER_OK)
		AERLogInfo("Spawned balloon at (%.1f, %.1f).", x, y);
	else
		AERLogWarn("Could not spawn balloon at (%.1f, %.1f).", x, y);

	return;
}

void KeybindPopBalloonsListener(void) {
	/* Get all balloon instances. */
	size_t numBalloons = AERObjectGetInstances(
			objects.balloonInflated,
			0,
			NULL
	);
	if (numBalloons == 0) {
		AERLogWarn("No balloons to pop.");
		return;
	}
	AERInstance ** balloons = malloc(numBalloons * sizeof(AERInstance *));
	AERObjectGetInstances(
			objects.balloonInflated,
			numBalloons,
			balloons
	);

	/* Destroy balloons. */
	for (uint32_t idx = 0; idx < numBalloons; idx++) {
		AERInstanceDestroy(balloons[idx]);
	}
	AERLogInfo(
			"Popped %zu balloon%s.",
			numBalloons,
			(numBalloons == 1) ?  "" : "s"
	);
	free(balloons);

	return;
}



/* ----- PUBLIC FUNCTIONS ----- */

void RoomStepListener(void) {
	const bool * keysPressed = AERGetKeysPressed();

	if (keysPressed[conf.keybindSpawnBalloon]) KeybindSpawnBalloonListener();
	if (keysPressed[conf.keybindPopBalloons]) KeybindPopBalloonsListener();

	return;
}
