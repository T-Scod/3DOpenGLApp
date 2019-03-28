#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh(const unsigned int maxTris, const unsigned int maxLines);
	~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour);
	void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour);
	void AddQuad(const glm::vec3& center, const glm::vec2& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddBox(const glm::vec3& center, const glm::vec3& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddCylinder(const glm::vec3& center, const float radius, const float halfLength,
		const unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	void AddSphere(const glm::vec3& center, const float radius, int rows, const int columns, const glm::vec4& colour,
		const glm::mat4* transform = nullptr, const float longMin = 0.0f, const float longMax = 360.0f, const float latMin = -90.0f, const float latMax = 90.0f);
	virtual void Draw();

protected:
	unsigned int m_maxTris;
	Vertex* m_triVertices;
	unsigned int m_triVertexCount;
	unsigned int* m_triIndices;
	unsigned int m_triIndexCount;
	unsigned int m_triVAO, m_triVBO, m_triIBO;

	unsigned int m_maxLines;
	Vertex* m_lineVertices;
	unsigned int m_lineVertexCount;
	//unsigned int* m_lineIndices;
	//unsigned int m_lineIndexCount;
	unsigned int m_lineVAO, m_lineVBO/*, m_lineIBO*/;
};