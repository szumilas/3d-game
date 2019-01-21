
#include "RelationalMapObject.h"

void RelationalMapObject::addMember(long long ref, std::string role)
{
	members.push_back({ ref, role == "inner"});

}

void RelationalMapObject::createNodeGeometry(std::map<long long, std::vector<long long>>& ways, std::map<long long, node>& nodes)
{
	std::sort(members.begin(), members.end(), [](Member& a, Member& b) {return a.isInner < b.isInner; });

	members.resize(2);

	if (members.size() == 2 && members[0].isInner + members[1].isInner == true)
	{
		std::vector<long long>* outerRefs = nullptr;
		std::vector<long long>* innerRefs = nullptr;

		if (ways.count(members[1].ref) && ways.count(members[0].ref))
		{
			innerRefs = &ways.at(members[1].ref);
			outerRefs = &ways.at(members[0].ref);
		}
		else
		{
			return;
		}

		

		std::vector<Point> outerPoints;
		std::vector<Point> innerPoints;

		for (auto& innerRef : *innerRefs)
		{
			innerPoints.push_back({nodes.at(innerRef).posX, nodes.at(innerRef).posY});
		}
		for (auto& outerRef : *outerRefs)
		{
			outerPoints.push_back({ nodes.at(outerRef).posX, nodes.at(outerRef).posY });
		}

		outerPoints.pop_back();
		innerPoints.pop_back();

		int outerClosestIt;
		int innerClosestIt;
		float minDist = 10000000.0f;


		for (int q = 0; q < outerPoints.size(); q++)
		{
			for (int w = 0; w < innerPoints.size(); w++)
			{
				if (outerPoints[q].distance2D(innerPoints[w]) < minDist)
				{
					minDist = outerPoints[q].distance2D(innerPoints[w]);
					outerClosestIt = q;
					innerClosestIt = w;
				}
			}
		}

		points.push_back(outerPoints[outerClosestIt]);

		for (int q = (outerClosestIt + 1) % outerPoints.size(); q != outerClosestIt; q = (q + 1) % outerPoints.size())
		{
			points.push_back(outerPoints[q]);
		}



		points.push_back(innerPoints[innerClosestIt]);

		for (int q = (innerClosestIt + 1) % innerPoints.size(); q != innerClosestIt; q = (q + 1) % innerPoints.size())
		{
			points.push_back(innerPoints[q]);
		}

		points.push_back(innerPoints[innerClosestIt]);

	}
	else if (members.size() == 2 && members[0].isInner + members[1].isInner == false)
	{
		if (ways.count(members[1].ref))
		{
			auto refs = &ways.at(members[1].ref);

			for (auto& ref : *refs)
			{
				points.push_back({ nodes.at(ref).posX, nodes.at(ref).posY });
			}
		}
		else if (ways.count(members[0].ref))
		{
			auto refs = &ways.at(members[0].ref);

			for (auto& ref : *refs)
			{
				points.push_back({ nodes.at(ref).posX, nodes.at(ref).posY });
			}
		}
	}



}