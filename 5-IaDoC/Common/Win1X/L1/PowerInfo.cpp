#include "PowerInfo.h"

PowerInfo::PowerInfo(SYSTEM_POWER_STATUS &ps, std::vector<BATTERY_INFORMATION>& bis)
{
    _source = static_cast<PowerSource>(ps.ACLineStatus);
    _bat_saver = static_cast<BatterySaver>(ps.SystemStatusFlag);

    if(ps.BatteryLifeTime == -1)
        _bat_lifetime = std::nullopt;
    else
        _bat_lifetime = ps.BatteryLifeTime;

    if(ps.BatteryLifePercent == 255)
        _bat_percentage = std::nullopt;
    else
        _bat_percentage = ps.BatteryLifePercent;

    for(auto bi: bis)
        _bats_vec.emplace_back(bi);
}

std::vector<BatteryInfo> const & PowerInfo::batteries_info() const {
    return _bats_vec;
}

PowerSource PowerInfo::source() const {
    return _source;
}

std::optional<int> PowerInfo::battery_percentage() const {
    return _bat_percentage;
}

std::optional<time_t> PowerInfo::battery_lifetime() const {
    return _bat_lifetime;
}

BatterySaver PowerInfo::battery_saver() const {
    return _bat_saver;
}