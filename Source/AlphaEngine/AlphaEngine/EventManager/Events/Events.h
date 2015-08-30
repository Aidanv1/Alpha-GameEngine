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

