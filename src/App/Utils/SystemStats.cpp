#include "App/Utils/SystemStats.h"

namespace APP {
    namespace Utils {
        std::unique_ptr<Process> GetData() {
            std::unique_ptr<Process> p(new Process());
            struct sysinfo ProcessInfo;
            sysinfo(&ProcessInfo);
            p->FreeRam = ProcessInfo.freeram;
            p->uptime = ProcessInfo.uptime;
            return p;
        }
    }
}
