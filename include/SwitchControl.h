#pragma once

#include <Arduino.h>

class SwitchControl
{
public:
    SwitchControl(String switchHost);
    void On();
    void Off();
    void Toggle();

private:
    String _switchHost;
    int Get(String command);
};