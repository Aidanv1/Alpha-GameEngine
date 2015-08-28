#include "GameLoop\GameLoop.h"
#include "Common/GameContext.h"
#include "EventManager\Events\Events.h"
#include "EventManager\EventManager.h"
#include "Window\IWindow.h"

//========================================================================
//GAME LOOP
//========================================================================
class TestGameLoop : public GameLoop
{
private:
	void VDoGameLogic() override
	{
		GameContext::Get()->GetGraphicsRenderer()->VDrawPoint(vec3(1), 20, vec4(1));
	}
};
//========================================================================
//INPUT FUNCTIONS
//========================================================================
void CameraLook(MotionEvent e)
{
	float lookSensitivity = 0.0004f;
	float adjust = lookSensitivity;
	LookEvent* lookEvent = new LookEvent();
	lookEvent->SetLook(e.m_x* adjust, e.m_y* adjust, 0, 0, 0);
	Queue_Event(lookEvent);
}
// -----------------------------------------------------------------------
void CameraZoom(MotionEvent e)
{
	LookEvent* lookEvent = new LookEvent();
	lookEvent->SetLook(0, 0, 0, 0, e.m_x);
	Queue_Event(lookEvent);
}
// -----------------------------------------------------------------------
void CameraMoveForward()
{
	LookEvent* lookEvent = new LookEvent();
	lookEvent->SetLook(0, 0, 1, 0, 0);
	Queue_Event(lookEvent);
}
//========================================================================
int main(int argc, char *argv[])
{
	TestGameLoop gameLoop;
	if (gameLoop.Init())
	{
		MotionCommand m(CameraLook);
		MotionCommand m2(CameraZoom);
		KeyCommand k;
		k.Bind(CameraMoveForward);
		gameLoop.GetWindow()->VSetMotionInputCommand(Mouse, m);
		gameLoop.GetWindow()->VSetMotionInputCommand(MouseWheel, m2);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_W, k);
		gameLoop.StartLoop();
	}
	return 0;
}
//========================================================================
