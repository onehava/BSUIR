#pragma once

#include <windows.h>
#include <winbase.h>

#include <vector>

#include "BatteryInfo.h"

using byte = unsigned char;

enum PowerSource : byte {
    BATTERY = 0,
    AC = 1,
    UNKNOWN = 255,
};

enum BatterySaver : byte {
    OFF = 0,
    ON = 1,
};

class PowerInfo {
private:
    PowerSource _source;
    std::optional<int> _bat_percentage;
    std::optional<int> _bat_lifetime;
    BatterySaver _bat_saver;
    std::vector<BatteryInfo> _bats_vec;

public:
    PowerInfo(SYSTEM_POWER_STATUS& ps, std::vector<BATTERY_INFORMATION>& bis);

    [[nodiscard]] PowerSource source() const;
    [[nodiscard]] std::optional<int> battery_percentage() const;
    [[nodiscard]] std::optional<time_t> battery_lifetime() const;
    [[nodiscard]] BatterySaver battery_saver() const;
    [[nodiscard]] std::vector<BatteryInfo> const & batteries_info() const;
};