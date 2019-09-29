#define NOMINMAX

#include "RaceTimer.h"


void RaceTimer::init(std::vector<Car>* cars)
{
	for (auto& car : *cars)
	{
		carsData.push_back({ &car });
	}
}

void RaceTimer::setAIpointsPosition(std::vector<Point> AIpointsPositions)
{
	RaceTimer::AIpointsPositions = AIpointsPositions;
}

void RaceTimer::display()
{
	auto possition = 0;
	for (auto& carData : carsData)
	{
		ColorName curentColor;

		if (carData.car->humanCar)
			curentColor = HumanColor;
		else
			curentColor = AIcolor;

		std::string time = std::to_string(carData.timeDelay / 1000 / 60) + ":" + std::to_string(100 + (carData.timeDelay / 1000) % 60).substr(1, 2) + "." + std::to_string(100 + carData.timeDelay / 10).substr(1, 2);
		if (carData.timeDelay < 10)
			time = "-:--.--";
		Screen2D::Instance()->addTestValueToPrint(curentColor, -80, 5 + possition * 3, carDB.at(carData.car->carBrand).name + " " + time, &(Screen2D::Instance()->squada_one_regular));
		possition++;
	}
}

void RaceTimer::update()
{
	if (beforeRace && begin_time != 0)
	{
		if (clock() - begin_time > 4400)
			state = State::Green;
		else if (clock() - begin_time > 3300)
			state = State::Red1;
		else if (clock() - begin_time > 2200)
			state = State::Red2;
		else if (clock() - begin_time > 1100)
			state = State::Red3;
	}
	else if (begin_time != 0)
	{
		if (clock() - begin_time > 2000)
			state = State::Inactive;

		for (auto& carData : carsData)
		{
			carData.nextAIPoint = carData.car->getAIcurrentPoint();
			carData.AISegmentDone = getAISegmentDoneValue(carData.nextAIPoint, carData.car->position);
		}

		std::sort(carsData.begin(), carsData.end(), [](const RaceTimerData& d1, const RaceTimerData& d2) {return (d1.lapsDone < d2.lapsDone) || (d1.lapsDone == d2.lapsDone && d1.nextAIPoint < d2.nextAIPoint) || (d1.lapsDone == d2.lapsDone && d1.nextAIPoint == d2.nextAIPoint && d1.AISegmentDone < d2.AISegmentDone); });

		setLeaderTime(carsData.back());

		for (auto& carData : carsData)
		{
			carData.timeDelay = std::max(static_cast<int>(clock() - begin_time - getLeaderTime(carData)), 0);
		}

		carsData.back().timeDelay = 0;

		countLaps();
		checkCheckboxes();
	}
}

void RaceTimer::checkCheckboxes()
{
	for (auto& carData : carsData)
	{
		if (!carData.checkboxVisited && carData.nextAIPoint > AIpointsPositions.size() / 2)
			carData.checkboxVisited = true;
	}
}

void RaceTimer::countLaps()
{
	for (auto& carData : carsData)
	{
		if (carData.checkboxVisited && carData.nextAIPoint == 0)
		{
			carData.checkboxVisited = false;
			carData.lapsDone++;
		}
	}
}

int RaceTimer::calculateCurrentPositionId(const RaceTimerData& leader)
{
	int segment = round(leader.AISegmentDone * 10);
	if (segment > 9)
		segment = 9;
	else if (segment < 0)
		segment = 0;

	int currentPositionId = (leader.lapsDone * AIpointsPositions.size() + leader.nextAIPoint) * 10 + segment;

	return currentPositionId;
}

void RaceTimer::setLeaderTime(const RaceTimerData& leader)
{
	int time = clock() - begin_time;

	int currentPositionId = calculateCurrentPositionId(leader);

	while (currentPositionId >= leaderTime.size())
	{
		leaderTime.push_back(time);
	}
}

int RaceTimer::getLeaderTime(const RaceTimerData& car)
{
	int currentPositionId = calculateCurrentPositionId(car);

	if (leaderTime.size() == 1)
	{
		return 0;
	}
	else if (currentPositionId >= leaderTime.size())
	{
		return clock() - begin_time;
	}
	else
	{
		return leaderTime[currentPositionId];
	}
}

float RaceTimer::getAISegmentDoneValue(int nextAIPoint, const Point& position)
{
	if (AIpointsPositions.size() < 2)
	{
		return 0.0;
	}

	int previousAIPoint = nextAIPoint - 1;
	if (previousAIPoint < 0)
	{
		previousAIPoint = AIpointsPositions.size() - 1;
	}

	Point& p1 = AIpointsPositions[previousAIPoint];
	Point& p2 = AIpointsPositions[nextAIPoint];

	auto u = vector2D::dotProduct(vector2D(p1, position), vector2D(p1, p2)) / vector2D::dotProduct(vector2D(p2, p1), vector2D(p2, p1));

	return u;
}

void RaceTimer::startTimer()
{
	if(begin_time == 0)
		begin_time = clock();
}

void RaceTimer::resetTimer()
{
	begin_time = 0;
}