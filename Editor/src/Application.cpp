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
#include "Inspector.h"
#include "Viewport.h"
#include "Resources.h"
#include "ObjectsHierarchy.h"

class Editor final : public NGN::Application
{
public:
	explicit Editor(const NGN::List<NGN::String>& args)
		: Application(args)
	{}

	void OnUpdate() override
	{		
		NGN::HashMap<NGN::String, std::shared_ptr<NGN::GUI::Window>> windows = {};

		static_cast<Viewport&>(m_GUI.GetWindow("Viewport")).Update();
		static_cast<Inspector&>(m_GUI.GetWindow("Inspector")).Update();

		NGN::GUI::Get().Render(windows);

		m_RendererViewportX = m_GUI.GetWindow("Viewport").ComputedX;
		m_RendererViewportY = m_GUI.GetWindow("Viewport").ComputedY;
		m_RendererViewportWidth = m_GUI.GetWindow("Viewport").ComputedWidth;
		m_RendererViewportHeight = m_GUI.GetWindow("Viewport").ComputedHeight;
	}
			
protected:
	void InitInner() override
	{
		// GUI
		NGN::HashMap<NGN::String, std::shared_ptr<NGN::GUI::Window>> windows;

		windows.Insert("Resources", std::make_shared<Resources>());
		windows.Insert("Objects Hierarchy", std::make_shared<ObjectsHierarchy>());
		windows.Insert("Inspector", std::make_shared<Inspector>());
		windows.Insert("Viewport", std::make_shared<Viewport>(static_cast<size_t>(m_RendererViewportWidth), static_cast<size_t>(m_RendererViewportHeight)));

		m_GUI.Init(windows);

		// Pipeline
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
			.AddElement("COLOR", NGN::VertexStructureElement::Float3, 1)
			.Build();

		// Cube
		vbuffer.AddVertex(8);

		vbuffer.SetElement(0, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(1, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(2, "POSITION", NGN::Math::Vec3<float>(-1.0f, 1.0f, -1.0f));
		vbuffer.SetElement(3, "POSITION", NGN::Math::Vec3<float>(1.0f, 1.0f, -1.0f));
		vbuffer.SetElement(4, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, 1.0f));
		vbuffer.SetElement(5, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, 1.0f));
		vbuffer.SetElement(6, "POSITION", NGN::Math::Vec3<float>(-1.0f, 1.0f, 1.0f));
		vbuffer.SetElement(7, "POSITION", NGN::Math::Vec3<float>(1.0f, 1.0f, 1.0f));

		vbuffer.SetElement(0, "COLOR", NGN::Math::Vec3<float>(1.0f, 0.0f, 0.0f));
		vbuffer.SetElement(1, "COLOR", NGN::Math::Vec3<float>(0.0f, 1.0f, 0.0f));
		vbuffer.SetElement(2, "COLOR", NGN::Math::Vec3<float>(0.0f, 0.0f, 1.0f));
		vbuffer.SetElement(3, "COLOR", NGN::Math::Vec3<float>(1.0f, 1.0f, 0.0f));
		vbuffer.SetElement(4, "COLOR", NGN::Math::Vec3<float>(1.0f, 0.0f, 1.0f));
		vbuffer.SetElement(5, "COLOR", NGN::Math::Vec3<float>(0.0f, 1.0f, 1.0f));
		vbuffer.SetElement(6, "COLOR", NGN::Math::Vec3<float>(1.0f, 1.0f, 1.0f));
		vbuffer.SetElement(7, "COLOR", NGN::Math::Vec3<float>(0.0f, 0.0f, 0.0f));

		auto ibuffer = NGN::List<uint32_t>({
			1, 0, 2, 1, 2, 3,
			4, 5, 6, 5, 7, 6,
			0, 4, 6, 0, 6, 2,
			1, 3, 7, 1, 7, 5,
			0, 1, 5, 0, 5, 4,
			2, 6, 7, 2, 7, 3
		});

		NGN::ResourceManager::Get().AddMesh("Square", vbuffer, ibuffer, pipeline);

		vbuffer = NGN::VertexBufferBuilder()
			.AddElement("POSITION", NGN::VertexStructureElement::Float3, 1)
			.AddElement("COLOR", NGN::VertexStructureElement::Float3, 1)
			.Build();

		// Pyramid
		vbuffer.AddVertex(5);

		vbuffer.SetElement(0, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(1, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(2, "POSITION", NGN::Math::Vec3<float>(0.0f, 1.0f, 0.0f));
		vbuffer.SetElement(3, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, 1.0f));
		vbuffer.SetElement(4, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, 1.0f));

		vbuffer.SetElement(0, "COLOR", NGN::Math::Vec3<float>(1.0f, 0.0f, 0.0f));
		vbuffer.SetElement(1, "COLOR", NGN::Math::Vec3<float>(0.0f, 1.0f, 0.0f));
		vbuffer.SetElement(2, "COLOR", NGN::Math::Vec3<float>(0.0f, 0.0f, 1.0f));
		vbuffer.SetElement(3, "COLOR", NGN::Math::Vec3<float>(1.0f, 1.0f, 0.0f));
		vbuffer.SetElement(4, "COLOR", NGN::Math::Vec3<float>(1.0f, 0.0f, 1.0f));

		ibuffer = NGN::List<uint32_t>({
			0, 1, 2,
			1, 4, 2,
			4, 3, 2,
			3, 0, 2,
			0, 4, 1,
			3, 1, 0
		});

		NGN::ResourceManager::Get().AddMesh("Pyramid", vbuffer, ibuffer, pipeline);

		NGN::InstanceBuffer buffer = NGN::InstanceBufferBuilder()
			.AddElement("COLOR", NGN::VertexStructureElement::Float3, 1)
			.Build();

		entt::entity entity1 = m_Registry.create();
		m_Registry.get<NGN::Component::Tag>(entity1).SetName("Cube1");
		m_Registry.emplace<NGN::Component::Mesh>(entity1, "Square", buffer, entity1);

		entt::entity entity2 = m_Registry.create();
		m_Registry.get<NGN::Component::Transform>(entity2).SetPosition(NGN::Math::Vec3<float>(0.0f, -0.5f, 0.0f));
		m_Registry.get<NGN::Component::Transform>(entity2).SetRotation(NGN::Math::Vec3<float>(0.0f,  0.0f, 44.0f));
		m_Registry.get<NGN::Component::Tag>(entity2).SetName("Cube2");
		m_Registry.emplace<NGN::Component::Mesh>(entity2, "Square", buffer, entity2);

		m_Registry.get<NGN::Component::Transform>(entity2).SetParent(entity1);

		NGN::Renderer::Get().SetRenderMode(NGN::RenderMode::ToTexture);
	}
};

NGN_ENTRY_POINT(Editor)