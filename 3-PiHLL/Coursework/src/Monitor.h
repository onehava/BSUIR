#pragma once

#include <functional>
#include "Source.h"

class Monitor {
public:
    using process_t = std::function<void(void)>;

    Monitor(Source& source, process_t process);
    void run();

private:
    process_t m_process;
    Source& m_source;

    void indicate(std::size_t row, std::size_t pos,
                  const std::string& message);

    void underline(std::size_t row, std::size_t pos1, std::size_t pos2,
                   const std::string& message);
};
