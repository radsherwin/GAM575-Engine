#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H
#include "Animation.h"
#include "Mesh.h"
#include "Clip.h"
#include "Skeleton.h"
#include "ManBase.h"

namespace Azul
{
class AnimationManager : public ManBase

{
public:

    AnimationManager(int reserveNum = 3, int reserveGrow = 1);
    AnimationManager() = delete;
    AnimationManager(const AnimationManager &) = delete;
    AnimationManager &operator=(const AnimationManager &) = delete;
    ~AnimationManager();

    static void Create(int reserveNum = 3, int reserveGrow = 2);
    static void Destroy();
    
    static void Add(Animation *pAnimation, Clip::Name _clipName);

    static Clip *Find(Clip::Name clipName);
    static void Remove(Clip::Name clipName);
    static void Remove(Clip *pClip);

    static void SetClip(Clip::Name _clipName, Skeleton::Name _skelName);
    static void AddController(Skeleton::Name skelName, Clip::Name _clip);

    static void IncreaseSpeed(Skeleton::Name _skelName, float speed);
    static void DecreaseSpeed(Skeleton::Name _skelName, float speed);
    static void Forward(Skeleton::Name _skelName);
    static void Reverse(Skeleton::Name _skelName);
    static void PlayPause(Skeleton::Name _skelName);

protected:
    DLink *derivedCreateNode() override;

private:
    static AnimationManager *privGetInstance();
    static AnimationManager *posInstance;
    Clip *poNodeCompare;
};
}

#endif