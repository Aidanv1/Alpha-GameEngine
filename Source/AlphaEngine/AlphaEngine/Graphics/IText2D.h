#ifndef ITEXT2D_H
#define ITEXT2D_H

class IText2D
{
public:
	virtual bool VInitText2D(string fontTexturePath, int maxChar) = 0;
	virtual void VPrintText2D(string text, float x, float y, float size, float aspectRatio = 16.0/9.0) = 0;
	virtual void VCleanUpText2D() = 0;
};

#endif