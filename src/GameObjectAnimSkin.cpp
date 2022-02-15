//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameObjectAnimSkin.h"
#include "Anim.h"
#include "GraphicsObject_SkinTexture.h"


namespace Azul
{
	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObjectHdr* pGraphicsObjectHdr, Mesh *pMesh)
		: GameObjectAnim(pGraphicsObjectHdr)
	{
		assert(pGraphicsObjectHdr);
		this->pMesh = pMesh;
		this->poBoneWorld = new Matrix[pMesh->jointCount]();
		((GraphicsObject_SkinTexture *)this->poGraphicsObjHdr->poGraphicsObject)->SetBoneWorld(poBoneWorld);
		
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete[] this->poBoneWorld;
	}

	void GameObjectAnimSkin::privUpdateBoneWorldArray()
	{
		/*Anim *pAnim = pAnimController->GetAnim();
		Skeleton *pSkel = pAnim->GetSkeleton();

		for (int i = 0; i < 8; i++)
		{
			GameObjectAnim *pGameObject = pSkel->FindBoneByIndex(i+4);
			this->poBoneWorld[i] = *pGameObject->GetWorld();
		}*/

	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		this->baseUpdateBoundingSphere();

		this->privUpdateBoneWorldArray();
	}

	void GameObjectAnimSkin::SetIndex(int i )
	{
		AZUL_UNUSED_VAR(i);
	}
}

// --- End of File ---