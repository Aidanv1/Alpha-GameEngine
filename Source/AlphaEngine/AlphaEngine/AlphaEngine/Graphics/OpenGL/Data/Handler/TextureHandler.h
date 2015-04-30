//TextureHandler serves a similar purpose to ResourceManager. It is different
//in that it manages the texture buffers currently stored in video memory (VRAM)
//whereas the ResourseManager deals only with system memory.

#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H
#include "../../../../AlphaStd.h"
#include <GL/glew.h>
typedef GLuint TextureID;
//========================================================================
class TextureHandler
{

	struct Handle
	{
		TextureID m_textureID;
		int m_size;
		string m_textureFileName;
		Handle() :
			m_textureID(0),
			m_size(0),
			m_textureFileName("")
		{
		}
		bool operator == (const Handle& rhs)
		{
			if (this->m_textureID == rhs.m_textureID)
			{
				return true;
			}
			return false;
		}
	};
	typedef map<TextureID, Handle> TextureHandlerMap; 

public:
	~TextureHandler();
	//Return static instance
	static TextureHandler& Get();
	//Returns a valid texture ID
	TextureID GetTextureHandle(int size, string textureFileName);
	//Checks if the textureID is still referencing the same texture resource
	bool ValidateID(TextureID textureID, string textureFileName);
	//Free the specified texturebuffer
	void FreeTextureBuffer(TextureID textureID);
private:
	TextureHandler();
private:
	TextureHandlerMap m_textureHandlerMap;
	list<Handle> m_handleLRUList;
	int m_sizeTotal;
	int m_sizeAllocated;
};
//========================================================================
#endif