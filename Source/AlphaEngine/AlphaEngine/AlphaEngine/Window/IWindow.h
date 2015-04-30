#ifndef IWINDOW_H
#define IWINDOW_H
//========================================================================
class IWindow
{
public:
	virtual bool VInit() = 0;
	virtual bool VUpdate() = 0;
};
//========================================================================
#endif