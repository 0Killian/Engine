#pragma once

#include "GUI.h"
#include "GuiComponent\Table.h"
#include "GuiComponent\Button.h"
#include "Renderer.h"
#include "Events.h"

class Resources : public NGN::GUI::Window, public NGN::EventListener
{
public:
	Resources()
		: Window({
			.Components = {
				std::make_shared<NGN::GuiComponent::Table>(NGN::GuiComponent::Table::Parameters {
					.Name = "Resources",
					.CellConfiguration = {
						.Padding = 16,
					},
					.CellWidth = 64,
					.CellHeight = 64
				})
			},
			.DockSide = NGN::GUI::Side::Bottom,
		})
		, EventListener({
			NGN::EventType::RESOURCE_ADDED,
			NGN::EventType::RESOURCE_REMOVED,
		})
	{}

	bool OnEvent(NGN::EventType type, NGN::EventData data) override
	{
		switch (type)
		{
		case NGN::EventType::RESOURCE_ADDED:
			{
				NGN::GuiComponent::Table& table = *std::dynamic_pointer_cast<NGN::GuiComponent::Table>(Components[0]);
				auto& resource = data.Resource;
				switch (resource.Type)
				{
				case NGN::ResourceType::Pipeline:
					AddResource("Pipeline", resource.Name);
					break;

				case NGN::ResourceType::Mesh:
					AddResource("Mesh", resource.Name);
					break;
				}
			}
			break;
		case NGN::EventType::RESOURCE_REMOVED:
			{
				NGN::GuiComponent::Table& table = *std::dynamic_pointer_cast<NGN::GuiComponent::Table>(Components[0]);
				auto& resource = data.Resource;
				switch (resource.Type)
				{
				case NGN::ResourceType::Pipeline:
					RemoveResource("Pipeline", resource.Name);
					break;

				case NGN::ResourceType::Mesh:
					RemoveResource("Mesh", resource.Name);
					break;
				}
			}
			break;
		}

		return true;
	}

	void AddResource(const NGN::String& type, const NGN::String& name)
	{
		NGN::GuiComponent::Table& table = *std::dynamic_pointer_cast<NGN::GuiComponent::Table>(Components[0]);
		size_t i = 0;
		for (; i < table.m_Children.Size(); i++)
		{
			if(table.m_Children[i]->m_Name > name)
				break;
		}

		table.m_Children.Insert(i, std::make_shared<NGN::GuiComponent::Button>(NGN::GuiComponent::Button::Parameters{
			.Name = name,
			.DragDropSource = true,
			.DragDropPayload = std::tuple<std::shared_ptr<NGN::GUI::Component>, NGN::String, std::any>(
				std::make_shared<NGN::GuiComponent::Button>(NGN::GuiComponent::Button::Parameters {
					.Name = name,
					.Width = 64,
					.Height = 64,
				}),
				type,
				name
			)
		}));
	}

	void RemoveResource(const NGN::String& type, const NGN::String& name)
	{
		NGN::GuiComponent::Table& table = *std::dynamic_pointer_cast<NGN::GuiComponent::Table>(Components[0]);
		size_t i = 0;
		for (; i < table.m_Children.Size(); i++)
		{
			if (table.m_Children[i]->m_Name == name)
				break;
		}

		table.m_Children.Erase(i);
	}

private:
	NGN::HashMap<NGN::String, size_t> m_ResourcesIndices;
};