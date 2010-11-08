#ifndef _ASEFILE_H
#define _ASEFILE_H

#include "init.h"
#include "texture.h"
#include "Frustum.h"
#include <vector>
using namespace std;

struct z_face{	vec	a,b,c,normal;};		// for Collision Detection

struct zASE_Material
{
	int		id;
	char	name[128];
//	char	classname[128];
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;
	float	shine;
	float	shineStrength;
	float	transparency;		// I don't use this now
	float	wiresize;			// I don't use this now
//	char	shading[128];
//	float	xp_falloff;
	float	selfIllum;
//	char	falloff[128];
//	char	xp_type[128];
	int		textureId;
	int		numOfSubMaterials;
	zASE_Material *pSubMaterials;
};

struct zASE_Face
{
	unsigned short index[3];			// indicies for the verts (or the tex coords) that make up this triangle
};

struct z_Light 
{
	vec lpos;			// light position in object space
	vec color;
	float light_range;	// = light_range
	int modification;	// == 1 if lpos or light_range was change
};

struct zASE_Object
{
	char	name[128];
	vec		*pVerts;			// = numOfVerts
	vec		*pNormals;			// The object's normals == CROSS(Tangent,Binormal)
	vec		*pTangent;			// texture axis s in object space
	vec		*pBinormal;			// texture axis t in object space
	int		numOfVerts;

	vec		*pFaceNormals;		// = numOfFaces
	zASE_Face	*pFaces;		// = numOfFaces
	int		numOfFaces;

	// tex 0
	vec2	*pTexVerts;			// = numOfTexVerts
	int		numOfTexVerts;
	zASE_Face	*pFaceCoord;	// = numOfFaces
		
	// tex 1 - for light map
	vec2	*pTexVerts_;		// = numOfTexVerts_
	int		numOfTexVerts_;
	zASE_Face	*pFaceCoord_;	// = numOfFaces
		
	int		materialID;
	vec		rotmatrix[3];
	vec		min,max;			// bounding box
	int		visible;			// set on 1 if object is visible
};

class AseFile  
{
public:
	int Init(char *file_name, int MipLevels=0, int rotateX90=1, float scale=1.0f, int loadNormalMap=0, int loadHeightMap=0);

	void CheckVisibility(Frustum &f);
	void CheckVisibility(const mat4 &m){ Frustum f; f.Init(m); CheckVisibility(f);}
	void CheckVisibility(){ Frustum f; f.Init(); CheckVisibility(f);}

	void AddCollisionFaces(vector<z_face> *faces, const vec center, float radius);
	vec CheckSimpleCollision(const vec &vp, float radius);

	void Rescale(float scale=1.f);
	void RotateX90();

	void ComputeNormals(zASE_Object &obj);
	void ComputeTangentBinormal();
	void MakeBoundingBoxs();

	void DeleteObjects();
	void DeleteAll();
	
	int LoadTexturesDiffuse(int MipLevels=0);
	int LoadTexturesNormal(int MipLevels=0, int plane=0, float strong=1.0f);
	int LoadTexturesHeight(int MipLevels=0);

	AseFile();
	virtual ~AseFile();

	vector<zASE_Material>   materials;
	vector<zASE_Object>     objects;
	vector<z_Light>         static_lights;		// lights read from ase file
	vector<z_Light>         lights;

	vector<char*> textureNames;
	texture* tDiffuse;			// size = textureNames.size()
	texture* tNormal;
	texture* tHeight;

	vec min,max;	// bounding box
	int visible;	// set on 1 if object is visible
private:
	FILE *file;
	char directory[255];

	int ReadAseFile();
	int ReadGroup();
	int ReadMaterial_List();
	int ReadMaterial(int num);
	int ReadMaterialMap(zASE_Material &mat, int subMaterial=-1);
	int ReadMaterialSubMaterial(zASE_Material &mat, int subMaterial);
	int ReadGeomObject();
	int ReadNode_TM(zASE_Object &obj);
	int ReadMesh(zASE_Object &obj);
	int ReadMesh_Vertex_List(zASE_Object &obj);
	int ReadMesh_Face_List(zASE_Object &obj);
	int ReadMesh_TVertList(zASE_Object &obj);
	int ReadMesh_TFaceList(zASE_Object &obj);
	int ReadMesh_Normals(zASE_Object &obj);
	int ReadMesh_MappingChannel(zASE_Object &obj);
	int ReadMesh_TVertList_(zASE_Object &obj);
	int ReadMesh_TFaceList_(zASE_Object &obj);
	int ReadLightObject();
	int ReadLightObjectNodeTM(z_Light &light);
	int ReadLightObjectLightSettings(z_Light &light);
	int ReadUnknown(char *text);
};

#endif // _ASEFILE_H