#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::Mesh(unsigned int maxTris) : m_indexCount(0), m_vertexCount(0), m_vao(0), m_vbo(0), m_ibo(0)
{
	m_maxTris = maxTris;
	m_vertices = new Vertex[m_maxTris * 3];
	m_indices = new unsigned int[m_maxTris * 3];

	glGenVertexArrays(1, &m_vao);
	// bind vertex array
	glBindVertexArray(m_vao);

	// generate buffers
	glGenBuffers(1, &m_vbo);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * 3 * sizeof(Vertex), m_vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_ibo);
	// bind vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	// fill vertex buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxTris * 3 * sizeof(unsigned int), m_indices, GL_DYNAMIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::AddTri(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec4 & colour)
{
	if (((float)m_indexCount / 3.0f) < m_maxTris)
	{
		bool exists[3] = { false, false, false };
		for (int i = 0; i < m_vertexCount; i++)
		{
			if (glm::vec3(m_vertices[i].position) == v0)
			{
				exists[0] = true;
				m_indices[m_indexCount] = i;
			}
			if (glm::vec3(m_vertices[i].position) == v1)
			{
				exists[1] = true;
				m_indices[m_indexCount + 1] = i;
			}
			if (glm::vec3(m_vertices[i].position) == v2)
			{
				exists[2] = true;
				m_indices[m_indexCount + 2] = i;
			}
		}
		if (!exists[0])
		{
			m_vertices[m_vertexCount].position.x = v0.x;
			m_vertices[m_vertexCount].position.y = v0.y;
			m_vertices[m_vertexCount].position.z = v0.z;
			m_vertices[m_vertexCount].position.w = 1;
			m_vertices[m_vertexCount].normal.x = colour.x;
			m_vertices[m_vertexCount].normal.y = colour.y;
			m_vertices[m_vertexCount].normal.z = colour.z;
			m_vertices[m_vertexCount].normal.w = colour.w;
			m_indices[m_indexCount] = m_vertexCount;
			m_vertexCount++;
		}
		if (!exists[1])
		{
			m_vertices[m_vertexCount].position.x = v1.x;
			m_vertices[m_vertexCount].position.y = v1.y;
			m_vertices[m_vertexCount].position.z = v1.z;
			m_vertices[m_vertexCount].position.w = 1;
			m_vertices[m_vertexCount].normal.x = colour.x;
			m_vertices[m_vertexCount].normal.y = colour.y;
			m_vertices[m_vertexCount].normal.z = colour.z;
			m_vertices[m_vertexCount].normal.w = colour.w;
			m_indices[m_indexCount + 1] = m_vertexCount;
			m_vertexCount++;
		}
		if (!exists[2])
		{
			m_vertices[m_vertexCount].position.x = v2.x;
			m_vertices[m_vertexCount].position.y = v2.y;
			m_vertices[m_vertexCount].position.z = v2.z;
			m_vertices[m_vertexCount].position.w = 1;
			m_vertices[m_vertexCount].normal.x = colour.x;
			m_vertices[m_vertexCount].normal.y = colour.y;
			m_vertices[m_vertexCount].normal.z = colour.z;
			m_vertices[m_vertexCount].normal.w = colour.w;
			m_indices[m_indexCount + 2] = m_vertexCount;
			m_vertexCount++;
		}

		m_indexCount += 3;
	}
}

