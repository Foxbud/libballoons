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

#include "aer/confman.h"
#include "aer/err.h"
#include "aer/log.h"

#include "confvars.h"



/* ----- PUBLIC GLOBALS ----- */

ConfVars conf = {0};



/* ----- PRIVATE FUNCTIONS ----- */

static int32_t ParseInt(
		const char * key,
		int32_t defaultVal,
		int32_t minVal,
		int32_t maxVal
) {
	uint8_t result = defaultVal;

	aererr = AER_OK;
	int32_t rawVal = AERConfManGetInt(key);
	if (aererr == AER_FAILED_LOOKUP) {
		AERLogWarn(
				"Configuration key \"%s\" not defined. "
				"Using default value \"%d\".",
				key,
				defaultVal
		);
	} else if (aererr == AER_FAILED_PARSE) {
		AERLogErr(
				"Configuration key \"%s\" could not be parsed as an integer.",
				key
		);
		abort();
	} else if (rawVal < minVal || rawVal > maxVal) {
		AERLogErr(
				"Configuration key \"%s\" must be between "
				"\"%d\" and \"%d\" (inclusive).",
				key,
				minVal,
				maxVal
		);
		abort();
	} else {
		result = rawVal;
	}

	return result;
}



/* ----- PUBLIC FUNCTIONS ----- */

void ConfVarsConstructor(void) {
	AERLogInfo("Initializing configuration variables...");

	/* Keybindings. */
	conf.keybindSpawnBalloon = (uint8_t)ParseInt(
			"keybind.spawn_balloon",
			'B',
			0,
			255
	);
	conf.keybindPopBalloons = (uint8_t)ParseInt(
			"keybind.pop_balloons",
			'P',
			0,
			255
	);

	/* Alarm indexes. */
	conf.alarmBalloonInflatedPop = (uint8_t)ParseInt(
			"alarm.balloon_inflated.pop",
			0,
			0,
			11
	);
	conf.alarmBalloonCarcassFade = (uint8_t)ParseInt(
			"alarm.balloon_carcass.fade",
			0,
			0,
			11
	);

	AERLogInfo("Done initializing configuration variables.");
	return;
}

void ConfVarsDestructor(void) {
	AERLogInfo("Deinitializing configuration variables...");

	AERLogInfo("Done deinitializing configuration variables.");
	return;
}
