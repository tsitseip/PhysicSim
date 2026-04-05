/**
 * @file SystemMonitor.h
 * @brief Provides cross-platform system resource monitoring (CPU, RAM, GPU).
 */

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "core/Globals.h"
#include <glad/glad.h>

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
    #include <intrin.h>
#else
    #include <cpuid.h>
    #include <sys/types.h>
    #include <sys/sysinfo.h>
#endif

namespace core {

    /**
     * @brief Structure containing system resource snapshots.
     */
    struct SysInfo {
        std::string cpuModel;   ///< CPU brand name.
        int cores;               ///< Logical core count.
        double cpuUsage;         ///< Total CPU usage percentage (0.0 - 100.0).
        double ramUsage;         ///< RAM usage percentage (0.0 - 100.0).
        double ramTotal;         ///< Total RAM in GB.
        double ramAvailable;     ///< Available RAM in GB.
        std::string gpuModel;   ///< GPU renderer name.
        double vramUsage;        ///< VRAM usage percentage (0.0 - 100.0).
        double vramTotal;        ///< Total VRAM in GB.
        double vramAvailable;    ///< Available VRAM in GB.
    };

    /**
     * @brief Class for monitoring system resources across Windows and Linux.
     */
    class SystemMonitor {
    private:
        // OpenGL extensions for GPU Memory
        const unsigned int GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX = 0x9048;
        const unsigned int GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX = 0x9049;
        const unsigned int GL_VBO_FREE_MEMORY_ATI = 0x87FB;

        // CPU Usage state (Linux)
        long long m_lastTotalTick = 0;
        long long m_lastIdleTick = 0;

#ifdef _WIN32
        // CPU Usage state (Windows)
        FILETIME m_prevIdleTime;
        FILETIME m_prevKernelTime;
        FILETIME m_prevUserTime;

        long long fileTimeToLong(FILETIME ft) {
            return (((long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
        }
#endif

    public:
        SystemMonitor() {
#ifdef _WIN32
            GetSystemTimes(&m_prevIdleTime, &m_prevKernelTime, &m_prevUserTime);
#else
            updateLinuxCPUTicks(m_lastTotalTick, m_lastIdleTick);
#endif
        }

        /**
         * @brief Fetches a snapshot of current system resources.
         * @return SysInfo struct containing the snapshot data.
         */
        SysInfo getSnapshot() {
            SysInfo info;
            info.cpuModel = getCPUModel();
            info.cores = (int)std::thread::hardware_concurrency();
            
            fetchRAMInfo(info);
            fetchVRAMInfo(info);

            info.cpuUsage = getCPUUsage();
            info.gpuModel = getGPUModel();
            return info;
        }

    private:
        void fetchRAMInfo(SysInfo& info) {
#ifdef _WIN32
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            if (GlobalMemoryStatusEx(&statex)) {
                info.ramTotal = (double)statex.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
                info.ramAvailable = (double)statex.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
                info.ramUsage = (double)statex.dwMemoryLoad;
            }
#else
            std::ifstream meminfo("/proc/meminfo");
            std::string line;
            long long total = 0, available = 0;
            while (std::getline(meminfo, line)) {
                if (line.compare(0, 8, "MemTotal") == 0) sscanf(line.c_str(), "MemTotal: %lld", &total);
                else if (line.compare(0, 12, "MemAvailable") == 0) sscanf(line.c_str(), "MemAvailable: %lld", &available);
            }
            info.ramTotal = (double)total / (1024.0 * 1024.0);
            info.ramAvailable = (double)available / (1024.0 * 1024.0);
            info.ramUsage = total > 0 ? 100.0 * (1.0 - (double)available / total) : 0.0;
#endif
        }

        void fetchVRAMInfo(SysInfo& info) {
            int total_kb = 0;
            int avail_kb = 0;
            
            // Try NVIDIA extension
            glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_kb);
            glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &avail_kb);

            if (total_kb <= 0) {
                // Try AMD/Mesa extension (Works for Intel on Mesa Linux too!)
                int free_mem_ati[4];
                glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, free_mem_ati);
                if (free_mem_ati[0] > 0) {
                    avail_kb = free_mem_ati[0];
                    total_kb = 0; 
                }
            }

            info.vramTotal = (double)total_kb / (1024.0 * 1024.0);
            info.vramAvailable = (double)avail_kb / (1024.0 * 1024.0);
            info.vramUsage = (total_kb > 0) ? 100.0 * (1.0 - (double)avail_kb / total_kb) : 0.0;
        }

