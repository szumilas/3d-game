#pragma once

#include <vector>
#include <map>
#include <string>

#include "GlobalStructures.h"
#include "Footway.h"

enum TrackName
{
	SmallLoop = 1,
	BigLoop = 2,

};

struct TrackDetails
{
	std::string name;
	std::string difficulty;
};

static std::map<TrackName, TrackDetails> trackDB
{
	{ TrackName::SmallLoop, {"Small Loop", "Easy" } },
	{ TrackName::BigLoop, {"Big Loop", "Medium" } },

};

class Track : public Footway
{
public:

	Track(TrackName trackName);

	void display();

	TrackName getTrackName() { return trackName; }
	float getLength() { return length; }
	std::pair<Point, Point> getMeta() { return meta; };
	std::vector<Point> getAIPoints() { return AIPoints; }
	std::vector<std::pair<Point, Point>> getBarrierPoints() { return barrierPoints; }
	int getLapRecord() { return lapRecord; }
	static void updateLapRecordInTxtFile(TrackName trackName, int newRecord);

private:

	void calculateCoordinates();
	void calculateLength();

public:


	std::vector<Point> AIPointsCoordinates;
private:

	std::vector<Point> AIPoints;
	std::vector<std::pair<Point, Point>> barrierPoints;
	std::pair<Point, Point> meta;
	int lapRecord = -1;

	float gauge = 2.0f;

	TrackName trackName;
	float length;
};