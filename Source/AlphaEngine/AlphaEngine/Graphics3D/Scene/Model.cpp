#include "Model.h"

// -----------------------------------------------------------------------
Model::Model() :
SceneNode(),
m_modelFileName(""),
m_modelID(-1),
m_modelResourceManager(NULL),
m_modelResource(NULL),
m_meshChildren()
{
}
// -----------------------------------------------------------------------
Model::~Model()
{
}
//========================================================================
// IActorComponent Functions
//========================================================================
void Model::VUpdate()
{

}
// -----------------------------------------------------------------------
bool Model::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* nextElem = pElement->FirstChildElement();
	//loop through elements
	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "Properties")
		{
			m_modelFileName = nextElem->Attribute("modelFileName");

		}
		if (val == "Position")
		{
			nextElem->QueryFloatAttribute("x", &m_positionInWorld.x);
			nextElem->QueryFloatAttribute("y", &m_positionInWorld.y);
			nextElem->QueryFloatAttribute("z", &m_positionInWorld.z);
		}
		if (val == "RotationInDegrees")
		{
			nextElem->QueryFloatAttribute("xAxis", &m_rotationInWorld.x);
			nextElem->QueryFloatAttribute("yAxis", &m_rotationInWorld.y);
			nextElem->QueryFloatAttribute("zAxis", &m_rotationInWorld.z);
			//convert to radians			
			m_rotationInWorld.x = radians<float>(m_rotationInWorld.x);
			m_rotationInWorld.y = radians<float>(m_rotationInWorld.y);
			m_rotationInWorld.z = radians<float>(m_rotationInWorld.z);
		}
		nextElem = nextElem->NextSiblingElement();
	}
	m_modelResourceManager = GraphicsSystem::Get().GetMeshResourceManager();
	return true;
}
// -----------------------------------------------------------------------
bool Model::VPostInit()
{
	return true;
}
//========================================================================
// Scene Node Functions
//========================================================================
bool Model::VInitNode()
{
	bool success = Load();	
	for (auto child = m_meshChildren.begin(); child != m_meshChildren.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}

	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}
	return success;
}
// -----------------------------------------------------------------------
void Model::VRender(Scene* pScene)
{
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
bool Model::Load()
{
	//Steps:
	//1)	Request the resource manager to load the model resource. Fails if the request is denied
	//2)	Check if the resource buffer has been loaded. This will always return false on the first
	//		pass because the resource loader has not yet had a chance to load it.
	//3)	Validate the mesh buffers in video memory. If there are currently no meshes loaded, this
	//		this will return invalid.
	//4)	If all the mesh data is valid, ensure that the relevant textures are also loaded.
	//5)	If validation failed in step 3, then the mesh data will need to be loaded.


	bool success = true;
	if (!RequestLoadResource())
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
	if (!ValidateBuffers())
	{
		m_modelID = -1;
	}
	//if video memory (mesh data) is still valid
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

	m_meshChildren.clear(); // start from scratch
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		stringstream meshName;
		meshName << m_modelFileName <<"_"<< i; // create unique mesh ID
		//opengl mesh
		Mesh* mesh = ALPHA_NEW Mesh(meshName.str().c_str());
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
bool Model::RequestLoadResource()
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
// -----------------------------------------------------------------------
void Model::VUpdateNode(Scene* pScene, float deltaMS)
{
	m_nodeProperties.m_lightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	m_nodeProperties.m_rotationMatrix = rotate(mat4(1.0f), -1.57f, vec3(1.0f, 0.0f, 0.0f));
	m_nodeProperties.m_toWorld = translate(mat4(1.0f), m_positionInWorld);
	m_nodeProperties.m_toWorld = m_nodeProperties.m_toWorld * m_nodeProperties.m_rotationMatrix;
}
// -----------------------------------------------------------------------
void Model::VRenderChildren(Scene* pScene)
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
bool Model::ValidateBuffers()
{
	bool validCheck = true;
	//if there are no meshes
	if (m_meshChildren.size() == 0)
	{
		return false;
	}
	bool* validArray = ALPHA_NEW bool[m_meshChildren.size()];
	//consider all meshes as valid initially
	for (unsigned int i = 0; i < m_meshChildren.size(); i++)
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
		m_modelID = 1; //valid model
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

