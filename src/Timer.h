#pragma once
#include <string>
#include <chrono>
#include <unordered_map>
#include <iostream>

class Timer
{
public:
	void addTimer(int type, const std::string& typeName);
	void startTimer(int type);
	void stopTimer(int type);
	double getAverageDuration(int type);
	void removeTimer(int type);
	void print() const;
private:
	struct TimerInfo
	{
		std::string name;
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::duration<double> latestDuration{ 0 };
		std::chrono::duration<double> totalDuration{ 0 };
		int runCount = 0; // Number of iterations
		bool isRunning = false;
	};

	std::unordered_map<int, TimerInfo> timers;
};

