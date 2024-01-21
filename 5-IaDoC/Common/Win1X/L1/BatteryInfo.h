#pragma once

#include <windows.h>
#include <winbase.h>
#include <poclass.h>

#include <optional>
#include <ctime>
#include <string_view>

using byte = unsigned char;

enum BatteryTechnology : byte {
    NONRECHARGEABLE = 0,
    RECHARGEABLE = 1,
};

class BatteryInfo {
private:
    UCHAR _chemistry[4];
    BatteryTechnology _technology;

public:
    explicit BatteryInfo(const BATTERY_INFORMATION& bi);

    [[nodiscard]] std::string_view chemistry() const;
    [[nodiscard]] BatteryTechnology technology() const;
};

#undef byte