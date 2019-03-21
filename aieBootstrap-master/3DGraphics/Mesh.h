#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh();
	~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 textCoord;
	};

	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	virtual void Draw();

protected:
	unsigned int m_triCount;
	unsigned int m_vao, m_vbo, m_ibo;
};