#pragma once

#include <vector>
#include <map>

#include "GlobalStructures.h"
#include "Footway.h"

enum TrackName
{
	SmallLoop = 1,
	BigLoop = 2,

};

static std::map<TrackName, std::string> trackDB
{
	{ TrackName::SmallLoop, "Small Loop" },
	{ TrackName::BigLoop, "Big Loop" },

};

class Track : public Footway
{
public:

	Track(TrackName trackName);

	void display();

	TrackName getTrackName() { return trackName; }

private:

	void calculateCoordinates();

public:


	std::vector<Point> AIPointsCoordinates;
private:

	std::vector<Point> AIPoints;
	std::vector<std::pair<Point, Point>> barrierPoints;
	std::pair<Point, Point> meta;

	float gauge = 2.0f;

	TrackName trackName;
};