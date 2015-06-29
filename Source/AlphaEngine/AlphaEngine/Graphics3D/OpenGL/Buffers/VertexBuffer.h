#pragma once
#include "../../../AlphaStd.h"
#include <GL/glew.h>
#include "../Handler/VertexBufferHandler.h"
//========================================================================
class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	int Init(int numvertices, GLfloat vertices[], GLfloat normals[], GLfloat textures[], int compSizeV, int compSizeN, int compSizeT, string name);
	int Init(int numvertices, GLfloat vertices[], GLfloat textures[], int compSizeV, int compSizeT, string name);
	int Init(int numvertices, GLfloat vertices[], int compSizeV, string name);
	int Init(int size, GLfloat data[], string name);
	void BindSubData(int numVertices, int componentSize, int offsetSize, GLfloat vertices[]);
	bool Validate();
	int GetID() const { return m_vertexInfo.m_vertexBufferID; }
	void FreeVertexBuffer();
	void SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset);
private:
	struct VertexBufferInfo
	{
		GLuint m_vertexBufferID;
		GLuint m_vertexArrayID;
		VertexBufferInfo() :
			m_vertexBufferID(-1),
			m_vertexArrayID(-1)
		{
		}
	};
	VertexBufferInfo m_vertexInfo;
	string m_name;
};
//========================================================================
