#pragma once
/*
	쉐이더 하나와 대응되는 렌더러를 그리는 리스트

*/
#include "Foundation/Interface.h"
namespace Sophia
{

class IRenderer;
class Material;
class MultiSub;
class cCameraNode;
class cMeshNode;
class cRscVertexBuffer;
class cRscIndexBuffer;

struct SCENE_KEY
{
	cRscVertexBuffer*	pVertexBuffer;
	Material*			pMaterial;
	cRscIndexBuffer*	pIndexBuffer;	

	SCENE_KEY();
	SCENE_KEY(cRscVertexBuffer* param0,Material* param1,cRscIndexBuffer* param2);
	bool operator<(const SCENE_KEY& other) const;
};

class cRendererQueue
{
public:
	cRendererQueue();
	~cRendererQueue();

	typedef std::pair<cMeshNode*,Material*> MESH_MATERIAL_PAIR;
	typedef std::pair<cMeshNode*,float>		MESH_DISTANCE_PAIR;
	std::vector<cMeshNode*>	m_vecMesh;

	std::vector<MESH_DISTANCE_PAIR>					m_distanceOrder;
	std::map<Material*,std::list<cMeshNode*>>		m_materialOrder;	// Material같고 index,vertex다를수있다.
	std::map<SCENE_KEY,std::list<cMeshNode*>>		m_sceneOrder;		// Material,index,vertex모두같다.
private:
public:
	void	InsertIntoMeshList(cMeshNode* pItem);

	void	InsertIntoMaterialOrder(cRendererQueue& renderQueue);
	void	InsertIntoSceneOrder(cRendererQueue& renderQueue);

	void	InsertNotAlphaBlend(cRendererQueue& renderQueue); 
	void	InsertIntoDistanceOrder(cRendererQueue& renderQueue,D3DXVECTOR3* pCameraWorldPosition );
	

	void	Clear();	

	void	RenderShadowByMaterialOrder( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest );
	void	RenderShadowNormalInstancing(D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
	void	RenderShadowSkinnedInstancing(D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
	
	void	RenderSceneNotAlphaBlendByMaterialOrder(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderSceneNormalInstancing(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderSceneSkinnedInstancing(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderSceneAlphaBlendByDistanceOrder(std::vector<D3DXHANDLE>& vecTechnique);

protected:
	void	ChangeMaterial(Material* pMaterial);
	void	ChangeMaterialForShadow(Material* pMaterial);
	void	SubRenderAlphaBlend(std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_DISTANCE_PAIR>& containerTemp );
};
}