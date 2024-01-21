#include "System.h"

#define MAX_BATTERY_COUNT 2

std::optional<SYSTEM_POWER_STATUS> Console::power_status(){
    SYSTEM_POWER_STATUS status;

    if(!GetSystemPowerStatus(&status)){
        return std::nullopt;
    }
    return status;
}

std::vector<BATTERY_INFORMATION> Console::batteries_info(){
    HDEVINFO hdev =
            SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY,
                                nullptr,
                                nullptr,
                                DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    std::vector<BATTERY_INFORMATION> batteries;

    if(hdev != INVALID_HANDLE_VALUE)
    {
        for (int idev = 0; idev < MAX_BATTERY_COUNT; idev++)
        {
            SP_DEVICE_INTERFACE_DATA did = {0};
            did.cbSize = sizeof(SP_DEVINFO_DATA);

            if(SetupDiEnumDeviceInterfaces(hdev,
                                           nullptr,
                                           &GUID_DEVCLASS_BATTERY,
                                           idev,
                                           &did))
            {
                DWORD cbRequired = 0;

                SetupDiGetDeviceInterfaceDetail(hdev,
                                                &did,
                                                nullptr,
                                                0,
                                                &cbRequired,
                                                nullptr);

                if(GetLastError() == ERROR_INSUFFICIENT_BUFFER){
                    auto pdidd = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(LocalAlloc(LPTR, cbRequired));

                    if(pdidd){
                        pdidd->cbSize = sizeof(*pdidd);

                        if (SetupDiGetDeviceInterfaceDetail(hdev,
                                                            &did,
                                                            pdidd,
                                                            cbRequired,
                                                            &cbRequired,
                                                            nullptr))
                        {
                            HANDLE hBattery =
                                    CreateFile(pdidd->DevicePath,
                                               GENERIC_READ | GENERIC_WRITE,
                                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                                               nullptr,
                                               OPEN_EXISTING,
                                               FILE_ATTRIBUTE_NORMAL,
                                               nullptr);

                            if(hBattery != INVALID_HANDLE_VALUE){
                                BATTERY_QUERY_INFORMATION bqi = {0};
                                DWORD dwWait = 0;
                                DWORD dwOut;

                                if (DeviceIoControl(hBattery,
                                                    IOCTL_BATTERY_QUERY_TAG,
                                                    &dwWait,
                                                    sizeof(dwWait),
                                                    &bqi.BatteryTag,
                                                    sizeof(bqi.BatteryTag),
                                                    &dwOut,
                                                    nullptr)
                                    && bqi.BatteryTag)
                                {
                                    BATTERY_INFORMATION bi = {0};
                                    bqi.InformationLevel = BatteryInformation;

                                    if(DeviceIoControl(hBattery,
                                                       IOCTL_BATTERY_QUERY_INFORMATION,
                                                       &bqi,
                                                       sizeof(bqi),
                                                       &bi,
                                                       sizeof(bi),
                                                       &dwOut,
                                                       nullptr))
                                    {
                                        batteries.push_back(bi);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if(GetLastError() == ERROR_NO_MORE_ITEMS){
                break;
            }
        }
    }

    return batteries;
}