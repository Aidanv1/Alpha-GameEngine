#include "GraphicsSettings.h"
// -----------------------------------------------------------------------
bool	GraphicsSettings::s_WIREFRAME_ENABLED = false;	string GraphicsSettings::tag_WIREFRAME_ENABLED = "WIRE_FRAME";
bool	GraphicsSettings::s_DEVMODE_ENABLED = false;	string GraphicsSettings::tag_DEVMODE_ENABLED = "DEVMODE";
int		GraphicsSettings::s_MAXLIGHTS = 10;				string GraphicsSettings::tag_MAX_LIGHTS = "MAX_LIGHTS";
bool	GraphicsSettings::s_FOG_ENABLED = true;			string GraphicsSettings::tag_FOG_ENABLED = "FOG_ENABLED";
float	GraphicsSettings::s_FOG_CLOSE = 40;				string GraphicsSettings::tag_FOG_CLOSE = "FOG_CLOSE";
float	GraphicsSettings::s_FOG_FAR = 40;				string GraphicsSettings::tag_FOG_FAR = "FOG_FAR";
int		GraphicsSettings::s_MSAA_LEVEL = 1;			string GraphicsSettings::tag_MSAA_LEVEL = "MSAA_LEVEL";
int		GraphicsSettings::s_ANISOTROPY = 1;			string GraphicsSettings::tag_ANISOTROPY = "ANISOTROPIC_FILTERING_LEVEL";
// -----------------------------------------------------------------------
bool GraphicsSettings::Init(TiXmlElement* pSettingsElement)
{
	//WireFrame
	if (pSettingsElement->Attribute(tag_WIREFRAME_ENABLED.c_str()))
	{
		bool val = false;
		pSettingsElement->QueryBoolAttribute(tag_WIREFRAME_ENABLED.c_str(), &val);
		s_WIREFRAME_ENABLED = val;
	}
	//DevMode
	if (pSettingsElement->Attribute(tag_DEVMODE_ENABLED.c_str()))
	{
		bool val = false;
		pSettingsElement->QueryBoolAttribute(tag_DEVMODE_ENABLED.c_str(), &val);
		s_DEVMODE_ENABLED = val;
	}
	//Max Lights
	if (pSettingsElement->Attribute(tag_MAX_LIGHTS.c_str()))
	{
		int val = 0;
		pSettingsElement->QueryIntAttribute(tag_MAX_LIGHTS.c_str(), &val);
		s_MAXLIGHTS = val;
	}
	//Fog enabled
	if (pSettingsElement->Attribute(tag_FOG_ENABLED.c_str()))
	{
		bool val = false;
		pSettingsElement->QueryBoolAttribute(tag_FOG_ENABLED.c_str(), &val);
		s_FOG_ENABLED = val;
	}
	//Fog close
	if (pSettingsElement->Attribute(tag_FOG_CLOSE.c_str()))
	{
		float val = 0;
		pSettingsElement->QueryFloatAttribute(tag_FOG_CLOSE.c_str(), &val);
		s_FOG_CLOSE = val;
	}
	//Fog far
	if (pSettingsElement->Attribute(tag_FOG_FAR.c_str()))
	{
		float val = 0;
		pSettingsElement->QueryFloatAttribute(tag_FOG_FAR.c_str(), &val);
		s_FOG_FAR = val;
	}
	//MSAA
	if (pSettingsElement->Attribute(tag_MSAA_LEVEL.c_str()))
	{
		int val = 0;
		pSettingsElement->QueryIntAttribute(tag_MSAA_LEVEL.c_str(), &val);
		s_MSAA_LEVEL = val;
	}
	//Anisotropy
	if (pSettingsElement->Attribute(tag_ANISOTROPY.c_str()))
	{
		int val = 0;
		pSettingsElement->QueryIntAttribute(tag_ANISOTROPY.c_str(), &val);
		s_ANISOTROPY = val;
	}

	return true;
}