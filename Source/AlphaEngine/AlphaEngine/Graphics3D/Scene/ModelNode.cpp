#include "ModelNode.h"

// -----------------------------------------------------------------------
ModelNode::ModelNode() :
SceneNode(),
GraphicsComponent(),
m_modelFileName(""),
m_modelID(-1),
m_modelResourceManager(NULL),
m_modelResource(NULL),
m_meshChildren()
{
}
// -----------------------------------------------------------------------
ModelNode::~ModelNode()
{
}
//========================================================================
// IActorComponent Functions
//========================================================================
void ModelNode::VUpdate()
{

}
// -----------------------------------------------------------------------
bool ModelNode::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* nextElem = pElement->FirstChildElement();
	//loop through elements
	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "Properties")
		{
			m_modelFileName = nextElem->Attribute("modelFileName");
			if (nextElem->Attribute("alphaType"))
			{
				string attrib = nextElem->Attribute("alphaType");
				if (attrib == "Transparent")
				{
					m_nodeProperties.m_alphaType = tTRANSPARENT;
				}
				if (attrib == "Opaque")
				{
					m_nodeProperties.m_alphaType = tOPAQUE;
				}
			}

			if (nextElem->Attribute("renderPass"))
			{
				string attrib = nextElem->Attribute("renderPass");
				if (attrib == "Transparent")
				{
					m_nodeProperties.m_renderPass = RenderPass_Actor;
				}
				else if (attrib == "Opaque")
				{
					m_nodeProperties.m_renderPass = RenderPass_Static;
				}
			}
			else
			{
				//default to actor render pass
				m_nodeProperties.m_renderPass = RenderPass_Actor;
			}
			
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

	// initial transforms
	//m_nodeProperties.m_relativeRotation = rotate(mat4(1.0f), m_rotationInWorld.x, vec3(1.0f, 0.0f, 0.0f));
	//m_nodeProperties.m_relativeRotation = rotate(m_nodeProperties.m_relativeRotation, m_rotationInWorld.y, vec3(0.0f, 1.0f, 0.0f));
	//m_nodeProperties.m_relativeRotation = rotate(m_nodeProperties.m_relativeRotation, m_rotationInWorld.z, vec3(0.0f, 0.0f, 1.0f));

	m_nodeProperties.m_relativeTransform = translate(mat4(1.0f), m_positionInWorld) * m_nodeProperties.m_relativeRotation;
	return true;
}
// -----------------------------------------------------------------------
bool ModelNode::VPostInit()
{
	GraphicsComponent::VPostInit();
	return true;
}
//========================================================================
// Scene Node Functions
//========================================================================
bool ModelNode::VInitNode()
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
void ModelNode::VRender(Scene* pScene)
{
	//dont render if the object is not within in the view frustum
	if (!pScene->GetCamera()->GetFrustum().VInside(m_positionInWorld, m_radius))
	{
		return;
	}
	//if the node is transparent and it is not currently the alpha pass,
	//dont render it yet
	if (	m_nodeProperties.m_alphaType == tTRANSPARENT && 
			!pScene->isAlphaPass())
	{
		pScene->AddTransparentNode(this);
		mat4 viewMat;
		pScene->GetCamera()->GetViewMatrix(viewMat);
		vec4 pos4 = viewMat*m_nodeProperties.m_toWorld*vec4(m_positionInWorld, 1.0f);
		m_screenZ = pos4.z;
	}
	else
	{
		//render mesh children
		for (auto child = m_meshChildren.begin(); child != m_meshChildren.end(); child++)
		{
			(*child)->VRender(pScene);
		}
	}
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
bool ModelNode::Load()
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
			IMesh* mesh = dynamic_cast<IMesh*>((*it).get());
			if (mesh)
			{
				success = mesh->VLoadMaterial();
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
	//node factory to instantiate mesh objects
	DrawableNodeFactory nodeFactory;
	m_meshChildren.clear(); // start from scratch
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		stringstream meshName;
		meshName << m_modelFileName <<"_"<< i; // create unique mesh ID
		//opengl mesh
		IMesh* mesh = dynamic_cast<IMesh*>(nodeFactory.CreateDrawableNode(Node_Mesh)); 
		ALPHA_ASSERT(mesh);
		//init mesh
		mesh->VInitMesh(meshName.str().c_str());
		mesh->VSetNodeProperties(m_nodeProperties);
		//get relative transformation for child
		aiMatrix4x4 m = scene->mRootNode->mChildren[i]->mTransformation;
		vec3 T(m.a4, m.b4, m.c4);
		mat4 trans = translate(mat4(1.0f), T);
		//set relative transform
		NodeProperties n = mesh->VGetNodeProperties();	
		n.m_relativeTransform = trans;
		mesh->VSetNodeProperties(n);
		int meshID = mesh->VLoadMesh(scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
		if (meshID == -1)
		{
			success = false;
		}
		//attempt to load textures
		success = mesh->VLoadMaterial();
		m_meshChildren.push_back(StrongSceneNodePtr(mesh));
	}
	m_modelID = 1;
	return success;
}
// -----------------------------------------------------------------------
bool ModelNode::RequestLoadResource()
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
void ModelNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	//update children transforms
	//for (auto child = m_meshChildren.begin(); child != m_meshChildren.end(); child++)
	//{
	//	NodeProperties n = (*child)->GetNodeProperties();
	//	n.m_toWorld = m_nodeProperties.m_toWorld * n.m_relativeTransform;
	//	n.m_rotationMatrix = m_nodeProperties.m_rotationMatrix * n.m_relativeRotation;
	//	(*child)->SetNodeProperties(n);
	//	(*child)->VUpdateNode(pScene, deltaMS);		
	//}
	//for (auto child = m_children.begin(); child != m_children.end(); child++)
	//{
	//	NodeProperties n = (*child)->GetNodeProperties();
	//	n.m_toWorld = m_nodeProperties.m_toWorld * n.m_relativeTransform;
	//	n.m_rotationMatrix = m_nodeProperties.m_rotationMatrix * n.m_relativeRotation;
	//	(*child)->SetNodeProperties(n);
	//	(*child)->VUpdateNode(pScene, deltaMS);
	//}
}
// -----------------------------------------------------------------------
void ModelNode::VRenderChildren(Scene* pScene)
{	
	//base class render children
	SceneNode::VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
bool ModelNode::ValidateBuffers()
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
		shared_ptr<IMesh> mesh = dynamic_pointer_cast<IMesh>(*it);
		if (!mesh->VValidate())
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
			shared_ptr<IMesh> mesh = dynamic_pointer_cast<IMesh>(*it);
			mesh->VFreeBuffer();
		}
	}
	SAFE_DELETE_ARRAY(validArray);
	return false;
}

