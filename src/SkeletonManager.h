#ifndef SKELETON_MANAGER_H
#define SKELETON_MANAGER_H

#include "ManBase.h"
#include "Skeleton.h"
#include "Vect.h"

class SkeletonManager : public ManBase
{
    //-----------------------------------------------------------------------------
    //	    Constructors
    //-----------------------------------------------------------------------------
private:
    SkeletonManager(int reserveNum = 3, int reserveGrow = 1);
    SkeletonManager() = delete;
    SkeletonManager(const SkeletonManager &) = delete;
    SkeletonManager &operator=(const SkeletonManager &) = delete;
    virtual ~SkeletonManager();

    //-----------------------------------------------------------------------------
    //	   Static Methods
    //-----------------------------------------------------------------------------
public:
    static void Create(int reserveNum = 5, int reserveGrow = 2);
    static void Destroy();

    static void Add(Skeleton::Name skelName, Mesh *pMesh, Vect pos);
    static Skeleton *Find(Skeleton::Name skelName);
    
    static void Remove(Skeleton *pSkel);
    static void Remove(Skeleton::Name skelName);
    static void Dump();

    //-----------------------------------------------------------------------------
    //	   Private Functions
    //-----------------------------------------------------------------------------
private:
    static SkeletonManager *privGetInstance();

protected:
    DLink *derivedCreateNode() override;

private:
    Skeleton *poNodeCompare;
    static SkeletonManager *posInstance;
};

#endif