        std::string getCPUModel() {
            int cpuInfo[4] = { -1 };
            char cpuBrand[49];
            memset(cpuBrand, 0, sizeof(cpuBrand));

#ifdef _WIN32
            __cpuid(cpuInfo, 0x80000002);
            memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
            __cpuid(cpuInfo, 0x80000003);
            memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
            __cpuid(cpuInfo, 0x80000004);
            memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));
#else
            __get_cpuid(0x80000002, (unsigned int*)&cpuInfo[0], (unsigned int*)&cpuInfo[1], (unsigned int*)&cpuInfo[2], (unsigned int*)&cpuInfo[3]);
            memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
            __get_cpuid(0x80000003, (unsigned int*)&cpuInfo[0], (unsigned int*)&cpuInfo[1], (unsigned int*)&cpuInfo[2], (unsigned int*)&cpuInfo[3]);
            memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
            __get_cpuid(0x80000004, (unsigned int*)&cpuInfo[0], (unsigned int*)&cpuInfo[1], (unsigned int*)&cpuInfo[2], (unsigned int*)&cpuInfo[3]);
            memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));
#endif
            std::string brand(cpuBrand);
            brand.erase(0, brand.find_first_not_of(' '));
            size_t last = brand.find_last_not_of(' ');
            if (last != std::string::npos) brand.erase(last + 1);
            return brand;
        }

        double getCPUUsage() {
#ifdef _WIN32
            FILETIME idleTime, kernelTime, userTime;
            if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
                long long idle = fileTimeToLong(idleTime) - fileTimeToLong(m_prevIdleTime);
                long long kernel = fileTimeToLong(kernelTime) - fileTimeToLong(m_prevKernelTime);
                long long user = fileTimeToLong(userTime) - fileTimeToLong(m_prevUserTime);

                m_prevIdleTime = idleTime;
                m_prevKernelTime = kernelTime;
                m_prevUserTime = userTime;

                long long total = kernel + user;
                if (total > 0) return 100.0 * (1.0 - (double)idle / total);
            }
#else
            long long total, idle;
            if (updateLinuxCPUTicks(total, idle)) {
                long long totalDiff = total - m_lastTotalTick;
                long long idleDiff = idle - m_lastIdleTick;
                m_lastTotalTick = total;
                m_lastIdleTick = idle;
                if (totalDiff > 0) return 100.0 * (1.0 - (double)idleDiff / totalDiff);
            }
#endif
            return 0.0;
        }

        bool updateLinuxCPUTicks(long long& total, long long& idle) {
#ifndef _WIN32
            std::ifstream stat("/proc/stat");
            std::string line;
            if (std::getline(stat, line)) {
                long long user, nice, system, idleTime, iowait, irq, softirq;
                if (sscanf(line.c_str(), "cpu  %lld %lld %lld %lld %lld %lld %lld", 
                           &user, &nice, &system, &idleTime, &iowait, &irq, &softirq) == 7) {
                    idle = idleTime + iowait;
                    total = user + nice + system + idle + irq + softirq;
                    return true;
                }
            }
#endif
            return false;
        }

        std::string getGPUModel() {
            const char* renderer = (const char*)glGetString(GL_RENDERER);
            if (!renderer) return "Unknown GPU";
            
            std::string s(renderer);

            // Mesa drivers (Linux) often put extra info in parentheses at the end.
            // But we want to keep things like "Intel(R)" or "Core(TM)".
            // So we only strip a parenthetical block if it is at the very end of the string.
            if (!s.empty() && s.back() == ')') {
                size_t lastOpen = s.find_last_of('(');
                // Check if this looks like a Mesa driver block (contains commas or 'Mesa')
                if (lastOpen != std::string::npos && lastOpen > 0) {
                    std::string internal = s.substr(lastOpen);
                    if (internal.find(',') != std::string::npos || internal.find("Mesa") != std::string::npos) {
                        s = s.substr(0, lastOpen);
                    }
                }
            }
            
            // Trim trailing spaces
            size_t last = s.find_last_not_of(' ');
            if (last != std::string::npos) s.erase(last + 1);
            
            return s;
        }
    };

} // namespace core

#endif
