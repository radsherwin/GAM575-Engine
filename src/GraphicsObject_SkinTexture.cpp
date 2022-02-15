#include "GraphicsObject_SkinTexture.h"
#include "Mesh.h"
#include "CameraManager.h"

using namespace Azul;


GraphicsObject_SkinTexture::GraphicsObject_SkinTexture(const Mesh * const _pMesh, const ShaderObject * const _pShaderObj, Texture::Name _name)
	: GraphicsObject(_pMesh, _pShaderObj), textName(_name)
{
	// check the base class
	assert(this->pMesh);
	assert(this->pShaderObj);
	assert(this->poWorld);

	this->pBoneWorld = nullptr;
}

void GraphicsObject_SkinTexture::SetBoneWorld(Matrix *_pBoneWorld)
{
	assert(_pBoneWorld);
	this->pBoneWorld = _pBoneWorld;
}

void GraphicsObject_SkinTexture::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint *textureID = TextureManager::FindID(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void GraphicsObject_SkinTexture::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();	
	
	// set the vao
	glBindVertexArray(this->GetMesh()->vao);

	Camera* pCam = CameraManager::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix world = this->GetWorld();
	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();

	// Send the uniforms
	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *) &proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *) &view);
	//glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *) &world);

	assert(this->pBoneWorld);
	//glUniformMatrix4fv(this->pShaderObj->GetLocation("invBind"), 8, GL_FALSE, (float *)pMesh->poInverseBind);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), 8, GL_FALSE, (float *)this->pBoneWorld);

	//uniform mat4 view_matrix;
	//uniform mat4 proj_matrix;
	//uniform mat4 invPose[8];
	//uniform mat4 boneWorld[8];
	


}

void GraphicsObject_SkinTexture::Draw()
{
	//The starting point of the IBO index buffer object
	glDrawElements(GL_TRIANGLES, 3 * this->GetMesh()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_SkinTexture::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}
