#include "Device.h"

#include <initguid.h>
#include <usbiodef.h>
#include <memory>

Device::Device(const Device &other){
    this->m_hwid = other.m_hwid;
    this->m_name = other.m_name;
    this->m_is_ejectable = other.m_is_ejectable;
    this->m_devinst = other.m_devinst;
    this->m_devnotify = other.m_devnotify;
}

bool Device::operator==(const Device &other) const {
    return this->m_hwid == other.m_hwid;
}

Device::Device(PDEV_BROADCAST_DEVICEINTERFACE_W devInfo, HWND hwnd){
    HDEVINFO devList = SetupDiCreateDeviceInfoList(NULL, NULL);
    SetupDiOpenDeviceInterfaceW(devList, (LPCWSTR) devInfo->dbcc_name, NULL, NULL);

    SP_DEVINFO_DATA devInfoData = {0};
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    SetupDiEnumDeviceInfo(devList, 0, &devInfoData);

    *this = Device(devList, devInfoData, hwnd);
}

Device::Device(HDEVINFO devList, SP_DEVINFO_DATA devInfoData, HWND hwnd){
    this->m_devinst = devInfoData.DevInst;

    WCHAR temp[1024];

    ZeroMemory(temp, sizeof(temp));
    SetupDiGetDeviceRegistryPropertyW(devList, &devInfoData, SPDRP_DEVICEDESC, NULL, (BYTE*) temp, sizeof(temp), NULL);
    m_name = std::wstring(temp);

    ZeroMemory(temp, sizeof(temp));
    SetupDiGetDeviceRegistryPropertyW(devList, &devInfoData, SPDRP_HARDWAREID, NULL, (BYTE*) temp, sizeof(temp), NULL);
    m_hwid = std::wstring(temp);

    DWORD caps;
    SetupDiGetDeviceRegistryPropertyW(devList, &devInfoData, SPDRP_CAPABILITIES, NULL, (PBYTE) &caps, sizeof(caps), NULL);
    m_is_ejectable = caps & CM_DEVCAP_REMOVABLE;

    if(hwnd != NULL && m_is_ejectable){
        SP_DEVICE_INTERFACE_DATA  devIntData;
        devIntData.cbSize = sizeof(devIntData);
        SetupDiEnumDeviceInterfaces(devList, &devInfoData, &GUID_DEVINTERFACE_USB_DEVICE, 0, &devIntData);

        DWORD reqLength;
        SetupDiGetDeviceInterfaceDetailW(devList, &devIntData, NULL, 0, &reqLength, NULL);

        auto storage = std::unique_ptr<BYTE>(new BYTE[reqLength]);

        auto devIntDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W) storage.get();
        devIntDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA_W);
        SetupDiGetDeviceInterfaceDetailW(devList, &devIntData, devIntDetailData, reqLength, NULL, &devInfoData);
        auto path = std::wstring(devIntDetailData->DevicePath);

        // register handles
        HANDLE handle = CreateFileW(path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        DEV_BROADCAST_HANDLE filter = {0};
        filter.dbch_size = sizeof(filter);
        filter.dbch_devicetype = DBT_DEVTYP_HANDLE;
        filter.dbch_handle = handle;

        m_devnotify = RegisterDeviceNotification(hwnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
        CloseHandle(handle);
    }
}

Device::~Device(){
    UnregisterDeviceNotification(m_devnotify);
}

std::wstring_view Device::name() const {
    return m_name;
}

bool Device::ejectable() const {
    return m_is_ejectable;
}

void Device::eject() const {
    CM_Request_Device_EjectW(m_devinst, NULL, NULL, NULL, NULL);
}
