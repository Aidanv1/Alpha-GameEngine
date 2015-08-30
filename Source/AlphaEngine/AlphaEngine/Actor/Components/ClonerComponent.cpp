#include "ClonerComponent.h"
#include "../../AlphaStd.h"
#include "../RoleSystem.h"
// -----------------------------------------------------------------------
ClonerComponent::ClonerComponent()
{
	
}
// -----------------------------------------------------------------------
ClonerComponent::~ClonerComponent()
{
	
}
// -----------------------------------------------------------------------
void ClonerComponent::VUpdate(float deltaMs)
{

}
// -----------------------------------------------------------------------
bool ClonerComponent::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* cloneElem = pElement->FirstChildElement();
	bool gotActorInitData = false;
	while (cloneElem != NULL)
	{
		string value = cloneElem->Value();
		if (value == "Properties")
		{
			string type = cloneElem->Attribute("transformType");
			if (type == "positions")
			{
				string pointFile = cloneElem->Attribute("positionsFile");
				TiXmlDocument pointDoc(pointFile.c_str());
				pointDoc.LoadFile();
				TiXmlElement* posElem = pointDoc.FirstChildElement()->FirstChildElement();
				while (posElem != NULL)
				{
					vec3 pos;
					posElem->QueryFloatAttribute("pX", &pos.x);
					posElem->QueryFloatAttribute("pY", &pos.y);
					posElem->QueryFloatAttribute("pZ", &pos.z);

					vec3 rot;
					posElem->QueryFloatAttribute("rX", &rot.x);
					posElem->QueryFloatAttribute("rY", &rot.y);
					posElem->QueryFloatAttribute("rZ", &rot.z);

					Position p;
					p.m_rot = rot;
					p.m_trans = pos;

					m_positionList.push_back(p);
					posElem = posElem->NextSiblingElement();
				}
			}
		}
		if (value == "Actor" && 
			!gotActorInitData)
		{
			m_actorInitData = ALPHA_NEW TiXmlElement(*cloneElem);
			gotActorInitData = true;
		}
		cloneElem = cloneElem->NextSiblingElement();
	}
	if (!gotActorInitData)
	{
		ALPHA_ERROR("Cloner is missing an Actor element");
	}
	return true;
}
// -----------------------------------------------------------------------
bool ClonerComponent::VPostInit()
{
	int i = 0;
	for (auto it = m_positionList.begin(); it != m_positionList.end(); it++, i++)
	{
		vec3 pos = it->m_trans;
		vec3 rot = it->m_rot;

		TiXmlElement tempElem(*m_actorInitData);
		TiXmlElement transformElem("Transform");
		TiXmlElement posTransformElem("Position");
		TiXmlElement rotTransformElem("RotationInDegrees");

		posTransformElem.SetAttribute("x", to_string(pos.x).c_str());
		posTransformElem.SetAttribute("y", to_string(pos.y).c_str());
		posTransformElem.SetAttribute("z", to_string(pos.z).c_str());

		rotTransformElem.SetAttribute("xAxis", to_string(rot.x).c_str());
		rotTransformElem.SetAttribute("yAxis", to_string(rot.y).c_str());
		rotTransformElem.SetAttribute("zAxis", to_string(rot.z).c_str());

		transformElem.InsertEndChild(posTransformElem);
		transformElem.InsertEndChild(rotTransformElem);

		tempElem.InsertEndChild(transformElem);
		string name = tempElem.Attribute("name");
		tempElem.RemoveAttribute("name");
		name.append(".").append(to_string(i));
		tempElem.SetAttribute("name", name.c_str());
		RoleSystem::Get().AddActor(&tempElem);
	}
	return true;
}
// -----------------------------------------------------------------------
