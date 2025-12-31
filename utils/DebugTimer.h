//
// Created by Deepak Shenoy on 12/31/25.
//

#ifndef AIRPORT_PASSENGER_FORECASTING_TIMER_H
#define AIRPORT_PASSENGER_FORECASTING_TIMER_H

#include <chrono>

class DebugTimer {
public:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point stop__time;

    void start() {
        start_time = std::chrono::steady_clock::now();
    }

    double stop() {
        stop__time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = stop__time - start_time;
        return elapsed_seconds.count();
    }
};

#endif //AIRPORT_PASSENGER_FORECASTING_TIMER_H