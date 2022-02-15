#include "GraphicsObjectHdr.h"

#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_ColorSpecLighting.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_Null.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GraphicsObject_Sprite.h"
#include "GraphicsObject_SkinTexture.h"

GraphicsObjectHdr::GraphicsObjectHdr(GraphicsObject *_pGraphicsObj)
    : pNext(nullptr), poGraphicsObject(nullptr)
{
    this->poGraphicsObject = _pGraphicsObj;
}

GraphicsObjectHdr::GraphicsObjectHdr(const Mesh *const pMesh, const ShaderObject *const pShaderObj)
    : pNext(nullptr), poGraphicsObject(nullptr)
{
    this->poGraphicsObject = new GraphicsObject_Null(pMesh, pShaderObj);
}

GraphicsObjectHdr::GraphicsObjectHdr()
    : pNext(nullptr), poGraphicsObject(nullptr)
{
}

GraphicsObjectHdr::~GraphicsObjectHdr()
{
    delete this->poGraphicsObject;
}

// Array mesh

void GraphicsObjectHdr::Set_FlatTexture(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj,
                                        Texture::Name _name)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_FlatTexture(pMesh[i], pShaderObj, _name));
    }
}

void GraphicsObjectHdr::Set_ConstColor(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj,
                                       Vect &color, Vect &pos)
{
    GraphicsObjectHdr *current = this->pNext;
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_ConstColor(pMesh[i], pShaderObj, color, pos));
    }
}

void GraphicsObjectHdr::Set_ColorByPosition(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_ColorByPosition(pMesh[i], pShaderObj));
    }
}

void GraphicsObjectHdr::Set_ColorSpecLighting(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj,
                                              Vect &color, Vect &pos)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_ColorSpecLighting(pMesh[i], pShaderObj, color, pos));
    }
}

void GraphicsObjectHdr::Set_Null(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_Null(pMesh[i], pShaderObj));
    }
}

void GraphicsObjectHdr::Set_TextureLight(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj, Vect &color, Vect &pos, Texture::Name _name)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_TextureLight(pMesh[i], pShaderObj, _name, color, pos));
    }
}

void GraphicsObjectHdr::Set_Wireframe(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_Wireframe(pMesh[i], pShaderObj));
    }
}

void GraphicsObjectHdr::Set_WireframeConstColor(Mesh **pMesh, const int size, const ShaderObject *const pShaderObj,
                                                const Vect &wireColor)
{
    for (int i = 0; i < size; i++)
    {
        AddToEnd(new GraphicsObject_WireframeConstantColor(pMesh[i], pShaderObj, wireColor));
    }
}

//--------------------------------------------------------------------------------
// Non vector GraphicsObjects
//--------------------------------------------------------------------------------

void GraphicsObjectHdr::Set_FlatTexture(const Mesh *const pMesh, const ShaderObject *const pShaderObj,
                                        Texture::Name _name)
{
    this->poGraphicsObject = new GraphicsObject_FlatTexture(pMesh, pShaderObj, _name);
}

void GraphicsObjectHdr::Set_ConstColor(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Vect &color,
                                       Vect &pos)
{
    this->poGraphicsObject = new GraphicsObject_ConstColor(pMesh, pShaderObj, color, pos);
}

void GraphicsObjectHdr::Set_ColorByPosition(const Mesh *const pMesh, const ShaderObject *const pShaderObj)
{
    this->poGraphicsObject = new GraphicsObject_ColorByPosition(pMesh, pShaderObj);
}

void GraphicsObjectHdr::Set_ColorSpecLighting(const Mesh *const pMesh, const ShaderObject *const pShaderObj,
                                              Vect &color, Vect &pos)
{
    this->poGraphicsObject = new GraphicsObject_ColorSpecLighting(pMesh, pShaderObj, color, pos);
}

void GraphicsObjectHdr::Set_Null(const Mesh *const pMesh, const ShaderObject *const pShaderObj)
{
    this->poGraphicsObject = new GraphicsObject_Null(pMesh, pShaderObj);
}

void GraphicsObjectHdr::Set_TextureLight(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Vect &color, Vect &pos,
                                         Texture::Name _name)
{
    this->poGraphicsObject = new GraphicsObject_TextureLight(pMesh, pShaderObj, _name, color, pos);
}

void GraphicsObjectHdr::Set_Wireframe(const Mesh *const pMesh, const ShaderObject *const pShaderObj)
{
    this->poGraphicsObject = new GraphicsObject_Wireframe(pMesh, pShaderObj);
}

void GraphicsObjectHdr::Set_WireframeConstColor(const Mesh *const pMesh, const ShaderObject *const pShaderObj,
                                                const Vect &wireColor)
{
    this->poGraphicsObject = new GraphicsObject_WireframeConstantColor(pMesh, pShaderObj, wireColor);
}

void GraphicsObjectHdr::Set_SkinTexture(const Mesh *const pMesh, const ShaderObject *const pShaderObj, const Texture::Name _name)
{
    this->poGraphicsObject = new GraphicsObject_SkinTexture(pMesh, pShaderObj, _name);
}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
void GraphicsObjectHdr::Connect(GraphicsObject *_pGraphicsObj)
{
    this->poGraphicsObject = _pGraphicsObj;
}

void GraphicsObjectHdr::SetNext(GraphicsObjectHdr *_pNextHdr)
{
    this->pNext = _pNextHdr;
}

void GraphicsObjectHdr::AddToEnd(GraphicsObject *_pGraphicsObj)
{
    if (this->poGraphicsObject == nullptr)
    {
        this->poGraphicsObject = _pGraphicsObj;
    }
    else
    {
        GraphicsObjectHdr *tmp = this;
        while (tmp->pNext != nullptr)
        {
            tmp = tmp->pNext;
        }

        if (tmp->pNext == nullptr)
        {
            tmp->pNext = new GraphicsObjectHdr(_pGraphicsObj);
        }
    }
}

GraphicsObjectHdr *GraphicsObjectHdr::GetNext()
{
    return this->pNext;
}