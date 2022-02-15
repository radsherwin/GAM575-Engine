#include "SkeletonManager.h"

SkeletonManager *SkeletonManager::posInstance = nullptr;

//-----------------------------------------------------------------------------
//	    Constructors
//-----------------------------------------------------------------------------
SkeletonManager::SkeletonManager(int reserveNum, int reserveGrow)
    : ManBase(reserveGrow)
{
    this->proFillReservedPool(reserveNum);
    this->poNodeCompare = new Skeleton();
}

SkeletonManager::~SkeletonManager()
{
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Skeleton *pDeleteMe = (Skeleton *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Skeleton *pDeleteMe = (Skeleton *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//-----------------------------------------------------------------------------
//	   Static Methods
//-----------------------------------------------------------------------------
void SkeletonManager::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new SkeletonManager(reserveNum, reserveGrow);
    }
}

void SkeletonManager::Destroy()
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);
    AZUL_UNUSED_VAR(pMan);

    delete SkeletonManager::posInstance;
    SkeletonManager::posInstance = nullptr;
}

void SkeletonManager::Update()
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    // iterate through the list and delete
    Iterator *pIt = pMan->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Skeleton *pSkel = (Skeleton *)pIt->Curr();
        if(pSkel->GetController() != nullptr) pSkel->GetController()->Update();
        pNode = pIt->Next();
    }

}

void SkeletonManager::Add(Skeleton::Name _name, Mesh *pMesh, Vect pos)
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    Skeleton *pNode = (Skeleton *)pMan->baseAddToFront();
    pNode->Set(_name, pMesh, pos);
}

Skeleton *SkeletonManager::Find(Skeleton::Name _name)
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->poNodeCompare->name = _name;
    Skeleton *pNode = (Skeleton *)pMan->baseFind(pMan->poNodeCompare);
    assert(pNode != nullptr);

    return pNode;
}

void SkeletonManager::Remove(Skeleton::Name _name)
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    Skeleton *pNode = Find(_name);
    assert(pNode != nullptr);

    pMan->baseRemove(pNode);
}

void SkeletonManager::Remove(Skeleton *pSkel)
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    assert(pSkel != nullptr);

    pMan->baseRemove(pSkel);
}

void SkeletonManager::Dump()
{
    SkeletonManager *pMan = SkeletonManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
//      Private methods
//----------------------------------------------------------------------
SkeletonManager *SkeletonManager::privGetInstance()
{
    // If assert -> Create() not called
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
//      Override Abstract methods
//----------------------------------------------------------------------
DLink *SkeletonManager::derivedCreateNode()
{
    DLink *pNodeBase = new Skeleton();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}