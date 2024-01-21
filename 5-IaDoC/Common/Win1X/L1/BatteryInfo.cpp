#include "BatteryInfo.h"

BatteryInfo::BatteryInfo(const BATTERY_INFORMATION &bi)
    : _technology(static_cast<BatteryTechnology>(bi.Technology)),
      _chemistry{bi.Chemistry[0], bi.Chemistry[1], bi.Chemistry[2], bi.Chemistry[3]}{}

std::string_view BatteryInfo::chemistry() const {
    return {reinterpret_cast<const char*>(_chemistry), sizeof(_chemistry)};
}

BatteryTechnology BatteryInfo::technology() const {
    return _technology;
}