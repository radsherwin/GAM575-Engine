//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TextureManager.h"
#include "TGA.h"
#include "textureDataHelper.h"
#include "File.h"

TextureManager* TextureManager::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TextureManager::TextureManager(int reserveNum, int reserveGrow)
    : ManBase(reserveGrow)
{
    // Preload the reserve
    this->proFillReservedPool(reserveNum);

    // initialize derived data here
    this->poNodeCompare = new Texture();
}

TextureManager::~TextureManager()
{
    delete this->poNodeCompare;
    this->poNodeCompare = nullptr;

    // iterate through the list and delete
    Iterator* pIt = this->baseGetActiveIterator();

    DLink* pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Texture* pDeleteMe = (Texture*)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }

    pIt = this->baseGetReserveIterator();

    pNode = pIt->First();

    // Walk through the nodes
    while (!pIt->IsDone())
    {
        Texture* pDeleteMe = (Texture*)pIt->Curr();
        pNode = pIt->Next();
        delete pDeleteMe;
    }
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void TextureManager::Create(int reserveNum, int reserveGrow)
{
    // make sure values are ressonable 
    assert(reserveNum > 0);
    assert(reserveGrow > 0);

    // initialize the singleton here
    assert(posInstance == nullptr);

    // Do the initialization
    if (posInstance == nullptr)
    {
        posInstance = new TextureManager(reserveNum, reserveGrow);
    }

}

void TextureManager::Destroy()
{
    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);
    AZUL_UNUSED_VAR(pMan);

    delete TextureManager::posInstance;
    TextureManager::posInstance = nullptr;
}

Texture* TextureManager::Add(const char* const pName, Texture::Name name)
{
    TextureManager* pMan = TextureManager::privGetInstance();

    GLuint* pTextureID = new GLuint;

    //Read and recreate model data
    Azul::File::Handle fh;
    Azul::File::Error err;
    assert(pName);
    err = Azul::File::Open(fh, pName, Azul::File::Mode::READ);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Seek(fh, Azul::File::Position::END, 0);
    assert(err == Azul::File::Error::SUCCESS);
    DWORD FileLength;
    err = Azul::File::Tell(fh, FileLength);
    assert(err == Azul::File::Error::SUCCESS);
    char *poNewBuff = new char[FileLength]();
    assert(poNewBuff);
    err = Azul::File::Seek(fh, Azul::File::Position::BEGIN, 0);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Read(fh, poNewBuff, FileLength);
    assert(err == Azul::File::Error::SUCCESS);
    err = Azul::File::Close(fh);
    assert(err == Azul::File::Error::SUCCESS);

    std::string strIn(poNewBuff, FileLength);
    meshData_proto mB_proto;
    mB_proto.ParseFromString(strIn);
    meshData mB;
    mB.Deserialize(mB_proto);

    delete[] poNewBuff;

    pMan->privLoadTexture(mB, pTextureID);
    Texture *pNode = (Texture*)pMan->baseAddToFront();
    assert(pNode != nullptr);

    pNode->Set
    (
        mB.text_color->pFileName,
        name,
        pTextureID,
        textureDataHelper::Get(mB.text_color->minFilter),
        textureDataHelper::Get(mB.text_color->magFilter),
        textureDataHelper::Get(mB.text_color->wrapS),
        textureDataHelper::Get(mB.text_color->wrapT),
        mB.text_color->width,
        mB.text_color->height
    );

    return pNode;
}
Texture* TextureManager::Add(meshData& mB, Texture::Name _name)
{
    TextureManager* pMan = TextureManager::privGetInstance();
    GLuint* pTextureID = new GLuint[mB.texCount];
    

    //Load the texture and get the textureID
    pMan->privLoadTexture(mB, pTextureID);
    //std::vector<Texture*> pNode(mB.texCount);

    Texture* pNode = (Texture*)pMan->baseAddToFront();

    //Initialize the date
    for(unsigned int i = 0; i < mB.texCount; i++)
    {
        pNode->Set
        (
            mB.text_color[i].pFileName,
            _name,
            pTextureID,
            textureDataHelper::Get(mB.text_color[i].minFilter),
            textureDataHelper::Get(mB.text_color[i].magFilter),
            textureDataHelper::Get(mB.text_color[i].wrapS),
            textureDataHelper::Get(mB.text_color[i].wrapT),
            mB.text_color->width,
            mB.text_color->height
        );
    }

    return pNode;
}

