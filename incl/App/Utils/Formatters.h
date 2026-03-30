#ifndef APP_UTILS_FORMATTERS_H
#define APP_UTILS_FORMATTERS_H

#include <string>

namespace APP {
    namespace Utils {
        std::string format_bytes(unsigned long bytes);
        std::string format_time(unsigned long seconds);
    }
}

#endif
