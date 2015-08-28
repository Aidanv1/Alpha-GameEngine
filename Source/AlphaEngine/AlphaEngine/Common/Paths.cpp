#include "Paths.h"
#include "../AlphaStd.h"
// -----------------------------------------------------------------------
Paths::Paths()
{

}
// -----------------------------------------------------------------------
Paths::~Paths()
{

}
// -----------------------------------------------------------------------
bool Paths::Load(const char* pathsFileName)
{
	TiXmlDocument doc(pathsFileName);
	doc.LoadFile();
	TiXmlElement* pElem = doc.FirstChildElement()->FirstChildElement();
	while (pElem != NULL)
	{
		string value = pElem->Value();
		if (value == "Path")
		{
			PathID id = pElem->Attribute("id");
			Path location = pElem->Attribute("relPath");
			m_pathMap[id] = location;
		}
		pElem = pElem->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
Path Paths::GetPath(PathID id) const
{
	auto findIt = m_pathMap.find(id);
	if (findIt != m_pathMap.end())
	{
		return findIt->second;
	}
	return "";
}
// -----------------------------------------------------------------------