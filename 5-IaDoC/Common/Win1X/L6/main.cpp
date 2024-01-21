#include <windows.h>
#undef max
#include <bluetoothapis.h>

#include <ws2bth.h>
#include <winsock.h>

#include <vector>
#include <shared_mutex>

#include <fstream>
#include <filesystem>

#include <ios>
#include <iostream>
#include <conio.h>

class BluetoothDevice {
private:
    std::wstring m_name;
    uint64_t m_address;

public:
    BluetoothDevice(){
        m_address = BLUETOOTH_NULL_ADDRESS;
    }

    explicit BluetoothDevice(BLUETOOTH_DEVICE_INFO deviceInfo){
        m_name = deviceInfo.szName;
        m_address = deviceInfo.Address.ullLong;
    }

    [[nodiscard]] std::wstring_view name() const {
        return m_name;
    }

    [[nodiscard]] uint64_t address() const {
        return m_address;
    }
};

std::vector<BluetoothDevice> get_bluetooth_devices()
{
    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };
    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };

    searchParams.fReturnAuthenticated = TRUE;
    searchParams.fReturnRemembered = FALSE;
    searchParams.fReturnConnected = TRUE;
    searchParams.fReturnUnknown = TRUE;
    searchParams.fIssueInquiry = TRUE;
    searchParams.cTimeoutMultiplier = 5;

    HBLUETOOTH_RADIO_FIND hRadioFind;
    BLUETOOTH_FIND_RADIO_PARAMS radioParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HBLUETOOTH_RADIO_FIND hRadio = BluetoothFindFirstRadio(&radioParams, &hRadioFind);

    std::vector<BluetoothDevice> devices;

    if(hRadio != nullptr){
        do {
            HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);
            if(hFind != nullptr){
                do {
                    devices.emplace_back(deviceInfo);
                }
                while(BluetoothFindNextDevice(hFind, &deviceInfo));
                BluetoothFindDeviceClose(hFind);
            }
        }
        while(BluetoothFindNextRadio(&hRadioFind, &hRadio));
        BluetoothFindRadioClose(hRadio);
    }

    return devices;
}

#define error(msg)                  \
    std::cerr << msg << std::endl;  \
    closesocket(sock);              \
    return 1

#ifdef LOG
    #define log(msg) std::cout << msg << std::endl
#else
    #define log(msg)
#endif

void print_progressbar(const char* head, std::size_t x, std::size_t n){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    COORD cursorPosition = consoleInfo.dwCursorPosition;

    std::cout << std::string(consoleInfo.dwSize.X, ' ');
    SetConsoleCursorPosition(hConsole, {0, cursorPosition.Y});

    std::size_t percentage = 100 * x / n;
    std::cout << head << ' ' << x << '/' << n << " bytes (" << percentage << "%)";
    std::cout.flush();
}

