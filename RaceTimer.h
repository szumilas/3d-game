#pragma once

#include <vector>
#include "Car.h"

class RaceTimer
{
	friend class Menu;

public:

	enum State
	{
		Intro = -2,
		Inactive = -1,
		Red4 = 0,
		Red3 = 1,
		Red2 = 2,
		Red1 = 3,
		Green = 4,
		WaitingToDisappear = 9,
		Outro = 10,
		RaceFinished = 11,
	};

	struct RaceTimerData
	{
		Car* car;
		int nextAIPoint = 0;
		float AISegmentDone = 0.0f;
		int timeDelay = 0;
		bool checkboxVisited = false;
		int lapsDone = 0;
		bool useForStats = false;
		std::vector<int> lapTimes;
		bool raceDone = false;
		float vMax = 0;
	};

	void init(std::vector<Car>* cars);
	void setAIpointsPosition(std::vector<Point> AIpointsPositions);
	void startTimer();
	void resetTimer();

	void display();
	void update();
	void setMaxNoOfLaps(int laps) { maxNoOfLaps = laps; }

private:

	float getAISegmentDoneValue(int nextAIPoint, const Point& position);
	void setLeaderTime(const RaceTimerData& leader);
	int getLeaderTime(const RaceTimerData& car);
	int calculateCurrentPositionId(const RaceTimerData& leader);
	void checkCheckboxes();
	void countLaps();
	void setCarForStats();
	void checkRaceFinished();
	void prepareFinalStats();

public:

	State state = State ::Inactive;
	bool beforeRace = true;


private:

	clock_t begin_time = 0;

	std::vector<RaceTimerData> carsData;
	std::vector<Point> AIpointsPositions;
	RaceTimerData* carForStats;

	ColorName AIcolor = ColorName::DARK_GRAY;
	ColorName HumanColor = ColorName::RED;

	std::vector<int> leaderTime{0};

	int maxNoOfLaps;
	bool raceFinished = false;

	std::vector<std::tuple<std::string, int, float>> raceResults; //name, time [ms], Max [m/s]
	std::vector<int> humanLaps;
	CarBrand humanCarBrand;

};