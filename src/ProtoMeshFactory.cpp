#include "ProtoMeshFactory.h"
#include "File.h"
#include "protoData.h"
#include "MeshNodeManager.h"
#include "ProtoBuffMesh.h"
#include "TextureManager.h"
#include "StringThis.h"

ProtoMeshFactory::ProtoMeshFactory()
{
}

void ProtoMeshFactory::LoadProto(const char *const pMeshFileName, protoData &mB)
{
    //Read file
    //Create meshes and save to MeshNodeManager
    std::string fullMeshName = "./Models/" + (std::string)pMeshFileName;
    File::Handle fh;
    File::Error err;

    assert(fullMeshName.c_str());

    err = File::Open(fh, fullMeshName.c_str(), File::Mode::READ);
    assert(err == File::Error::SUCCESS);

    err = File::Seek(fh, File::Position::END, 0);
    assert(err == File::Error::SUCCESS);

    DWORD FileLength;
    err = File::Tell(fh, FileLength);
    assert(err == File::Error::SUCCESS);

    char *poNewBuff = new char[FileLength]();
    assert(poNewBuff);

    err = File::Seek(fh, File::Position::BEGIN, 0);
    assert(err == File::Error::SUCCESS);

    err = File::Read(fh, poNewBuff, FileLength);
    assert(err == File::Error::SUCCESS);

    err = File::Close(fh);
    assert(err == File::Error::SUCCESS);

    Trace::out("--------------\n");
    Trace::out("--------------\n");
    Trace::out("--------------\n");

    std::string strIn(poNewBuff, FileLength);
    protoData_proto mB_proto;

    mB_proto.ParseFromString(strIn);

    mB.Deserialize(mB_proto);

    delete poNewBuff;
}

void ProtoMeshFactory::CreateMeshArray(const char *const pFileName, Mesh **meshArray, Texture::Name texName)
{
    protoData pB;
    LoadProto(pFileName, pB);

    // Mesh has texture
    if (texName != Texture::Name::NOT_INITIALIZED)
    {
        // If it already exists do nothing
        if (!TextureManager::Exist(texName))
        {
            TextureManager::Add(pB, texName);
        }
        else
        {
            Trace::out("Texture %s already exists in manager!\n", StringMe(texName));
        }
    }

    // LOAD MESH
    for (unsigned int i = 0; i < pB.meshCount; i++)
    {
        meshArray[i] = new ProtoBuffMesh(pB.pMeshData[i], pB.totalBones);
    }
}

void ProtoMeshFactory::CreateMeshSingle(const char *const pFileName, Mesh *&mesh, Texture::Name texName)
{
    protoData pB;
    LoadProto(pFileName, pB);

    // Mesh has texture
    if (texName != Texture::Name::NOT_INITIALIZED)
    {
        // If it already exists do nothing
        if (!TextureManager::Exist(texName))
        {
            TextureManager::Add(pB, texName);
        }
        else
        {
            Trace::out("Texture %s already exists in manager!\n", StringMe(texName));
        }
    }

    // LOAD MESH
    mesh = new ProtoBuffMesh(pB.pMeshData[0], pB.totalBones);
}

void ProtoMeshFactory::GetAnimation(const char *const pMeshFileName, Animation *&pAnim)
{
    protoData pB;
    ProtoMeshFactory::LoadProto(pMeshFileName, pB);
    pAnim = new Animation();
    pAnim->poAnimData = new Animation::AnimData[pB.animCount];
    pAnim->totalBones = 12;//pB.totalBones;
    pAnim->animatedJointCount = pB.animCount;
    for (unsigned int index = 0; index < pB.animCount; index++)
    {
        animData *pAD = &pB.pAnimData[index];
        Animation::AnimData *animStruct = &pAnim->poAnimData[index];
        animStruct->parent = pAD->parentIndex;
        animStruct->joint = pAD->jointIndex;
        animStruct->frames = pAD->totalAnimFrames;
        animStruct->protoName = pAD->animName;

        Bone tmpBone;
        animStruct->meshBone.reserve(animStruct->frames);
        for (int i = 0; i < animStruct->frames; i++)
        {
            float *pTrans = (float *)&pAD->bone_data[i].pTranslation[0];
            tmpBone.T.set(*&pTrans[0], *&pTrans[1], *&pTrans[2]);

            float *pQuat = (float *)&pAD->bone_data[i].pRotation[0];
            tmpBone.Q.set(*&pQuat[0], *&pQuat[1], *&pQuat[2], *&pQuat[3]);

            float *pScale = (float *)&pAD->bone_data[i].pScale[0];
            tmpBone.S.set(*&pScale[0], *&pScale[1], *&pScale[2]);

            animStruct->meshBone.push_back(tmpBone);
        }
        
        //AnimationManager::Add(pAnim);
    }
}