int transmit(unsigned long long address, unsigned long port, const char* path){
    SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

    SOCKADDR_BTH bthaddr = {0};
    bthaddr.addressFamily = AF_BTH;
    bthaddr.btAddr = address;
    bthaddr.port = port;

    if(connect(sock, (SOCKADDR*) &bthaddr, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR){
        error("Failed to connect socket");
    }
    log("> Socket connect");

    std::ifstream input(path, std::ios::binary);
    if(!input.is_open()){
        error("Failed to open input stream");
    }
    log("> Input stream open");

    std::string name = std::filesystem::path(path).filename().string();
    std::size_t name_size = name.length();
    std::size_t data_size = std::filesystem::file_size(path);

    if(send(sock, reinterpret_cast<const char*>(&name_size), sizeof(name_size), 0) != sizeof(name_size)
    || send(sock, name.c_str(), static_cast<int>(name_size), 0) != name_size
    || send(sock, reinterpret_cast<const char*>(&data_size), sizeof(data_size), 0) != sizeof(data_size))
    {
        error("Failed to send <name_size>, <name> or <data_size>");
    }
    log("> Transmit <name_size> = " << name_size << ", <name> = " << name << ", <data_size> = " << data_size);

    constexpr std::size_t BUFSIZE = 1024;
    char buffer[BUFSIZE];

    std::size_t total(0);
    std::streamsize prepared;
    while(!input.eof()){
        input.read(buffer, BUFSIZE);
        prepared = input.gcount();

        if(prepared > 0){
            if(send(sock, buffer, static_cast<int>(prepared), 0) != prepared){
                error("Failed to send buffer");
            }
            total += prepared;
            print_progressbar("Transmit", total, data_size);
        }
    }

    if(total != data_size){
        error("Failed to transmit all data");
    }
    else {
        std::cout << std::endl;
    }

    closesocket(sock);
    input.close();
    return 0;
}

int receive(unsigned long port){
    SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

    SOCKADDR_BTH bthaddr = {0};
    bthaddr.addressFamily = AF_BTH;
    bthaddr.btAddr = 0;
    bthaddr.port = port;

    if(bind(sock, (SOCKADDR*)&bthaddr, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR){
        error("Failed to bind socket");
    }
    log("> Socket bind");

    if(listen(sock, 1) == SOCKET_ERROR){
        error("Failed to listen socket");
    }
    log("> Socket listen");

    int bthaddr_size = sizeof(SOCKADDR_BTH);
    SOCKET connection = accept(sock, (SOCKADDR*)&bthaddr, &bthaddr_size);

    if(connection == INVALID_SOCKET){
        error("Failed to accept connection");
    }
    log("> Connection accept 0x" << std::hex << bthaddr.btAddr << std::resetiosflags(std::ios::hex));

    std::string name;
    std::size_t name_size;
    std::size_t data_size; // не используется; возможно использовать при многофайловой отправке

    if(recv(connection,  reinterpret_cast<char*>(&name_size), sizeof(name_size), 0) != sizeof(name_size) ||
       (name.resize(name_size), recv(connection, name.data(), static_cast<int>(name_size), 0) != name_size) ||
       recv(connection, reinterpret_cast<char*>(&data_size), sizeof(data_size), 0) != sizeof(data_size))
    {
        error("Failed to receive <name_size>, <name> or <data_size>");
    }
    log("> Receive <name_size> = " << name_size << ", <name> = " << name << ", <data_size> = " << data_size);

    constexpr std::size_t BUFSIZE = 1024;
    char buffer[BUFSIZE];

    std::ofstream output(name, std::ios::binary);
    if(!output.is_open()){
        error("Failed to open output stream");
    }
    log("> Open output stream");

    std::size_t total(0);
    std::streamsize received;
    while((received = recv(connection, buffer, BUFSIZE, 0))){
        output.write(buffer, received);
        total += received;
        print_progressbar("Receive", total, data_size);
    }

    if(total != data_size){
        error("Failed to read all data");
    }
    else {
        std::cout << std::endl;
    }

    closesocket(connection);
    closesocket(sock);
    output.close();
    return 0;
}

std::vector<BluetoothDevice> g_device_list;
std::mutex g_device_list_mutex;

std::atomic<bool> g_stop_requested(false);

void update_device_list(){
    while(!g_stop_requested){
        auto device_list = get_bluetooth_devices();

        std::lock_guard lock(g_device_list_mutex);
        g_device_list = std::move(device_list);
    }
}

void print_device_list(const std::vector<BluetoothDevice>& devices){
    if(g_device_list.empty()){
        std::cout << "Device list: <empty>\n" << std::endl;
    }
    else {
        std::cout << "Device list:\n";
        // это просто... позор...
        std::size_t enumerate = 0;
        for(const auto &device: devices){
            std::wcout << ++enumerate << ". " << device.name() << " (0x" << std::hex << device.address() << ')' << std::endl;
        }
        std::cout << std::endl;
    }
}

void list_bluetooth_devices(){
    std::lock_guard lock(g_device_list_mutex);
    print_device_list(g_device_list);
}

void work_as_transmitter(){
    std::unique_lock lock(g_device_list_mutex);
    auto devices = g_device_list;
    lock.unlock();

    std::cout << "To return back choose zero as device number\n" << std::endl;

    print_device_list(devices);

choose:
    std::size_t number;
    while(true){
        std::cout << "\nEnter device number to transmit: ";
        std::cin >> number;

        if(!std::cin.fail()){
            break;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if(number == 0){
        return;
    }

    try {
        BluetoothDevice device = devices.at(number - 1);

        std::wcout << "Selected device \"" << device.name() << "\""
                      "\nEnter resource path to transfer: ";

        std::string path;
        std::cin >> path;

        if(transmit(device.address(), 10, path.c_str())){
            std::cout << "Transfer completed with error" << std::endl;
        }
        else {
            std::cout << "Transfer completed successfully" << std::endl;
        }
    }
    catch(std::exception&){
        std::cout << "Wrong device number, try again" << std::endl;
        goto choose;
    }
}

void work_as_receiver(){
    if(receive(10)){
        std::cout << "Receive completed with error" << std::endl;
    }
    else {
        std::cout << "Receive completed successfully" << std::endl;
    }
}

int main(){
    WSADATA data;
    if(WSAStartup(MAKEWORD(2, 2), &data)){
        std::cerr << "Winsock2 initialization failed" << std::endl;
        std::exit(1);
    }

    std::atexit([]{
        g_stop_requested = true;
    });

    std::thread update_thread(update_device_list);

    while(!g_stop_requested){
        std::cout << "Choose option:\n"
                     "\t1. list bluetooth devices\n"
                     "\t2. work as transmitter\n"
                     "\t3. work as receiver\n"
                     "\t4. exit\n"
                  << std::endl;

        switch(_getch()){
            case '1':
                system("cls");
                list_bluetooth_devices();
                break;
            case '2':
                system("cls");
                work_as_transmitter();
                break;
            case '3':
                system("cls");
                work_as_receiver();
                break;
            case '4':
                std::exit(0);
            default:
                continue;
        }
        std::cout << std::endl;
    }

    update_thread.join();
}