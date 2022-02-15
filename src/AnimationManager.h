#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H
#include "AnimController.h"
#include "Mesh.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{
class AnimTime;
class PCSTree;
class AnimController;
class AnimationManager

{
public:

    AnimationManager();
    AnimationManager(const AnimationManager &) = delete;
    AnimationManager &operator=(const AnimationManager &) = delete;
    ~AnimationManager();

    //static void Add(Mesh** pMesh, Clip::Name _clipName, int _numBones);
    static void Add(Animation *pAnimation, Clip::Name _clipName);
    static void Add(Animation *anim);

    static void SetClip(Clip::Name _clipName, Skeleton::Name _skelName);
    static void AddController(Skeleton::Name skelName, Clip::Name _clip);

    static void IncreaseSpeed(Skeleton::Name _skelName, float speed);
    static void DecreaseSpeed(Skeleton::Name _skelName, float speed);
    static void Forward(Skeleton::Name _skelName);
    static void Reverse(Skeleton::Name _skelName);
    static void PlayPause(Skeleton::Name _skelName);

    //static void Demo();
    //static void Demo2();
    static void Create();
    static void Destroy();

private:
    static AnimationManager *privGetInstance();
    static Clip *privFind(Clip::Name _clipName);

public:
    PCSTree *poClipTree;
    PCSTree *poAnimTree;

private:
    static AnimationManager *posInstance;
};
}

#endif