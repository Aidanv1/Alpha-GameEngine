#pragma once
#include "..\AlphaStd.h"
using namespace std;
//------------
//========================================================================
class IResourceLoader
{
public:
	virtual string VGetPattern() = 0;
	virtual bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) = 0;
	virtual unsigned int  VGetSize(string resName) = 0;
};
//========================================================================