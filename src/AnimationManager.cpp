#include "AnimationManager.h"

#include "meshData.h"
#include "SkeletonManager.h"

using namespace Azul;

AnimationManager *AnimationManager::posInstance = nullptr;

AnimationManager::AnimationManager(int reserveNum, int reserveGrow)
    : ManBase(reserveGrow)
{
    this->proFillReservedPool(reserveNum);
    this->poNodeCompare = new Clip();
}

AnimationManager::~AnimationManager()
{
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator *pIt = this->baseGetActiveIterator();

    DLink *pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Clip *pDeleteMe = (Clip *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Clip *pDeleteMe = (Clip *)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

void AnimationManager::Create(int reserveNum, int reserveGrow)
{
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new AnimationManager(reserveNum, reserveGrow);
    }
}

void AnimationManager::Destroy()
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan != nullptr);
    AZUL_UNUSED_VAR(pMan);

    delete AnimationManager::posInstance;
    AnimationManager::posInstance = nullptr;
}

void AnimationManager::Add(Animation *pAnimation, Clip::Name _clipName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);
    
    Clip *pClip = (Clip *)pMan->baseAddToFront();
    pClip->Set(pAnimation, _clipName);
}

Clip *AnimationManager::Find(Clip::Name _clipName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    pMan->poNodeCompare->name = _clipName;
    Clip *pNode = (Clip *)pMan->baseFind(pMan->poNodeCompare);
    assert(pNode);

    return pNode;
}

void AnimationManager::Remove(Clip *pClip)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    assert(pClip);
    pMan->baseRemove(pClip);
}

void AnimationManager::Remove(Clip::Name _name)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Clip *pNode = Find(_name);

    pMan->baseRemove(pNode);
}

void AnimationManager::SetClip(Clip::Name _clipName, Skeleton::Name _skelName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Clip *pClip = pMan->Find(_clipName);
    assert(pClip);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->SetClip(pClip);
}

void AnimationManager::AddController(Skeleton::Name skelName, Clip::Name _clip)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Clip *pClip = pMan->Find(_clip);
    assert(pClip);

    Skeleton *pSkel = SkeletonManager::Find(skelName);
    assert(pSkel != nullptr);

    AnimTime delta = 0.1f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
    pSkel->AddController(new AnimController(delta, pClip, pSkel));
}

void AnimationManager::IncreaseSpeed(Skeleton::Name _skelName, float speed)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->IncreaseSpeed(speed);
}

void AnimationManager::DecreaseSpeed(Skeleton::Name _skelName, float speed)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->DecreaseSpeed(speed);
}

void AnimationManager::Forward(Skeleton::Name _skelName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->Forward();
}

void AnimationManager::Reverse(Skeleton::Name _skelName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->Reverse();
}

void AnimationManager::PlayPause(Skeleton::Name _skelName)
{
    AnimationManager *pMan = AnimationManager::privGetInstance();
    assert(pMan);

    Skeleton *pSkel = SkeletonManager::Find(_skelName);
    assert(pSkel != nullptr);

    pSkel->GetController()->PlayPause();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
AnimationManager *AnimationManager::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *AnimationManager::derivedCreateNode()
{
    DLink *pNodeBase = new Clip();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

//void AnimationManager::Demo()
//{
//    AnimationManager *pMan = AnimationManager::privGetInstance();
//    assert(pMan);
//
//    //Update all controllers
//    PCSTree *pTree = pMan->poControllerTree;
//    PCSNode *pNode = nullptr;
//
//    PCSTreeForwardIterator pForIter(pTree->GetRoot());
//    pNode = pForIter.First();
//    AnimController *pTmp = nullptr;
//    while (!pForIter.IsDone())
//    {
//        pTmp = (AnimController *)pForIter.CurrentItem();
//        if (pTmp->name != AnimController::AnimName::DEFAULT && pTmp->name != AnimController::AnimName::MESH1) pTmp->Hide();
//        pNode = pForIter.Next();
//    }
//}
//
//void AnimationManager::Demo2()
//{
//    AnimationManager *pMan = AnimationManager::privGetInstance();
//    assert(pMan);
//
//    //Update all controllers
//    PCSTree *pTree = pMan->poControllerTree;
//    PCSNode *pNode = nullptr;
//
//    PCSTreeForwardIterator pForIter(pTree->GetRoot());
//    pNode = pForIter.First();
//    AnimController *pTmp = nullptr;
//    while (!pForIter.IsDone())
//    {
//        pTmp = (AnimController *)pForIter.CurrentItem();
//        if (pTmp->name != AnimController::AnimName::DEFAULT && pTmp->name != AnimController::AnimName::MESH1) pTmp->Show();
//        pNode = pForIter.Next();
//    }
//}


