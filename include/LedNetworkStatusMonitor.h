#pragma once

#include <Network.h>

class LedNetworkStatusMonitor : public NetworkStatusMonitor
{
public:
    LedNetworkStatusMonitor(uint8_t pin);
    void StartConnectingToOtherAp();
    void ProgressConnectingToOtherAp();
    void StopConnectingToOtherAp(bool succeeded);
    void StartedOwnAp();

private:
    uint8_t _pin;
    bool _currentlyOn;
    void WriteToPin(bool on);
};