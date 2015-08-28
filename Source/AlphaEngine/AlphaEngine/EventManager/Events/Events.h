#pragma once
#include "..\IEvent.h"
#include "../../AlphaEngineDLL.h"
//========================================================================
//Base event from which all events inherit
class DLLExport BaseEvent : public IEvent
{

public:
	explicit BaseEvent(float timeStamp = 0.0f);
	virtual const float VGetTimeStamp() const override { return m_timeStamp; }
	virtual EventType VGetEventType() const override { return 0; }
private:
	virtual void VSetTimeStamp(float timestamp) override;
private:
	float m_timeStamp;
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
public:
	static const EventType s_eventType;

private:
	LookPos m_lookPos;
};

//========================================================================
