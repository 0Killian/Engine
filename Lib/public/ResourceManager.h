#pragma once

#include "Containers/HashMap.h"
#include "Containers/String.h"
#include "Pipeline.h"
#include "Mesh.h"

namespace NGN
{
	enum class ResourceType
	{
		Pipeline,
		Mesh
	};

	class ResourceManager
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		void AddPipeline(const String& name, String vertexSource, String pixelSource);
		void AddPipeline(const String& name, List<uint8_t> compiledVertex, List<uint8_t> compiledPixel);
		void RemovePipeline(const String& name);

		Pipeline& GetPipeline(const String& name);
		auto& GetPipelines() { return m_Pipelines; }

		void AddMesh(const String& name, VertexBuffer vertices, List<uint32_t> indices, Pipeline& pipeline);
		void RemoveMesh(const String& name);

		Mesh& GetMesh(const String& name);
		auto& GetMeshes() { return m_Meshes; }

		static ResourceManager& Get();

	private:
		HashMap<String, Pipeline> m_Pipelines;
		HashMap<String, Mesh> m_Meshes;
	};
}