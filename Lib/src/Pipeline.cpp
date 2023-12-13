#include "Pipeline.h"
#include "ResourceManager.h"
#include "Containers/String.h"
#include "Renderer.h"

namespace NGN
{
	size_t Pipeline::AddMesh(const String& mesh, Mesh& meshData)
	{
		if(m_Meshes.ContainsKey(mesh))
			throw std::runtime_error("Mesh already exists in pipeline");

		m_Meshes[mesh] = m_NextMeshID;

		Renderer::Get().UpdateMeshInPipeline(m_RendererID, m_NextMeshID, mesh, meshData);

		return m_NextMeshID++;
	}

	size_t Pipeline::AddMeshInstance(size_t meshID, InstanceBuffer buffer)
	{
		return Renderer::Get().AddMeshInstanceInPipeline(m_RendererID, meshID, buffer);
	}

	void Pipeline::UpdateMeshInstance(size_t meshID, size_t instanceID, InstanceBuffer buffer)
	{
		Renderer::Get().UpdateMeshInstanceInPipeline(m_RendererID, meshID, instanceID, buffer);
	}
}