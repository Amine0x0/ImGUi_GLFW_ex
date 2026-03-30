#include "App/Utils/SystemStats.h"
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>

namespace APP {
    namespace Utils {
        RAMHistory& RAMHistory::getInstance() {
            static RAMHistory instance;
            return instance;
        }

        RAMHistory::RAMHistory() {}

        void RAMHistory::recordSample(unsigned long freeRam, unsigned long totalRam) {
            float free_mb = freeRam / (1024.0f * 1024.0f);
            float used_mb = (totalRam - freeRam) / (1024.0f * 1024.0f);

            free_ram_history.push_back(free_mb);
            used_ram_history.push_back(used_mb);

            if (free_ram_history.size() > max_samples) {
                free_ram_history.pop_front();
                used_ram_history.pop_front();
            }
        }

        const std::deque<float>& RAMHistory::getFreeRamHistory() const {
            return free_ram_history;
        }

        const std::deque<float>& RAMHistory::getUsedRamHistory() const {
            return used_ram_history;
        }

        std::unique_ptr<Process> GetSystemData() {
            std::unique_ptr<Process> p(new Process());
            struct sysinfo info;
            if (sysinfo(&info) == 0) {
                p->FreeRam = info.freeram;
                p->TotalRam = info.totalram;
                p->uptime = info.uptime;
            }

            p->ChildMax = sysconf(_SC_CHILD_MAX);
            p->Clockticks = sysconf(_SC_CLK_TCK);
            p->POSIX_VERSION = sysconf(_SC_VERSION);
            p->Processors = sysconf(_SC_NPROCESSORS_ONLN);

            struct utsname uts;
            if (uname(&uts) == 0) {
                p->OSName = uts.sysname;
                p->OSVersion = uts.release;
                p->Architecture = uts.machine;
            }

            std::ifstream osfile("/etc/os-release");
            std::string line;
            while (std::getline(osfile, line)) {
                if (line.find("PRETTY_NAME=") == 0) {
                    p->Distro = line.substr(13, line.size() - 14);
                    break;
                }
            }

            std::ifstream cpuinfo("/proc/cpuinfo");
            while (std::getline(cpuinfo, line)) {
                if (line.find("model name") == 0) {
                    p->CPUModel = line.substr(line.find(":") + 2);
                    break;
                }
            }

            struct passwd *pw = getpwuid(getuid());
            if (pw) p->Username = pw->pw_name;

            char host[256];
            if (gethostname(host, sizeof(host)) == 0)
                p->Hostname = host;

            struct ifaddrs *ifaddr;
            if (getifaddrs(&ifaddr) == 0) {
                for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
                    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                        char ip[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET,
                                  &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr,
                                  ip, sizeof(ip));
                        p->NetworkInterfaces.push_back({ifa->ifa_name, ip});
                    }
                }
                freeifaddrs(ifaddr);
            }

            RAMHistory::getInstance().recordSample(p->FreeRam, p->TotalRam);

            return p;
        }
    }
}