Texture* TextureManager::Add(textureData& tB, Texture::Name _name)
{
    TextureManager* pMan = TextureManager::privGetInstance();
    GLuint* pTextureID = new GLuint[1];


    //Load the texture and get the textureID
    pMan->privLoadTexture(tB, pTextureID);
    //std::vector<Texture*> pNode(mB.texCount);

    Texture* pNode = (Texture*)pMan->baseAddToFront();

    //Initialize the date
    pNode->Set
    (
        tB.pFileName,
        _name,
        pTextureID,
        textureDataHelper::Get(tB.minFilter),
        textureDataHelper::Get(tB.magFilter),
        textureDataHelper::Get(tB.wrapS),
        textureDataHelper::Get(tB.wrapT),
        tB.width,
        tB.height
    );


    return pNode;
}

Texture* TextureManager::Find(Texture::Name _name)
{
    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);

    // Compare functions only compares two Nodes

    // So:  Use the Compare Node - as a reference
    //      use in the Compare() function
    pMan->poNodeCompare->name = _name;

    Texture* pData = (Texture*)pMan->baseFind(pMan->poNodeCompare);
    if(pData == nullptr) pData = pMan->Find(Texture::Name::PINK_ERROR);
    return pData;
}

GLuint* TextureManager::FindID(Texture::Name name)
{
    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);

    Texture* pTexture = pMan->Find(name);
    assert(pTexture);

    return pTexture->textureID;
}

GLuint TextureManager::FindID(Texture::Name name, int _index)
{
    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);

    Texture* pTexture = pMan->Find(name);
    assert(pTexture);

    return pTexture->textureID[_index];
}

void TextureManager::Remove(Texture* pNode)
{
    assert(pNode != nullptr);

    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseRemove(pNode);
}

void TextureManager::Dump()
{
    TextureManager* pMan = TextureManager::privGetInstance();
    assert(pMan != nullptr);

    pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
TextureManager* TextureManager::privGetInstance()
{
    // Safety - this forces users to call Create() first before using class
    assert(posInstance != nullptr);

    return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink* TextureManager::derivedCreateNode()
{
    DLink* pNodeBase = new Texture();
    assert(pNodeBase != nullptr);

    return pNodeBase;
}

void TextureManager::privLoadTexture(meshData& mB, GLuint*& textureID)
{
    //Get an ID, for textures (OpenGL poor man's handle)
    glGenTextures(mB.texCount, textureID);

    //Bind it
    for(int i = 0; i < (int)mB.texCount; i++)
    {
        this->privLoadRawTexture(mB.text_color[i], textureID[i]);
    }   
}

void TextureManager::privLoadTexture(textureData& tB, GLuint*& textureID)
{
    //Get an ID, for textures (OpenGL poor man's handle)
    glGenTextures(1, textureID);

    //Bind it
	this->privLoadRawTexture(tB, textureID[0]);
}

bool TextureManager::privLoadRawTexture(textureData& tD, GLuint texture)
{

    glBindTexture(GL_TEXTURE_2D, texture);
    assert(tD.as_is == false);
    void* pBits = tD.poData;
    assert(pBits);

    int nWidth = (int)tD.width;
    int nHeight = (int)tD.height;

    // Hard to verify.. best guess.. input RGBA, output RGBA
    int nComponents = this->privGetOpenGLValue(tD.nComponent);
    GLenum eFormat = this->privGetOpenGLValue(tD.eFormat);

    GLint minFilter = (GLint)textureDataHelper::Get(tD.minFilter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)textureDataHelper::Get(tD.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)textureDataHelper::Get(tD.wrapT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)textureDataHelper::Get(tD.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)textureDataHelper::Get(tD.magFilter));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

GLenum TextureManager::privGetOpenGLValue(textureData::TEXTURE_EFORMAT eFormat) const
{
    GLenum val = 0;

    switch (eFormat)
    {
    case textureData::TEXTURE_EFORMAT::EFORMAT_BGR:
        val = GL_BGR;
        break;

    case textureData::TEXTURE_EFORMAT::EFORMAT_BGRA:
        val = GL_BGRA;
        break;

    case textureData::TEXTURE_EFORMAT::EFORMAT_RGB:
        val = GL_RGB;
        break;

    case textureData::TEXTURE_EFORMAT::EFORMAT_RGBA:
        val = GL_RGBA;
        break;

    default:
        assert(false);
    }

    return val;
}

GLint TextureManager::privGetOpenGLValue(textureData::TEXTURE_NCOMPONENT nComponent) const
{
    GLint val = 0;

    switch (nComponent)
    {
    case textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGR:
        val = GL_BGR;
        break;

    case textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGRA:
        val = GL_BGRA;
        break;

    case textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB:
        val = GL_RGB;
        break;

    case textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA:
        val = GL_RGBA;
        break;

    default:
        assert(false);
    }

    return val;
}
