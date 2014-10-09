#pragma once
/*
	���̴� �ϳ��� �����Ǵ� �������� �׸��� ����Ʈ

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
	std::vector<MESH_SPEC_PAIR>	m_vecNode;
	std::map<Material*,std::list<MESH_SPEC_PAIR>>	m_materialOrder;
private:
public:
	void	Insert(cMeshNode* pItem,Material* pMaterial);
	void	Insert(cRendererQueue& renderQueue);
	void	InsertIntoMaterialOrder(cRendererQueue& renderQueue);
	void	Render();
	void	Clear();
	void	SortByCamera(cCameraNode* pCamera);
	void	SortByMaterial();

	void	RenderShadowByMaterialOrder( D3DXHANDLE hTechniqueNotAlphaTest,D3DXHANDLE hTechniqueAlphaTest );
	void	RenderNotAlphaBlendByMaterialOrder(std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderAlphaBlendByDistanceOrder(std::vector<D3DXHANDLE>& vecTechnique,cCameraNode* pCamera);
protected:
	void	ChangeMaterial(Material& material,bool bForShadow);
	void	SubRenderAlphaBlend(std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_SPEC_PAIR>& containerTemp );
};
}