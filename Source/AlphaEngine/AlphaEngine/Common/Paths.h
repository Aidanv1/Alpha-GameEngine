#pragma once
#include <map>
#include <string>
#include "../AlphaEngineDLL.h"
//========================================================================
//forward declaration
class TiXmlElement;
//========================================================================
typedef std::string PathID;
typedef std::string Path;
typedef std::map<PathID, Path> PathMap;
//========================================================================
class DLLExport Paths
{
public:
	Paths();
	~Paths();
	bool Load(const char* pathsFileName);
	Path GetPath(PathID id) const;
private:
	PathMap m_pathMap;
};
//========================================================================