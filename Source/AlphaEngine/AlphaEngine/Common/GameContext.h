#pragma once
class Clock;
//========================================================================
class GameContext
{
public:
	void InitContext(Clock* gameClock);
	//singleton
	static GameContext* Get();
	__int64 GetGameTime();
private:
	GameContext();
	~GameContext();
private:
	bool	m_isInitialized;
	Clock*	m_gameClock;
};
//========================================================================