//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObjectHdr.h"
#include "GameObjectBasic.h"
#include "GameObjectManager.h"
#include "ShaderManager.h"
#include "Skeleton.h"

#include <PCSTreeForwardIterator.h>

#include "MeshNodeManager.h"
#include "GameObjectAnimSkeleton.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject_SkinTexture.h"
#include "Bone.h"

namespace Azul
{
Skeleton::Skeleton()
    : name(Name::UNINITIALIZED),
    pFirstBone{ nullptr },
    pPivot{ nullptr },
    poBoneResult{ nullptr },
    poAnimController{nullptr},
    numBones(0)

{
}

Skeleton::Skeleton(Mesh *pSkelMesh)
    : pFirstBone(nullptr),
    pPivot{nullptr},
    poBoneResult(new Bone[12]),
    poAnimController(nullptr),
    numBones(pSkelMesh->jointCount)
{
    this->privSetAnimationHierarchy(pSkelMesh, this->poBoneResult);
    assert(pFirstBone);
}

Skeleton::~Skeleton()
{
    //delete[] this->poBoneResult;
    delete this->poAnimController;
}

void Skeleton::Set(Name _name, Mesh *pSkelMesh, Vect pos)
{
    assert(pSkelMesh != nullptr);
    this->pFirstBone = nullptr;
    this->numBones = pSkelMesh->jointCount;
    this->name = _name;
    this->poBoneResult = new Bone[(unsigned int)numBones];
    this->privSetAnimationHierarchy(pSkelMesh, this->poBoneResult);
    assert(pFirstBone);

    this->pPivot->SetTrans(pos);
}

AnimController *Skeleton::GetController() const
{
  
    return this->poAnimController;
}

void Skeleton::AddController(AnimController *pController)
{
    this->poAnimController = pController;
}

GameObjectAnim *Skeleton::FindBoneByIndex(int index)
{
    GameObjectAnim *pFound = nullptr;

    GameObjectAnim *pObj = this->GetFirstBone();

    // TODO - add test bed for an interator of a tree with only one node
    if (pObj->index == index)
    {
        pFound = pObj;
    }
    else
    {
        PCSTreeForwardIterator pIter(pObj);
        PCSNode *pNode = pIter.First();
        GameObjectAnim *pGameObj = nullptr;

        // walks the anim node does the pose for everything that
        while (pNode != nullptr)
        {
            assert(pNode);
            // Update the game object
            pGameObj = (GameObjectAnim *)pNode;
            if (pGameObj->index == index)
            {
                pFound = pGameObj;
                break;
            }
            pNode = pIter.Next();
        }
    }

    return pFound;
}

GameObjectAnim *Skeleton::GetFirstBone()
{
    assert(pFirstBone);
    return this->pFirstBone;
}

Bone *Skeleton::BoneResult()
{
    return this->poBoneResult;
}

void Skeleton::Show()
{
    GameObjectAnim *pObj = this->GetFirstBone();

    PCSTreeForwardIterator pIter(pObj);
    PCSNode *pNode = pIter.First();
    GameObjectAnim *pGameObj = nullptr;

    // walks the anim node does the pose for everything that
    while (pNode != nullptr)
    {
        assert(pNode);
        // Update the game object
        pGameObj = (GameObjectAnim *)pNode;
        pGameObj->DrawEnable();
        pNode = pIter.Next();
    }
}

void Skeleton::Hide()
{
    GameObjectAnim *pObj = this->GetFirstBone();

    PCSTreeForwardIterator pIter(pObj);
    PCSNode *pNode = pIter.First();
    GameObjectAnim *pGameObj = nullptr;

    // walks the anim node does the pose for everything that
    while (pNode != nullptr)
    {
        assert(pNode);
        // Update the game object
        pGameObj = (GameObjectAnim *)pNode;
        pGameObj->DrawDisable();
        pNode = pIter.Next();
    }
}

void Skeleton::SetPos(float x, float y, float z) const
{
    this->pPivot->SetTrans(x, y, z);
}

void Skeleton::privSetAnimationHierarchy(Mesh *pSkeletonMesh, Bone *pBoneResult)
{
    Vect pos(1, 1, 1);
    Vect color(1.0f, 1.0f, 1.0f, 1.0f);
    GraphicsObjectHdr *pGraphicsHdr;

    ShaderObject *pShaderObject_skinTexture = ShaderManager::Find(ShaderObject::Name::SKIN_TEXTURE);
    assert(pShaderObject_skinTexture);

    ShaderObject *pShaderObject_texture = ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE);
    assert(pShaderObject_texture);

