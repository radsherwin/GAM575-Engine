#ifndef GRAPHICS_OBJ_HDR_SPRITE_H
#define GRAPHICS_OBJ_HDR_SPRITE_H

#include "GraphicsObjectHdr.h"
#include "GraphicsObject_Sprite.h"
#include "Font.h"

namespace Azul
{
class GraphicsObjectHdr_Sprite : public GraphicsObjectHdr
{
public:
	GraphicsObjectHdr_Sprite(GraphicsObject *pGraphicsObj);
	GraphicsObjectHdr_Sprite();
	GraphicsObjectHdr_Sprite(const GraphicsObjectHdr_Sprite &) = delete;
	GraphicsObjectHdr_Sprite &operator=(const GraphicsObjectHdr_Sprite &) = delete;
	virtual ~GraphicsObjectHdr_Sprite();

	void Set_Sprite(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Image *image, Rect rect);
	void Set_Font(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Font::Name fontName);
	

};
} //namespace Azul

#endif // GRAPHICS_OBJ_HDR_SPRITE_H
