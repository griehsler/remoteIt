#pragma once

#include <Arduino.h>

class SwitchControl
{
public:
    SwitchControl(String switchHost);
    void On();
    void Off();
    void Toggle();
    String GetStatus();

private:
    String _switchHost;
    String Get(String command);
};