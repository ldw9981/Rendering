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
class cRendererQueue
{
public:
	cRendererQueue();
	~cRendererQueue();

	struct Specific 
	{		
		Material* pMaterial;
		float distancesq;
	};
	typedef std::pair<cMeshNode*,Specific> MESH_SPEC_PAIR;
	typedef std::pair<cMeshNode*,Material*> MESH_MATERIAL_PAIR;
	std::vector<MESH_SPEC_PAIR>	m_vecNode;
	std::map<Material*,std::list<MESH_SPEC_PAIR>>	m_materialOrder;
	std::map<std::string,std::list<MESH_SPEC_PAIR>>	m_entityMeshNameOrder;	// key최적화 필요
private:
public:
	void	Insert(cMeshNode* pItem,Material* pMaterial);

	void	Insert(cRendererQueue& renderQueue);
	void	InsertIntoMaterialOrder(cRendererQueue& renderQueue);
	void	InsertIntoEntityMeshNameOrder( std::string& strEntityName,cRendererQueue& renderQueue);
	
	void	Render();
	void	Clear();
	void	SortByCamera(cCameraNode* pCamera);
	void	SortByMaterial();

	void	RenderShadowByMaterialOrder( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest );
	void	RenderNotAlphaBlendByMaterialOrder(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderAlphaBlendByDistanceOrder(std::vector<D3DXHANDLE>& vecTechnique,cCameraNode* pCamera);
	void	RenderInstancing(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderShadowInstancing(D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
protected:
	void	ChangeMaterial(Material& material,bool bForShadow);
	void	SubRenderAlphaBlend(std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_SPEC_PAIR>& containerTemp );
};
}