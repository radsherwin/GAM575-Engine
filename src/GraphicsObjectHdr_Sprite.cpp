#include "GraphicsObjectHdr_Sprite.h"
#include "GraphicsObject_Font.h"
#include "FontManager.h"

namespace Azul
{
GraphicsObjectHdr_Sprite::GraphicsObjectHdr_Sprite(GraphicsObject *pGraphicsObj)
    : GraphicsObjectHdr(pGraphicsObj)
{
}

GraphicsObjectHdr_Sprite::GraphicsObjectHdr_Sprite()
    : GraphicsObjectHdr()
{
}

GraphicsObjectHdr_Sprite::~GraphicsObjectHdr_Sprite()
{
}

void GraphicsObjectHdr_Sprite::Set_Sprite(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Image *image, Rect rect)
{
    this->poGraphicsObject = new GraphicsObject_Sprite(pMesh, pShaderObj, image, rect);
}

void GraphicsObjectHdr_Sprite::Set_Font(const Mesh *const pMesh, const ShaderObject *const pShaderObj)
{
    this->poGraphicsObject = new GraphicsObject_Font(pMesh, pShaderObj);
}
} //namespace Azul