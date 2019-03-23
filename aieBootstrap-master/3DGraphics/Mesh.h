#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh(const unsigned int maxTris);
	~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 textCoord;
	};

	void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour);
	void AddBox(const glm::vec3& center, const glm::vec3& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddCylinder(const glm::vec3& center, const float radius, const float halfLength,
		const unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth,
		const glm::vec4& colour, const glm::mat4* transform);
	void AddSphere(const glm::vec3& center, const float radius, int rows, const int columns, const glm::vec4& colour,
		const glm::mat4* transform = nullptr, const float longMin = 0.0f, const float longMax = 360.0f, const float latMin = -90.0f, const float latMax = 90.0f);
	virtual void Draw();

protected:
	unsigned int m_maxTris;
	Vertex* m_vertices;
	unsigned int m_vertexCount;
	unsigned int* m_indices;
	unsigned int m_indexCount;
	unsigned int m_vao, m_vbo, m_ibo;
};