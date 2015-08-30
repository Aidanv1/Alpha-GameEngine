#pragma once
#include "../EventManager/Events/Events.h"
//========================================================================
//Unique Event IDs
#define EVENT_GRAPHICS_LOOKEVENT 0xfa52d07b
#define EVENT_GRAPHICS_LOADED 0xb9416c4
//========================================================================
class GraphicsLoadedEvent : public BaseEvent
{
public:
	GraphicsLoadedEvent();
	~GraphicsLoadedEvent();
	EventType VGetEventType() const override { return EVENT_GRAPHICS_LOADED; }
};
//========================================================================
//Look Event - For camera use
class DLLExport LookEvent : public BaseEvent
{
	struct LookPos
	{
		float m_dThetaX;
		float m_dThetaY;
		float m_dZ;
		float m_dX;
		float m_dY;
		LookPos() :
			m_dThetaX(0),
			m_dThetaY(0),
			m_dZ(0),
			m_dX(0),
			m_dY(0)
		{
		}
	};
public:
	explicit LookEvent(const float timeStamp = 0.0f);
	void SetLook(float dThetaX, float dThetaY, float dZ, float dX, float dY);
	LookPos GetLookPos() const;
	virtual EventType VGetEventType() const override;
private:
	LookPos m_lookPos;
};

//========================================================================