#pragma once

//========================================================================
class IWindow
{
public:
	virtual bool VInit() = 0;
	virtual bool VUpdate(float deltaMs) = 0;
};
//========================================================================
