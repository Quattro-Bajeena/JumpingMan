#pragma once


class Collider
{
public:
	float minX, maxX, minY, maxY, minZ, maxZ;

	Collider(objl::Mesh mesh);
	bool PointInside(glm::vec3 point);
	bool PointInSquare(glm::vec3 point);
	float GetFloorLevel();
};

