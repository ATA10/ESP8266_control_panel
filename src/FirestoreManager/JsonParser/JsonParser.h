#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <ArduinoJson.h>
#include "Project_info_panel.h"

class JsonParser {
public:
    static bool getRelayState(const String& json, const String& relayName);
};

#endif
