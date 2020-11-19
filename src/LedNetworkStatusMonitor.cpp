#include "LedNetworkStatusMonitor.h"

LedNetworkStatusMonitor::LedNetworkStatusMonitor(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    WriteToPin(false);
}

void LedNetworkStatusMonitor::StartConnectingToOtherAp()
{
    WriteToPin(true);
}

void LedNetworkStatusMonitor::ProgressConnectingToOtherAp()
{
    WriteToPin(!_currentlyOn);
}

void LedNetworkStatusMonitor::StopConnectingToOtherAp(bool succeeded)
{
    WriteToPin(false);
}

void LedNetworkStatusMonitor::StartedOwnAp()
{
    WriteToPin(true);
}

void LedNetworkStatusMonitor::WriteToPin(bool on)
{
    if (on)
        digitalWrite(_pin, LOW);
    else
        digitalWrite(_pin, HIGH);

    _currentlyOn = on;
}