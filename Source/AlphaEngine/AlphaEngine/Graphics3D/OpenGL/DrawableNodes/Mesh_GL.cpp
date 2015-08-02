#include "Mesh_GL.h"
#include "../../../Animation/AnimationSystem.h"
#include "../../Scene/ModelNode.h"
// -----------------------------------------------------------------------
Mesh_GL::Mesh_GL() :
m_numVertices(0),
m_meshFileName(""),
m_material(NULL),
m_shaderProgram(NULL),
m_vertexBuffer(),
m_meshID(-1),
m_cullFace(false),
m_parentModel(NULL)
{
}
// -----------------------------------------------------------------------
Mesh_GL::~Mesh_GL()
{
}
// -----------------------------------------------------------------------
bool Mesh_GL::VInitMesh(string meshFile, ModelNode* parentModel)
{
	m_parentModel = parentModel;
	m_meshFileName = meshFile;
	AnimatedMeshShaderProgram* pShaderProgram = dynamic_cast<Renderer_GL*>(GraphicsSystem::Get().GetRenderer())->GetMeshShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
void Mesh_GL::VRender(Scene* pScene)
{
	//push to stack
	mat4 toWorldTransform = pScene->Stack()->Top() * m_nodeProperties.m_toWorld;
	pScene->Stack()->Push(toWorldTransform);

	m_shaderProgram->VUseProgram();
	//get view and projection matrix from main camera
	mat4 viewMat;
	mat4 projMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	pScene->GetCamera()->GetProjectionMatrix(projMat);
	int texID = -1;
	bool hasTexture = m_material->HasTexture();
	if (hasTexture)
	{
		texID = m_material->Texture()->VGetTextureID();
	}
	//set shader uniforms
	m_shaderProgram->SetUniforms(	toWorldTransform,
									viewMat,
									projMat,
									texID,
									hasTexture);
	//set lighting uniforms
	WeakLightArray lights;
	pScene->GetLightsInScene(lights);
	m_shaderProgram->SetLights(lights);
	m_shaderProgram->SetMaterial(m_material.get());
	//set bone uniforms
	if (m_boneIDList.size() > 0)
	{
		if (m_parentModel)
		{
			SkeletalAnimation* sk = dynamic_cast<SkeletalAnimation*>(m_parentModel->GetCurrentAnimation());
			if (sk)
			{
				vector<mat4> bonelist;
				if (sk->GetBoneTransform(m_boneIDList, bonelist))
				{
					m_shaderProgram->SetBoneUniforms(bonelist);
				}
			}
		}
	}
	//
	BindData();
	if (m_cullFace)
	{
		glEnable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	}
	VRenderChildren(pScene);

	//pop from stack
	pScene->Stack()->Pop();
}
// -----------------------------------------------------------------------
void Mesh_GL::BindData()
{
	int offset = 3 * m_numVertices*sizeof(GLfloat);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), m_componentSizes.x , 0, 0);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetNormalID(), m_componentSizes.y, 0, (const void*)offset);
	offset += 3 * m_numVertices*sizeof(GLfloat);
	if (m_material->HasTexture())
	{
		m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetTextureID(), m_componentSizes.z, 0, (const void*)(offset));
		offset += 2 * m_numVertices*sizeof(GLfloat);
	}
	//bone attributes
	if (m_boneIDList.size() > 0)
	{
		int boneSize = 0;
		GLuint* id = m_shaderProgram->GetBoneWeightsID(boneSize);
		boneSize = glm::min(boneSize, (int)m_boneIDList.size());
		for (int i = 0; i < boneSize; i++)
		{
			m_vertexBuffer.SetVertexAttribPointer(id[i], 1, 0, (const void*)(offset));
			offset += m_numVertices*sizeof(GLfloat);
		}

	}

}
// -----------------------------------------------------------------------
//The messh object's Validate function is called by the Model container class
bool Mesh_GL::VValidate()
{
	return m_vertexBuffer.Validate();
}
// -----------------------------------------------------------------------
int Mesh_GL::VLoadMesh(MeshInfo* pMesh)
{
	m_numVertices = pMesh->m_numberOfVertices;
	//create material---
	m_material = StrongMaterialPtr(ALPHA_NEW Material_GL());

	string path = m_meshFileName;
	int end = path.find_last_of("/");
	string textureLoc = pMesh->m_materialInfo.m_diffuseTexture.ToString();
	Texture_GL* tex = NULL;
	if (textureLoc != "")
	{
		path = path.substr(0, end + 1).append(textureLoc.c_str());
		tex = ALPHA_NEW Texture_GL(path.c_str());
		m_material->SetTexture(StrongTexturePtr(tex));
	}
	else
	{
		m_material->SetHasTexture(false);
	}
	m_material->SetAmbient(pMesh->m_materialInfo.m_ambient);
	m_material->SetEmissive(pMesh->m_materialInfo.m_emissive);
	m_material->SetDiffuse(pMesh->m_materialInfo.m_diffuse);
	m_material->SetSpecular(pMesh->m_materialInfo.m_specular, pMesh->m_materialInfo.m_shininess);
	//---
	//transform 
	m_nodeProperties.m_toWorld = pMesh->m_transform;
	//component sizes
	m_componentSizes.x = pMesh->m_positionComponentSize;
	m_componentSizes.y = pMesh->m_normalComponentSize;
	m_componentSizes.z = pMesh->m_uvComponentSize;
	//Bones
	int boneDataSize = 0;
	if (pMesh->m_hasBones && 
		pMesh->m_numberOfBones>0)
	{
		for (int i = 0; i < pMesh->m_numberOfBones; i++)
		{
			m_boneIDList.push_back(pMesh->m_bones[i].m_nodeName.ToString());			
		}
		boneDataSize = pMesh->m_numberOfBones*m_numVertices;
	}

	//initialize vertex buffer	
	int vid = m_vertexBuffer.Init(pMesh->m_dataSize + boneDataSize, pMesh->m_data, m_meshFileName);
	if (boneDataSize > 0)
	{
		m_vertexBuffer.BindSubData(pMesh->m_dataSize, boneDataSize, &pMesh->m_bones[0].m_weightsData->m_weight);
	}
	return vid;
}
// -----------------------------------------------------------------------
void Mesh_GL::VFreeBuffer()
{
	m_vertexBuffer.FreeVertexBuffer();
}
// -----------------------------------------------------------------------
bool Mesh_GL::VLoadMaterial()
{
	if (m_material->HasTexture())
	{
		return m_material->LoadTexture();
	}
	else
	{
		return true;
	}

}
// -----------------------------------------------------------------------
void Mesh_GL::VCullFace(bool cull)
{
	m_cullFace = cull;
}