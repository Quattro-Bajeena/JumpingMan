#pragma once


class Collider
{
public:
	float minX, maxX, minY, maxY, minZ, maxZ;

	Collider(objl::Mesh mesh);
	bool PointInside(glm::vec3 point);
};

