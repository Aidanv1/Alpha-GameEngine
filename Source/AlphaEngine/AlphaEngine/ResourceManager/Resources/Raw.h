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
///***************************DEFINITIONS*********************************
//BITMAP BUFFER DEFINITIONS
#define BMPSIZE_LOC						0
#define BMPHEIGHT_LOC					sizeof(int)
#define BMPWIDTH_LOC					BMPHEIGHT_LOC + sizeof(int)
#define BMPPIXELS_LOC					BMPWIDTH_LOC + sizeof(int)
//========================================================================
//MESH BUFFER DEFINITIONS
#define MESH_NUMVERT_LOC				0											//location for number of vertices
//geometry
#define MESH_VERT_COMPONENTSIZE_LOC		sizeof(int)									//location of position component size
#define MESH_NORM_COMPONENTSIZE_LOC		MESH_VERT_COMPONENTSIZE_LOC + sizeof(int)	//location of normal component size
#define MESH_TEXT_COMPONENTSIZE_LOC		MESH_NORM_COMPONENTSIZE_LOC + sizeof(int)	//location of uv component size
#define MESH_DATA_LOC					MESH_TEXT_COMPONENTSIZE_LOC + sizeof(int)	//location of mesh data
//========================================================================
///*****************************STRUCTS***********************************
struct BitmapInfo
{
	int m_height;
	int m_width;
	int m_size;
};
// -----------------------------------------------------------------------
struct MeshInfo
{
	int m_numberOfVertices;
	int m_positionComponentSize;
	int m_normalComponentSize;
	int m_uvComponentSize;
};
//========================================================================
//BITMAP BUFFER READER
class BitMapBufferReader
{
public:
	BitMapBufferReader(unsigned char* buffer);
	BitmapInfo GetInfo();
	unsigned char* PixelData();
private:
	unsigned char* m_pBuffer;
};
//========================================================================
//MESH BUFFER READER
class MeshBufferReader
{
public:
	MeshBufferReader(unsigned char* buffer);
	MeshInfo GetMeshInfo();
	unsigned char* MeshDataBuffer();
private:
	unsigned char* m_pBuffer;
};