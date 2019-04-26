/*
	\file Mesh.h
	\brief A file of declarations for a mesh class.
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

/*
	\class Mesh
	\brief Used to create a mesh with an index buffer.
*/
class Mesh
{
public:
	/*
		\fn Mesh(const unsigned int maxTris, const unsigned int maxLines)
		\brief Initalises the buffers.
		\fn ~Mesh()
		\brief Default destructor.
	*/
	Mesh(const unsigned int maxTris, const unsigned int maxLines);
	~Mesh();

	/*
		\fn void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour)
		\brief Adds a line between v0 and v1 to the line buffers.
		\param v0 The start of the line.
		\param v1 The end of the line.
		\param colour The colour of the line.
	*/
	void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour);
	/*
		\fn void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour)
		\brief Adds a triangle between v0, v1 and v2 to the tri buffers.
		\param v0 The first vertex of the triangle.
		\param v1 The second vertex of the triangle.
		\param v2 The third vertex of the triangle.
		\param colour The colour of the triangle.
	*/
	void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour);

	/*
		\fn void AddQuadColoured(const glm::vec3& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr)
		\brief Adds a coloured quad to the mesh.
		\param center The center of the quad.
		\param extents The extents of the quad.
		\param colour The colour of the quad.
		\param transform The optional transform of the quad.
	*/
	void AddQuadColoured(const glm::vec3& center, const glm::vec2& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	/*
		\fn void AddQuadTextured(const glm::vec3& center, const glm::vec2& extents, const glm::mat4* transform = nullptr)
		\brief Adds a textured quad to the mesh.
		\param center The center of the quad.
		\param extents The extents of the quad.
		\param transform The optional transform of the quad.
	*/
	void AddQuadTextured(const glm::vec3& center, const glm::vec2& extents,
		const glm::mat4* transform = nullptr);
	/*
		\fn void AddBox(const glm::vec3& center, const glm::vec3& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr)
		\brief Adds a box to the mesh.
		\param center The center of the box.
		\param extents The extents of the box.
		\param colour The colour of the box.
		\param transform The optional transform of the box.
	*/
	void AddBox(const glm::vec3& center, const glm::vec3& extents,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	/*
		\fn void AddCylinder(const glm::vec3& center, const float radius, const float halfLength, const unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr)
		\brief Adds a cylinder to the mesh.
		\param center The center of the cylinder.
		\param radius The radius of the cylinder.
		\param halfLength The half the height of the cylinder.
		\param segments The amount of segments on the circle component of the cylinder.
		\param colour The colour of the cylinder.
		\param transform The optional transform of the cylinder.
	*/
	void AddCylinder(const glm::vec3& center, const float radius, const float halfLength,
		const unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr);
	/*
		\fn void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth, const glm::vec4& colour, const glm::mat4* transform = nullptr)
		\brief Adds a pyramid to the pyramid.
		\param center The center of the pyramid.
		\param halfWidth The half the width of the pyramid.
		\param halfHeight The half the height of the pyramid.
		\param colour The colour of the pyramid.
		\param transform The optional transform of the pyramid.
	*/
	void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth,
		const glm::vec4& colour, const glm::mat4* transform = nullptr);
	/*
		\fn void AddSphere(const glm::vec3& center, const float radius, int rows, const int columns, const glm::vec4& colour, const glm::mat4* transform = nullptr, const float longMin = 0.0f, const float longMax = 360.0f, const float latMin = -90.0f, const float latMax = 90.0f)
		\brief Adds a sphere to the mesh.
		\param center The center of the sphere.
		\param radius The radius of the sphere.
		\param rows The rows of the sphere.
		\param columns The columns of the sphere.
		\param colour The colour of the sphere.
		\param transform The optional transform of the sphere.
		\param longMin The optional minimum longitude of the sphere.
		\param longMax The optional maximum longitude of the sphere.
		\param latMin The optional minimum latitude of the sphere.
		\param latMax The optional maximum latitude of the sphere.
	*/
	void AddSphere(const glm::vec3& center, const float radius, int rows, const int columns, const glm::vec4& colour,
		const glm::mat4* transform = nullptr, const float longMin = 0.0f, const float longMax = 360.0f, const float latMin = -90.0f, const float latMax = 90.0f);

	/*
		\fn void Draw()
		\brief Draws the mesh.
	*/
	virtual void Draw();

protected:
	/*
		\struct Vertex
		\brief A vertex of a tri or line.
		\var glm::vec4 position
		The position of the vertex.
		\var glm::vec4 normal
		The normal of the vertex.
		\var glm::vec2 texCoord
		The texCoord of the vertex.
	*/
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	/*
		\var unsigned int m_maxTris
		The maximum amount of tris.
		\var Vertex* m_triVertices
		Collection of vertices for tris.
		\var unsigned int m_triVertexCount
		The amount of tri vertices.
		\var unsigned int* m_triIndices
		Collection of indices for tris.
		\var unsigned int m_triIndexCount
		The amount of indices.
		\var unsigned int m_triVAO
		Vertex array for tris.
		\var unsigned int m_triVBO
		Vertex buffer for tris.
		\var unsigned int m_triIBO
		Index buffer for tris.
	*/
	unsigned int m_maxTris;
	Vertex* m_triVertices;
	unsigned int m_triVertexCount;
	unsigned int* m_triIndices;
	unsigned int m_triIndexCount;
	unsigned int m_triVAO, m_triVBO, m_triIBO;

	/*
		\var unsigned int m_maxLines
		The maximum amount of lines.
		\var Vertex* m_lineVertices
		Collection of vertices for lines.
		\var unsigned int m_lineVertexCount
		The amount of line vertices.
		\var unsigned int m_lineVAO
		Vertex array for lines.
		\var unsigned int m_lineVBO
		Vertex buffer for lines.
	*/
	unsigned int m_maxLines;
	Vertex* m_lineVertices;
	unsigned int m_lineVertexCount;
	unsigned int m_lineVAO, m_lineVBO;
};