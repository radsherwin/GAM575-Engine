#ifndef GRAPHICS_OBJECT_HDR_H
#define GRAPHICS_OBJECT_HDR_H

#include <vector>

#include "GraphicsObject.h"
#include "Texture.h"
#include "Vect.h"

using namespace Azul;
class GraphicsObjectHdr
{
public:
	GraphicsObjectHdr(GraphicsObject* _pGraphicsObj);
	GraphicsObjectHdr(const Mesh* const pMesh, const ShaderObject* const);

	GraphicsObjectHdr();
	GraphicsObjectHdr(const GraphicsObjectHdr&) = delete;
	GraphicsObjectHdr& operator=(const GraphicsObjectHdr&) = delete;
	virtual ~GraphicsObjectHdr();

	void Set_FlatTexture(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj, Texture::Name _name = Texture::Name::PINK_ERROR);
	void Set_ConstColor(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj, Azul::Vect& color, Azul::Vect& pos);
	void Set_ColorByPosition(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj);
	void Set_ColorSpecLighting(Mesh **pMesh, const int size, const ShaderObject* const _pShaderObj, Azul::Vect& color, Azul::Vect& pos);
	void Set_Null(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj);
	void Set_TextureLight(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj, Azul::Vect& color, Azul::Vect& pos, Texture::Name  _name = Texture::Name::PINK_ERROR);
	void Set_Wireframe(Mesh **pMesh, const int size, const ShaderObject* const _pShaderObj);
	void Set_WireframeConstColor(Mesh **pMesh, const int size, const ShaderObject* const pShaderObj, const Vect& wireColor);

	//Non vector version
	void Set_FlatTexture(const Mesh* const pMesh, const ShaderObject* const pShaderObj, Texture::Name _name = Texture::Name::PINK_ERROR);
	void Set_ConstColor(const Mesh* const pMesh, const ShaderObject* const pShaderObj, Azul::Vect& color, Azul::Vect& pos);
	void Set_ColorByPosition(const Mesh* const mesh, const ShaderObject* const pShaderObj);
	void Set_ColorSpecLighting(const Mesh* const _pMesh, const ShaderObject* const _pShaderObj, Azul::Vect& color, Azul::Vect& pos);
	void Set_Null(const Mesh* const pMesh, const ShaderObject* const pShaderObj);
	void Set_TextureLight(const Mesh* const pMesh, const ShaderObject* const pShaderObj, Azul::Vect& color, Azul::Vect& pos, Texture::Name  _name = Texture::Name::PINK_ERROR);
	void Set_Wireframe(const Mesh* const _pMesh, const ShaderObject* const _pShaderObj);
	void Set_WireframeConstColor(const Mesh* const mesh, const ShaderObject* const pShaderObj, const Vect& wireColor);

	void Connect(GraphicsObject* _pGraphicsObj);
	void SetNext(GraphicsObjectHdr* _pNextHdr);
	void AddToEnd(GraphicsObject* _pGraphicsObj);

	GraphicsObjectHdr* GetNext();


	GraphicsObjectHdr* pNext;
	GraphicsObject* poGraphicsObject;
};

#endif
