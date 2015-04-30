#pragma once
#include "../../AlphaStd.h"
#include <GL/glew.h>
//========================================================================
struct BmpData
{
	int height;
	int width;
	shared_ptr<unsigned char> pixel_data;
	int size;
};
//========================================================================
struct MeshData
{
	vector<GLfloat> m_vArray;
	vector<GLfloat> m_nArray;
	vector<GLfloat> m_tArray;
	int m_numVertices;
};
//========================================================================