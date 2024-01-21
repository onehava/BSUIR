#pragma once

#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>

#include <dbt.h>

#include <xstring>

class Device {
private:
    DEVINST m_devinst;
    HDEVNOTIFY m_devnotify;

    std::wstring m_hwid;
    std::wstring m_name;
    bool m_is_ejectable;

public:
    Device(PDEV_BROADCAST_DEVICEINTERFACE_W devInfo, HWND hwnd);
    Device(HDEVINFO devList, SP_DEVINFO_DATA devInfoData, HWND hwnd);

    ~Device();

    Device(const Device& other);
    bool operator==(const Device& other) const;

    [[nodiscard]] std::wstring_view name() const;
    [[nodiscard]] bool ejectable() const;

    void eject() const;
};