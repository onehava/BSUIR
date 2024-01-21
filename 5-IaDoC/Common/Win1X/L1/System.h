#pragma once

#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <poclass.h>

#include <vector>
#include <optional>

namespace System {
    std::optional<SYSTEM_POWER_STATUS> power_status();
    std::vector<BATTERY_INFORMATION> batteries_info();
}