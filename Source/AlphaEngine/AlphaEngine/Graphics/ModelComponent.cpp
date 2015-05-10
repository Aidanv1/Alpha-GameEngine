#include "ModelComponent.h"
//========================================================================
// IGameObjectComponent Functions
//========================================================================
ModelComponent::ModelComponent(string modelFileName, vec3 pos, vec3 rot) :
SceneNode(),
m_modelFileName(modelFileName),
m_modelID(-1)
{
	m_positionInWorld = pos;
}
// -----------------------------------------------------------------------
ModelComponent::~ModelComponent()
{

}
// -----------------------------------------------------------------------
ComponentType ModelComponent::VGetType()
{
	return 1;
}
// -----------------------------------------------------------------------
void ModelComponent::VUpdate()
{

}
// -----------------------------------------------------------------------
bool ModelComponent::VInit(TiXmlElement* pElement)
{
	m_modelResourceManager = GraphicsSystem::Get().GetMeshResourceManager();
	return true;
}
// -----------------------------------------------------------------------
bool ModelComponent::VPostInit()
{
	return true;
}
//========================================================================
// Scene Node Functions
//========================================================================
void ModelComponent::VRender(Scene* pScene)
{
	if (!Load())
	{
		return;
	}
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
bool ModelComponent::Load()
{
	bool success = true;
	if (!LoadResource())
	{
		ALPHA_ERROR("Unable to load resource");
		return false;
	}
	//check if system memory buffer is loaded
	if (!m_modelResource->Buffer())
	{
		return false;
	}
	//check if video memory buffers are loaded
	if (!ValidateBuffers() && 
		m_modelID != -1)
	{
		m_modelID = -1;
	}
	if (m_modelID != -1)
	{
		for (auto it = m_meshChildren.begin(); it != m_meshChildren.end();it++)
		{
			Mesh* mesh = dynamic_cast<Mesh*>((*it).get());
			if (mesh)
			{
				success = mesh->GetMaterial()->LoadTexture();
			}
		}
		return success;
	}

	//attempt to load meshes
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(m_modelResource->Buffer(), m_modelResource->GetSize(), NULL);
	if (!scene)
	{
		stringstream ss;
		ss << "Assimp load error: "<<importer.GetErrorString();
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		//opengl mesh
		Mesh* mesh = ALPHA_NEW Mesh(m_modelFileName);		
		mesh->SetNodeProperties(m_nodeProperties);
		//get relative transformation for child
		aiMatrix4x4 m = scene->mRootNode->mChildren[i]->mTransformation;
		vec3 T(m.a4, m.b4, m.c4);
		mat4 trans = translate(mat4(1.0f), T);
		NodeProperties n = mesh->GetNodeProperties();	
		n.m_toWorld = n.m_toWorld* trans;
		mesh->SetNodeProperties(n);
		int meshID = mesh->LoadMesh(scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
		if (meshID == -1)
		{
			success = false;
		}
		//init mesh
		mesh->Init();		
		//attempt to load textures
		success = mesh->GetMaterial()->LoadTexture();
		m_meshChildren.push_back(StrongSceneNodePtr(mesh));
	}
	m_modelID = 1;
	return success;
}
// -----------------------------------------------------------------------
bool ModelComponent::LoadResource()
{
	//if no resource currently exists
	if (!m_modelResource)
	{
		//Request load Resource
		Resource* meshResource = ALPHA_NEW Resource(m_modelFileName);
		meshResource->RequestLoad();
		m_modelResource = StrongModelPtr(meshResource);

		if (!m_modelResourceManager->AddResource(m_modelResource))
		{
			return false;
		}
	}
	else if (!m_modelResource->IsLoaded())
	{
		m_modelResource->RequestLoad();
	}
	else if (!m_modelResource->Buffer())
	{
		return false;
	}


	return true;
}
// -----------------------------------------------------------------------
void ModelComponent::VUpdateNode(Scene* pScene, float deltaMS)
{
	m_nodeProperties.m_lightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	m_nodeProperties.m_rotationMatrix = rotate(mat4(1.0f), -1.57f, vec3(1.0f, 0.0f, 0.0f));
	m_nodeProperties.m_toWorld = translate(mat4(1.0f), m_positionInWorld);
	m_nodeProperties.m_toWorld = m_nodeProperties.m_toWorld * m_nodeProperties.m_rotationMatrix;
}
// -----------------------------------------------------------------------
void ModelComponent::VRenderChildren(Scene* pScene)
{
	//render mesh children
	for (auto child = m_meshChildren.begin(); child != m_meshChildren.end(); child++)
	{
		(*child)->VRender(pScene);
	}
	//base class render children
	SceneNode::VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
bool ModelComponent::ValidateBuffers()
{
	bool validCheck = true;
	bool* validArray = ALPHA_NEW bool[m_meshChildren.size()];
	//consider all meshes as valid initially
	for (int i = 0; i < m_meshChildren.size(); i++)
	{
		validArray[i] = true;
	}
	//check if all meshes are still valid in vram
	int i = 0;
	for (auto it = m_meshChildren.begin(); it != m_meshChildren.end(); it++, i++)
	{
		shared_ptr<Mesh> mesh = dynamic_pointer_cast<Mesh>(*it);
		if (!mesh->Validate())
		{
			validArray[i] = false;
			validCheck = false;
		}
	}

	if (validCheck)
	{
		SAFE_DELETE_ARRAY(validArray);
		return true;
	}
	//if one mesh is not valid, free all valid mesh buffers
	//(invalid mesh buffers are in use by other mesh objects)
	i = 0;
	for (auto it = m_meshChildren.begin(); it != m_meshChildren.end(); it++, i++)
	{
		if (validArray[i] == true)
		{
			shared_ptr<Mesh> mesh = dynamic_pointer_cast<Mesh>(*it);
			mesh->FreeVertexBuffer();
		}
	}
	SAFE_DELETE_ARRAY(validArray);
	return false;
}
// -----------------------------------------------------------------------
//========================================================================
// Component Creator Functions
//========================================================================
IGameObjectComponent* ModelComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	string modelFileName = pElement->Attribute("modelFileName");
	TiXmlElement* nextElem = pElement->FirstChildElement();

	vec3 pos(0.0f);
	vec3 rot(0.0f);
	//loop through elements
	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "Position")
		{
			nextElem->QueryFloatAttribute("x", &pos.x);
			nextElem->QueryFloatAttribute("y", &pos.y);
			nextElem->QueryFloatAttribute("z", &pos.z);
		}
		if (val == "RotationInDegrees")
		{
			nextElem->QueryFloatAttribute("xAxis", &rot.x);
			nextElem->QueryFloatAttribute("yAxis", &rot.y);
			nextElem->QueryFloatAttribute("zAxis", &rot.z);
			//convert to radians			
			rot.x = radians<float>(rot.x);
			rot.y = radians<float>(rot.y);
			rot.z = radians<float>(rot.z);
		}
		nextElem = nextElem->NextSiblingElement();
	}
	ModelComponent* modelComponent = ALPHA_NEW ModelComponent(modelFileName, pos, rot);
	modelComponent->VInit(pElement);
	return modelComponent;
}