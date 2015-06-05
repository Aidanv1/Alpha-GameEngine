#pragma once
#include "../AlphaStd.h"
//========================================================================
class GraphicsSettings
{
	friend class GraphicsSystem;
public:
	static bool DevMode() { return s_DEVMODE_ENABLED; }
	static bool WireFrame() { return s_WIREFRAME_ENABLED;  }
	//LIGHT---------------------
	static int MaxLights() { return s_MAXLIGHTS; }
	//FOG-----------------------
	static bool FogEnabled() { return s_FOG_ENABLED; }
	static float FogClose() { return s_FOG_CLOSE; }
	static float FogFar() { return s_FOG_FAR; }	
	static int Msaa() { return s_MSAA_LEVEL; }
	static int Anisotropy() { return s_ANISOTROPY; }
private:
	static bool Init(TiXmlElement* pSettingsElement);
private:
	static bool s_WIREFRAME_ENABLED;	static string tag_WIREFRAME_ENABLED;
	static bool s_DEVMODE_ENABLED;		static string tag_DEVMODE_ENABLED;
	//LIGHT---------------------
	static int s_MAXLIGHTS;				static string tag_MAX_LIGHTS;
	//FOG-----------------------
	static bool s_FOG_ENABLED;			static string tag_FOG_ENABLED;
	static float s_FOG_CLOSE;			static string tag_FOG_CLOSE;
	static float s_FOG_FAR;				static string tag_FOG_FAR;
	//AA and Anisotropic filtering
	static int s_MSAA_LEVEL;			static string tag_MSAA_LEVEL;
	static int s_ANISOTROPY;			static string tag_ANISOTROPY;
};
//========================================================================