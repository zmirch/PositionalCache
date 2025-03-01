#include "Timer.h"

void Timer::addTimer(int type, const std::string& typeName)
{
    timers[type] = TimerInfo{ typeName };
}

void Timer::startTimer(int type)
{
    if (timers.find(type) != timers.end()) // First check if timer was previously added
    {
        TimerInfo& timer = timers[type];
        if (!timer.isRunning)
        {
            timer.startTime = std::chrono::high_resolution_clock::now();
            timer.isRunning = true;
            timer.latestDuration = std::chrono::duration<double>(0);
        }
        else
        {
            std::cerr << "Timer \"" << timer.name << "\" is already running!" << std::endl;
        }
    }
    else
    {
        std::cerr << "startTimer: Timer type not found!" << std::endl;
    }
}

void Timer::stopTimer(int type)
{
    if (timers.find(type) != timers.end())
    {
        TimerInfo& timer = timers[type];
        if (timer.isRunning)
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            timer.latestDuration = endTime - timer.startTime; // Get the duration of the current timing
            timer.totalDuration += timer.latestDuration; // Add to the total duration
            timer.runCount++;
            timer.isRunning = false;
        }
        else
        {
            std::cerr << "Timer \"" << timer.name << "\" is not running!" << std::endl;
        }
    }
    else
    {
        std::cerr << "stopTimer: Timer type not found!" << std::endl;
    }
}

double Timer::getAverageDuration(int type)
{
    double averageDuration = 0;
    
    if (timers.find(type) != timers.end()) // First check if timer was previously added
    {
        TimerInfo& timer = timers[type];
        averageDuration = (timer.runCount > 0) ? timer.totalDuration.count() / timer.runCount : 0.0;
    }
    else
    {
        std::cerr << "Timer type not found!" << std::endl;
    }
    return averageDuration;
}

void Timer::removeTimer(int type)
{
    if (timers.find(type) != timers.end()) // Check if the timer exists
    {
        timers.erase(type); // Remove the timer from the map
    }
    else
    {
        std::cerr << "removeTimer: Timer type not found!" << std::endl;
    }
}

void Timer::print() const
{
    for (const auto& [type, timer] : timers)
    {

        double averageDuration = (timer.runCount > 0) ? timer.totalDuration.count() / timer.runCount : 0.0;

        std::cout << "Timer: \"" << timer.name
            << "\"\n\tDuration: " << timer.latestDuration.count() << " seconds"
            << "\n\tAverage: " << averageDuration << " seconds"
            << "\n\tIterations: " << timer.runCount
            << std::endl;
    }
}