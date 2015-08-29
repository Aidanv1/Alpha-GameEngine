#include "GameLoop\GameLoop.h"
#include "Common/GameContext.h"
#include "EventManager\Events\Events.h"
#include "Actor/ActorEvents.h"
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
	float lookSensitivity = 0.0001f;
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
void MoveForward()
{
	ActorMovedEvent* e = new ActorMovedEvent(4);
	e->SetVelocity(vec3(0, 0, -10));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void MoveBackward()
{
	ActorMovedEvent* e = new ActorMovedEvent(4);
	e->SetVelocity(vec3(0, 0, 10));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void TurnLeft()
{
	ActorMovedEvent* e = new ActorMovedEvent(4);
	e->SetAngularVelocity(vec3(0, 2, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void TurnRight()
{
	ActorMovedEvent* e = new ActorMovedEvent(4);
	e->SetAngularVelocity(vec3(0, -2, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void StopTurning()
{
	ActorMovedEvent* e = new ActorMovedEvent(4);
	e->SetAngularVelocity(vec3(0, 0, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void DestroyActor()
{
	ActorDestroyedEvent* e = new ActorDestroyedEvent(3);
	Queue_Event(e);
}
//========================================================================
int main(int argc, char *argv[])
{
	TestGameLoop gameLoop;
	if (gameLoop.Init())
	{
		MotionCommand m(CameraLook);
		MotionCommand m2(CameraZoom);
		KeyCommand k(MoveForward);
		KeyCommand k3(MoveBackward);
		KeyCommand k2(DestroyActor);

		KeyCommand k4(TurnLeft);
		KeyCommand k5(TurnRight);
		KeyCommand k6(StopTurning);


		gameLoop.GetWindow()->VSetMotionInputCommand(Mouse, m);
		gameLoop.GetWindow()->VSetMotionInputCommand(MouseWheel, m2);
		//
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_W, k);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_S, k3);

		gameLoop.GetWindow()->VSetKeyInputCommand(Key_A, k4);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_D, k5);

		gameLoop.GetWindow()->VSetKeyInputCommand(Key_A, k6, false);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_D, k6, false);


		gameLoop.GetWindow()->VSetKeyInputCommand(Key_X, k2);
		gameLoop.StartLoop();
	}
	return 0;
}
//========================================================================
