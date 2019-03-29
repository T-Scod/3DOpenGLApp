#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh(const unsigned int maxTris, const unsigned int maxLines);
	~Mesh();

	// adds a line to be drawn
	void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour);
	// adds a tri to be drawn
	void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour);

	// adds a coloured quad
	void AddQuadColoured(const glm::vec3& center, const glm::vec2& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	// adds a textured quad
	void AddQuadTextured(const glm::vec3& center, const glm::vec2& extents,
		const glm::mat4* transform = nullptr);
	// adds a box to the mesh
	void AddBox(const glm::vec3& center, const glm::vec3& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	// adds a cylinder to the mesh
	void AddCylinder(const glm::vec3& center, const float radius, const float halfLength,
		const unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr);
	// adds a pyrimid to the mesh
	void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	// adds a sphere to the mesh
	void AddSphere(const glm::vec3& center, const float radius, int rows, const int columns, const glm::vec4& colour,
		const glm::mat4* transform = nullptr, const float longMin = 0.0f, const float longMax = 360.0f, const float latMin = -90.0f, const float latMax = 90.0f);

	// draws the elements and arrays
	virtual void Draw();

protected:
	// properties of a point of a tri or line
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// the maximum amount of tris
	unsigned int m_maxTris;
	// collection of vertices for tris
	Vertex* m_triVertices;
	// the amount of tri vertices
	unsigned int m_triVertexCount;
	// collection of indices for tris
	unsigned int* m_triIndices;
	// the amount of indices
	unsigned int m_triIndexCount;
	// vertex array, vertex buffer and index buffer for tris
	unsigned int m_triVAO, m_triVBO, m_triIBO;

	// the maximum amount of lines
	unsigned int m_maxLines;
	// collection of vertices for lines
	Vertex* m_lineVertices;
	// the amount of line vertices
	unsigned int m_lineVertexCount;
	// vertex array and vertex buffer for lines
	unsigned int m_lineVAO, m_lineVBO;
};