/*
	\file Mesh.cpp
	\brief A file of definitions for a mesh class
*/
#include "Mesh.h"
#include <gl_core_4_4.h>

/*
	\fn Mesh(const unsigned int maxTris, const unsigned int maxLines)
	\brief Dedicates space for the mesh based on the maximum amount of tris and lines.
*/
Mesh::Mesh(const unsigned int maxTris, const unsigned int maxlines) :
	m_triIndexCount(0), m_triVertexCount(0), m_triVAO(0), m_triVBO(0), m_triIBO(0),
	m_lineVertexCount(0), m_lineVAO(0), m_lineVBO(0)
{
	m_maxTris = maxTris;
	// sets the tri vertices to be 3 times the amount of maximum tris (won't likely fill the space because some vertices will often overlap)
	m_triVertices = new Vertex[m_maxTris * 3];
	// sets the tri indices to be 3 time the amount of maximum tris
	m_triIndices = new unsigned int[m_maxTris * 3];
	m_maxLines = maxlines;
	// sets the line vertices to be 2 times the amount of maximum lines
	m_lineVertices = new Vertex[m_maxLines * 2];

	// generate tri vertex array and bind it
	glGenVertexArrays(1, &m_triVAO);
	glBindVertexArray(m_triVAO);

	// generate tri vertex buffer, bind it and fill it
	glGenBuffers(1, &m_triVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * 3 * sizeof(Vertex), m_triVertices, GL_DYNAMIC_DRAW);
	// generate tri index buffer, bind it and fill it
	glGenBuffers(1, &m_triIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxTris * 3 * sizeof(unsigned int), m_triIndices, GL_DYNAMIC_DRAW);

	// enable position, colour/normal and texture attributes of the shader for the vertex array
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// points the attribute to the corresponding property of the vertex
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// generate line vertex array and bind it
	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);

	// generate line vertex buffer, bind it and fill it
	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxLines * 2 * sizeof(Vertex), m_lineVertices, GL_DYNAMIC_DRAW);

	// enable position and colour attributes of the shader for the vertex array
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// points the attribute to the corresponding property of the vertex
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

	// unbind buffers and arrays
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
/*
	\fn ~Mesh()
	\brief Default destructor.
*/
Mesh::~Mesh()
{
	// dealocate containers
	delete[] m_triVertices;
	delete[] m_triIndices;
	// delete buffers and arrays
	glDeleteBuffers(1, &m_triVBO);
	glDeleteBuffers(1, &m_triIBO);
	glDeleteVertexArrays(1, &m_triVAO);
	// dealocate container
	delete[] m_lineVertices;
	// delete buffers and arrays
	glDeleteBuffers(1, &m_lineVBO);
	glDeleteVertexArrays(1, &m_lineVAO);
}

