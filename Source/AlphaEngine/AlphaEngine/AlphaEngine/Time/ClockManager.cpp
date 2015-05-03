#include "ClockManager.h"

ClockManager::ClockManager()
{
}
ClockManager& ClockManager::Get()
{
	static ClockManager* instance = new ClockManager();
	return *instance;
}
// -----------------------------------------------------------------------
void ClockManager::AddClock(Clock* clock)
{
	m_ClockList.push_back(clock);

}
// -----------------------------------------------------------------------
void ClockManager::RemoveClock(Clock* clock)
{
	vector<Clock*>::iterator clockIt = m_ClockList.begin();
	int i = 0;
	for ( clockIt ; clockIt < m_ClockList.end() ; clockIt++, i++ )
	{
		if (m_ClockList.at(i) == clock)
		{
			m_ClockList.erase(clockIt);
			return;
		}
		
	}
}
// -----------------------------------------------------------------------
void ClockManager::UpdateClocks(float deltaMs)
{
	auto clockIt = m_ClockList.begin();
	int i = 0;
	for (clockIt; clockIt < m_ClockList.end(); clockIt++, i++)
	{
		m_ClockList.at(i)->Update(deltaMs);
	}
}
// -----------------------------------------------------------------------
int ClockManager::GetNumberOfClocks() const
{
	return m_ClockList.size();
}
// -----------------------------------------------------------------------