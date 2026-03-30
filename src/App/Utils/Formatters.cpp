#include "App/Utils/Formatters.h"
#include <cstdio>

namespace APP {
    namespace Utils {
        std::string format_bytes(unsigned long bytes)
        {
            double size = (double)bytes;
            const char *units[] = {"B", "KB", "MB", "GB", "TB"};
            int i = 0;

            while (size >= 1024 && i < 4)
            {
                size /= 1024;
                i++;
            }

            char buf[64];
            snprintf(buf, sizeof(buf), "%.2f %s", size, units[i]);
            return std::string(buf);
        }

        std::string format_time(unsigned long seconds)
        {
            unsigned long days = seconds / 86400;
            seconds %= 86400;

            unsigned long hours = seconds / 3600;
            seconds %= 3600;

            unsigned long minutes = seconds / 60;
            seconds %= 60;

            char buf[128];
            snprintf(buf, sizeof(buf),
                "%lu d %lu h %lu m %lu s",
                days, hours, minutes, seconds);

            return std::string(buf);
        }
    }
}
