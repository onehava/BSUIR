#include "System.h"
#include "PowerInfo.h"

#include <windows.h>
#include <powrprof.h>
#undef max

#include <iostream>
#include <format>
#include <chrono>
#include <cctype>
#include <limits>
#include <utility>

inline void do_print(){
    auto power_status = Console::power_status();
    auto batteries_info = Console::batteries_info();

    if(!power_status){
        std::cout << "No system power status" << std::endl;
    }
    else {
        PowerInfo power_info(*power_status, batteries_info);

        auto v_percentage = power_info.battery_percentage();
        auto v_lifetime = power_info.battery_lifetime();

        // Print system power information

        std::cout << "Power:\n"
                     "\tSource: " << [](auto ps){
                        switch(ps){
                            case BATTERY: return "BAT";
                            case AC: return "AC";
                            case UNKNOWN: return "Unknown";
                            default: std::unreachable();
                        }
                     }(power_info.source()) << '\n';

        if(v_percentage)
            std::cout << "\tBattery percentage: " << *v_percentage << "%\n";

        if(v_lifetime){
            auto time = std::chrono::time_point_cast<std::chrono::seconds>(
                std::chrono::system_clock::from_time_t(*v_lifetime)
            );
            std::cout << "\tBattery lifetime: " << std::format("{:%T}", time) << '\n';
        }

        std::cout << "\tBattery saver: " <<
                     [](auto bs){
                         switch(bs){
                             case OFF: return "Off";
                             case ON: return "On";
                             default: std::unreachable();
                         }
                     }(power_info.battery_saver()) << std::endl;

        // Print batteries information

        const std::vector<BatteryInfo>& bis = power_info.batteries_info();

        for(std::size_t i = 0; i < bis.size(); i++){
            auto bi = bis[i];

            std::cout << "\n\tBattery " << i << ":\n"
                         "\t\tTechnology: " <<
                         [](auto bt){
                                switch(bt){
                                    case NONRECHARGEABLE: return "Non rechargeable";
                                    case RECHARGEABLE: return "Rechargeable";
                                    default: std::unreachable();
                                }
                            }(bi.technology()) << "\n"
                         "\t\tChemistry: " << bi.chemistry() << "\n";
        }

        std::cout << std::endl;
    }
}

inline void do_suspend(){
    SetSuspendState(FALSE, FALSE, FALSE);
}

inline void do_hibernate(){
    SetSuspendState(TRUE, FALSE, FALSE);
}

int main(){
    while(true){
        std::cout << "Options:\n"
                     "\tP - print power info\n"
                     "\tS - suspend\n"
                     "\tH - hibernate\n"
                     "\tQ - quit\n" << std::endl;

        std::cout << ">";
        std::cout.flush();
        int option = std::getchar();

        switch(option){
            case 'P':
                do_print();
                break;
            case 'S':
                do_suspend();
                break;
            case 'H':
                do_hibernate();
                break;
            case 'Q':
                std::exit(0);
            default:
                if(std::isgraph(option)){
                    std::cout << "Unknown option '" << option << "'" << std::endl;
                }
                else {
                    std::cout << "Unknown option (" << option << ")" << std::endl;
                }
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}