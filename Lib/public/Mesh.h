#pragma once

#include "Containers/List.h"
#include "Containers/VertexBuffer.h"
#include "Pipeline.h"

namespace NGN
{
	class Mesh
	{
	public:
		Mesh(VertexBuffer vertices, List<uint32_t> indices, Pipeline& pipeline, const String& name)
			: m_Vertices(vertices)
			, m_Indices(indices)
			, m_Pipeline(pipeline)
			, m_RendererID(pipeline.AddMesh(name, *this))
		{}

		const VertexBuffer& GetVertices() const { return m_Vertices; }
		const List<uint32_t>& GetIndices() const { return m_Indices; }

		size_t AddInstance(InstanceBuffer buffer);
		void UpdateInstance(size_t instanceId, InstanceBuffer buffer);

		NGN::Mesh& operator=(const NGN::Mesh& other)
		{
			if(&other == this)
				return *this;

			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;
			m_RendererID = other.m_RendererID;
			m_Pipeline = other.m_Pipeline;

			return *this;
		}

	private:
		VertexBuffer m_Vertices;
		List<uint32_t> m_Indices;

		size_t m_RendererID;

		Pipeline& m_Pipeline;
	};
}