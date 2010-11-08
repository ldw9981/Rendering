#ifndef _3DS_H
#define _3DS_H

//------ Primary chunk

#define MAIN3DS       0x4D4D

//------ Main Chunks

#define EDIT3DS       0x3D3D  // this is the start of the editor config
#define KEYF3DS       0xB000  // this is the start of the keyframer config
#define VERSION       0x0002

//------ sub defines of EDIT3DS
#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100
#define EDIT_OBJECT   0x4000

#define EDIT_UNKNW01  0x1100
#define EDIT_UNKNW02  0x1201
#define EDIT_UNKNW03  0x1300
#define EDIT_UNKNW04  0x1400
#define EDIT_UNKNW05  0x1420
#define EDIT_UNKNW06  0x1450
#define EDIT_UNKNW07  0x1500
#define EDIT_UNKNW08  0x2200
#define EDIT_UNKNW09  0x2201
#define EDIT_UNKNW10  0x2210
#define EDIT_UNKNW11  0x2300
#define EDIT_UNKNW12  0x2302
#define EDIT_UNKNW13  0x3000
#define EDIT_UNKNW14  0xAFFF

//------ sub defines of EDIT_OBJECT
#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

#define OBJ_UNKNWN01  0x4010
#define OBJ_UNKNWN02  0x4012 //---- Could be shadow

//------ sub defines of OBJ_CAMERA
#define CAM_UNKNWN01  0x4710
#define CAM_UNKNWN02  0x4720

//------ sub defines of OBJ_LIGHT
#define LIT_OFF       0x4620
#define LIT_SPOT      0x4610
#define LIT_UNKNWN01  0x465A

//------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXL   0x4110
#define TRI_FACEL2    0x4111
#define TRI_FACEL1    0x4120
#define TRI_SMOOTH    0x4150
#define TRI_LOCAL     0x4160
#define TRI_VISIBLE   0x4165

//------ sub defs of KEYF3DS

#define KEYF_UNKNWN01 0xB009
#define KEYF_UNKNWN02 0xB00A
#define KEYF_FRAMES   0xB008
#define KEYF_OBJDES   0xB002

//------  these define the different color chunk types
#define COL_RGB  0x0010
#define COL_TRU  0x0011
#define COL_UNK  0x0013

//------ defines for viewport chunks

#define TOP           0x0001
#define BOTTOM        0x0002
#define LEFT          0x0003
#define RIGHT         0x0004
#define FRONT         0x0005
#define BACK          0x0006
#define USER          0x0007
#define CAMERA        0x0008 // 0xFFFF is the actual code read from file
#define LIGHT         0x0009
#define DISABLED      0x0010
#define BOGUS         0x0011

#include "init.h"				// zakladne nastavenia a zakladne hlavickove subory
#include "texture.h"
#include "Matrix.h"
#include <vector>
using namespace std;

// This holds the chunk info
struct z3ds_Chunk
{
	unsigned short int ID;					// The chunk's ID		
	unsigned int length;					// The length of the chunk
	unsigned int bytesRead;					// The amount of bytes read within that chunk
};

// This is our face structure.  This is is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct z3ds_Face
{
	unsigned short vertIndex[3];			// indicies for the verts that make up this triangle
//	unsigned short coordIndex[3];			// indicies for the tex coords to texture this face
};

struct z3ds_FaceShadow
{
	int	neighbourIndices[3];
	vec normal;
	float delta;							// normal+delta - plane
	char visible;
};
// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
struct z3ds_MaterialInfo
{
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	vec4  ambient;				// The ambient color of the object
	vec4  diffuse;				// The diffuse color of the object
	vec4  specular;				// The specular color of the object
	float shininess;			// The shininess - exponent of material
	vec4  emission;				// The emission color of the object
	int   textureId;			// the texture ID
	int   textureNormalmapId;	// the texture Normalmap ID
	float uTile;				// u tiling of texture  (Currently not used)
	float vTile;				// v tiling of texture	(Currently not used)
	float uOffset;			    // u offset of texture	(Currently not used)
	float vOffset;				// v offset of texture	(Currently not used)
} ;
struct z3ds_Line
{
	unsigned short a,b;
};
// struct tLineLoop{vector<unsigned short> i;};
// This holds all the information for our model/scene. 
struct z3ds_Object 
{
	unsigned int  numOfVerts;			// The number of verts in the model
	unsigned int  numOfFaces;			// The number of faces in the model
	unsigned int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	vec  *pVerts;			// The object's vertices
	vec  *pNormals;			// The object's normals == CROSS(Tangent,Binormal)
	vec  *pTangent;			// texture axis s in object space
	vec  *pBinormal;		// texture axis t in object space
	vec2 *pTexVerts;		// The texture's UV coordinates
	z3ds_Face    *pFaces;			// The faces information of the object
	z3ds_FaceShadow *pFacesShadow;	// The faces information for shadow
	vector<z3ds_Line> silhouette;	//
//	vector<tLineLoop> silLoop;
};

