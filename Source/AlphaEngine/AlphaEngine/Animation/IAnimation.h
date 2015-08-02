#pragma once
#include "../AlphaStd.h"
typedef string AnimationID;
//========================================================================
enum AnimationState
{
	AnimState_Playing,
	AnimState_Stopped,
	AnimState_Paused
};
//========================================================================
class IAnimation
{
	friend class AnimationSystem;
public:
	virtual void VPlayAnimation(float startTime = 0) = 0;
	virtual void VPauseAnimation() = 0;
	virtual void VStopAnimation() = 0;
	virtual bool VIsPlaying() = 0;
	virtual AnimationState VGetAnimationState() = 0;
	virtual AnimationID VGetID() const = 0;
	virtual void VSetID(AnimationID id) = 0;
private:
	virtual void VUpdate(float deltaMs) = 0;
};
//========================================================================