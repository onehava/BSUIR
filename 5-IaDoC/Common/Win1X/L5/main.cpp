#include <windows.h>
#undef max

#include <atomic>
#include <thread>

#include <iostream>

#include <io.h>
#include <fcntl.h>

#include <conio.h>

#include <vector>
#include <iterator>

#include <mutex>

#include <initguid.h>
#include <usbiodef.h>

#include "Device.h"

std::atomic<bool> g_stop_requested;

std::vector<Device> g_device_list;
std::mutex g_device_list_mutex;

LRESULT FAR WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    if(message == WM_DEVICECHANGE){
        switch(wParam){
            case DBT_DEVICEARRIVAL:
            {
                if(((PDEV_BROADCAST_DEVICEINTERFACE) lParam)->dbcc_classguid != GUID_DEVINTERFACE_USB_DEVICE){
                    break;
                }

                Device device((PDEV_BROADCAST_DEVICEINTERFACE_W) lParam, hwnd);

                if(device.name().empty())
                    break;

                std::lock_guard lock(g_device_list_mutex);
                g_device_list.push_back(device);

                std::wcout << "Device '" << device.name() << "' connected" << std::endl;
                break;
            }
            case DBT_DEVICEREMOVECOMPLETE:
            {
                Device device((PDEV_BROADCAST_DEVICEINTERFACE_W) lParam, NULL);

                if(device.name().empty())
                    break;

                std::lock_guard lock(g_device_list_mutex);
                std::erase(g_device_list, device);

                std::wcout << "Device '" << device.name() << "' disconnected" << std::endl;
                break;
            }
            case DBT_DEVICEQUERYREMOVE:
            {
                std::wcout << "Some (*) device query remove" << std::endl;
                break;
            }
            case DBT_DEVICEQUERYREMOVEFAILED:
            {
                std::wcout << "Some (*) device query remove failed" << std::endl;
                break;
            }
            default:
                break;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void init_device_list(HWND hwnd){
    std::lock_guard lock(g_device_list_mutex);

    SP_DEVINFO_DATA devInfoData = {0};
    const HDEVINFO devInfoSet = SetupDiGetClassDevsA(&GUID_DEVINTERFACE_USB_DEVICE, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    for (int i = 0; ; i++){
        devInfoData.cbSize = sizeof(devInfoData);
        if(SetupDiEnumDeviceInfo(devInfoSet, i, &devInfoData) == FALSE){
            break;
        }

        Device device(devInfoSet, devInfoData, hwnd);
        g_device_list.push_back(device);
    }
    SetupDiDestroyDeviceInfoList(devInfoSet);
}

void RunThread(){
    WNDCLASSEXW wx = {0};
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = (WNDPROC) WndProc;
    wx.lpszClassName = L"DevWndCls";
    RegisterClassExW(&wx);

    HWND hwnd = CreateWindowW(L"DevWndCls", L"DevWnd", WS_ICONIC, 0, 0, CW_USEDEFAULT, 0, 0, NULL, GetModuleHandle(NULL), NULL);

    DEV_BROADCAST_DEVICEINTERFACE_W filter = {0};
    filter.dbcc_size = sizeof(filter);
    filter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
    filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    RegisterDeviceNotificationW(hwnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);

    init_device_list(hwnd);

    MSG msg;
    while(GetMessageW(&msg, hwnd, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void print_device_list(std::vector<Device>& devices){
    if(g_device_list.empty()){
        std::wcout << "List: <empty>\n" << std::endl;
    }
    else {
        std::wcout << "List:\n";
        // копипастим позор
        std::size_t enumerate = 0;
        for(const auto &device: devices){
            std::wcout << "Device " << ++enumerate <<
                          "\n\tName: " << device.name() <<
                          "\n\tEjectable: " << std::boolalpha << device.ejectable() << std::endl;
        }
        std::wcout << std::endl;
    }
}

void work_as_prompt(){
    std::unique_lock lock(g_device_list_mutex);
    auto devices = g_device_list;
    lock.unlock();

    std::wcout << "Press:\n"
                  "\t- <n> to eject device\n"
                  "\t- '0' to update list\n"
                  "\t- 'x' to close exit\n" << std::endl;

    print_device_list(devices);

choose:
    int option = _getch();

    system("cls");

    if(option == '0'){
        return;
    }
    else if(option == 'x'){
        std::exit(0);
    }

    int number = option - '0';

    if(!(0 <= number && number <= 9)){
        goto choose;
    }

    try {
        Device device = devices.at(number - 1);

        std::wcout << "Selected device \'" << device.name() << "'\n" << std::endl;

        if(!device.ejectable()){
            std::cerr << "Error: selected device is not ejectable\n" << std::endl;
        }
        else {
            device.eject();
        }
    }
    catch(std::exception&){
        std::wcout << "Wrong device number, try again" << std::endl;
        goto choose;
    }
}

int main(){
    _setmode(_fileno(stdout), _O_U8TEXT);

    std::atexit([]{
        g_stop_requested = true;
    });

    std::thread run_thread(RunThread);

    while(!g_stop_requested){
        work_as_prompt();
    }

    run_thread.join();
}