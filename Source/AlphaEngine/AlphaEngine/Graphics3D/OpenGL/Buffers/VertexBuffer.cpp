#include "VertexBuffer.h"
// -----------------------------------------------------------------------
VertexBuffer::VertexBuffer() :
m_vertexInfo(),
m_name("")
{
}
// -----------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	//free blocks in video memory
	VertexBufferHandler::Get().FreeVertexBuffer(m_vertexInfo.m_vertexBufferID);
	glDeleteVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
}
// -----------------------------------------------------------------------
int VertexBuffer ::Init(int numvertices, GLfloat vertices[], GLfloat normals[], GLfloat textures[], int compSizeV, int compSizeN, int compSizeT, string name)
{
	m_name = name;
	const int floatsPerUV = compSizeT;
	const int floatsPerN = compSizeN;
	const int floatsPerV = compSizeV;
	const int floatsPerVertex = floatsPerUV + floatsPerN + floatsPerV;
	//initialize vertex buffer
	if (m_vertexInfo.m_vertexArrayID == -1)
	{
		glGenVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	}
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	m_vertexInfo.m_vertexBufferID = VertexBufferHandler::Get().GetVertexBufferHandle(floatsPerVertex * numvertices*sizeof(GLfloat), name);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, floatsPerVertex * numvertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, floatsPerV * numvertices*sizeof(GLfloat), vertices);
	// Load the normals
	glBufferSubData(GL_ARRAY_BUFFER, floatsPerV * numvertices*sizeof(GLfloat), floatsPerN * numvertices*sizeof(GLfloat), normals);
	// load the uv coords
	glBufferSubData(GL_ARRAY_BUFFER, (floatsPerV + floatsPerN) * numvertices*sizeof(GLfloat), floatsPerUV * numvertices*sizeof(GLfloat), textures);
	//-----------------------------------------------
	return m_vertexInfo.m_vertexBufferID;
}
// -----------------------------------------------------------------------
int VertexBuffer::Init(int numvertices, GLfloat vertices[], GLfloat textures[], int compSizeV, int compSizeT, string name)
{
	const int floatsPerUV = compSizeT;
	const int floatsPerV = compSizeV;
	const int floatsPerVertex = floatsPerUV + floatsPerV;
	m_name = name;
	//initialize vertex buffer
	if (m_vertexInfo.m_vertexArrayID == -1)
	{
		glGenVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	}
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	m_vertexInfo.m_vertexBufferID = VertexBufferHandler::Get().GetVertexBufferHandle(floatsPerVertex * numvertices*sizeof(GLfloat), name);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);

	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, floatsPerVertex * numvertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, floatsPerV * numvertices*sizeof(GLfloat), vertices);
	// Load the uv 
	glBufferSubData(GL_ARRAY_BUFFER, (floatsPerV) * numvertices*sizeof(GLfloat), floatsPerUV * numvertices*sizeof(GLfloat), textures);
	//-----------------------------------------------
	return m_vertexInfo.m_vertexBufferID;
}
// -----------------------------------------------------------------------
int VertexBuffer::Init(int numvertices, GLfloat vertices[], int compSizeV, string name)
{
	const int floatsPerV = compSizeV;
	const int floatsPerVertex = floatsPerV;
	m_name = name;
	//initialize vertex buffer
	if (m_vertexInfo.m_vertexArrayID == -1)
	{
		glGenVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	}
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	m_vertexInfo.m_vertexBufferID = VertexBufferHandler::Get().GetVertexBufferHandle(floatsPerVertex * numvertices*sizeof(GLfloat), name);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);

	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, floatsPerVertex * numvertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, floatsPerV * numvertices*sizeof(GLfloat), vertices);
	//-----------------------------------------------
	return m_vertexInfo.m_vertexBufferID;
}
// -----------------------------------------------------------------------
void VertexBuffer::BindSubData(int numVertices, int componentSize, int offsetSize,GLfloat textures[])
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, offsetSize * numVertices*sizeof(GLfloat), componentSize * numVertices*sizeof(GLfloat), textures);

}
// -----------------------------------------------------------------------
bool VertexBuffer::Validate()
{
	if (m_vertexInfo.m_vertexBufferID != -1)
	{
		//if the vertex buffer is still valid, return its ID.
		if (VertexBufferHandler::Get().ValidateID(m_vertexInfo.m_vertexBufferID, m_name))
		{
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
void VertexBuffer::FreeVertexBuffer()
{
	//free blocks in video memory
	VertexBufferHandler::Get().FreeVertexBuffer(m_vertexInfo.m_vertexBufferID);
	glDeleteVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	m_vertexInfo.m_vertexBufferID = -1;
	m_vertexInfo.m_vertexArrayID = -1;
}
// -----------------------------------------------------------------------
void VertexBuffer::SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset)
{
	Validate();
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	glVertexAttribPointer(attribID, componentCount, GL_FLOAT, GL_FALSE, stride, dataoffset);
	glEnableVertexAttribArray(attribID);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}