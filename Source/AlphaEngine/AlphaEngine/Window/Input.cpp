#include "Input.h"
// -----------------------------------------------------------------------
BindingSetHandler::BindingSetHandler()
{

}
// -----------------------------------------------------------------------
BindingSetHandler::~BindingSetHandler()
{

}
// -----------------------------------------------------------------------
void BindingSetHandler::AddBinding(KeyName key, KeyCommand command, bool onPress)
{
	Key keyInput = key;
	//	keyInput.m_name = key;
	//	keyInput.m_onPress = onPress;
	if (onPress)
	{
		m_keyDownCommandMap[keyInput] = command;
	}
	else
	{
		m_keyUpCommandMap[keyInput] = command;
	}
	m_keyCurrentStateMap[keyInput] = false;
}
// -----------------------------------------------------------------------
void BindingSetHandler::AddBinding(MotionType motionType, MotionCommand command)
{
	m_motionCommandMap[motionType] = command;
}
// -----------------------------------------------------------------------
void BindingSetHandler::DoCommand(KeyName key, bool pressed)
{
	//if a command is bound to the key then execute it
	if (pressed)
	{
		auto findIt = m_keyDownCommandMap.find(key);
		if (findIt != m_keyDownCommandMap.end())
		{
			if (m_keyCurrentStateMap[key] != pressed)
			{
				findIt->second();
			}
		}
	}
	else
	{
		auto findIt = m_keyUpCommandMap.find(key);
		if (findIt != m_keyUpCommandMap.end())
		{
			if (m_keyCurrentStateMap[key] != pressed)
			{
				findIt->second();
			}
		}
	}
	m_keyCurrentStateMap[key] = pressed;
}
// -----------------------------------------------------------------------
void BindingSetHandler::DoCommand(MotionType motionType, MotionEvent e)
{
	//if a command is bound to the key then execute it
	auto findIt = m_motionCommandMap.find(motionType);
	if (findIt != m_motionCommandMap.end())
	{
		findIt->second(e);
	}
}
// -----------------------------------------------------------------------
void BindingSetHandler::ResetStates()
{
	for (auto it = m_keyCurrentStateMap.begin(); it != m_keyCurrentStateMap.end(); it++)
	{
		it->second = false;
	}
}