//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectAnim.h"
#include "GameObjectBasic.h"
#include "DLink.h"
#include "Vect.h"
#include "AnimController.h"

namespace Azul
{
struct Bone;

class Skeleton : public DLink
{
public:
    struct Data
    {
        int index;
        int parentIndex;
        char name[64];
    };

    // HACK FOR DEMOS
    enum class Name
    {
        CHICKEN_BOT_1,
        CHICKEN_BOT_2,
        CHICKEN_BOT_3,
        UNINITIALIZED
    };

public:
    Skeleton(Mesh *SkeletonMesh);

    Skeleton();
    Skeleton(const Skeleton &) = delete;
    Skeleton &operator = (const Skeleton &) = delete;
    ~Skeleton();

    void Set(Name _name, Mesh *pMesh, Vect pos);

    // AnimController
    void AddController(AnimController *pController);
    AnimController *GetController() const;
    GameObjectAnim *FindBoneByIndex(int index);
    GameObjectAnim *GetFirstBone();
    Bone *BoneResult();

    void Show();
    void Hide();
    void SetPos(float x, float y, float z) const;

    virtual char *GetName() override;
    virtual bool Compare(DLink *pTarget) override;
    virtual void Dump() override;
    virtual void Wash() override;

private:
    void privSetAnimationHierarchy(Mesh *SkeletonMesh, Bone *pBoneResult);
    

public:
    Name name;
private:
    GameObjectAnim *pFirstBone;
    GameObjectBasic *pPivot;
    Bone *poBoneResult;
    AnimController *poAnimController;
    int             numBones;
};
}

#endif

// --- End of File ---