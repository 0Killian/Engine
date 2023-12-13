#include "ResourceManager.h"
#include "Application.h"

namespace NGN
{
	void ResourceManager::AddPipeline(const String& name, String vertexSource, String pixelSource)
	{
		m_Pipelines.Insert(name, Pipeline(Renderer::Get().CreatePipeline(vertexSource, pixelSource)));
	}

	void ResourceManager::AddPipeline(const String& name, List<uint8_t> compiledVertex, List<uint8_t> compiledPixel)
	{
		m_Pipelines.Insert(name, Pipeline(Renderer::Get().CreatePipeline(compiledVertex, compiledPixel)));
	}

	void ResourceManager::RemovePipeline(const String& name)
	{
		m_Pipelines.Erase(name);
	}

	Pipeline& ResourceManager::GetPipeline(const String& name)
	{
		return m_Pipelines.Get(name);
	}

	void ResourceManager::AddMesh(const String& name, VertexBuffer vertices, List<uint32_t> indices, Pipeline& pipeline)
	{
		m_Meshes.Insert(name, Mesh(vertices, indices, pipeline, name));
	}

	void ResourceManager::RemoveMesh(const String& name)
	{
		m_Meshes.Erase(name);
	}

	Mesh& ResourceManager::GetMesh(const String& name)
	{
		return m_Meshes.Get(name);
	}

	ResourceManager& ResourceManager::Get()
	{
		return Application::Get().GetResourceManager();
	}
}