#include "Mesh.h"

namespace NGN
{
	size_t Mesh::AddInstance(InstanceBuffer buffer)
	{
		return m_Pipeline.AddMeshInstance(m_RendererID, buffer);
	}

	void Mesh::UpdateInstance(size_t instanceId, InstanceBuffer buffer)
	{
		m_Pipeline.UpdateMeshInstance(m_RendererID, instanceId, buffer);
	}
}