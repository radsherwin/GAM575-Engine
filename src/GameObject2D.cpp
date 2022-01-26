#include "MathEngine.h"
#include "GameObject2D.h"
#include "GraphicsObjectHdr_Sprite.h"
#include "GraphicsObject_Sprite.h"

namespace Azul
{
GameObject2D::GameObject2D(GraphicsObjectHdr_Sprite *pGraphicsObjHdr)
    : GameObject(pGraphicsObjHdr)
{
    assert(pGraphicsObjHdr != 0);

    this->angle = 0.f;
    //If the GraphicsObject is note _Sprite then this will fail.
    //Maybe to a special graphics object header?
    this->posX = ((GraphicsObject_Sprite *)pGraphicsObjHdr->poGraphicsObject)->origPosX;
    this->posY = ((GraphicsObject_Sprite *)pGraphicsObjHdr->poGraphicsObject)->origPosY;
    this->scaleX = 1.f;
    this->scaleY = 1.f;
}

GameObject2D::~GameObject2D()
{
}

void GameObject2D::Update(AnimTime currentTime)
{
    AZUL_UNUSED_VAR(currentTime);

    Matrix Scale(Matrix::Scale::XYZ, this->scaleX, this->scaleY, 1.0f);
    Matrix Rot(Matrix::Rot1::Z, this->angle);
    Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

    *this->poWorld = Scale * Rot * Trans;

    // push to graphics object
    GraphicsObjectHdr *tmp = this->poGraphicsObjHdr;
    while (tmp != nullptr)
    {
        if (!this->IsHidden())
        {
            tmp->poGraphicsObject->SetWorld(*this->poWorld);
        }
        tmp = tmp->pNext;
    }
}
} //namespace Azul