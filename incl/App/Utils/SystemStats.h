#ifndef APP_UTILS_SYSTEMSTATS_H
#define APP_UTILS_SYSTEMSTATS_H

#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <sys/sysinfo.h>
#include <unistd.h>

namespace APP {
    namespace Utils {
        struct NetworkInterface {
            std::string Name;
            std::string IP;
        };

        typedef struct {
            unsigned long FreeRam;
            unsigned long TotalRam;
            long uptime;
            int Processors;
            long Clockticks;
            long ChildMax;
            long POSIX_VERSION;

            std::string OSName;
            std::string OSVersion;
            std::string Architecture;
            std::string Distro;
            std::string CPUModel;
            std::string Username;
            std::string Hostname;

            std::vector<NetworkInterface> NetworkInterfaces;
        } Process;

        class RAMHistory {
        public:
            static RAMHistory& getInstance();
            void recordSample(unsigned long freeRam, unsigned long totalRam);
            const std::deque<float>& getFreeRamHistory() const;
            const std::deque<float>& getUsedRamHistory() const;
            size_t getMaxSamples() const { return max_samples; }
            
        private:
            RAMHistory();
            std::deque<float> free_ram_history;
            std::deque<float> used_ram_history;
            static const size_t max_samples = 120;
        };

        std::unique_ptr<Process> GetSystemData();
    }
}

#endif
