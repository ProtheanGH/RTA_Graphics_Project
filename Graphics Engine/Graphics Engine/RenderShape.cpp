#include "RenderShape.h"

#include "ConstantBufferManager.h"
#include "Renderer.h"
#include "VertexFormats.h"
#include "VertexTypes.h"

// ===== Constructor / Destructor ===== //
RenderShape::RenderShape()
{
	m_Func = DefaultShape_RenderProcess;
	m_TopologyType = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

RenderShape::~RenderShape()
{

}
// ==================================== //

// === Private Interface === //
void RenderShape::DefaultShape_RenderProcess(RenderNode& _node)
{
	RenderShape& rShape = (RenderShape&)_node;

	// === Does the Object have a valid Mesh?
	if (rShape.m_pObject->GetMesh() != nullptr) {
		// == Update the Constant Buffer
		ToShaderObject toShaderObj;
		toShaderObj.ObjectWorldMatrix = rShape.m_pObject->GetWorld();
		ConstantBufferManager::GetInstance()->ApplyObjectBuffer(&toShaderObj);

		// == Set the Topology 
		Renderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(rShape.m_TopologyType);

		// == Set the Vertex Buffer
		UINT strides[] = { sizeof(Vertex_POSNORMUV) };
		UINT offsets[] = { 0 };
		ID3D11Buffer* vBuffer = rShape.m_pObject->GetVertexBuffer();
		Renderer::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vBuffer, strides, offsets);

		// == Set the Index Buffer
		Renderer::GetInstance()->GetDeviceContext()->IASetIndexBuffer(rShape.m_pObject->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// == Draw
		unsigned int size = (unsigned int)rShape.m_pObject->GetMesh()->GetIndices().size();
		Renderer::GetInstance()->GetDeviceContext()->DrawIndexed((UINT)rShape.m_pObject->GetMesh()->GetIndices().size(), 0, 0);
	}
}
// ========================= //
