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

#include "aer/envconf.h"
#include "aer/err.h"
#include "aer/log.h"

#include "confvars.h"



/* ----- PUBLIC GLOBALS ----- */

ConfVars conf = {0};



/* ----- PRIVATE FUNCTIONS ----- */

static int32_t ParseInt(
		const char * name,
		int32_t defaultVal,
		int32_t minVal,
		int32_t maxVal
) {
	uint8_t result = defaultVal;

	aererr = AER_OK;
	int32_t rawVal = AEREnvConfGetInt(name);
	if (aererr == AER_FAILED_LOOKUP) {
		AERLogWarn(
				"Environment variable \"%s\" not defined. "
				"Using default value \"%d\".",
				name,
				defaultVal
		);
	} else if (aererr == AER_FAILED_PARSE) {
		AERLogErr(
				"Environment variable \"%s\" could not be parsed as an integer.",
				name
		);
		abort();
	} else if (rawVal < minVal || rawVal > maxVal) {
		AERLogErr(
				"Environment variable \"%s\" must be between "
				"\"%d\" and \"%d\" (inclusive).",
				name,
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
			"BALLOONS_KEYBIND_SPAWN_BALLOON",
			'B',
			0,
			255
	);
	conf.keybindPopBalloons = (uint8_t)ParseInt(
			"BALLOONS_KEYBIND_POP_BALLOONS",
			'P',
			0,
			255
	);

	/* Alarm indexes. */
	conf.alarmBalloonInflatedPop = (uint8_t)ParseInt(
			"BALLOONS_ALARM_BALLOONINFLATED_POP",
			0,
			0,
			11
	);
	conf.alarmBalloonCarcassFade = (uint8_t)ParseInt(
			"BALLOONS_ALARM_BALLOONCARCASS_FADE",
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
