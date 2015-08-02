#pragma once
#include "../AlphaStd.h"
#include "IAnimation.h"

typedef shared_ptr<IAnimation> StrongAnimationPtr;
typedef map < AnimationID, StrongAnimationPtr > AnimationMap;
//========================================================================
class AnimationSystem
{
public:
	~AnimationSystem();
	bool Init();
	void Update(float deltaMs);
	static AnimationSystem* Get();
	IAnimation* GetAnimation(AnimationID animationID) const;
	void AddAnimation(StrongAnimationPtr animation);
	void RemoveAnimation(AnimationID id);
private:
	AnimationSystem();
private:
	AnimationMap m_animationMap;
};
//========================================================================