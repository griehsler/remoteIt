#include "SwitchControl.h"

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

SwitchControl::SwitchControl(String switchHost)
{
    _switchHost = switchHost;
}

int SwitchControl::Get(String command)
{
    HTTPClient client;
    WiFiClient wifiClient;
    String url = "http://" + _switchHost + "/command?name=" + command;
    String result;
    client.begin(wifiClient, url);
    int httpCode = client.GET();
    client.end();
    return httpCode;
}

void SwitchControl::On()
{
    Get("on");
}

void SwitchControl::Off()
{
    Get("off");
}

void SwitchControl::Toggle()
{
    Get("switch");
}