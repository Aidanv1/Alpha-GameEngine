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
