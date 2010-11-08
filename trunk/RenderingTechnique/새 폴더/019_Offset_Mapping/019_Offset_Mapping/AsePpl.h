// AsePpl - Ase model with per pixel lighting
// For each triangle is computing light intensity
// according to intensity are sorted and then are rendered
// Is possible to set max lights per triangle
// draw only objects which are in view frustum (Frustum Culling)

#ifndef _ASEPPL_H
#define _ASEPPL_H

#include "AseFile.h"
#include "Util.h"

// A structure for our custom vertex type
struct zAsePpl_CUSTOMVERTEX
{
	vec v;		// vertex
	vec	n;		// normal
	vec2 t0;	// texture coord 0
	vec s;		// texture axis s in object space, s = Tangent
	vec t;		// texture axis t in object space, t = Binormal
	vec2 ladr;	// light address 0 and 1
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX_ASEPPL (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX4| D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE3(1)|D3DFVF_TEXCOORDSIZE3(2)|D3DFVF_TEXCOORDSIZE1(3))

struct zAsePpl_intensity
{
	float *intensity;		// intenzity[i] is  intenzity of lights[light_index[i]]
	int   *light_index;		// size allocated_size_of_intenzity
};

struct zAsePpl_DXobj_ppl
{
	LPDIRECT3DVERTEXBUFFER8 pvb;
	int count;						// size of vertex buffer = 3*count, count = triangle count
	int render_count;				// triangles are render
};

struct zAsePpl_DXobj
{
	LPDIRECT3DVERTEXBUFFER8 pvb;	// Buffer to hold vertices
	int have_texture_coord_0;
	int count;						// count of primitive
	int	materialID;
	zAsePpl_DXobj_ppl *ppl;			// = numOfppl
	int numOfppl;
	zAsePpl_intensity *pIntensity;	// = numOfIntensity = numOfFaces, intensity of light
	int numOfIntensity;
};

class AsePpl : public AseFile
{
public:
	void ppl_Render_ps14( int max_lights);
	void ppl_Render_ps11( int max_lights);
	int ppl_MakeVertexBuffers_ps14(int max_lights);
	void ppl_Render_( int max_lights, DWORD hPixelShader_ambient, DWORD hPixelShader, int heightMap=0, int two_lights_per_pass=0);
	void ppl_Render(int max_lights, DWORD hPixelShader_ambient, DWORD hPixelShader, int heightMap=0, int two_lights_per_pass=0);
	int ppl_MakeVertexBuffers_ps11(int max_lights);
	void ppl_Quicksort(zAsePpl_intensity *pint, int l, int r);
	int ppl_compute_light_intenzity();
	int ppl_allocate_intenzity();
	void Render(int texturing, int material, DWORD hVertexShader=D3DFVF_CUSTOMVERTEX_ASEPPL);
	int Init( char *file_name, int MipLevels=0, int rotateX90=1, float scale=1.0f, int loadHeightMap=0);
	
	int CreateDXObjects();
	void DeleteAll();

	AsePpl();
	virtual ~AsePpl();

	vector<zAsePpl_DXobj> dxobjs;
	int allocated_size_of_intenzity;
//	vector<z_Light> lights;				// for ppl
	static DWORD dwDecl[];

	uPixelShader ps11_one_light;
	uPixelShader ps11_one_light_with_ambient;
	uPixelShader ps14_one_light;
	uPixelShader ps14_one_light_with_ambient;
	uVertexShader vs11_one_light;

	static char ps11_one_light_text[];
	static char ps11_one_light_with_ambient_text[];
	static char ps14_one_light_text[];
	static char ps14_one_light_with_ambient_text[];
	static char vs11_one_light_text[];
};

#endif // _ASEPPL_H
