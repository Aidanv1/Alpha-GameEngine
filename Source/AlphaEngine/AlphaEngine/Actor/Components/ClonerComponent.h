#pragma once
#include "../IActorComponent.h"
#include "../../Maths/GameMaths.h"
#include <vector>
class TiXmlElement;
//========================================================================
struct Position
{
	vec3 m_trans;
	vec3 m_rot;
};
//========================================================================
typedef std::vector<Position> PositionList;
//========================================================================
class DLLExport ClonerComponent : public IActorComponent
{
public:
	ClonerComponent();
	~ClonerComponent();
	//IActorComponent functions
	ComponentType VGetType() const override { return "Cloner"; }
	void VUpdate(float deltaMs) override;
	bool VInitComponent(TiXmlElement* pElement) override;
	bool VPostInit() override;
private:
	TiXmlElement* m_actorInitData;
	PositionList m_positionList;
};
//========================================================================