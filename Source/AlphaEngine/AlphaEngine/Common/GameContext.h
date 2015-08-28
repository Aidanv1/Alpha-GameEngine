#pragma once
#include "../AlphaEngineDLL.h"
#include "Paths.h"
#include "../Graphics3D/IRenderer.h"
class Clock;
class RoleSystem;
//========================================================================
class DLLExport GameContext
{
public:
	void InitContext(Clock* gameClock);
	//singleton
	static GameContext* Get();
	__int64 GetGameTime() const;
	IRenderer* GetGraphicsRenderer() const;
	RoleSystem* GetRoleSystem() const;
	Path GetPath(PathID id);
private:
	GameContext();
	~GameContext();
private:
	bool	m_isInitialized;
	Clock*	m_gameClock;
	Paths   m_paths;
};
//========================================================================