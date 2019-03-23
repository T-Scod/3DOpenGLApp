#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class AABB
{
public:
	AABB();
	AABB(const glm::vec3& min, const glm::vec3& max);
	AABB(const glm::vec3& position, const float width, const float height, const float depth);
	~AABB();

	void SetWidth(const float width);
	float GetWidth() const { return m_width; }
	void SetHeight(const float height);
	float GetHeight() const { return m_height; }
	void SetDepth(const float depth);
	float GetDepth() const { return m_depth; }

	glm::vec3 GetMin() const { return glm::vec3(m_position.x - (m_width / 2.0f), m_position.y - (m_height / 2.0f), m_position.z - (m_depth / 2.0f)); }
	glm::vec3 GetMax() const { return glm::vec3(m_position.x + (m_width / 2.0f), m_position.y + (m_height / 2.0f), m_position.z + (m_depth / 2.0f)); }
	glm::vec3 GetExtents() const { return glm::vec3(m_width / 2.0f, m_height / 2.0f, m_depth / 2.0f); }
	std::vector<glm::vec3> GetCorners() const;

protected:
	glm::vec3 m_position;

	union
	{
		struct
		{
			float m_width;
			float m_height;
			float m_depth;
		};
		float m_dimensions[3];
	};
};