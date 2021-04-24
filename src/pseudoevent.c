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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "aer/core.h"
#include "aer/err.h"
#include "aer/input.h"
#include "aer/instance.h"
#include "aer/log.h"
#include "aer/object.h"

#include "obj/mod/balloondying.h"
#include "obj/mod/ballooninflated.h"
#include "obj/mod/ballooninflating.h"
#include "object.h"
#include "option.h"
#include "pseudoevent.h"

/* ----- PRIVATE FUNCTIONS ----- */

static bool CheckKeybind(size_t keybindSize,
                         const int64_t* keybind,
                         const bool* keysPressed,
                         const bool* keysHeld) {
    uint32_t numHeld = keybindSize - 1;
    for (uint32_t idx = 0; idx < numHeld; idx++)
        if (!keysHeld[keybind[idx]])
            return false;

    return keysPressed[keybind[numHeld]];
}

static void KeybindSpawnBalloonListener(void) {
    /* Get player instance. */
    AERInstance* player = NULL;
    size_t numPlayers =
        AERInstanceGetByObject(AER_OBJECT_CHAR, true, 1, &player);
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
    aererr = AER_TRY;
    AERInstanceCreate(objects.balloonInflating, x, y);
    if (aererr == AER_OK)
        AERLogInfo("Spawned balloon at (%.1f, %.1f).", x, y);
    else
        AERLogWarn("Could not spawn balloon at (%.1f, %.1f).", x, y);

    return;
}

static void KeybindPopBalloonsListener(void) {
    /* Get all balloon instances. */
    size_t numBalloons =
        AERInstanceGetByObject(objects.balloonInflated, true, 0, NULL);
    if (numBalloons == 0) {
        AERLogWarn("No balloons to pop.");
        return;
    }
    AERInstance** balloons = malloc(numBalloons * sizeof(AERInstance*));
    AERInstanceGetByObject(objects.balloonInflated, true, numBalloons,
                           balloons);

    /* Pop balloons. */
    for (uint32_t idx = 0; idx < numBalloons; idx++)
        AERInstanceDestroy(balloons[idx]);
    AERLogInfo("Popped %zu balloon%s.", numBalloons,
               (numBalloons == 1) ? "" : "s");
    free(balloons);

    return;
}

/* ----- INTERNAL FUNCTIONS ----- */

void GameStepListener(void) {
    /* Handle bugcon integration. */
    static int32_t bugconInstId = -1;
    if (AERGetNumSteps() == 1) {
        aererr = AER_TRY;
        int32_t objIdx = AERObjectGetByName("BugconConsole");
        if (aererr == AER_OK) {
            AERLogInfo("Detected \"bugcon\" mod.");
            AERInstance* inst = NULL;
            size_t numInsts = AERInstanceGetByObject(objIdx, false, 1, &inst);
            if (numInsts != 1) {
                AERLogErr(
                    "Could not locate \"bugcon\" debug console instance! Make "
                    "sure that mod \"bugcon\" has a higher priority than this "
                    "mod.");
                abort();
            }
            bugconInstId = AERInstanceGetId(inst);
        } else {
            AERLogInfo("Did not detect \"bugcon\" mod.");
        }
    }

    /* Emulate pause event if bugcon console visibility changes. */
    bool paused = AERGetPaused();
    if (bugconInstId >= 0 && !paused) {
        AERInstance* bugconInst = AERInstanceGetById(bugconInstId);
        static bool prevVis = false;
        bool curVis = paused = AERInstanceGetVisible(bugconInst);
        if (prevVis != curVis) {
            GamePauseListener(curVis);
        }
        prevVis = curVis;
    }

    /* Handle user input. */
    if (!opts.enableKeybinds || paused)
        return;
    const bool* keysPressed = AERInputGetKeysPressed();
    const bool* keysHeld = AERInputGetKeysHeld();
    if (CheckKeybind(opts.sizeKeybindSpawnBalloon, opts.keybindSpawnBalloon,
                     keysPressed, keysHeld)) {
        KeybindSpawnBalloonListener();
    }
    if (CheckKeybind(opts.sizeKeybindPopBalloons, opts.keybindPopBalloons,
                     keysPressed, keysHeld)) {
        KeybindPopBalloonsListener();
    }

    return;
}

void GamePauseListener(bool paused) {
    size_t numInsts;
    AERInstance** insts = NULL;

    numInsts = AERInstanceGetByObject(objects.balloonInflating, true, 0, NULL);
    insts = malloc(numInsts * sizeof(AERInstance*));
    AERInstanceGetByObject(objects.balloonInflating, true, numInsts, insts);
    for (uint32_t idx = 0; idx < numInsts; idx++)
        BalloonInflatingSetPaused(insts[idx], paused);
    free(insts);

    numInsts = AERInstanceGetByObject(objects.balloonInflated, true, 0, NULL);
    insts = malloc(numInsts * sizeof(AERInstance*));
    AERInstanceGetByObject(objects.balloonInflated, true, numInsts, insts);
    for (uint32_t idx = 0; idx < numInsts; idx++)
        BalloonInflatedSetPaused(insts[idx], paused);
    free(insts);

    numInsts = AERInstanceGetByObject(objects.balloonDying, true, 0, NULL);
    insts = malloc(numInsts * sizeof(AERInstance*));
    AERInstanceGetByObject(objects.balloonDying, true, numInsts, insts);
    for (uint32_t idx = 0; idx < numInsts; idx++)
        BalloonDyingSetPaused(insts[idx], paused);
    free(insts);

    insts = NULL;
    return;
}