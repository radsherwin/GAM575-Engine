#include "GraphicsObject_Font.h"
#include "FontManager.h"
#include "CameraManager.h"
#include "GlyphManager.h"

namespace Azul
{
GraphicsObject_Font::GraphicsObject_Font(const Mesh *const _pMesh,
                                         const ShaderObject *const _pShaderObj)
    : GraphicsObject(_pMesh, _pShaderObj)
{

    this->poMatrix_orig = new Matrix();
    this->poMatrix_uv = new Matrix();
}
GraphicsObject_Font::~GraphicsObject_Font()
{
    delete poMatrix_orig;
    delete poMatrix_uv;
}

void GraphicsObject_Font::SetState()
{
    glActiveTexture(GL_TEXTURE0);

    //Bind the texture
    GLuint *textureID = this->pTexture->textureID;
    glBindTexture(GL_TEXTURE_2D, *textureID);

    //comment out one
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);

    //Blends for sprites
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
}

void GraphicsObject_Font::SetDataGPU()
{
    // Get camera
    Camera *pCam = CameraManager::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
    // World matrix ----------------------------

    // FOR LOOP?

    Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, (float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
    glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&pCam->getProjMatrix());
    glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&pCam->getViewMatrix());
    // set the vao
    glBindVertexArray(this->GetMesh()->vao);
    // use this shader
    this->pShaderObj->SetActive();

    Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;
    glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&tmpMatrix);
    glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float *)&this->poMatrix_uv);

}

void GraphicsObject_Font::Draw()
{
    // draw
    glDrawElements(GL_TRIANGLES, 3 * this->GetMesh()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
}

void GraphicsObject_Font::RestoreState()
{
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}
}