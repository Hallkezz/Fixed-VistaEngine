#pragma once

class cTexLibrary;
class cObjectNode;
class cObjectNodeRoot;
class cAllMeshBank;

class cObjLibrary : public cUnknownClass
{
public:
	cObjLibrary();
	~cObjLibrary();
	
	virtual void Free(FILE* f=NULL);
	virtual void Compact(FILE* f=NULL);
	virtual cObjectNodeRoot* GetElement(const char* pFileName,const char* pTexturePath);

	MTSection* GetLock(){return &lock;}
private:
	typedef vector<cAllMeshBank*> OBJECTS;
	OBJECTS objects;
	cAllMeshBank* LoadM3D(char *fname,char *TexturePath,char *DefTexturePath,bool enable_error_not_found);
	inline int GetNumberObj()									{ return objects.size(); }
	inline cAllMeshBank* GetObj(int number)						{ return objects[number]; }

	cObjectNodeRoot* LoadLod(char *fname,char *TexturePath);
	cObjectNodeRoot* GetElementInternal(const char* pFileName,const char* pTexturePath,bool enable_error_not_found);

	void FreeOne(FILE* f);
	MTSection lock;
};
