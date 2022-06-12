#pragma once


class Collider
{
public:
	float minX, maxX, minY, maxY, minZ, maxZ;
	bool collision;

	Collider(objl::Mesh mesh);
	Collider();
	bool PointInside(glm::vec3 point);
	bool PointInSquare(glm::vec3 point);
	float GetFloorLevel();
};

