#pragma once
#include "Vector.h"

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
};

