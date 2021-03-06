#include "MyLog.h"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <cstring>

uint32_t gLogLevel = LOG_LEVEL_INFO; // default level
constexpr int PROPERTY_VALUE_MAX = 20;

const char *toStr(const int level) {
    switch (level) {
        case LOG_LEVEL_VERBOSE: return "verbose";
        case LOG_LEVEL_DEBUG:   return "debug";
        case LOG_LEVEL_INFO:    return "info";
        case LOG_LEVEL_WARNING: return "warning";
        case LOG_LEVEL_ERROR:   return "error";
        default:                return "?";
    }
}

void updateLogLevel() {
    char loglevel[PROPERTY_VALUE_MAX] = {0};
    if (getProperty(kLogsLevelProperty, loglevel, "0")) {
        gLogLevel = strtoul(loglevel, nullptr, 16);
        LOGI("update log level to : %s(%u)", toStr(gLogLevel), gLogLevel);
    }
}

// property format: property_key[space]property_value
int getProperty(const char *key, char *value, const char *defaultValue) {
    std::ifstream fin(kFileName, std::ios::binary);
    if (!fin.is_open()) {
        LOGW("Fatal: failed to open property file");
    } else {
        char prop[PROPERTY_VALUE_MAX] = {0};
        auto keySize = strlen(key);
        while (fin.getline(prop, PROPERTY_VALUE_MAX, '\n')) {
            if (!strncmp(key, prop, keySize) && strlen(prop) > keySize + 1) {
                strcpy(value, prop + keySize + 1);
                return strlen(value);
            }
        }
    }
    if (defaultValue) {
        strcpy(value, defaultValue);
        return strlen(defaultValue);
    }
    return 0;
}
