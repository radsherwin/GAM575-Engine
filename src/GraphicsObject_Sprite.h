#ifndef GRAPHICS_OBJECT_SPRITE_H
#define GRAPHICS_OBJECT_SPRITE_H

#include "GraphicsObject.h"
#include "Image.h"
#include "ShaderObject.h"

namespace Azul
{

struct Rect;

class GraphicsObject_Sprite : public GraphicsObject
{
public:
	GraphicsObject_Sprite(const Mesh *const pMesh, const ShaderObject *const pShaderObj, Image *image, Rect rect);
	GraphicsObject_Sprite() = delete;
	GraphicsObject_Sprite(const GraphicsObject_Sprite &) = delete;
	GraphicsObject_Sprite &operator=(const GraphicsObject_Sprite &) = delete;
	virtual ~GraphicsObject_Sprite();

	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	virtual void RenderSprite();

	Image *pImage;
	Matrix *poMatrix_uv;
	Matrix *poMatrix_orig;

	float origWidth;
	float origHeight;

	float origPosX;
	float origPosY;
};

} //namespace Azul

#endif // !GRAPHICS_OBJECT_SPRITE_H
