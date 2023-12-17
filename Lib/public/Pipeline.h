#pragma once

#include "Containers/String.h"
#include "Containers/HashMap.h"
#include "Containers/VertexBuffer.h"

namespace NGN
{
	class Pipeline
	{
	public:
		Pipeline(size_t rendererID)
			: m_RendererID(rendererID)
		{}

		size_t AddMesh(const String& mesh, class Mesh& meshData);
		size_t AddMeshInstance(size_t meshID, InstanceBuffer buffer);
		void UpdateMeshInstance(size_t meshID, size_t instanceID, InstanceBuffer buffer);
		void RemoveMeshInstance(size_t meshID, size_t instanceID);

	private:
		size_t m_RendererID;
		HashMap<String, size_t> m_Meshes;
		size_t m_NextMeshID = 0;
	};
}