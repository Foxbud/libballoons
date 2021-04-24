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
#include <stdlib.h>
#include <string.h>

#include "aer/conf.h"
#include "aer/err.h"
#include "aer/log.h"

#include "option.h"

/* ----- PRIVATE GLOBALS ----- */

static const int64_t keybindSpawnBalloonDef[] = {160, 66};

static const int64_t keybindPopBalloonsDef[] = {160, 80};

/* ----- INTERNAL GLOBALS ----- */

Options opts = {0};

/* ----- PRIVATE FUNCTIONS ----- */

static bool ParseBool(const char* key, bool defaultVal) {
    bool result = defaultVal;

    aererr = AER_OK;
    bool rawVal = AERConfGetBool(key);
    if (aererr == AER_FAILED_LOOKUP) {
        AERLogInfo(
            "Configuration key \"%s\" not defined. "
            "Using default value \"%s\".",
            key, (defaultVal) ? "true" : "false");
    } else if (aererr == AER_FAILED_PARSE) {
        AERLogErr("Configuration key \"%s\" could not be parsed as a boolean.",
                  key);
        abort();
    } else {
        result = rawVal;
    }

    return result;
}

static int64_t ParseInt(const char* key,
                        int64_t defaultVal,
                        int64_t minVal,
                        int64_t maxVal) {
    int64_t result = defaultVal;

    aererr = AER_OK;
    int64_t rawVal = AERConfGetInt(key);
    if (aererr == AER_FAILED_LOOKUP) {
        AERLogInfo(
            "Configuration key \"%s\" not defined. "
            "Using default value \"%d\".",
            key, defaultVal);
    } else if (aererr == AER_FAILED_PARSE) {
        AERLogErr("Configuration key \"%s\" could not be parsed as an integer.",
                  key);
        abort();
    } else if (rawVal < minVal || rawVal > maxVal) {
        AERLogErr(
            "Configuration key \"%s\" value must be between "
            "\"%d\" and \"%d\" (inclusive).",
            key, minVal, maxVal);
        abort();
    } else {
        result = rawVal;
    }

    return result;
}

static int64_t* ParseInts(const char* key,
                          size_t defaultNum,
                          size_t minNum,
                          size_t maxNum,
                          const int64_t* defaultVal,
                          int64_t minVal,
                          int64_t maxVal,
                          size_t* actualNum) {
    /* Get number of ints. */
    aererr = AER_OK;
    size_t num = AERConfGetInts(key, 0, NULL);
    if (aererr == AER_FAILED_LOOKUP) {
        AERLogInfo(
            "Configuration key \"%s\" not defined. "
            "Using default value: [",
            key);
        for (uint32_t idx = 0; idx < defaultNum; idx++)
            AERLogInfo("\t%d,", defaultVal[idx]);
        AERLogInfo("]");
        /* Use defaults. */
        *actualNum = defaultNum;
        size_t numBytes = defaultNum * sizeof(int64_t);
        return memcpy(malloc(numBytes), defaultVal, numBytes);
    } else if (num < minNum || num > maxNum) {
        AERLogErr(
            "Configuration key \"%s\" must have between "
            "\"%d\" and \"%d\" values (inclusive).",
            key, minNum, maxNum);
        abort();
    }

    /* Get ints. */
    *actualNum = num;
    int64_t* result = malloc(num * sizeof(int64_t));
    aererr = AER_OK;
    AERConfGetInts(key, num, result);
    if (aererr == AER_FAILED_PARSE) {
        AERLogErr("Configuration key \"%s\" could not be parsed as integers.",
                  key);
        abort();
    }

    /* Validate ints. */
    for (uint32_t idx = 0; idx < num; idx++) {
        int64_t curInt = result[idx];
        if (curInt < minVal || curInt > maxVal) {
            AERLogErr(
                "Configuration key \"%s\" values must be between "
                "\"%d\" and \"%d\" (inclusive).",
                key, minVal, maxVal);
            abort();
        }
    }

    return result;
}

/* ----- INTERNAL FUNCTIONS ----- */

void OptionConstructor(void) {
    AERLogInfo("Initializing options...");

    /* Enables. */
    opts.enableKeybinds = ParseBool("enable.keybinds", true);
    opts.enableEnemyBalloons = ParseBool("enable.enemyBalloons", true);

    /* Keybindings. */
    opts.keybindSpawnBalloon = ParseInts(
        "input.keybind.spawn_balloon",
        sizeof(keybindSpawnBalloonDef) / sizeof(int64_t), 1, 5,
        keybindSpawnBalloonDef, 0, 255, &opts.sizeKeybindSpawnBalloon);
    opts.keybindPopBalloons =
        ParseInts("input.keybind.pop_balloons",
                  sizeof(keybindPopBalloonsDef) / sizeof(int64_t), 1, 5,
                  keybindPopBalloonsDef, 0, 255, &opts.sizeKeybindPopBalloons);

    /* Alarm indexes. */
    opts.alarmBalloonCarcassFade =
        ParseInt("alarm.balloon_carcass.fade", 0, 0, 11);

    AERLogInfo("Done initializing options.");
    return;
}

void OptionDestructor(void) {
    AERLogInfo("Deinitializing options...");

    free(opts.keybindSpawnBalloon);
    free(opts.keybindPopBalloons);

    AERLogInfo("Done deinitializing options.");
    return;
}
