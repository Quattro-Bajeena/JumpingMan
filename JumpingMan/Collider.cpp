#include "pch.h"
#include "Collider.h"

Collider::Collider(objl::Mesh mesh)
{
	maxX = maxY = maxZ = -1 * std::numeric_limits<float>::infinity();
	minX = minY = minZ = std::numeric_limits<float>::infinity();

	for (int j = 0; j < mesh.Vertices.size(); j++)
	{
		auto vertex = mesh.Vertices[j].Position;
		maxX = std::max(maxX, vertex.X);
		maxY = std::max(maxY, vertex.Y);
		maxZ = std::max(maxZ, vertex.Z);

		minX = std::min(minX, vertex.X);
		minY = std::min(minY, vertex.Y);
		minZ = std::min(minZ, vertex.Z);
	}
}

bool Collider::PointInside(glm::vec3 point)
{
	return (point.x >= minX && point.x <= maxX) &&
		(point.y >= minY && point.y <= maxY) &&
		(point.z >= minZ && point.z <= maxZ);
}

float Collider::GetFloorLevel()
{
	return maxY;
}

bool Collider::PointInSquare(glm::vec3 point)
{
	return (point.x >= minX && point.x <= maxX) && (point.z >= minZ && point.z <= maxZ);
}