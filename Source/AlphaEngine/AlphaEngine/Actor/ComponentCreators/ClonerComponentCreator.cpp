#include "ClonerComponentCreator.h"
#include "../Components/ClonerComponent.h"
#include "../../AlphaStd.h"
// -----------------------------------------------------------------------
IActorComponent* ClonerComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	ClonerComponent* cloneComp = ALPHA_NEW ClonerComponent();
	return cloneComp;
}
// -----------------------------------------------------------------------