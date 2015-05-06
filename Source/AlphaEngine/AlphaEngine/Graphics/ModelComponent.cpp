#include "ModelComponent.h"
//========================================================================
// IGameObjectComponent Functions
//========================================================================
ModelComponent::ModelComponent(string modelFileName) :
SceneNode(),
m_modelFileName(modelFileName),
m_modelID(-1)
{
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
void ModelComponent::VRender()
{
	if (!Load())
	{
		return;
	}
	VRenderChildren();
}
bool ModelComponent::Load()
{
	bool success = true;
	if (!LoadResource())
	{
		ALPHA_ERROR("Unable to load resource");
		return false;
	}
	if (!m_modelResource->Buffer())
	{
		return false;
	}
	if (m_modelID != -1)
	{
		for (auto it = m_children.begin(); it != m_children.end();it++)
		{
			Mesh* mesh = dynamic_cast<Mesh*>((*it).get());
			if (mesh)
				success = mesh->GetMaterial()->LoadTexture();
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
		n.ModelMatrix = n.ModelMatrix* trans;
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
		VAddChild(StrongSceneNodePtr(mesh));
	}
	m_modelID = 1;
	return success;
}
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


	return true;
}
//========================================================================
// Component Creator Functions
//========================================================================
IGameObjectComponent* ModelComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	string modelFileName = pElement->Attribute("modelFileName");
	ModelComponent* modelComponent = ALPHA_NEW ModelComponent(modelFileName);
	modelComponent->VInit(pElement);
	return modelComponent;
}