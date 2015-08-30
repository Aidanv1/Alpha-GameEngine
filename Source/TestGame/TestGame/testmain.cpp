#include "GameLoop\GameLoop.h"
#include "Common/GameContext.h"
#include "Graphics3D\GraphicsEvents.h"
#include "Actor/ActorEvents.h"
#include "EventManager\EventManager.h"
#include "Window\IWindow.h"
#include "Physics\PhysicsEvents.h"
#include "Actor\RoleSystem.h"
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
//GAME STATE TEST CLASS
//========================================================================
class GameStateClass
{
public:
	GameStateClass() :
		m_actorOnGround(true)
	{}
	void OnRemovedCollision(StrongEventPtr e)
	{
		RemovedCollisionEvent* colEvent = dynamic_cast<RemovedCollisionEvent*>(e.get());
		if (colEvent->GetCollisionPair().first == 4 ||
			colEvent->GetCollisionPair().second == 4)
		{
			m_actorOnGround = false;
			ActorMovedEvent* e = new ActorMovedEvent(4);
			e->SetVelocity(vec3(0, 0, 0));
			Queue_Event(e);
		}
	}
	void OnNewCollision(StrongEventPtr e)
	{
		NewCollisionEvent* colEvent = dynamic_cast<NewCollisionEvent*>(e.get());
		if (colEvent->GetCollisionPair().first.m_actorID == 4 ||
			colEvent->GetCollisionPair().second.m_actorID == 4)
		{
			m_actorOnGround = true;
		}
	}
	bool IsActorOnGround() { return m_actorOnGround; }
private:
	bool m_actorOnGround;
};

GameStateClass g_gameState;
ActorID m_mainActorID = -1;
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
	if (!g_gameState.IsActorOnGround())
	{
		return;
	}
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetVelocity(vec3(0, 0, -10));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void MoveBackward()
{
	if (!g_gameState.IsActorOnGround())
	{
		return;
	}
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetVelocity(vec3(0, 0, 10));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void TurnLeft()
{
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetAngularVelocity(vec3(0, 2, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void TurnRight()
{
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetAngularVelocity(vec3(0, -2, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void StopTurning()
{
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetAngularVelocity(vec3(0, 0, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void StopMoving()
{
	ActorMovedEvent* e = new ActorMovedEvent(m_mainActorID);
	e->SetVelocity(vec3(0, 0, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void DestroyActor()
{
	ActorDestroyedEvent* e = new ActorDestroyedEvent(m_mainActorID);
	Queue_Event(e);
}
// -----------------------------------------------------------------------
void Jump()
{
	ActorJumpedEvent* e = new ActorJumpedEvent(m_mainActorID, vec3(0, 150, 0));
	Queue_Event(e);
}
// -----------------------------------------------------------------------

//========================================================================
int main(int argc, char *argv[])
{
	TestGameLoop gameLoop;
	if (gameLoop.Init())
	{
		Register_Listener(EVENT_PHYSICS_REMOVEDCOLLISION, &g_gameState, &GameStateClass::OnRemovedCollision);
		Register_Listener(EVENT_PHYSICS_NEWCOLLISION, &g_gameState, &GameStateClass::OnNewCollision);
		MotionCommand m(CameraLook);
		MotionCommand m2(CameraZoom);
		KeyCommand k(MoveForward);
		KeyCommand k3(MoveBackward);
		KeyCommand k2(DestroyActor);

		KeyCommand k4(TurnLeft);
		KeyCommand k5(TurnRight);
		KeyCommand k6(StopTurning);

		KeyCommand k7(StopMoving);

		KeyCommand k8(Jump);


		gameLoop.GetWindow()->VSetMotionInputCommand(Mouse, m);
		gameLoop.GetWindow()->VSetMotionInputCommand(MouseWheel, m2);
		//
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_W, k);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_S, k3);

		gameLoop.GetWindow()->VSetKeyInputCommand(Key_W, k7, false);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_S, k7, false);

		gameLoop.GetWindow()->VSetKeyInputCommand(Key_A, k4);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_D, k5);

		gameLoop.GetWindow()->VSetKeyInputCommand(Key_A, k6, false);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_D, k6, false);


		gameLoop.GetWindow()->VSetKeyInputCommand(Key_X, k2);
		gameLoop.GetWindow()->VSetKeyInputCommand(Key_Space, k8);

		m_mainActorID = RoleSystem::Get().GetActor("test")->GetID();
		gameLoop.StartLoop();
	}
	return 0;
}
//========================================================================
