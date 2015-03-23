#include "IEventManager.h"

IEventManager* IEventManager::globalManager;

IEventManager::IEventManager(bool setAsGlobal)
{
	if (setAsGlobal)
	{
		if (!globalManager)
			globalManager = this;
	}
}
IEventManager::~IEventManager()
{
	if (globalManager == this)
		globalManager = NULL;
}

IEventManager* IEventManager::Get()
{
	if (globalManager)
		return globalManager;
	else
		return NULL;
}