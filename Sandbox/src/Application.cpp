//
// Created by Killian on 27/11/2023.
//

#include <optional>
#include <Window.h>

#include <utility>

#include "Engine.h"
#include "Renderer.h"

#include <fstream>
#include <Components.h>

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
    {
    }

    void OnUpdate() override
    {
		m_GUI.SampleWindow();
    }

protected:
	void InitInner() override
	{
		std::ifstream vertexShaderFile("Assets/Shaders/Main.vert.hlsl", std::ios::ate);
		std::ifstream pixelShaderFile("Assets/Shaders/Main.pixel.hlsl", std::ios::ate);

		if (!vertexShaderFile.is_open() || !pixelShaderFile.is_open())
		{
			throw std::runtime_error("Failed to open shader files");
			return;
		}

		// Read all the file into a string
		size_t vertexShaderFileSize = (size_t)vertexShaderFile.tellg();
		size_t pixelShaderFileSize = (size_t)pixelShaderFile.tellg();

		vertexShaderFile.seekg(0);
		pixelShaderFile.seekg(0);

		NGN::String vertexShaderSource;
		NGN::String pixelShaderSource;
		vertexShaderSource.Resize(vertexShaderFileSize);
		pixelShaderSource.Resize(pixelShaderFileSize);

		vertexShaderFile.read(vertexShaderSource.GetData(), vertexShaderFileSize);
		pixelShaderFile.read(pixelShaderSource.GetData(), pixelShaderFileSize);

		vertexShaderFile.close();
		pixelShaderFile.close();

		NGN::ResourceManager::Get().AddPipeline("Main", vertexShaderSource, pixelShaderSource);
		NGN::Pipeline& pipeline = NGN::ResourceManager::Get().GetPipeline("Main");

		auto vbuffer = NGN::VertexBufferBuilder()
			.AddElement("POSITION", NGN::VertexStructureElement::Float3, 1)
			.Build();

		// Cube
		vbuffer.AddVertex(8);
		
		vbuffer.SetElement(0, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(1, "POSITION", NGN::Math::Vec3<float>( 1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(2, "POSITION", NGN::Math::Vec3<float>(-1.0f,  1.0f, -1.0f));
		vbuffer.SetElement(3, "POSITION", NGN::Math::Vec3<float>( 1.0f,  1.0f, -1.0f));
		vbuffer.SetElement(4, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f,  1.0f));
		vbuffer.SetElement(5, "POSITION", NGN::Math::Vec3<float>( 1.0f, -1.0f,  1.0f));
		vbuffer.SetElement(6, "POSITION", NGN::Math::Vec3<float>(-1.0f,  1.0f,  1.0f));
		vbuffer.SetElement(7, "POSITION", NGN::Math::Vec3<float>( 1.0f,  1.0f,  1.0f));

		auto ibuffer = NGN::List<uint32_t>({
			1, 0, 2, 1, 2, 3,
			4, 5, 6, 5, 7, 6,
			0, 4, 6, 0, 6, 2,
			1, 3, 7, 1, 7, 5,
			0, 1, 5, 0, 5, 4,
			2, 6, 7, 2, 7, 3
		});

		NGN::ResourceManager::Get().AddMesh("Square", vbuffer, ibuffer, pipeline);

		entt::entity entity1 = m_Registry.create();
		m_Registry.emplace<NGN::Component::Transform>(entity1);
		m_Registry.emplace<NGN::Component::Mesh>(entity1, "Square");

		/*entt::entity entity2 = m_Registry.create();
		m_Registry.emplace<NGN::Component::Transform>(entity2,
			NGN::Math::Vec3<float>(0.0f, -0.5f, 0.0f),
			NGN::Math::Vec3<float>(0.0f, 0.0f, 45.0f),
			NGN::Math::Vec3<float>(1.0f, 1.0f, 1.0f));
		m_Registry.emplace<NGN::Component::Mesh>(entity2, "Square");*/
	}
};

NGN_ENTRY_POINT(Sandbox)