//VertexBufferHandler serves a similar purpose to ResourceManager. It is different
//in that it manages the texture buffers currently stored in video memory (VRAM)
//whereas the ResourseManager deals only with system memory.
#pragma once

#include "../../../../AlphaStd.h"
#include <GL/glew.h>
typedef GLuint VertexBufferID;
class VertexBufferHandler
{
	struct Handle
	{
		VertexBufferID m_vertexBufferID;
		int m_size;
		string m_meshFileName;
		Handle() :
			m_vertexBufferID(0),
			m_size(0),
			m_meshFileName("")
		{
		}
		bool operator == (const Handle& rhs)
		{
			if (this->m_vertexBufferID == rhs.m_vertexBufferID)
			{
				return true;
			}
			return false;
		}
	};

	typedef map<VertexBufferID, Handle> VertexBufferHandlerMap;

public:
	~VertexBufferHandler();
	//Return static instance
	static VertexBufferHandler& Get();
	//Returns a valid vertexbuffer ID
	VertexBufferID GetVertexBufferHandle(int size, string meshFileName);
	//Checks if the textureID is still referencing the same texture resource
	bool ValidateID(VertexBufferID vertexBufferID, string meshFileName);
	//Free the specified vertexbuffer
	void FreeVertexBuffer(VertexBufferID vertexBufferID);
private:
	VertexBufferHandler();
private:
	VertexBufferHandlerMap m_vBufferHandlerMap;
	list<Handle> m_handleLRUList;
	int m_sizeTotal;
	int m_sizeAllocated;
};
