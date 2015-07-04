#pragma once
#include "..\IEvent.h"

//========================================================================
//Base event from which all events inherit
class BaseEvent : public IEvent
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
class LookEvent : public BaseEvent
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
	explicit LookEvent(const float timeStamp = 0.0f) :
		BaseEvent(timeStamp)
	{
	}
	void SetLook(float dThetaX, float dThetaY, float dZ, float dX, float dY)
	{
		m_lookPos.m_dThetaX = dThetaX;
		m_lookPos.m_dThetaY = dThetaY;
		m_lookPos.m_dZ = dZ;
		m_lookPos.m_dX = dX;
		m_lookPos.m_dY = dY;
	}
	LookPos GetLookPos() const { return m_lookPos; }
	virtual EventType VGetEventType() const override { return s_eventType; }
public:
	static const EventType s_eventType;

private:
	LookPos m_lookPos;
};

//========================================================================
