#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <windows.h>
#include <ctime>
#include <iomanip>

#include <chrono>
#include <mutex>
#include <thread>

#include <setupapi.h>
#include <devguid.h>

#include <io.h>
#include <fcntl.h>

HHOOK g_hook_keyboard;
HWND g_hwnd;

std::atomic<bool> g_stop_requested(false);
bool g_permit(false);
bool g_hidden(false);

std::condition_variable g_cv;
std::mutex g_cvm;

LRESULT keyboard_hook_hd(int nCode, WPARAM wParam, LPARAM lParam){
    auto* ks = (KBDLLHOOKSTRUCT*)lParam;

    if(ks->vkCode == 0x1B)
    {
        std::exit(0);
    }
    if(ks->vkCode == 0x50 && (ks->flags & 0x80) == 0)
    {
        g_permit ^= true;
        g_cv.notify_one();
    }
    if(ks->vkCode == 0x48 && (ks->flags & 0x80) == 0)
    {
        if((g_hidden ^= true))
            ShowWindow(g_hwnd, SW_HIDE);
        else
            ShowWindow(g_hwnd, SW_SHOW);
    }

    return CallNextHookEx(g_hook_keyboard, nCode, wParam, lParam);
}

void prepare_window(){
    auto style = GetWindowLong(g_hwnd, GWL_STYLE);
    style &= ~(WS_VISIBLE);
    SetWindowLong(g_hwnd, GWL_STYLE, style);
    ShowWindow(g_hwnd, SW_SHOW);
}

void enumerate_devices(){
    HDEVINFO devInfo = SetupDiGetClassDevsA(&GUID_DEVCLASS_CAMERA, "USB", nullptr, DIGCF_PRESENT);

    if(devInfo == INVALID_HANDLE_VALUE){
        std::cerr << "Failed to get USB camera devices" << std::endl;
        return;
    }

    SP_DEVINFO_DATA devInfoData;
    wchar_t name[256];
    wchar_t mfg[256];
    wchar_t instanceId[1024];

    for(int i = 0;; i++){
        devInfoData.cbSize = sizeof(devInfoData);

        if(SetupDiEnumDeviceInfo(devInfo, i, &devInfoData) == FALSE){
            break;
        }

        SetupDiGetDeviceInstanceIdW(devInfo, &devInfoData, (PWSTR) instanceId, sizeof(instanceId), nullptr);
        SetupDiGetDeviceRegistryPropertyW(devInfo, &devInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE) name, sizeof(name), NULL);
        SetupDiGetDeviceRegistryPropertyW(devInfo, &devInfoData, SPDRP_MFG, NULL, (PBYTE) mfg, sizeof(mfg), NULL);

        std::wcout << L"Camera device information:"
                      L"\nVendor name: " << mfg <<
                      L"\nDevice name: " << name <<
                      L"\nInstance ID: " << instanceId << '\n' << std::endl;

        SetupDiDeleteDeviceInfo(devInfo, &devInfoData);
    }

    SetupDiDestroyDeviceInfoList(devInfo);
}

std::string get_timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H-%M-%S");
    return ss.str();
}

void camera_capture(){
    cv::VideoCapture capture(0);
    capture.set(3, 1920);
    capture.set(3, 1080);

    if(!capture.isOpened()){
        std::cerr << "Failed to open camera capture" << std::endl;
        std::exit(1);
    }

    while(!g_stop_requested){
        // не атомик
        if(!g_permit){
            std::wcout << "Wait capture permission" << std::endl;
            // ну и что?
            std::unique_lock<std::mutex> lock(g_cvm);
            g_cv.wait(lock, []{ return g_permit; });
        }

        cv::Mat frame;
        capture >> frame;

        std::string name = get_timestamp() + ".jpg";

        if(!cv::imwrite(name, frame)){
            std::cerr << "Failed to save captured frame" << std::endl;
        }
        else {
            std::wcout << "Captured frame '" << name.c_str() << "'" << std::endl;
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5000ms);
    }
}

int main(){
    g_hwnd = GetForegroundWindow();
    g_hook_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_hd, GetModuleHandle(NULL), 0);

    _setmode(_fileno(stdout), _O_U8TEXT);

    prepare_window();
    enumerate_devices();

    std::atexit([]{
        g_stop_requested = true;
    });

    std::thread capture_thread(camera_capture);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    capture_thread.join();
}