void Mesh::AddBox(const glm::vec3& center, const glm::vec3& extents,
	const glm::vec4& colour, const glm::mat4* transform)
{
	glm::vec3 verts[8];
	glm::vec3 tempCenter = center;
	glm::vec3 vX(extents.x, 0.0f, 0.0f);
	glm::vec3 vY(0.0f, extents.y, 0.0f);
	glm::vec3 vZ(0.0f, 0.0f, extents.z);

	if (transform != nullptr)
	{
		vX = glm::vec3(*transform * glm::vec4(vX, 0.0f));
		vY = glm::vec3(*transform * glm::vec4(vY, 0.0f));
		vZ = glm::vec3(*transform * glm::vec4(vZ, 0.0f));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	// top verts
	verts[0] = tempCenter - vX - vZ - vY;
	verts[1] = tempCenter - vX + vZ - vY;
	verts[2] = tempCenter + vX + vZ - vY;
	verts[3] = tempCenter + vX - vZ - vY;

	// bottom verts
	verts[4] = tempCenter - vX - vZ + vY;
	verts[5] = tempCenter - vX + vZ + vY;
	verts[6] = tempCenter + vX + vZ + vY;
	verts[7] = tempCenter + vX - vZ + vY;

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

void Mesh::AddCylinder(const glm::vec3 & center, const float radius, const float halfLength, const unsigned int segments, const glm::vec4 & colour, const glm::mat4 * transform)
{
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float segmentSize = (2.0f * glm::pi<float>()) / segments;

	for (unsigned int i = 0; i < segments; ++i)
	{
		glm::vec3 v0top(0.0f, halfLength, 0.0f);
		glm::vec3 v1top(sinf(i * segmentSize) * radius, halfLength, cosf(i * segmentSize) * radius);
		glm::vec3 v2top(sinf((i + 1) * segmentSize) * radius, halfLength, cosf((i + 1) * segmentSize) * radius);
		glm::vec3 v0bottom(0.0f, -halfLength, 0.0f);
		glm::vec3 v1bottom(sinf(i * segmentSize) * radius, -halfLength, cosf(i * segmentSize) * radius);
		glm::vec3 v2bottom(sinf((i + 1) * segmentSize) * radius, -halfLength, cosf((i + 1) * segmentSize) * radius);

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
	}
}

void Mesh::AddPyramid(const glm::vec3 & center, const float halfHeight, const float halfWidth, const glm::vec4 & colour, const glm::mat4 * transform)
{
	glm::vec3 verts[5];
	glm::vec3 up(0.0f, halfHeight, 0.0f);
	glm::vec3 right(halfWidth, 0.0f, 0.0f);
	glm::vec3 forward(0.0f, 0.0f, halfWidth);
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	if (transform != nullptr)
	{
		up = glm::vec3(*transform * glm::vec4(up, 0.0f));
		right = glm::vec3(*transform * glm::vec4(right, 0.0f));
		forward = glm::vec3(*transform * glm::vec4(forward, 0.0f));
	}

	verts[0] = tempCenter - right - up - forward;
	verts[1] = tempCenter + right - up - forward;
	verts[2] = tempCenter + right - up + forward;
	verts[3] = tempCenter - right - up + forward;
	verts[4] = tempCenter + up;

	AddTri(verts[0], verts[1], verts[2], colour);
	AddTri(verts[2], verts[3], verts[0], colour);
	AddTri(verts[1], verts[0], verts[4], colour);
	AddTri(verts[2], verts[1], verts[4], colour);
	AddTri(verts[3], verts[2], verts[4], colour);
	AddTri(verts[0], verts[3], verts[4], colour);
}

void Mesh::AddSphere(const glm::vec3 & center, const float radius, int rows, const int columns, const glm::vec4 & colour, const glm::mat4 * transform, const float longMin, const float longMax, const float latMin, const float latMax)
{
	float inverseRadius = 1 / radius;

	// invert these first as the multiply is slightly quicker
	float invColumns = 1.0f / columns;
	float invRows = 1.0f / rows;

	float DEG2RAD = glm::pi<float>() / 180;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	// put latitude and longitude in radians
	float latitiudinalRange = (latMax - latMin) * DEG2RAD;
	float longitudinalRange = (longMax - longMin) * DEG2RAD;

	// for each row of the mesh
	glm::vec3* globe = new glm::vec3[rows * columns + columns];

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

			if (transform != nullptr)
			{
				point = glm::vec3((*transform * glm::vec4(point, 0)));
				normal = glm::vec3((*transform * glm::vec4(normal, 0)));
			}

			int index = row * columns + (col % columns);
			globe[index] = point;
		}
	}

	for (int face = 0; face < (rows * columns); face++)
	{
		int iNextFace = face + 1;

		if (iNextFace % columns == 0)
		{
			iNextFace = iNextFace - (columns);
		}

		if (face % columns == 0 && longitudinalRange < (glm::pi<float>() * 2))
			continue;

		AddTri(tempCenter + globe[iNextFace + columns], tempCenter + globe[face], tempCenter + globe[iNextFace], colour);
		AddTri(tempCenter + globe[iNextFace + columns], tempCenter + globe[face + columns], tempCenter + globe[face], colour);
	}

	delete[] globe;
}

void Mesh::Draw()
{
	if ((m_indexCount * 3) > 0)
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indexCount * sizeof(unsigned int), m_indices);		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(Vertex), m_vertices);
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
}