/*
	\fn void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour)
	\brief Adds a line between v0 and v1 of the colour to the line buffers.
	\param v0 The start of the line.
	\param v1 The end of the line.
	\param colour The colour of the line.
*/
void Mesh::AddLine(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec4 & colour)
{
	// checks if the amount of lines exceed the maximum allowed amount
	if (((float)m_lineVertexCount / 2.0f) < m_maxLines)
	{
		// adds vertex 0's properties to the array
		m_lineVertices[m_lineVertexCount].position.x = v0.x;
		m_lineVertices[m_lineVertexCount].position.y = v0.y;
		m_lineVertices[m_lineVertexCount].position.z = v0.z;
		m_lineVertices[m_lineVertexCount].position.w = 1;
		m_lineVertices[m_lineVertexCount].normal.x = colour.x;
		m_lineVertices[m_lineVertexCount].normal.y = colour.y;
		m_lineVertices[m_lineVertexCount].normal.z = colour.z;
		m_lineVertices[m_lineVertexCount].normal.w = colour.w;
		// increases the amount of added vertices
		m_lineVertexCount++;

		// adds vertex 1's properties to the array
		m_lineVertices[m_lineVertexCount].position.x = v1.x;
		m_lineVertices[m_lineVertexCount].position.y = v1.y;
		m_lineVertices[m_lineVertexCount].position.z = v1.z;
		m_lineVertices[m_lineVertexCount].position.w = 1;
		m_lineVertices[m_lineVertexCount].normal.x = colour.x;
		m_lineVertices[m_lineVertexCount].normal.y = colour.y;
		m_lineVertices[m_lineVertexCount].normal.z = colour.z;
		m_lineVertices[m_lineVertexCount].normal.w = colour.w;
		// increases the amount of added vertices
		m_lineVertexCount++;
	}
}
/*
	\fn void AddTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour)
	\brief Adds a triangle between v0, v1 and v2 of the colour to the tri buffers.
	\param v0 The first vertex of the triangle.
	\param v1 The second vertex of the triangle.
	\param v2 The third vertex of the triangle.
	\param colour The colour of the triangle.
*/
void Mesh::AddTri(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec4 & colour)
{
	// checks if the amount of tris exceed the maximum allowed amount
	if (((float)m_triIndexCount / 3.0f) < m_maxTris)
	{
		// used to check if vertex 0, 1 or 2 exist in the array
		bool exists[3] = { false, false, false };
		// checks each added vertex in the array
		for (int i = 0; i < m_triVertexCount; i++)
		{
			// checks if the vertex exists
			if (glm::vec3(m_triVertices[i].position) == v0)
			{
				// sets the flag of the vertex to existing
				exists[0] = true;
				// uses the index of the original vertex
				m_triIndices[m_triIndexCount] = i;
			}
			if (glm::vec3(m_triVertices[i].position) == v1)
			{
				exists[1] = true;
				m_triIndices[m_triIndexCount + 1] = i;
			}
			if (glm::vec3(m_triVertices[i].position) == v2)
			{
				exists[2] = true;
				m_triIndices[m_triIndexCount + 2] = i;
			}
		}
		// checks if the vertex has not been added to the array yet
		if (!exists[0])
		{
			// adds the vertex's properties to the array
			m_triVertices[m_triVertexCount].position.x = v0.x;
			m_triVertices[m_triVertexCount].position.y = v0.y;
			m_triVertices[m_triVertexCount].position.z = v0.z;
			m_triVertices[m_triVertexCount].position.w = 1;
			m_triVertices[m_triVertexCount].normal.x = colour.x;
			m_triVertices[m_triVertexCount].normal.y = colour.y;
			m_triVertices[m_triVertexCount].normal.z = colour.z;
			m_triVertices[m_triVertexCount].normal.w = colour.w;
			// adds the new index to the array
			// does not increment the count until the end because it's easier to keep track of the current index amount
			m_triIndices[m_triIndexCount] = m_triVertexCount;
			m_triVertexCount++;
		}
		if (!exists[1])
		{
			m_triVertices[m_triVertexCount].position.x = v1.x;
			m_triVertices[m_triVertexCount].position.y = v1.y;
			m_triVertices[m_triVertexCount].position.z = v1.z;
			m_triVertices[m_triVertexCount].position.w = 1;
			m_triVertices[m_triVertexCount].normal.x = colour.x;
			m_triVertices[m_triVertexCount].normal.y = colour.y;
			m_triVertices[m_triVertexCount].normal.z = colour.z;
			m_triVertices[m_triVertexCount].normal.w = colour.w;
			m_triIndices[m_triIndexCount + 1] = m_triVertexCount;
			m_triVertexCount++;
		}
		if (!exists[2])
		{
			m_triVertices[m_triVertexCount].position.x = v2.x;
			m_triVertices[m_triVertexCount].position.y = v2.y;
			m_triVertices[m_triVertexCount].position.z = v2.z;
			m_triVertices[m_triVertexCount].position.w = 1;
			m_triVertices[m_triVertexCount].normal.x = colour.x;
			m_triVertices[m_triVertexCount].normal.y = colour.y;
			m_triVertices[m_triVertexCount].normal.z = colour.z;
			m_triVertices[m_triVertexCount].normal.w = colour.w;
			m_triIndices[m_triIndexCount + 2] = m_triVertexCount;
			m_triVertexCount++;
		}

		// adds 3 counts of indices to the total
		m_triIndexCount += 3;
	}
}

