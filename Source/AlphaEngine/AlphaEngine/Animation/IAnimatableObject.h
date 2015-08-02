#pragma once
#include "IAnimation.h"
//Intended for objects that have an IAnimation member
class IAnimatableObject
{
	//Determine if the object has any animations.
	virtual bool VHasAnimation() = 0;
	//Determine if an animation exists.
	virtual bool VHasAnimation(AnimationID animationID) = 0;
	//Starts an animation corresponding to the given ID.
	virtual bool VPlayAnimation(AnimationID animationID) = 0;
	//Stop an animation corresponding to the given ID.
	virtual bool VStopAnimation(AnimationID animationID) = 0;
	//Stop any animation currently playing.
	virtual bool VStopAnimation() = 0;
	//Pause an animation corresponding to the given ID.
	virtual bool VPauseAnimation(AnimationID animationID) = 0;
	//Get currently active animation
	virtual IAnimation* GetCurrentAnimation() = 0;
};