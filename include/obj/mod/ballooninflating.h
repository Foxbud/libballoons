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
#ifndef BALLOONINFLATING_H
#define BALLOONINFLATING_H

#include <stdbool.h>

#include "aer/instance.h"

/* ----- INTERNAL FUNCTIONS ----- */

void BalloonInflatingSetPaused(AERInstance* target, bool paused);

void RegisterBalloonInflatingObject(void);

void RegisterBalloonInflatingListeners(void);

#endif /* BALLOONINFLATING_H */
