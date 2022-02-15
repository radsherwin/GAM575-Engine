#include "Animation.h"

Animation::Animation()
    : poAnimData{ nullptr },
    animatedJointCount(0),
    totalBones(0)
{
}

Animation::~Animation()
{
    delete[] this->poAnimData;
}