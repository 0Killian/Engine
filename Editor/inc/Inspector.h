#pragma once

#include "GUI.h"
#include "GuiComponent\Section.h"
#include "GuiComponent\Vec3.h"
#include "GuiComponent\Button.h"
#include "GuiComponent\InputText.h"
#include "Components.h"
#include "Application.h"
#include "entt.h"

class Inspector : public NGN::GUI::Window
{
public:
	Inspector()
		: Window({
			.Open = false,
			.Components = {
				std::make_shared<NGN::GuiComponent::Section>(NGN::GuiComponent::Section::Parameters {
					.Name = "Tag",
					.Open = false,
					.Children = {
						std::make_shared<NGN::GuiComponent::InputText>(NGN::GuiComponent::InputText::Parameters {
							.Name = "EntityName",
							.Value = "Entity",
							.OnChange = [this](const NGN::String& text) {
								NGN::Application::Get().GetRegistry().get<NGN::Component::Tag>(m_SelectedEntity.value()).SetName(text);
							}
						}),
					}
				}),
				std::make_shared<NGN::GuiComponent::Section>(NGN::GuiComponent::Section::Parameters {
					.Name = "Transform",
					.Open = false,
					.Children = {
						std::make_shared<NGN::GuiComponent::Vec3>(NGN::GuiComponent::Vec3::Parameters {
							.Name = "Position",
							.Value = m_Position,
							.OnChange = [&](const NGN::Math::Vec3<float>& value) {
								NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(m_SelectedEntity.value()).SetPosition(value);
							}
						}),
						std::make_shared<NGN::GuiComponent::Vec3>(NGN::GuiComponent::Vec3::Parameters {
							.Name = "Rotation",
							.Value = m_Rotation,
							.OnChange = [&](const NGN::Math::Vec3<float>& value) {
								NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(m_SelectedEntity.value()).SetRotation(value);
							}
						}),
						std::make_shared<NGN::GuiComponent::Vec3>(NGN::GuiComponent::Vec3::Parameters {
							.Name = "Scale",
							.Value = m_Scale,
							.OnChange = [&](const NGN::Math::Vec3<float>& value) {
								NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(m_SelectedEntity.value()).SetScale(value);
							}
						}),
					}
				}),
				std::make_shared<NGN::GuiComponent::Section>(NGN::GuiComponent::Section::Parameters {
					.Name = "Mesh",
					.Open = false,
					.Children = {
						std::make_shared<NGN::GuiComponent::Button>(NGN::GuiComponent::Button::Parameters {
							.Name = "Mesh",
							.DragDropTarget = true,
							.OnDragDrop = [this](auto type, std::any data) {
								if (type == "Mesh")
								{
									NGN::String str = std::any_cast<NGN::String>(data);
									auto& mesh = NGN::Application::Get().GetRegistry().get<NGN::Component::Mesh>(m_SelectedEntity.value());
									NGN::InstanceBuffer buffer = mesh.GetBuffer();
									NGN::Application::Get().GetRegistry().replace<NGN::Component::Mesh>(m_SelectedEntity.value(), str, buffer, m_SelectedEntity.value());
									
									m_MeshBufferLayout.Clear();
									NGN::GuiComponent::Section& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[2]);
									for(size_t i = section.m_Children.Size() - 1; i > 0; i--)
									{
										section.m_Children.Erase(i);
									}
								}
							},
						}),
					}
				}),
			},
			.DockSide = NGN::GUI::Side::Right,
		})
	{}

	void SetSelectedEntity(std::optional<entt::entity> entity)
	{
		m_SelectedEntity = entity;
		if (m_SelectedEntity)
			Open = true;
		else
			Open = false;

		m_MeshBufferLayout.Clear();
		NGN::GuiComponent::Section& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[2]);
		for (size_t i = section.m_Children.Size() - 1; i > 0; i--)
		{
			section.m_Children.Erase(i);
		}
	}
	std::optional<entt::entity> GetSelectedEntity() { return m_SelectedEntity; }

	void Update()
	{
		if(m_SelectedEntity.has_value())
		{
			if(NGN::Application::Get().GetRegistry().all_of<NGN::Component::Tag>(m_SelectedEntity.value()))
			{
				auto& tag = NGN::Application::Get().GetRegistry().get<NGN::Component::Tag>(m_SelectedEntity.value());
				NGN::GuiComponent::Section& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[0]);
				NGN::GuiComponent::InputText& inputText = *std::dynamic_pointer_cast<NGN::GuiComponent::InputText>(section.m_Children[0]);
				section.m_Open = true;
				inputText.m_Value = tag.GetName();
			}

			if (NGN::Application::Get().GetRegistry().all_of<NGN::Component::Transform>(m_SelectedEntity.value()))
			{
				auto& transform = NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(m_SelectedEntity.value());
				m_Position = transform.GetPosition();
				m_Rotation = transform.GetRotation();
				m_Scale = transform.GetScale();

				Components[1]->m_Open = true;
			}

			if (NGN::Application::Get().GetRegistry().all_of<NGN::Component::Mesh>(m_SelectedEntity.value()))
			{
				auto& mesh = NGN::Application::Get().GetRegistry().get<NGN::Component::Mesh>(m_SelectedEntity.value());
			
				NGN::GuiComponent::Section& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[2]);
				NGN::GuiComponent::Button& button = *std::dynamic_pointer_cast<NGN::GuiComponent::Button>(section.m_Children[0]);
				section.m_Open = true;
				button.m_Name = mesh.MeshName;

				const NGN::InstanceBuffer& instanceBuffer = mesh.GetBuffer();
				auto& layout = instanceBuffer.GetStructure();

				for (auto& [name, element] : layout)
				{
					if (m_MeshBufferLayout.ContainsKey(name))
					{
						UpdateBufferElement(name, element, instanceBuffer);
					}
					else
					{
						AddBufferElement(name, element, instanceBuffer);
					}
				}
			}
		}
	}

	void UpdateBufferElement(const NGN::String& name, const NGN::VertexStructureElement& element, const NGN::InstanceBuffer& buffer)
	{
		auto& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[2]);
		std::shared_ptr<NGN::GUI::Component> component = section.m_Children[m_MeshBufferLayout.Get(name).Index];

		switch (element.m_Type)
		{
		case NGN::VertexStructureElement::Bool:
		case NGN::VertexStructureElement::Int:
		case NGN::VertexStructureElement::Int2:
		case NGN::VertexStructureElement::Int3:
		case NGN::VertexStructureElement::Int4:
		case NGN::VertexStructureElement::Float:
		case NGN::VertexStructureElement::Float2:
		case NGN::VertexStructureElement::Float3:
		case NGN::VertexStructureElement::Mat4Column:
			break;

		case NGN::VertexStructureElement::Float4:
			m_MeshBufferLayout.Get(name).Vec3f = buffer.GetElement<NGN::Math::Vec3<float>>(name);
			break;
		}
	}

	void AddBufferElement(const NGN::String& name, const NGN::VertexStructureElement& element, const NGN::InstanceBuffer& buffer)
	{
		auto& section = *std::dynamic_pointer_cast<NGN::GuiComponent::Section>(Components[2]);

		switch (element.m_Type)
		{
		case NGN::VertexStructureElement::Bool:
		case NGN::VertexStructureElement::Int:
		case NGN::VertexStructureElement::Int2:
		case NGN::VertexStructureElement::Int3:
		case NGN::VertexStructureElement::Int4:
		case NGN::VertexStructureElement::Float:
		case NGN::VertexStructureElement::Float2:
		case NGN::VertexStructureElement::Float3:
		case NGN::VertexStructureElement::Mat4Column:
			break;

		case NGN::VertexStructureElement::Float4:
			m_MeshBufferLayout.Insert(name, { .Index = section.m_Children.Size(), .Vec3f = buffer.GetElement<NGN::Math::Vec3<float>>(name) });

			section.m_Children.PushBack(std::make_shared<NGN::GuiComponent::Vec3>(NGN::GuiComponent::Vec3::Parameters {
				.Name = name,
				.Value = m_MeshBufferLayout.Get(name).Vec3f,
				.OnChange = [&](const NGN::Math::Vec3<float>& value) {
					auto buffer = NGN::Application::Get().GetRegistry().get<NGN::Component::Mesh>(m_SelectedEntity.value()).GetBuffer();
					buffer.SetElement(name, value);
					NGN::Application::Get().GetRegistry().get<NGN::Component::Mesh>(m_SelectedEntity.value()).SetBuffer(buffer);
				}
			}));
			break;
		}
	}

private:
	std::optional<entt::entity> m_SelectedEntity = std::nullopt;
	NGN::Math::Vec3<float> m_Position = { 0.0f, 0.0f, 0.0f };
	NGN::Math::Vec3<float> m_Rotation = { 0.0f, 0.0f, 0.0f };
	NGN::Math::Vec3<float> m_Scale = { 1.0f, 1.0f, 1.0f };

	struct BufferElement
	{
		size_t Index;
		union
		{
			NGN::Math::Vec3<float> Vec3f;
		};
	};

	NGN::HashMap<NGN::String, BufferElement> m_MeshBufferLayout;
};