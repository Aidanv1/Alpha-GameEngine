#pragma once
#include "../Common/FuncDelegate.h"
//========================================================================
enum KeyName
{
	//letters
	Key_Q,
	Key_W,
	Key_E,
	Key_R,
	Key_T,
	Key_Y,
	Key_U,
	Key_I,
	Key_O,
	Key_P,
	Key_A,
	Key_S,
	Key_D,
	Key_F,
	Key_G,
	Key_H,
	Key_J,
	Key_K,
	Key_L,
	Key_Z,
	Key_X,
	Key_C,
	Key_V,
	Key_B,
	Key_N,
	Key_M,
	//Misc
	Key_Up,
	Key_Down,
	Key_Left,
	Key_Right,
	Key_Shift_Left,
	Key_Shift_Right,
	Key_CapsLock,
	Key_Ctrl_Left,
	Key_Ctrl_Right
	//add to this as needed
};
typedef KeyName Key;
// -----------------------------------------------------------------------
enum MotionType
{
	Mouse,
	MouseWheel
};
// -----------------------------------------------------------------------
struct MotionEvent
{
	int m_x;
	int m_y;

	MotionEvent(int x, int y) :
		m_x(x),
		m_y(y)
	{
	}
};
typedef AJ::FuncDelegate<void, void> KeyCommand;
typedef AJ::FuncDelegate<void, MotionEvent> MotionCommand;

//========================================================================