/*
	\fn void AddQuadColoured(const glm::vec3& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr)
	\brief Adds a coloured quad to the mesh.
	\brief Calculates the vertices by adding the extents to the center.
	\param center The center of the quad.
	\param extents The extents of the quad.
	\param colour The colour of the quad.
	\param transform The optional transform of the quad.
*/
void Mesh::AddQuadColoured(const glm::vec3 & center, const glm::vec2 & extents,
	const glm::vec4 & colour, const glm::mat4 * transform /* = nullptr */)
{
	// collection of the corners of the quad
	glm::vec3 verts[4];
	// center of the object that can be transformed
	glm::vec3 tempCenter = center;
	// the width as a vector along the x axis
	glm::vec3 vX(extents.x, 0.0f, 0.0f);
	// the height as a vector along the z axis
	glm::vec3 vZ(0.0f, 0.0f, extents.y);

	// transforms all the properties based on the provided transform
	if (transform != nullptr)
	{
		vX = glm::vec3(*transform * glm::vec4(vX, 0.0f));
		vZ = glm::vec3(*transform * glm::vec4(vZ, 0.0f));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	// gets the position on either side of the center for both extents to get the verts
	verts[0] = tempCenter - vX - vZ;
	verts[1] = tempCenter - vX + vZ;
	verts[2] = tempCenter + vX + vZ;
	verts[3] = tempCenter + vX - vZ;

	// adds to tris of the colour
	AddTri(verts[0], verts[1], verts[2], colour);
	AddTri(verts[2], verts[3], verts[0], colour);
}
/*
	\fn void AddQuadTextured(const glm::vec3& center, const glm::vec2& extents, const glm::mat4* transform = nullptr)
	\brief Adds a textured quad to the mesh.
	\brief Calculates the vertices by adding the extents to the center.
	\param center The center of the quad.
	\param extents The extents of the quad.
	\param transform The optional transform of the quad.
*/
void Mesh::AddQuadTextured(const glm::vec3 & center, const glm::vec2 & extents, const glm::mat4 * transform)
{
	// collection of the corners of the quad
	glm::vec3 verts[4];
	// center of the object that can be transformed
	glm::vec3 tempCenter = center;
	// the width as a vector along the x axis
	glm::vec3 vX(extents.x, 0.0f, 0.0f);
	// the height as a vector along the z axis
	glm::vec3 vZ(0.0f, 0.0f, extents.y);

	// transforms all the properties based on the provided transform
	if (transform != nullptr)
	{
		vX = glm::vec3(*transform * glm::vec4(vX, 0.0f));
		vZ = glm::vec3(*transform * glm::vec4(vZ, 0.0f));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	// gets the position on either side of the center for both extents to get the verts
	verts[0] = tempCenter - vX - vZ;
	verts[1] = tempCenter - vX + vZ;
	verts[2] = tempCenter + vX + vZ;
	verts[3] = tempCenter + vX - vZ;

	// adds two tris with an upward normal
	AddTri(verts[0], verts[1], verts[2], { 0, 1, 0, 0 });
	AddTri(verts[2], verts[3], verts[0], { 0, 1, 0, 0 });

	// sets the texture coordinate for each corner of the quad to a corner of the texture
	m_triVertices[m_triVertexCount - 4].texCoord = { 0, 0 };
	m_triVertices[m_triVertexCount - 3].texCoord = { 0, 1 };
	m_triVertices[m_triVertexCount - 2].texCoord = { 1, 1 };
	m_triVertices[m_triVertexCount - 1].texCoord = { 1, 0 };
}

/*
	\fn void AddBox(const glm::vec3& center, const glm::vec3& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr)
	\brief Adds a box to the mesh.
	\brief Calculates the vertices by adding the extents to the center.
	\param center The center of the box.
	\param extents The extents of the box.
	\param colour The colour of the box.
	\param transform The optional transform of the box.
*/
void Mesh::AddBox(const glm::vec3& center, const glm::vec3& extents,
	const glm::vec4& colour, const glm::mat4* transform /* = nullptr */)
{
	// collection of the corners of the box
	glm::vec3 verts[8];
	// center of the box that can be transformed
	glm::vec3 tempCenter = center;
	// the width as a vector along the x axis
	glm::vec3 vX(extents.x, 0.0f, 0.0f);
	// the height as a vector along the y axis
	glm::vec3 vY(0.0f, extents.y, 0.0f);
	// the depth as a vector along the z axis
	glm::vec3 vZ(0.0f, 0.0f, extents.z);

	// transforms all the properties based on the provided transform
	if (transform != nullptr)
	{
		vX = glm::vec3(*transform * glm::vec4(vX, 0.0f));
		vY = glm::vec3(*transform * glm::vec4(vY, 0.0f));
		vZ = glm::vec3(*transform * glm::vec4(vZ, 0.0f));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	// using a particular combinations of the 3 vectors to determine the vertices
	verts[0] = tempCenter - vX - vY - vZ;
	verts[1] = tempCenter - vX - vY + vZ;
	verts[2] = tempCenter + vX - vY + vZ;
	verts[3] = tempCenter + vX - vY - vZ;

	verts[4] = tempCenter - vX + vY - vZ;
	verts[5] = tempCenter - vX + vY + vZ;
	verts[6] = tempCenter + vX + vY + vZ;
	verts[7] = tempCenter + vX + vY - vZ;

	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

	AddLine(verts[0], verts[1], white);
	AddLine(verts[1], verts[2], white);
	AddLine(verts[2], verts[3], white);
	AddLine(verts[3], verts[0], white);

	AddLine(verts[4], verts[5], white);
	AddLine(verts[5], verts[6], white);
	AddLine(verts[6], verts[7], white);
	AddLine(verts[7], verts[4], white);

	AddLine(verts[0], verts[4], white);
	AddLine(verts[1], verts[5], white);
	AddLine(verts[2], verts[6], white);
	AddLine(verts[3], verts[7], white);

	// top
	AddTri(verts[2], verts[1], verts[0], colour);
	AddTri(verts[3], verts[2], verts[0], colour);
	// bottom
	AddTri(verts[5], verts[6], verts[4], colour);
	AddTri(verts[6], verts[7], verts[4], colour);
	// front
	AddTri(verts[4], verts[3], verts[0], colour);
	AddTri(verts[7], verts[3], verts[4], colour);
	// back
	AddTri(verts[1], verts[2], verts[5], colour);
	AddTri(verts[2], verts[6], verts[5], colour);
	// left
	AddTri(verts[0], verts[1], verts[4], colour);
	AddTri(verts[1], verts[5], verts[4], colour);
	// right
	AddTri(verts[2], verts[3], verts[7], colour);
	AddTri(verts[6], verts[2], verts[7], colour);
}

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
void Mesh::AddCylinder(const glm::vec3 & center, const float radius, const float halfLength,
	const unsigned int segments, const glm::vec4 & colour, const glm::mat4 * transform /* = nullptr */)
{
	// the center of the cylinder
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	glm::vec4 white(1, 1, 1, 1);

	// segments a circle by the amount of segments
	float segmentSize = (2.0f * glm::pi<float>()) / segments;

	// used aie gizmos to figure out how to get the vetices
	for (unsigned int i = 0; i < segments; ++i)
	{
		glm::vec3 v0top(0.0f, halfLength, 0.0f);
		glm::vec3 v1top(sinf(i * segmentSize) * radius, halfLength, cosf(i * segmentSize) * radius);
		glm::vec3 v2top(sinf((i + 1) * segmentSize) * radius, halfLength, cosf((i + 1) * segmentSize) * radius);
		glm::vec3 v0bottom(0.0f, -halfLength, 0.0f);
		glm::vec3 v1bottom(sinf(i * segmentSize) * radius, -halfLength, cosf(i * segmentSize) * radius);
		glm::vec3 v2bottom(sinf((i + 1) * segmentSize) * radius, -halfLength, cosf((i + 1) * segmentSize) * radius);

		// transforms all the properties based on the provided transform
		if (transform != nullptr)
		{
			v0top = glm::vec3(*transform * glm::vec4(v0top, 0.0f));
			v1top = glm::vec3(*transform * glm::vec4(v1top, 0.0f));
			v2top = glm::vec3(*transform * glm::vec4(v2top, 0.0f));
			v0bottom = glm::vec3(*transform * glm::vec4(v0bottom, 0.0f));
			v1bottom = glm::vec3(*transform * glm::vec4(v1bottom, 0.0f));
			v2bottom = glm::vec3(*transform * glm::vec4(v2bottom, 0.0f));
		}

		// triangles
		AddTri(tempCenter + v0top, tempCenter + v1top, tempCenter + v2top, colour);
		AddTri(tempCenter + v0bottom, tempCenter + v2bottom, tempCenter + v1bottom, colour);
		AddTri(tempCenter + v2top, tempCenter + v1top, tempCenter + v1bottom, colour);
		AddTri(tempCenter + v1bottom, tempCenter + v2bottom, tempCenter + v2top, colour);

		// lines
		AddLine(tempCenter + v1top, tempCenter + v2top, white);
		AddLine(tempCenter + v1top, tempCenter + v1bottom, white);
		AddLine(tempCenter + v1bottom, tempCenter + v2bottom, white);
	}
}

/*
	\fn void AddPyramid(const glm::vec3& center, const float halfHeight, const float halfWidth, const glm::vec4& colour, const glm::mat4* transform = nullptr)
	\brief Adds a pyramid to the pyramid.
	\param center The center of the pyramid.
	\param halfWidth The half the width of the pyramid.
	\param halfHeight The half the height of the pyramid.
	\param colour The colour of the pyramid.
	\param transform The optional transform of the pyramid.
*/
void Mesh::AddPyramid(const glm::vec3 & center, const float halfHeight, const float halfWidth,
	const glm::vec4 & colour, const glm::mat4 * transform /* = nullptr */)
{
	// contains the 5 vertices of the pyramid mesh
	glm::vec3 verts[5];
	// the 3 axis of the pyramid using the given extents
	glm::vec3 up(0.0f, halfHeight, 0.0f);
	glm::vec3 right(halfWidth, 0.0f, 0.0f);
	glm::vec3 forward(0.0f, 0.0f, halfWidth);
	// the transformed center of the pyramid
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;
	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

	// transforms all the properties based on the provided transform
	if (transform != nullptr)
	{
		up = glm::vec3(*transform * glm::vec4(up, 0.0f));
		right = glm::vec3(*transform * glm::vec4(right, 0.0f));
		forward = glm::vec3(*transform * glm::vec4(forward, 0.0f));
	}

	// using different combinations of right and forward vectors with a negative up vector to determine the base
	verts[0] = tempCenter - right - up - forward;
	verts[1] = tempCenter + right - up - forward;
	verts[2] = tempCenter + right - up + forward;
	verts[3] = tempCenter - right - up + forward;
	// the peak of the pyramid
	verts[4] = tempCenter + up;

	// adds the tris in a particular order to ensure that it is not culled
	AddTri(verts[0], verts[1], verts[2], colour);
	AddTri(verts[2], verts[3], verts[0], colour);
	AddTri(verts[1], verts[0], verts[4], colour);
	AddTri(verts[2], verts[1], verts[4], colour);
	AddTri(verts[3], verts[2], verts[4], colour);
	AddTri(verts[0], verts[3], verts[4], colour);

	// adds the lines in a particular order to ensure that it is not culled
	AddLine(verts[0], verts[1], white);
	AddLine(verts[1], verts[2], white);
	AddLine(verts[2], verts[3], white);
	AddLine(verts[3], verts[0], white);
	AddLine(verts[0], verts[4], white);
	AddLine(verts[1], verts[4], white);
	AddLine(verts[2], verts[4], white);
	AddLine(verts[3], verts[4], white);
}

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
void Mesh::AddSphere(const glm::vec3 & center, const float radius, int rows, const int columns, const glm::vec4 & colour, const glm::mat4 * transform /* = nullptr */,
	const float longMin /* = 0.0f */, const float longMax /* = 360.0f */, const float latMin /* = -90.0f */, const float latMax /* = 90.0f */)
{
	float inverseRadius = 1 / radius;

	// invert these first as the multiply is slightly quicker
	float invColumns = 1.0f / columns;
	float invRows = 1.0f / rows;

	// used to convert degrees into radians
	float DEG2RAD = glm::pi<float>() / 180;

	// transforms the center based on the provided transform
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	// put latitude and longitude in radians
	float latitiudinalRange = (latMax - latMin) * DEG2RAD;
	float longitudinalRange = (longMax - longMin) * DEG2RAD;

	// for each row of the mesh
	glm::vec3* globe = new glm::vec3[rows * columns + columns];

	// got method from aie gizmos
	for (int row = 0; row <= rows; row++)
	{
		// navigation around the x axis in GL
		float ratioAroundXAxis = float(row) * invRows;
		float radiansAboutXAxis = ratioAroundXAxis * latitiudinalRange + (latMin * DEG2RAD);
		float y = radius * sin(radiansAboutXAxis);
		float z = radius * cos(radiansAboutXAxis);

		for (int col = 0; col <= columns; col++)
		{
			float ratioAroundYAxis = float(col) * invColumns;
			float theta = ratioAroundYAxis * longitudinalRange + (longMin * DEG2RAD);
			glm::vec3 point(-z * sinf(theta), y, -z * cosf(theta));
			glm::vec3 normal(inverseRadius * point.x, inverseRadius * point.y, inverseRadius * point.z);

			// transforms all the properties based on the provided transform
			if (transform != nullptr)
			{
				point = glm::vec3((*transform * glm::vec4(point, 0)));
				normal = glm::vec3((*transform * glm::vec4(normal, 0)));
			}

			int index = row * columns + (col % columns);
			globe[index] = point;
		}
	}

	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

	// adds all the lines and tris
	for (int face = 0; face < (rows * columns); face++)
	{
		int nextFace = face + 1;

		if (nextFace % columns == 0)
		{
			nextFace = nextFace - (columns);
		}

		AddLine(tempCenter + globe[face], tempCenter + globe[face + columns], white);

		if (face % columns == 0 && longitudinalRange < (glm::pi<float>() * 2))
		{
			continue;
		}

		AddLine(tempCenter + globe[nextFace + columns], tempCenter + globe[face + columns], white);

		AddTri(tempCenter + globe[nextFace + columns], tempCenter + globe[face], tempCenter + globe[nextFace], colour);
		AddTri(tempCenter + globe[nextFace + columns], tempCenter + globe[face + columns], tempCenter + globe[face], colour);
	}

	delete[] globe;
}

/*
	\fn void Draw()
	\brief Draws the mesh.
*/
void Mesh::Draw()
{
	// checks if there are any tris to draw
	if ((m_triIndexCount * 3) > 0)
	{
		// binds the vertex array
		glBindVertexArray(m_triVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triIBO);
		// sets the index data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_triIndexCount * sizeof(unsigned int), m_triIndices);		
		glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
		// sets the vertex data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_triVertexCount * sizeof(Vertex), m_triVertices);
		// draws the vertices
		glDrawElements(GL_TRIANGLES, m_triIndexCount, GL_UNSIGNED_INT, 0);
	}
	// checks if there are any lines to draw
	if ((m_lineVertexCount * 2) > 0)
	{
		// binds the vertex array
		glBindVertexArray(m_lineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
		// sets the vertex data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_lineVertexCount * sizeof(Vertex), m_lineVertices);
		// draws the vertices
		glDrawArrays(GL_LINES, 0, m_lineVertexCount);
	}
}