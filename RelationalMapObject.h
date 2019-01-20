#pragma once
#include "MapObject.h"

class RelationalMapObject : public MapObject
{
	struct Member
	{
		long long ref;
		bool isInner;
	};

public:
	RelationalMapObject(long long id) { setId(id); };
	void addMember(long long ref, std::string role);
	void createNodeGeometry(std::map<long long, std::vector<long long>>& ways, std::map<long long, node>& nodes);

private:


public:


	std::vector<Member> members;
private:



};