#pragma once
#include "Input.h"
//========================================================================
class IWindow
{
public:
	virtual bool VInit() = 0;
	virtual bool VUpdate(float deltaMs) = 0;
	virtual void VSetKeyInputCommand(KeyName key, KeyCommand command, bool onPress = true) = 0;
	virtual void VSetMotionInputCommand(MotionType motionType, MotionCommand command) = 0;
	virtual void VSetBindingSetHandler(StrongBindingSetHandlerPtr BindingSetHandler) = 0;
	virtual StrongBindingSetHandlerPtr VGetBindingSetHandler() const = 0;
};
//========================================================================