// A structure for our custom vertex type
struct z3ds_CUSTOMVERTEX
{
	vec v;		// vertex
	vec	n;		// normal
	vec2 t0;	// texture coord 0
	vec s;		// texture axis s in object space, s = Tangent
	vec t;		// texture axis t in object space, t = Binormal
};

// Our custom FVF, which describes our custom vertex structure
//#define D3DFVF_CUSTOMVERTEX_3ds (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX1| D3DFVF_TEXCOORDSIZE2(0))
#define D3DFVF_CUSTOMVERTEX_3ds (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX3| D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE3(1)|D3DFVF_TEXCOORDSIZE3(2))

struct z3ds_DXobj
{
	LPDIRECT3DVERTEXBUFFER8 pvb;	// Buffer to hold vertices
	int have_texture_coord_0;
	int count;						// count of primitive
	int	materialID;
};

class C3ds  
{
public:
	int Init(char *file_name, int filter=3, int faceNormal=0, int shadowObject=0, int texture=1, int compute_normals=1);
	int Init_CreateDXObjects();
	void Render_dot3( int mode=3, DWORD hVertexShader=D3DFVF_CUSTOMVERTEX_3ds);
	void Render(int texturing=1, int material=1, DWORD hVertexShader=D3DFVF_CUSTOMVERTEX_3ds);
	void Render_line(int texturing=1, int material=1);
	void Render_normals( float size=1.f);
private:
	void ReadChunk(z3ds_Chunk *pChunk);
	void ReadNextChunk( z3ds_Chunk *old);
	void ReadEdit( z3ds_Chunk* old);
	void ReadEditMaterial(z3ds_Chunk *old);
	void ReadEditMaterialTexture(z3ds_Chunk *old, z3ds_MaterialInfo &mat);
	void ReadEditObject(z3ds_Chunk *old);
	void ReadEditObjectTriMesh(z3ds_Chunk *old, z3ds_Object &obj);
	void ReadEditObjectTriMeshVertex(z3ds_Chunk *old, z3ds_Object &obj);
	void ReadEditObjectTriMeshFace(z3ds_Chunk *old, z3ds_Object &obj);
	void ReadEditObjectTriMeshMaterial(z3ds_Chunk *old, z3ds_Object &obj);
	void ReadEditObjectTriMeshUV(z3ds_Chunk *old, z3ds_Object &obj);
	int GetString(char *pBuffer);
public:
	void ModifyBox(const vec &v);
	void ComputeNormals();
	void ComputeTangentBinormal();
	void Transform(mat4 m);
	void Render_pixel_shader(DWORD hVertexShader);
	void Render_dot3_specular_cube_normalizer(int mode, DWORD hVertexShader, int specExp, vec4 specCol);
	void Render_dot3_specular(int mode, DWORD hVertexShader, int specExp, vec4 specCol);
	void Render_ambient(vec4 ambient);
	int LoadTextures(char *file_name, int filter, int dot3bump);
	void DeleteObjects();
	void Rescale(float scale);
	void RotateX90();
	void MakeSilhouette();
	void RenderShadowSilhouette();
	void RenderShadowVolumes( const vec &lg, int z_fail=0);
	void SetVisibility( const vec &lg);
	void SetConnectivity();
//	C3ds( char *file_name, int filter=2, int faceNormal=0, int shadowObject=0, int dot3bump=0);
	C3ds();
	virtual ~C3ds();

	texture *textures;
	vector<z3ds_MaterialInfo> materials;	// The list of material information (Textures and colors)
	vector<z3ds_Object> objects;			// The object list for our model
	char isShadowObject;				// ==0 if used pFace, == 1 if used pFaceShadow
	char isFaceNormal;

	vector<z3ds_DXobj> dxobjs;
	vec min,max;
private:
	FILE *file;
};

#endif // _3DS_H