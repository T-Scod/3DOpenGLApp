#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::Mesh(const unsigned int maxTris, const unsigned int maxlines) : m_triIndexCount(0), m_triVertexCount(0), m_triVAO(0), m_triVBO(0), m_triIBO(0)
{
	m_maxTris = maxTris;
	m_triVertices = new Vertex[m_maxTris * 3];
	m_triIndices = new unsigned int[m_maxTris * 3];
	m_maxLines = maxlines;
	m_lineVertices = new Vertex[m_maxLines * 2];
	//m_lineIndices = new unsigned int[m_maxLines * 2];

	// generate vertex array
	glGenVertexArrays(1, &m_triVAO);
	// bind vertex array
	glBindVertexArray(m_triVAO);

	// generate buffer
	glGenBuffers(1, &m_triVBO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * 3 * sizeof(Vertex), m_triVertices, GL_DYNAMIC_DRAW);
	// generate buffer
	glGenBuffers(1, &m_triIBO);
	// bind vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triIBO);
	// fill vertex buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxTris * 3 * sizeof(unsigned int), m_triIndices, GL_DYNAMIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// generate vertex array
	glGenVertexArrays(1, &m_lineVAO);
	// bind vertex array
	glBindVertexArray(m_lineVAO);

	// generate buffer
	glGenBuffers(1, &m_lineVBO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, m_maxLines * 2 * sizeof(Vertex), m_lineVertices, GL_DYNAMIC_DRAW);
	//// generate buffer
	//glGenBuffers(1, &m_lineIBO);
	//// bind vertex buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIBO);
	//// fill vertex buffer
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxLines * 2 * sizeof(unsigned int), m_lineIndices, GL_DYNAMIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Mesh::~Mesh()
{
	delete[] m_triVertices;
	delete[] m_triIndices;
	glDeleteBuffers(1, &m_triVBO);
	glDeleteBuffers(1, &m_triIBO);
	glDeleteVertexArrays(1, &m_triVAO);
	delete[] m_lineVertices;
	//delete[] m_lineIndices;
	glDeleteBuffers(1, &m_lineVBO);
	//glDeleteBuffers(1, &m_lineIBO);
	glDeleteVertexArrays(1, &m_lineVAO);
}

void Mesh::AddLine(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec4 & colour)
{
	if (((float)m_lineVertexCount / 2.0f) < m_maxLines)
	{
		//bool exists[2] = { false, false };
		//for (int i = 0; i < m_lineVertexCount; i++)
		//{
		//	if (glm::vec3(m_lineVertices[i].position) == v0)
		//	{
		//		exists[0] = true;
		//		m_lineIndices[m_lineIndexCount] = i;
		//	}
		//	if (glm::vec3(m_lineVertices[i].position) == v1)
		//	{
		//		exists[1] = true;
		//		m_lineIndices[m_lineIndexCount + 1] = i;
		//	}
		//}

		if (true)
		{
			m_lineVertices[m_lineVertexCount].position.x = v0.x;
			m_lineVertices[m_lineVertexCount].position.y = v0.y;
			m_lineVertices[m_lineVertexCount].position.z = v0.z;
			m_lineVertices[m_lineVertexCount].position.w = 1;
			m_lineVertices[m_lineVertexCount].normal.x = colour.x;
			m_lineVertices[m_lineVertexCount].normal.y = colour.y;
			m_lineVertices[m_lineVertexCount].normal.z = colour.z;
			m_lineVertices[m_lineVertexCount].normal.w = colour.w;
			//m_lineIndices[m_lineIndexCount + 1] = m_lineVertexCount;
			m_lineVertexCount++;
		}
		if (true)
		{
			m_lineVertices[m_lineVertexCount].position.x = v1.x;
			m_lineVertices[m_lineVertexCount].position.y = v1.y;
			m_lineVertices[m_lineVertexCount].position.z = v1.z;
			m_lineVertices[m_lineVertexCount].position.w = 1;
			m_lineVertices[m_lineVertexCount].normal.x = colour.x;
			m_lineVertices[m_lineVertexCount].normal.y = colour.y;
			m_lineVertices[m_lineVertexCount].normal.z = colour.z;
			m_lineVertices[m_lineVertexCount].normal.w = colour.w;
			//m_lineIndices[m_lineIndexCount + 2] = m_lineVertexCount;
			m_lineVertexCount++;
		}

		//m_lineIndexCount += 3;
	}
}

void Mesh::AddTri(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec4 & colour)
{
	if (((float)m_triIndexCount / 3.0f) < m_maxTris)
	{
		bool exists[3] = { false, false, false };
		for (int i = 0; i < m_triVertexCount; i++)
		{
			if (glm::vec3(m_triVertices[i].position) == v0)
			{
				exists[0] = true;
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
		if (!exists[0])
		{
			m_triVertices[m_triVertexCount].position.x = v0.x;
			m_triVertices[m_triVertexCount].position.y = v0.y;
			m_triVertices[m_triVertexCount].position.z = v0.z;
			m_triVertices[m_triVertexCount].position.w = 1;
			m_triVertices[m_triVertexCount].normal.x = colour.x;
			m_triVertices[m_triVertexCount].normal.y = colour.y;
			m_triVertices[m_triVertexCount].normal.z = colour.z;
			m_triVertices[m_triVertexCount].normal.w = colour.w;
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

		m_triIndexCount += 3;
	}
}

void Mesh::AddQuad(const glm::vec3 & center, const glm::vec2 & extents,
	const glm::vec4 & colour, const glm::mat4 * transform)
{
	glm::vec3 verts[4];
	glm::vec3 tempCenter = center;
	glm::vec3 vX(extents.x, 0.0f, 0.0f);
	glm::vec3 vZ(0.0f, 0.0f, extents.y);

	if (transform != nullptr)
	{
		vX = glm::vec3(*transform * glm::vec4(vX, 0.0f));
		vZ = glm::vec3(*transform * glm::vec4(vZ, 0.0f));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	verts[0] = tempCenter - vX - vZ;
	verts[1] = tempCenter - vX + vZ;
	verts[2] = tempCenter + vX + vZ;
	verts[3] = tempCenter + vX - vZ;

	AddTri(verts[0], verts[1], verts[2], colour);
	AddTri(verts[2], verts[3], verts[0], colour);

	m_triVertices[m_triVertexCount - 4].texCoord = { 0, 0 };
	m_triVertices[m_triVertexCount - 3].texCoord = { 0, 1 };
	m_triVertices[m_triVertexCount - 2].texCoord = { 1, 1 };
	m_triVertices[m_triVertexCount - 1].texCoord = { 1, 0 };

	m_triVertices[m_triVertexCount - 4].normal = { 0, 1, 0, 0 };
	m_triVertices[m_triVertexCount - 3].normal = { 0, 1, 0, 0 };
	m_triVertices[m_triVertexCount - 2].normal = { 0, 1, 0, 0 };
	m_triVertices[m_triVertexCount - 1].normal = { 0, 1, 0, 0 };
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

void Mesh::AddCylinder(const glm::vec3 & center, const float radius, const float halfLength,
	const unsigned int segments, const glm::vec4 & colour, const glm::mat4 * transform)
{
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	glm::vec4 white(1, 1, 1, 1);

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

		// lines
		AddLine(tempCenter + v1top, tempCenter + v2top, white);
		AddLine(tempCenter + v1top, tempCenter + v1bottom, white);
		AddLine(tempCenter + v1bottom, tempCenter + v2bottom, white);
	}
}

void Mesh::AddPyramid(const glm::vec3 & center, const float halfHeight, const float halfWidth,
	const glm::vec4 & colour, const glm::mat4 * transform)
{
	glm::vec3 verts[5];
	glm::vec3 up(0.0f, halfHeight, 0.0f);
	glm::vec3 right(halfWidth, 0.0f, 0.0f);
	glm::vec3 forward(0.0f, 0.0f, halfWidth);
	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;
	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

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

	AddLine(verts[0], verts[1], white);
	AddLine(verts[1], verts[2], white);
	AddLine(verts[2], verts[3], white);
	AddLine(verts[3], verts[0], white);
	AddLine(verts[0], verts[4], white);
	AddLine(verts[1], verts[4], white);
	AddLine(verts[2], verts[4], white);
	AddLine(verts[3], verts[4], white);
}

void Mesh::AddSphere(const glm::vec3 & center, const float radius, int rows, const int columns, const glm::vec4 & colour,
	const glm::mat4 * transform, const float longMin, const float longMax, const float latMin, const float latMax)
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

	glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

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

void Mesh::Draw()
{
	if ((m_triIndexCount * 3) > 0)
	{
		glBindVertexArray(m_triVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_triIndexCount * sizeof(unsigned int), m_triIndices);		
		glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_triVertexCount * sizeof(Vertex), m_triVertices);
		glDrawElements(GL_TRIANGLES, m_triIndexCount, GL_UNSIGNED_INT, 0);
	}
	if ((m_lineVertexCount * 2) > 0)
	{
		glBindVertexArray(m_lineVAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIBO);
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_lineIndexCount * sizeof(unsigned int), m_lineIndices);		
		glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_lineVertexCount * sizeof(Vertex), m_lineVertices);
		//glDrawElements(GL_LINES, m_lineIndexCount, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_LINES, 0, m_lineVertexCount);
	}
}