#pragma once

#include <vector>
#include <map>
#include <string>

#include "GlobalStructures.h"
#include "Footway.h"
#include "carDB.h"

enum TrackName
{
	Grunwaldzki = 1,
	CityDrift = 2,
	AroundTheWorld = 3,
	CenterPoint = 4,
	GreenAreaTrack = 5,
	TwoTowers = 6,
	BigLoop = 7,
	BotanicalGarden = 8,
	TheMall = 9,
	SmallLoop = 10,

};

struct TrackDetails
{
	std::string name;
	std::string difficulty;
};

struct TrackRecordData
{
	long lapRecord;
	CarBrand carBrand;
	std::string date;
};

static std::map<TrackName, TrackDetails> trackDB
{
	{ TrackName::Grunwaldzki,{ "Grunwaldzki", "Hard" } },
	{ TrackName::CityDrift,{ "City Drift", "Hard" } },
	{ TrackName::AroundTheWorld,{ "Around The World", "Medium" } },
	{ TrackName::CenterPoint,{ "Center Point", "Hard" } },
	{ TrackName::GreenAreaTrack,{ "Green Area", "Hard" } },
	{ TrackName::TwoTowers,{ "Two Towers", "Medium" } },
	{ TrackName::BigLoop,{ "Big Loop", "Medium" } },
	{ TrackName::BotanicalGarden,{ "Botanical Garden", "Medium" } },
	{ TrackName::TheMall,{ "The Mall", "Medium" } },
	{ TrackName::SmallLoop, {"Small Loop", "Easy" } },

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
	static void updateLapRecordInTxtFile(TrackName trackName, int newRecord, CarBrand carBrand);

	static TrackRecordData getLapRecordData(TrackName trackName);

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