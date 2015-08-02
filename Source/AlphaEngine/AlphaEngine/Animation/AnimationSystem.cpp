#include "AnimationSystem.h"
// -----------------------------------------------------------------------
AnimationSystem::AnimationSystem() 
{

}
// -----------------------------------------------------------------------
AnimationSystem::~AnimationSystem()
{

}
// -----------------------------------------------------------------------
bool AnimationSystem::Init()
{
	return true;
}
// -----------------------------------------------------------------------
void AnimationSystem::Update(float deltaMs)
{
	//call update function for all animations
	auto it = m_animationMap.begin();
	while (it != m_animationMap.end())
	{
		//if no other object is referencing this animation we can remove it
		if (it->second.use_count() == 1)
		{
			auto ittemp = it++;
			m_animationMap.erase(ittemp);
		}
		else
		{
			it->second->VUpdate(deltaMs);
			it++;
		}
	}
}
// -----------------------------------------------------------------------
AnimationSystem* AnimationSystem::Get()
{
	static AnimationSystem* instance = ALPHA_NEW AnimationSystem();
	return instance;
}
// -----------------------------------------------------------------------
IAnimation* AnimationSystem::GetAnimation(AnimationID animationID) const
{
	auto findIt = m_animationMap.find(animationID);
	if (findIt != m_animationMap.end())
	{
		return findIt->second.get();
	}
	return NULL;
}
// -----------------------------------------------------------------------
void AnimationSystem::AddAnimation(StrongAnimationPtr animation)
{
	//check if animation already exists
	auto findIt = m_animationMap.find(animation->VGetID());
	if (findIt == m_animationMap.end())
	{
		m_animationMap[animation->VGetID()] = animation;
	}
}
// -----------------------------------------------------------------------
void AnimationSystem::RemoveAnimation(AnimationID id)
{
	//remove animation if it exists
	auto findIt = m_animationMap.find(id);
	if (findIt != m_animationMap.end())
	{
		m_animationMap.erase(findIt);
	}
}