#include "animData.h"

animData::animData()
    : bone_data(nullptr), animName{0},jointIndex(-1), parentIndex(-1),
    totalAnimFrames(0)
{
    
}

animData::animData(const animData &r)
{
    this->bone_data = r.bone_data;
    this->jointIndex = r.jointIndex;
    this->parentIndex = r.parentIndex;
    this->totalAnimFrames = r.totalAnimFrames;

    strcpy_s(this->animName, animData::FILE_NAME_SIZE, r.animName);
}

animData &animData::operator=(const animData &r)
{
    if (this != &r)
    {
        this->bone_data = r.bone_data;
        this->jointIndex = r.jointIndex;
        this->parentIndex = r.parentIndex;
        this->totalAnimFrames = r.totalAnimFrames;

        strcpy_s(this->animName, animData::FILE_NAME_SIZE, r.animName);
    }

    return *this;
}

animData::~animData()
{
    delete[] this->bone_data;
}

animData::animData(boneData *_bone_data,
                   char _animName[animData::FILE_NAME_SIZE],
                   signed int _jointIndex,
                   signed int _parentIndex,
                   unsigned int _totalAnimFrames)
{
    this->bone_data = _bone_data;
    strcpy_s(this->animName, animData::FILE_NAME_SIZE, _animName);
    this->jointIndex = _jointIndex;
    this->parentIndex = _parentIndex;
    this->totalAnimFrames = _totalAnimFrames;
}

void animData::Serialize(animData_proto &out) const
{
    std::string sName((const char *)this->animName, animData::FILE_NAME_SIZE);
    out.set_animname(sName);

    out.set_jointindex(this->jointIndex);
    out.set_parentindex(this->parentIndex);
    out.set_totalanimframes(this->totalAnimFrames);

    for (int i = 0; i < this->totalAnimFrames; i++)
    {
        boneData_proto *pBone = new boneData_proto();
        this->bone_data[i].Serialize(*pBone);
        out.mutable_bone_data()->AddAllocated(pBone);
    }
}

void animData::Deserialize(const animData_proto &in)
{
    this->totalAnimFrames = in.totalanimframes();
    this->jointIndex = in.jointindex();
    this->parentIndex = in.parentindex();

    memcpy_s(this->animName, animData::FILE_NAME_SIZE, in.animname().data(), animData::FILE_NAME_SIZE);

    if (this->totalAnimFrames == 0)
    {
        this->bone_data = nullptr;
    }
    else
    {
        this->bone_data = new boneData[this->totalAnimFrames];
        for (int i = 0; i < this->totalAnimFrames; i++)
        {
            this->bone_data[i].Deserialize(in.bone_data(i));
        }
    }
}

void animData::Print(const char *const pName, int count) const
{
    Trace::out("\n\n--------Anim Data: %s, %d-------- \n", this->animName, count);
    Trace::out("Parent Name: %s\n", pName);
    Trace::out("Total Frames Count: %d\n", this->totalAnimFrames);
    Trace::out("Joint Index: %d\n", this->jointIndex);
    Trace::out("Parent Index: %d\n", this->parentIndex);

}