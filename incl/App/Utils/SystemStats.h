#ifndef APP_UTILS_SYSTEMSTATS_H
#define APP_UTILS_SYSTEMSTATS_H

#include <memory>
#include <sys/sysinfo.h>

namespace APP {
    namespace Utils {
        typedef struct {
            unsigned long FreeRam;
            long uptime;
        } Process;

        std::unique_ptr<Process> GetData();
    }
}

#endif
