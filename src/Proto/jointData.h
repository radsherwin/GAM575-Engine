#ifndef JOINT_DATA_H
#define JOINT_DATA_H

#include "jointData.pb.h"

class jointData
{
public:
    static const unsigned int FILE_NAME_SIZE = 32;

    jointData();
    jointData(const jointData &);
    jointData &operator=(const jointData &);
    ~jointData();

    void Serialize(jointData_proto &out) const;
    void Deserialize(const jointData_proto &in);

    void Print(const char *const pName, int index) const;

public:
    bool enabled;
    char pName[FILE_NAME_SIZE];
    int  jointIndex;
    int  parentIndex;
};

#endif
