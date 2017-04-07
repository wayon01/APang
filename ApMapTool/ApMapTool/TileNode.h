#pragma once
#include "Vector.h"
#include <map>

class TileNode {
public:
	TileNode(int id, int x, int y, int z);
	~TileNode();

	void setPosition(float x, float y, float z);

public:
	int id;

	int x;
	int y;
	int z;

	vec3 Position;

	std::map<std::string, bool> m_additionalBooleanType;
	std::map<std::string, int> m_additionalIntegerType;
};

