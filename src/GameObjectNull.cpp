#include "GameObjectNull.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

namespace Azul
{
GameObjectNull::GameObjectNull(GraphicsObjectHdr *pGraphicsObjectHdr) : GameObject(pGraphicsObjectHdr)
{
	assert(pGraphicsObjectHdr != nullptr);
}

GameObjectNull::~GameObjectNull() {}

void GameObjectNull::Update(AnimTime currentTime)
{
	AZUL_UNUSED_VAR(currentTime);

	//Goal: update the world Matrix
	Matrix Identity(Matrix::Special::Identity);
	*this->poWorld = Identity;

	//push to graphics object
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


}