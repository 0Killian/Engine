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

class Editor final : public NGN::Application
{
public:
	explicit Editor(const NGN::List<NGN::String>& args)
		: Application(args)
	{
	}

	bool DrawHierarchyNode(entt::entity entity)
	{
		bool result = false;

		m_GUI.PushID(static_cast<int>(entity));

		NGN::List<entt::entity> children;

		for (auto&& [child, transform] : m_Registry.view<NGN::Component::Transform>().each())
		{
			auto parent = transform.GetParent();

			if (parent == entity)
			{
				children.PushBack(child);
			}
		}

		bool nodeOpen = m_GUI.TreeNode(m_Registry.get<NGN::Component::Tag>(entity).Name.GetData(), children.Size() == 0 ? NGN::GUI::Leaf : static_cast<NGN::GUI::TreeNodeFlags>(0));

		if (m_GUI.IsItemClicked())
		{
			m_SelectedEntity = entity;
			result = true;
		}

		if (nodeOpen)
		{
			for (auto& child : children)
			{
				if (DrawHierarchyNode(child))
				{
					result = true;
				}
			}

			m_GUI.TreePop();
		}

		m_GUI.PopID();

		return result;
	}

	void OnUpdate() override
	{
		m_GUI.StartWindow("Objects Hierarchy");

		for (auto entity : m_Registry.view<NGN::Component::Transform, NGN::Component::Tag>())
		{
			if (!m_Registry.get<NGN::Component::Transform>(entity).GetParent())
				DrawHierarchyNode(entity);
		}

		m_GUI.EndWindow();

		if (m_SelectedEntity)
		{
			m_GUI.StartWindow("Inspector");

			auto& transform = m_Registry.get<NGN::Component::Transform>(*m_SelectedEntity);
			auto pos = transform.GetPosition();
			auto rot = transform.GetRotation();
			auto scale = transform.GetScale();

			m_GUI.Section("Transform");
			bool changed = m_GUI.Vec3("Position", pos);
			changed |= m_GUI.Vec3("Rotation", rot);
			changed |= m_GUI.Vec3("Scale", scale);

			if (changed)
			{
				transform.SetPosition(pos);
				transform.SetRotation(rot);
				transform.SetScale(scale);
			}

			m_GUI.EndWindow();
		}
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
		vbuffer.SetElement(1, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, -1.0f));
		vbuffer.SetElement(2, "POSITION", NGN::Math::Vec3<float>(-1.0f, 1.0f, -1.0f));
		vbuffer.SetElement(3, "POSITION", NGN::Math::Vec3<float>(1.0f, 1.0f, -1.0f));
		vbuffer.SetElement(4, "POSITION", NGN::Math::Vec3<float>(-1.0f, -1.0f, 1.0f));
		vbuffer.SetElement(5, "POSITION", NGN::Math::Vec3<float>(1.0f, -1.0f, 1.0f));
		vbuffer.SetElement(6, "POSITION", NGN::Math::Vec3<float>(-1.0f, 1.0f, 1.0f));
		vbuffer.SetElement(7, "POSITION", NGN::Math::Vec3<float>(1.0f, 1.0f, 1.0f));

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
		m_Registry.emplace<NGN::Component::Tag>(entity1, "Cube");

		entt::entity entity2 = m_Registry.create();
		m_Registry.emplace<NGN::Component::Transform>(entity2,
			NGN::Math::Vec3<float>(0.0f, -0.5f, 0.0f),
			NGN::Math::Vec3<float>(0.0f, 0.0f, 45.0f),
			NGN::Math::Vec3<float>(1.0f, 1.0f, 1.0f));
		m_Registry.emplace<NGN::Component::Mesh>(entity2, "Square");
		m_Registry.emplace<NGN::Component::Tag>(entity2, "Cube2");

		m_Registry.get<NGN::Component::Transform>(entity2).SetParent(entity1);
	}

	std::optional<entt::entity> m_SelectedEntity;
};

NGN_ENTRY_POINT(Editor)