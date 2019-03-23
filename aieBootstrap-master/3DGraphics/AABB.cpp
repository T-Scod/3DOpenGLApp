#include "AABB.h"

AABB::AABB()
{
	m_position = glm::vec3(0.0f);
	m_dimensions[0] = 0.0f;
	m_dimensions[1] = 0.0f;
	m_dimensions[2] = 0.0f;
}
AABB::AABB(const glm::vec3 & min, const glm::vec3 & max)
{
	m_position = (min + max) / 2.0f;
	m_dimensions[0] = max.x - min.x;
	m_dimensions[1] = max.y - min.y;
	m_dimensions[2] = max.z - min.z;
}
AABB::AABB(const glm::vec3 & position, const float width, const float height, const float depth)
{
	m_position = position;
	m_dimensions[0] = width;
	m_dimensions[1] = height;
	m_dimensions[2] = depth;
}
AABB::~AABB()
{
}

void AABB::SetWidth(const float width)
{
	m_width = width;
}
void AABB::SetHeight(const float height)
{
	m_height = height;
}
void AABB::SetDepth(const float depth)
{
	m_depth;
}

std::vector<glm::vec3> AABB::GetCorners() const
{
	glm::vec3 min = GetMin();
	glm::vec3 max = GetMax();
	std::vector<glm::vec3> corners;
	corners.push_back(min);						// back bottom left
	corners.push_back({ min.x, max.y, min.z });	// back top left
	corners.push_back({ min.x, max.y, max.z });	// front top left
	corners.push_back({ min.x, min.y, min.z });	// front bottom left
	corners.push_back({ max.x, min.y, min.z });	// back bottom right
	corners.push_back({ max.x, max.y, min.z });	// back top right
	corners.push_back(max);						// front top right
	corners.push_back({ max.x, min.y, max.z });	// front bottom right
	return corners;
}