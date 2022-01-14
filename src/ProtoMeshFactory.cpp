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
        meshArray[i] = new ProtoBuffMesh(pB.pMeshData[i]);
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
    mesh = new ProtoBuffMesh(pB.pMeshData[0]);
}

void ProtoMeshFactory::GetAnimation(const char *const pMeshFileName, Animation **AnimationArray)
{
    protoData pB;
    ProtoMeshFactory::LoadProto(pMeshFileName, pB);

    for (unsigned int index = 0; index < pB.animCount; index++)
    {
        animData *pAnim = &pB.pAnimData[index];
        Animation *tmpAnim = new Animation();

        tmpAnim->parent = pAnim->parentIndex;
        tmpAnim->joint = pAnim->jointIndex;
        tmpAnim->frames = pAnim->totalAnimFrames;
        tmpAnim->protoName = pAnim->animName;

        Bone tmpBone;
        tmpAnim->meshBone.reserve(tmpAnim->frames);
        for (int i = 0; i < tmpAnim->frames; i++)
        {
            float *pTrans = (float *)&pAnim->bone_data[i].pTranslation[0];
            tmpBone.T.set(*&pTrans[0], *&pTrans[1], *&pTrans[2]);

            float *pQuat = (float *)&pAnim->bone_data[i].pRotation[0];
            tmpBone.Q.set(*&pQuat[0], *&pQuat[1], *&pQuat[2], *&pQuat[3]);

            float *pScale = (float *)&pAnim->bone_data[i].pScale[0];
            tmpBone.S.set(*&pScale[0], *&pScale[1], *&pScale[2]);

            tmpAnim->meshBone.push_back(tmpBone);
        }

        AnimationManager::Add(tmpAnim);
        AnimationArray[index] = tmpAnim;
    }
}