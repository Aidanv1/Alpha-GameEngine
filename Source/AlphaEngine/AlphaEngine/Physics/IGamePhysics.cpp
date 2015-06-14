#include "IGamePhysics.h"
IGamePhysics* IGamePhysics::globalGamePhysics = NULL;
// -----------------------------------------------------------------------
IGamePhysics::IGamePhysics(bool isGlobal)
{
	if (isGlobal)
	{
		if (!globalGamePhysics)
		{
			globalGamePhysics = this;
		}
	}
}
// -----------------------------------------------------------------------
IGamePhysics::~IGamePhysics()
{
	if (globalGamePhysics == this)
	{
		globalGamePhysics = NULL;
	}
}
// -----------------------------------------------------------------------
IGamePhysics* IGamePhysics::Get()
{
	if (globalGamePhysics)
	{
		return globalGamePhysics;
	}
	return NULL;
}
// -----------------------------------------------------------------------