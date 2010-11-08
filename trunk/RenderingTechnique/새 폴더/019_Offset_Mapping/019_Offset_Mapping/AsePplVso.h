// AsePplVso - ase model with per pixel lighting and optimalization with vertex shader
// - change those triangles which are out from light range to triangle with zero surface with vertex shader
// - draw only objects which are in view frustum (Frustum Culling)
// - draw in order from camera position 

#ifndef _ASEPPLVSO_H
#define _ASEPPLVSO_H

#include "AseFile.h"
#include "Util.h"

// A structure for our custom vertex type
struct zAsePplVso_CUSTOMVERTEX
{
	vec v;		// vertex
	vec	n;		// normal
	vec2 t0;	// texture coord 0
	vec s;		// texture axis s in object space, s = Tangent
	vec t;		// texture axis t in object space, t = Binormal
	vec4 ct;	// ct.xyz, center of triangle, ct.w radius
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX_ASEPPLVSO (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_TEX4| D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE3(1)|D3DFVF_TEXCOORDSIZE3(2)|D3DFVF_TEXCOORDSIZE4(3))

struct zAsePplVso_DXobj
{
	LPDIRECT3DVERTEXBUFFER8 pvb;	// Buffer to hold vertices
	int have_texture_coord_0;
	int count;						// count of primitive
	int	materialID;
};

class AsePplVso : public AseFile
{
public:
	void ppl_Render_light(int index, int heightMap);
	void ppl_Quicksort(const vec &position, int *light_index, int l, int r);
	void ppl_Render( DWORD hVertexShader_ambient, DWORD hPixelShader_ambient, DWORD hVertexShader, DWORD hPixelShader, int heightMap, const vec &position, int max_lights);
	void Render_pixel_shader(int heightMap);
	void Render(int texturing, int material, DWORD hVertexShader=D3DFVF_CUSTOMVERTEX_ASEPPLVSO);
	int Init( char *file_name, int MipLevels=0, int rotateX90=1, float scale=1.0f, int loadHeightMap=0);
	
	int CreateDXObjects();
	void DeleteAll();

	AsePplVso();
	virtual ~AsePplVso();

	vector<zAsePplVso_DXobj> dxobjs;
//	vector<z_Light> lights;				// for ppl
	static DWORD dwDecl[];
};

#endif // _ASEPPLVSO_H
