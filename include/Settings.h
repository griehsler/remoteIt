#pragma once

#include "Storage.h"
#include <NetworkSettings.h>

class Settings : public virtual NetworkSettings
{
public:
    void load(Storage storage);
    void store(Storage storage);

private:
    String serialize();
};