#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	void CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();


private:
	unsigned int VBO, VAO, IBO;
	int indexCount;
};
