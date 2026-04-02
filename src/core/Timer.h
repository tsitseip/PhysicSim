/**
 * @file Timer.h
 * @brief Provides timing functionality for the simulation.
 */
#ifndef TIMER_H
#define TIMER_H

#include "Globals.h"

/**
 * @namespace core
 * @brief Core utilities and classes.
 */
namespace core {

    /**
     * @brief Counter struct to track counts and rates for frames and ticks.
     */
    struct Counter {
        /**
         * @brief Atomic integer to store the current count.
         */
        core::atomic<int> count{0};

        /**
         * @brief Atomic integer to store the current rate (count per second).
         */
        core::atomic<int> rate{0};

        /**
         * @brief Increments the current count.
         */
        void upd() { count++; }
    };

    /**
     * @brief Timer class to manage timing for the simulation, tracking frames and ticks.
     */
    class Timer {
    private:
        /**
         * @brief Time point of the last rate calculation.
         */
        std::chrono::steady_clock::time_point m_lastTime;
    public:
        /**
         * @brief Counter for frame rate.
         */
        Counter frame;

        /**
         * @brief Counter for tick rate.
         */
        Counter tick;

        /**
         * @brief Constructs a new Timer object and initializes the starting time.
         */
        Timer() : m_lastTime(std::chrono::steady_clock::now()) {}

        /**
         * @brief Checks if one second has elapsed and updates the frame and tick rates.
         * @return True if one second has elapsed, false otherwise.
         */
        bool tick_one_second() {
            auto now = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastTime).count();

            if (elapsed_ms >= core::constants::time::MS_PER_SEC) {
                frame.rate = (int)((frame.count * core::constants::time::MS_PER_SEC) / elapsed_ms);
                tick.rate = (int)((tick.count * core::constants::time::MS_PER_SEC) / elapsed_ms);
                
                frame.count = 0;
                tick.count = 0;
                m_lastTime = now;
                return true;
            }
            return false;
        }
    };

} // namespace core

#endif
