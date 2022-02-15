//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "Mixer.h"
#include "MeshNodeManager.h"

namespace Azul
{
Clip::Clip()
    : name(Clip::Name::NULL_CLIP),
    numBones(0),
    numFrames(0),
    TotalTime(AnimTime::Duration::ZERO),
    poHead(nullptr)
{
}

Clip::~Clip()
{
    FrameBucket *pTmp = poHead;

    while (pTmp != nullptr)
    {
        FrameBucket *pDeleteMe = pTmp;
        pTmp = pTmp->nextBucket;
        delete pDeleteMe;
    }
}

void Clip::Set(Animation *pAnimation, Clip::Name _name)
{
    name = _name;
    numBones = pAnimation->totalBones;
    numFrames = 0;
    TotalTime = AnimTime(AnimTime::Duration::ZERO);
    poHead = nullptr;
    this->privSetAnimationData(pAnimation);
    this->TotalTime = this->privFindMaxTime();
}

Clip::Name Clip::GetClipName() const
{
    return this->name;
}

AnimTime Clip::privFindMaxTime()
{
    AnimTime tMax;
    FrameBucket *pTmp = this->poHead;

    while (pTmp->nextBucket != nullptr)
    {
        pTmp = pTmp->nextBucket;
    }

    tMax = pTmp->KeyTime;

    return tMax;
}

AnimTime Clip::GetTotalTime()
{
    return this->TotalTime;
}

void Clip::AnimateBones(AnimTime tCurr, Bone *pResult)
{
    // First one
    FrameBucket *pTmp = this->poHead;

    // Find which key frames
    while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
    {
        pTmp = pTmp->nextBucket;
    }

    // pTmp is the "B" key frame
    // pTmp->prev is the "A" key frame
    FrameBucket *pA = pTmp->prevBucket;
    FrameBucket *pB = pTmp;

    // find the "S" of the time
    float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

    Mixer::Blend(pResult, pA->poBone, pB->poBone, tS, this->numBones);
}

void Clip::Print(std::vector<Mesh *> pMesh, int frameIndex)
{
    Bone bone;
    int joint;
    Trace::out("------------CLIP.CPP------------\n");

    /*for (size_t meshIndex = 0; meshIndex < pMesh.size(); meshIndex++)
    {
        bone = pMesh[meshIndex]->pAnim->meshBone[frameIndex];
        joint = pMesh[meshIndex]->pAnim->joint;

        Trace::out("KeyFrame: %d\n", frameIndex);
        Trace::out("Joint name: %s, joint index: %d\n", pMesh[meshIndex]->pAnim->protoName.c_str(), joint);
        Trace::out("T[%d]: %f %f %f\n", joint, bone.T.x(), bone.T.y(), bone.T.z());
        Trace::out("Q[%d]: %f %f %f %f\n", joint, bone.Q.qx(), bone.Q.qy(), bone.Q.qz(), bone.Q.real());
        Trace::out("S[%d]: %f %f %f\n", joint, bone.S.x(), bone.S.y(), bone.S.z());
    }*/
}
Vect baseT(0, 0, 0);
Quat baseR(0, 0, 0, 1);
Vect baseS(1, 1, 1);
float scale = 1.0f;
void Clip::privSetAnimationData(Animation *pAnimation)
{
    // --------  Result Frame  ----------------------

    FrameBucket *pTmp;

    FrameBucket *pPrev = nullptr;

    this->numFrames = pAnimation->poAnimData[0].frames;

    // --------  Frame 0  ----------------------------
    //pBone_Mesh->frames
    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++)
    {
        FrameBucket *pTmp0 = new FrameBucket();
        pTmp0->prevBucket = pPrev;
        pTmp0->nextBucket = nullptr;
        pTmp0->KeyTime = frameIndex * AnimTime(AnimTime::Duration::FILM_24_FRAME);
        pTmp0->poBone = new Bone[(unsigned int)this->numBones];
        if (pPrev == nullptr) this->poHead = pTmp0;
        else pPrev->nextBucket = pTmp0;

        pTmp = pTmp0;
        pPrev = pTmp0;
        pTmp->poBone[0].T = baseT;
        pTmp->poBone[0].Q = baseR;
        pTmp->poBone[0].S = baseS;

        pTmp->poBone[1].T = baseT;
        pTmp->poBone[1].Q = baseR;
        pTmp->poBone[1].S = baseS;

        pTmp->poBone[2].T = baseT;
        pTmp->poBone[2].Q = baseR;
        pTmp->poBone[2].S = baseS;

        pTmp->poBone[3].T = baseT;
        pTmp->poBone[3].Q = baseR;
        pTmp->poBone[3].S = baseS;

        for (int i = 0; i < pAnimation->animatedJointCount; i++)
        {
            Animation::AnimData *pAD = &pAnimation->poAnimData[i];
            pTmp->poBone[pAD->joint] = pAD->meshBone[frameIndex];

            // HACK
            if (pAD->joint == 4)
            {
                pTmp->poBone[4].S = scale * pTmp->poBone[4].S;
            }
        }
    }
}

bool Clip::Compare(DLink *pTarget)
{
    // This is used in ManBase.Find()
    assert(pTarget != nullptr);

    Clip *pDataB = (Clip *)pTarget;

    bool status = false;

    if (this->name == pDataB->name)
    {
        status = true;
    }

    return status;
}

void Clip::Dump()
{
    Trace::out("      Clip(%p)\n", this);

    // Data:
    //Trace::out("      Name: %s \n", STRING_ME(this->name));

    //Trace::out("         x: %d \n", this->x);

    DLink::Dump();
}

void Clip::Wash()
{
}

char *Clip::GetName()
{
    // todo - Hack understand why is this needed for print and fix...
    //static char pTmp[128];
    //strcpy_s(pTmp, 128, STRING_ME(this->name));
    //return pTmp;
    return nullptr;
}
}

// --- End of File ---