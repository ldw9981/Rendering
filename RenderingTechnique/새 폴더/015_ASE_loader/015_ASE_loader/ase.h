#ifndef _ASE_H
#define _ASE_H

// class for load ase files (ASCII scene export)

#include "init.h"
#include "texture.h"
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
	int		textureDiffuseId;
	int		numOfSubMaterials;
	zASE_Material *pSubMaterials;
};

struct zASE_Face
{
	unsigned short index[3];			// indicies for the verts (or the tex coords) that make up this triangle
};

struct zASE_Object
{
	char	name[128];
	vec		*pVerts;			// = numOfVerts
	vec		*pVertNormals;		// = numOfVerts
	int		numOfVerts;
	vec		*pFaceNormals;		// = numOfFaces
	zASE_Face	*pFaces;		// = numOfFaces
	int		numOfFaces;

	// tex 0
	vec2	*pTexVerts;			// = numOfTexVerts
	int		numOfTexVerts;
	zASE_Face	*pFaceCoord;	// = numOfFaces
	// tex 1
	vec2	*pTexVerts_;		// = numOfTexVerts_
	int		numOfTexVerts_;
	zASE_Face	*pFaceCoord_;	// = numOfFaces
	
	int		materialID;
	vec		rotmatrix[3];
};

// A structure for our custom vertex type
struct zASE_CUSTOMVERTEX
{
	vec v;		// vertex
	vec	n;		// normal
	vec2 t0;	// texture coord 0
	vec2 t1;	// texture coord 1
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX_ASE (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX2| D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))

struct zASE_DXobj
{
	LPDIRECT3DVERTEXBUFFER8 pvb;	// Buffer to hold vertices
	int have_texture_coord_0;
	int have_texture_coord_1;
	int count;						// count of primitive
	int	materialID;
};


class Case  
{
public:
	int Init( char *file_name, int MipLevels=0, int rotateX90=1, float scale=1.0f, int loadLightMap=1);
	void Render(int texturing=1, int material=1, int subMaterial=0);
	void RenderMultitexture(int texturing, int material);
	void AddCollisionFaces(vector<z_face> *faces, const vec center, float radius);
	vec CheckSimpleCollision( const vec &vp, float radius);
	void Rescale( float scale=1.f);
	void RotateX90();
private:
	int ReadAseFile();
	int ReadGroup();
	int ReadMaterial_List();
	int ReadMaterial( int num);
	int ReadMaterialMap(zASE_Material &mat, int subMaterial=-1);
	int ReadMaterialSubMaterial(zASE_Material &mat, int subMaterial);
	int ReadGeomObject();
	int ReadNode_TM( zASE_Object &obj);
	int ReadMesh( zASE_Object &obj);
	int ReadMesh_Vertex_List( zASE_Object &obj);
	int ReadMesh_Face_List( zASE_Object &obj);
	int ReadMesh_TVertList( zASE_Object &obj);
	int ReadMesh_TFaceList( zASE_Object &obj);
	int ReadMesh_Normals( zASE_Object &obj);
	int ReadMesh_MappingChannel( zASE_Object &obj);
	int ReadMesh_TVertList_( zASE_Object &obj);
	int ReadMesh_TFaceList_( zASE_Object &obj);
	int ReadUnknown( char *text);
	void ComputeNormals(zASE_Object &obj);
public:
	int LoadTextures(int MipLevels);
	void DeleteAll();
	void DeleteObjects();
	int CreateDXObjects();
	Case();
	virtual ~Case();

	vector<zASE_Material>	materials;
	vector<zASE_Object>		objects;

	vector<zASE_DXobj> dxobjs;
private:
	FILE *file;
	char directory[255];
	
	int loadLightMap;
	vector<char*> textureNames;
	texture* textures;			// size = textureNames.size()
};

#endif // _ASE_H
