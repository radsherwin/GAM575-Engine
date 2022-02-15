#ifndef GRAPHICS_OBJECT_SKIN_TEXTURE_H
#define GRAPHICS_OBJECT_SKIN_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureManager.h"

class GraphicsObject_SkinTexture : public GraphicsObject
{
public:
    GraphicsObject_SkinTexture(const Mesh *pMesh, const ShaderObject *pShaderObj, Texture::Name _name);
    GraphicsObject_SkinTexture() = delete;
    GraphicsObject_SkinTexture(const GraphicsObject_SkinTexture &) = delete;
    GraphicsObject_SkinTexture &operator=(const GraphicsObject_SkinTexture &) = delete;
    virtual ~GraphicsObject_SkinTexture() = default;

    void SetBoneWorld(Matrix *pBoneWorld);

    // Rendermaterial contract
    virtual void SetState() override;
    virtual void SetDataGPU() override;
    virtual void Draw() override;
    virtual void RestoreState() override;

    // data:  place uniform instancing here
    Matrix *pBoneWorld;
    Texture::Name textName;
};

#endif