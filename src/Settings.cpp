#include "Settings.h"
#include <ArduinoJson.h>

void Settings::load(Storage storage)
{
    StaticJsonDocument<512> document;
    String serialized = storage.readFile("settings.json");
    deserializeJson(document, serialized);
    NetworkSettings::readFromJson(document);
}

void Settings::store(Storage storage)
{
    StaticJsonDocument<512> document;
    NetworkSettings::writeToJson(document);
    String serialized;
    serializeJsonPretty(document, serialized);
    storage.writeFile("settings.json", serialized);
}
