#pragma once
#include "IAnimation.h"
#include "../AlphaStd.h"
#include "../Maths/LinearAlgebra.h"
//Forward declarations
struct AnimationInfo;
struct BoneInfo;
struct AnimationChannel;
//========================================================================
struct AnimationKey
{
	float	m_time;
	vec4	m_value;
};
//========================================================================
typedef vector<AnimationKey> Keys;
typedef string BoneID;
class Bone;
typedef shared_ptr<Bone> StrongBonePtr;
typedef list<StrongBonePtr> Bones;
//========================================================================
class Bone
{
public:
	Bone();
	~Bone();
	bool Init(BoneInfo* info, AnimationChannel* channel);
	void Update(float time);
	void AddChildBone(StrongBonePtr bone);
	void Transform(Matrix4x4& mat);
	void Reset();
	BoneID GetBoneID() const { return m_boneName; }
	void SetBoneID(BoneID id);
	Matrix4x4 GetTransform() const { return m_animOffsetMatrix; }
private:
	BoneID		m_boneName;
	Matrix4x4	m_animOffsetMatrix;
	Matrix4x4	m_offsetMatrix;
	Keys		m_positionKeys;
	Keys		m_rotationKeys;
	Bones		m_children;
};
//========================================================================
class SkeletalAnimation;
typedef shared_ptr<SkeletalAnimation> StrongSkAnimationPtr;
typedef map<BoneID, StrongBonePtr> SkeletonMap;
//========================================================================
class SkeletalAnimation : public IAnimation
{
public:
	SkeletalAnimation(AnimationID id);
	~SkeletalAnimation();
	//SkeletalAnimation functions
	Matrix4x4 GetBoneTransform(BoneID boneID);
	bool GetBoneTransform(vector<BoneID> idList, vector<mat4>& transforms);
	bool InitAnimation(AnimationInfo* animationInfo, BoneInfo* boneArray, int numBones);
	//IAnimation functions
	AnimationState VGetAnimationState() override;
	void VPlayAnimation(float startTime = 0) override;
	void VPauseAnimation() override;
	void VStopAnimation() override;
	bool VIsPlaying() override;
	AnimationID VGetID() const override { return m_animationID; }
	void VSetID(AnimationID id) override;
private:
	void VUpdate(float deltaMs);
private:
	AnimationState	m_state;
	float			m_animationTime;
	float			m_animationEndTime;
	AnimationID		m_animationID;
	StrongBonePtr	m_skeletonTree; //for hierarchical updating
	SkeletonMap		m_skeletonMap;	  //for fast lookups
};
//========================================================================