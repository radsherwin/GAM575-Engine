//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKIN_H
#define GAME_OBJECT_ANIM_SKIN_H

#include "GameObjectAnim.h"
#include "MathEngine.h"
#include "AnimTime.h"
#include "Texture.h"
#include "Mesh.h"

namespace Azul
{
	class Skeleton;
	struct Bone;

	class GameObjectAnimSkin : public GameObjectAnim
	{
	public:
		GameObjectAnimSkin(GraphicsObjectHdr* pGraphicsObjectHdr, Mesh *pMesh, Skeleton *pSkel);

		// Big four
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);

		virtual void SetIndex(int i) override;

	private:
		void privUpdateBoneWorldArray();

	public:   
		Matrix* poBoneWorld;
		Mesh *pMesh;
		Skeleton *pSkel;
	};
}

#endif

// --- End of File ---