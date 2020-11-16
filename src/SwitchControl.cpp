#include "SwitchControl.h"

#include <ESP8266HTTPClient.h>

SwitchControl::SwitchControl(String switchHost)
{
    _switchHost = switchHost;
}

String SwitchControl::Get(String command)
{
    HTTPClient client;
    String url = "http://" + _switchHost + "/command?name=" + command;
    String result;
    client.begin(url);
    int httpCode = client.GET();
    if (httpCode == 200)
    {
        result = client.getString();
    }

    client.end();
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

String SwitchControl::GetStatus()
{
    return Get("status");
}