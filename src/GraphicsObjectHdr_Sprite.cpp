#include "GraphicsObjectHdr_Sprite.h"

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

} //namespace Azul


