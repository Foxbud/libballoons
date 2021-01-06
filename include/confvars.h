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
#ifndef CONFVARS_H
#define CONFVARS_H

#include <stddef.h>
#include <stdint.h>



/* ----- PUBLIC TYPES ----- */

typedef struct ConfVars {
	int64_t * keybindSpawnBalloon;
	size_t sizeKeybindSpawnBalloon;
	int64_t * keybindPopBalloons;
	size_t sizeKeybindPopBalloons;
	int64_t alarmBalloonInflatedPop;
	int64_t alarmBalloonCarcassFade;
} ConfVars;



/* ----- PUBLIC GLOBALS ----- */

extern ConfVars conf;



/* ----- PUBLIC FUNCTIONS ----- */

void ConfVarsConstructor(void);

void ConfVarsDestructor(void);



#endif /* CONFVARS_H */
