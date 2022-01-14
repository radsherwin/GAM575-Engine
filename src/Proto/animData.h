#ifndef ANIM_DATA_H
#define ANIM_DATA_H

#include "animData.pb.h"
#include "boneData.h"

class animData
{
public:
    static const unsigned int FILE_NAME_SIZE = 64;

    animData();
    animData(const animData &);
    animData &operator=(const animData &);
    ~animData();

    animData(boneData *_bone_data,
             char _animName[animData::FILE_NAME_SIZE],
             signed int _jointIndex,
             signed int _parentIndex, 
             unsigned int _totalAnimFrames);

    //Serialization
    void Serialize(animData_proto &out) const;
    void Deserialize(const animData_proto &in);

    void Print(const char *const pName, int count) const;

public:
    boneData        *bone_data;
    char	        animName[FILE_NAME_SIZE]; //joint name
    signed int		jointIndex;
    signed int		parentIndex;
    unsigned int	totalAnimFrames;
};

#endif