    ShaderObject *pShaderNull = ShaderManager::Find(ShaderObject::Name::NULL_SHADER);
    assert(pShaderNull);

    //Add dummy pivot
    Mesh *pMeshNull = MeshNodeManager::Find(Mesh::Name::NULL_MESH);
    assert(pMeshNull);

    pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_Null(pMeshNull, pShaderNull);
    GameObjectBasic *pPivot = new GameObjectBasic(pGraphicsHdr);
    pPivot->SetName("Pivot");
    pPivot->DrawDisable();
    pPivot->curRotZ = 1.5708f;
    pPivot->SetScale(40, 40, 40);
    pPivot->SetTrans(0, 0, 0);
    this->pPivot = pPivot;

    GameObjectManager::Add(pPivot, GameObjectManager::GetRoot());

    //Root animation is treated differently

    pGraphicsHdr = new GraphicsObjectHdr();
    pGraphicsHdr->Set_SkinTexture(pSkeletonMesh, pShaderObject_skinTexture, Texture::Name::CHICKEN_BOT);
    GameObjectAnimSkin *pGameSkin = new GameObjectAnimSkin(pGraphicsHdr, pSkeletonMesh, this);
    pGameSkin->SetName("ChickenSkin");
    GameObjectManager::Add(pGameSkin, pPivot);

    GameObjectAnim *pGameObj;
    // Root animation is treated differently
    for (int i = 0; i < 1; i++)
    {
        pGraphicsHdr = new GraphicsObjectHdr();
        pGraphicsHdr->Set_Null(pMeshNull, pShaderNull);
        pGameObj = new GameObjectAnimSkeleton(pGraphicsHdr, pBoneResult);
        pGameObj->SetIndex(pSkeletonMesh->poJointData[i].jointIndex);
        pGameObj->SetName(pSkeletonMesh->poJointData[i].name);
        GameObjectManager::Add(pGameObj, pPivot);
        this->pFirstBone = pGameObj;
    }

    for (int i = 1; i < this->numBones; i++)
    {
        pGraphicsHdr = new GraphicsObjectHdr();
        pGraphicsHdr->Set_Null(pMeshNull, pShaderNull);
        pGameObj = new GameObjectAnimSkeleton(pGraphicsHdr, pBoneResult);
        pGameObj->SetIndex(pSkeletonMesh->poJointData[i].jointIndex);
        pGameObj->SetName(pSkeletonMesh->poJointData[i].name);

        GameObjectAnim *pParent = this->FindBoneByIndex(pSkeletonMesh->poJointData[i].parentIndex);
        assert(pParent);
        GameObjectManager::Add(pGameObj, pParent);
    }
}

bool Skeleton::Compare(DLink *pTarget)
{
    // This is used in ManBase.Find()
    assert(pTarget != nullptr);

    Skeleton *pDataB = (Skeleton *)pTarget;

    bool status = false;

    if (this->name == pDataB->name)
    {
        status = true;
    }

    return status;
}

void Skeleton::Dump()
{
    Trace::out("      Skeleton(%p)\n", this);

    // Data:

    //Trace::out("         x: %d \n", this->x);

    DLink::Dump();
}

void Skeleton::Wash()
{
}

char *Skeleton::GetName()
{
    // todo - Hack understand why is this needed for print and fix...
    return nullptr;
}
}

// --- End of File ---