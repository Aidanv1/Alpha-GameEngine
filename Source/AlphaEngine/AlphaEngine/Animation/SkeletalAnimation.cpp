#include "SkeletalAnimation.h"
#include "../ResourceManager/Resources/Model.h"
#include "../Maths/Interpolator.h"
//========================================================================
//Bone
//========================================================================
Bone::Bone() :
m_boneName(""),
m_animOffsetMatrix(1),
m_offsetMatrix(0),
m_positionKeys(),
m_rotationKeys()
{

}
// -----------------------------------------------------------------------
Bone::~Bone()
{

}
// -----------------------------------------------------------------------
bool Bone::Init(BoneInfo* info, AnimationChannel* channel)
{
	m_boneName = info->m_nodeName.ToString();
	m_offsetMatrix = info->m_offsetMatrix;
	m_animOffsetMatrix = m_offsetMatrix;
	//populate positionKeys
	for (int i = 0; i < channel->m_numPosKeys; i++)
	{
		KeyFrame* keyframe = &channel->m_posKeyFrames[i];
		AnimationKey key;
		key.m_time = keyframe->m_time;
		key.m_value = keyframe->m_value;
		m_positionKeys.push_back(key);
	}
	//populate rotationKeys
	for (int i = 0; i < channel->m_numPosKeys; i++)
	{
		KeyFrame* keyframe = &channel->m_rotKeyFrames[i];
		AnimationKey key;
		key.m_time = keyframe->m_time;
		key.m_value = keyframe->m_value;
		m_rotationKeys.push_back(key);
	}
	return true;
}
// -----------------------------------------------------------------------
void Bone::Update(float time)
{
	vec3 positionVec;
	vec4 quatVec;
	{
		// -----------------
		//find upper and lower key frame
		AnimationKey* upperPosKey = NULL;
		AnimationKey* lowerPosKey = NULL;
		//find the first key that has a time greater than the current time
		int i = 0;
		for (auto it = m_positionKeys.begin(); it != m_positionKeys.end(); it++, i++)
		{
			if (it->m_time >= time)
			{
				upperPosKey = &(*it);
				if (i > 0)
				{
					lowerPosKey = &m_positionKeys.at(i - 1);
				}
				else
				{
					lowerPosKey = upperPosKey;
				}
			}
		}
		if (lowerPosKey && upperPosKey)
		{
			if (lowerPosKey != upperPosKey)
			{
				vec3 positionA = vec3(lowerPosKey->m_value.x, lowerPosKey->m_value.y, lowerPosKey->m_value.z);
				vec3 positionB = vec3(upperPosKey->m_value.x, upperPosKey->m_value.y, upperPosKey->m_value.z);
				float percent = (time - lowerPosKey->m_time) / (upperPosKey->m_time - lowerPosKey->m_time);
				positionVec = LinearInterpValue(positionA, positionB, percent);
			}
			else
			{
				positionVec = vec3(upperPosKey->m_value.x, upperPosKey->m_value.y, upperPosKey->m_value.z);
			}

		}
		// -----------------
	}

	//find upper and lower key frame
	AnimationKey* upperRotKey = NULL;
	AnimationKey* lowerRotKey = NULL;
	//find the first key that has a time greater than the current time
	int i = 0;
	for (auto it = m_rotationKeys.begin(); it != m_rotationKeys.end(); it++, i++)
	{
		if (it->m_time >= time)
		{
			upperRotKey = &(*it);
			if (i > 0)
			{
				lowerRotKey = &m_positionKeys.at(i - 1);
			}
			else
			{
				lowerRotKey = upperRotKey;
			}
		}
	}
	if (lowerRotKey && upperRotKey)
	{
		if (lowerRotKey != upperRotKey)
		{
			vec4 rotationA = lowerRotKey->m_value;
			vec4 rotationB = upperRotKey->m_value;
			float percent = (time - lowerRotKey->m_time) / (upperRotKey->m_time - lowerRotKey->m_time);
			quatVec = LinearInterpValue(rotationA, rotationB, percent);
		}
		else
		{
			quatVec = upperRotKey->m_value;
		}
	}
	// -----------------
	//calculate relative transform
	quat quaternion(quatVec.x, quatVec.y, quatVec.z, quatVec.w);
	mat4 rotationMat = mat4_cast(quaternion);
	mat4 transMat = translate(mat4(1.0f), positionVec);
	mat4 transform = transMat * rotationMat;
	// -----------------
	m_animOffsetMatrix = m_animOffsetMatrix * transform;
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->Transform(m_animOffsetMatrix);
		(*it)->Update(time);
	}
}
// -----------------------------------------------------------------------
void Bone::AddChildBone(StrongBonePtr bone)
{
	m_children.push_back(bone);
}
// -----------------------------------------------------------------------
void Bone::Transform(Matrix4x4& mat)
{
	m_animOffsetMatrix = mat;
}
// -----------------------------------------------------------------------
void Bone::Reset()
{
	m_animOffsetMatrix = m_offsetMatrix;
}
// -----------------------------------------------------------------------
void Bone::SetBoneID(BoneID id)
{
	m_boneName = id;
}
//========================================================================
//SkeletalAnimation
//========================================================================
SkeletalAnimation::SkeletalAnimation(AnimationID id) :
m_skeletonTree(NULL),
m_animationID(id),
m_state(AnimState_Stopped),
m_animationTime(0),
m_animationEndTime(0)
{

}
// -----------------------------------------------------------------------
SkeletalAnimation::~SkeletalAnimation()
{

}
// -----------------------------------------------------------------------
bool SkeletalAnimation::InitAnimation(AnimationInfo* animationInfo, BoneInfo* boneArray, int numBones)
{
	int numChannels = animationInfo->m_numChannels;
	//populate skeleton map
	for (int i = 0; i < numBones; i++)
	{
		BoneInfo* boneinfo = &boneArray[i];
		AnimationChannel* channel = NULL;
		//find channel corresponding to bone
		int j = 0;
		bool foundChannel = false;
		while (	!foundChannel &&
				j<animationInfo->m_numChannels)
		{
			//determine animation end time
			float posEndTime = animationInfo->m_channels[j].m_posKeyFrames[animationInfo->m_channels[j].m_numPosKeys - 1].m_time;
			float rotEndTime = animationInfo->m_channels[j].m_posKeyFrames[animationInfo->m_channels[j].m_numPosKeys - 1].m_time;
			float greatestTime = rotEndTime;
			if (posEndTime > rotEndTime)
			{
				m_animationEndTime = posEndTime;
			}
			if (greatestTime > m_animationEndTime)
			{
				m_animationEndTime = greatestTime;
			}
			//
			if (boneinfo->m_nodeName.ToString() == animationInfo->m_channels[j].m_nodeName.ToString())
			{
				channel = &animationInfo->m_channels[j];	
				foundChannel = true;
			}
			j++;
		}
		StrongBonePtr bone(ALPHA_NEW Bone());
		bone->Init(boneinfo, channel);
		m_skeletonMap[bone->GetBoneID()] = bone;
	}
	m_skeletonTree = StrongBonePtr(ALPHA_NEW Bone());
	m_skeletonTree->SetBoneID("Armature");
	for (int i = 0; i < numBones; i++)
	{
		//if parent is root node
		if (boneArray[i].m_parentName.ToString() == "Armature")
		{
			auto findIt = m_skeletonMap.find(boneArray[i].m_nodeName.ToString());
			if (findIt != m_skeletonMap.end())
			{
				m_skeletonTree->AddChildBone(findIt->second);
			}
			else
			{
				ALPHA_ERROR("Skeletal Animation error: Armature node not found.");
			}
		}
		else
		{
			auto findIt = m_skeletonMap.find(boneArray[i].m_nodeName.ToString());
			auto findItParent = m_skeletonMap.find(boneArray[i].m_parentName.ToString());
			//if the parent and child can be found add child to parent
			if (findIt != m_skeletonMap.end() &&
				findItParent != m_skeletonMap.end())
			{
				findItParent->second->AddChildBone(findIt->second);
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}
// -----------------------------------------------------------------------
void SkeletalAnimation::VPlayAnimation(float startTime)
{
	m_state = AnimState_Playing;
}
// -----------------------------------------------------------------------
void SkeletalAnimation::VPauseAnimation()
{
	m_state = AnimState_Paused;
}
// -----------------------------------------------------------------------
void SkeletalAnimation::VStopAnimation()
{
	m_state = AnimState_Stopped;
}
// -----------------------------------------------------------------------
bool SkeletalAnimation::VIsPlaying()
{
	switch (m_state)
	{
	case AnimState_Playing:
		return true;
		break;
	case AnimState_Paused:
		return true;
		break;
	case AnimState_Stopped:
		return false;
		break;
	}
}
// -----------------------------------------------------------------------
void SkeletalAnimation::VSetID(AnimationID id)
{
	m_animationID = id;
}
// -----------------------------------------------------------------------
void SkeletalAnimation::VUpdate(float deltaMs)
{
	m_animationTime += deltaMs/MS_PER_SECOND;
	if (m_animationTime >= m_animationEndTime)
	{
		//m_state = AnimState_Stopped;
		m_animationTime = 0;
	}
	switch (m_state)
	{
	case AnimState_Playing:
		m_skeletonTree->Update(m_animationTime);
		break;
	case AnimState_Paused:
		//do nothing
		break;
	case AnimState_Stopped:
		break;
	}
}
// -----------------------------------------------------------------------
Matrix4x4 SkeletalAnimation::GetBoneTransform(BoneID boneID)
{
	auto findIt = m_skeletonMap.find(boneID);
	if (findIt != m_skeletonMap.end())
	{
		return findIt->second->GetTransform();
	}
	return mat4();
}
// -----------------------------------------------------------------------
bool SkeletalAnimation::GetBoneTransform(vector<BoneID> idList, vector<mat4>& transforms)
{
	for (auto it = idList.begin(); it != idList.end(); it++)
	{
		auto findIt = m_skeletonMap.find(*it);
		if (findIt != m_skeletonMap.end())
		{
			mat4 transform = *findIt->second->GetTransform().Get();
			transforms.push_back(transform);
		}
		else
		{
			return false;
			ALPHA_ERROR("Bone missing");
		}
	}
	return true;
}
// -----------------------------------------------------------------------
AnimationState SkeletalAnimation::VGetAnimationState()
{
	return m_state;
}
// -----------------------------------------------------------------------