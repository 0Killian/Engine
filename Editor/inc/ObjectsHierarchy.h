#pragma once

#include "GUI.h"
#include "Events.h"
#include "Application.h"
#include "Components.h"
#include "GuiComponent\TreeNode.h"
#include "Inspector.h"

class ObjectsHierarchy : public NGN::GUI::Window, public NGN::EventListener
{
public:
	ObjectsHierarchy()
		: NGN::GUI::Window({
			.Components = {},
			.DockSide = NGN::GUI::Side::Left
		})
		, NGN::EventListener({
			NGN::EventType::ENTITY_CREATED,
			NGN::EventType::ENTITY_DESTROYED,
			NGN::EventType::PARENT_CHANGED,
			NGN::EventType::TAG_CHANGED
		})
	{}

	bool OnEvent(NGN::EventType type, NGN::EventData data) override
	{
		switch (type)
		{
			case NGN::EventType::ENTITY_CREATED:
			{
				AddEntity(data.Entity);
				break;
			}
			case NGN::EventType::ENTITY_DESTROYED:
			{
				RemoveEntity(data.Entity);
				break;
			}
			case NGN::EventType::PARENT_CHANGED:
			{
				MoveEntity(data.ParentChanged.Entity, data.ParentChanged.OldParent);
				break;
			}
			case NGN::EventType::TAG_CHANGED:
			{
				UpdateName(data.Entity);
			}
		}

		return true;
	}

	void AddEntity(entt::entity entity)
	{
		auto& transform = NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(entity);
		auto& tag = NGN::Application::Get().GetRegistry().get<NGN::Component::Tag>(entity);

		NGN::List<entt::entity> path;
		GetNodePath(entity, path);

		auto finalNode = std::make_shared<NGN::GuiComponent::TreeNode>(NGN::GuiComponent::TreeNode::Parameters{
			.Name = tag.GetName(),
			.ID = static_cast<size_t>(entity),
			.OnDoubleClick = [this, entity]() {
				NGN::Logger::Info() << "Double clicked on entity " << static_cast<size_t>(entity) << NGN::Logger::EndLine;

				auto& inspector = static_cast<Inspector&>(NGN::GUI::Get().GetWindow("Inspector"));

				if (inspector.GetSelectedEntity() == entity)
					inspector.SetSelectedEntity(std::nullopt);
				else
					inspector.SetSelectedEntity(entity);
			},
			.Flags = NGN::GuiComponent::TreeNode::Leaf,
		});

		if (path.Size() == 0)
		{
			Components.PushBack(finalNode);

			m_EntityTree.Children.PushBack(TreeNode{
				.Entity = entity,
				.Children = NGN::List<TreeNode>{},
				.ChildMap = NGN::HashMap<entt::entity, size_t>{}
			});

			m_EntityTree.ChildMap.Insert(entity, m_EntityTree.Children.Size() - 1);
			return;
		}

		TreeNode* node = &m_EntityTree;
		std::shared_ptr<NGN::GuiComponent::TreeNode> comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(path[0])]);

		for (size_t i = 1; i < path.Size(); i++)
		{
			auto& entity = path[i];
			node = &node->Children[node->ChildMap.Get(entity)];
			comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(entity)]);
		}

		comp->m_Flags = NGN::GuiComponent::TreeNode::None;
		size_t i = 0;
		for(; i < comp->m_Children.Size(); i++)
		{
			if (comp->m_Children[i]->m_Name > tag.GetName())
				break;
		}

		comp->m_Children.Insert(i, finalNode);

		node->Children.Insert(i, TreeNode {
			.Entity = entity,
			.Children = NGN::List<TreeNode>{},
			.ChildMap = NGN::HashMap<entt::entity, size_t>{}
		});

		node->ChildMap.Insert(entity, i);
	}

	void RemoveEntity(entt::entity entity)
	{
		NGN::List<entt::entity> path;
		GetNodePath(entity, path);

		if (path.Size() == 0)
		{
			Components.Erase(m_EntityTree.ChildMap.Get(entity));
			m_EntityTree.Children.Erase(m_EntityTree.ChildMap.Get(entity));
			m_EntityTree.ChildMap.Erase(entity);
			return;
		}

		TreeNode* node = &m_EntityTree;
		std::shared_ptr<NGN::GuiComponent::TreeNode> comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(path[0])]);

		for (size_t i = 1; i < path.Size(); i++)
		{
			auto& entity = path[i];
			node = &node->Children[node->ChildMap.Get(entity)];
			comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(entity)]);
		}

		comp->m_Children.Erase(node->ChildMap.Get(entity));
		node->Children.Erase(node->ChildMap.Get(entity));
		node->ChildMap.Erase(entity);

		if(node->Children.Size() == 0)
		{
			comp->m_Flags = NGN::GuiComponent::TreeNode::Leaf;
		}
	}

	void GetNodePath(entt::entity entity, NGN::List<entt::entity>& path)
	{
		auto& transform = NGN::Application::Get().GetRegistry().get<NGN::Component::Transform>(entity);
		auto parent = transform.GetParent();

		if (parent)
		{
			path.Insert(0, *parent);
			GetNodePath(*parent, path);
		}
	}

	void MoveEntity(entt::entity entity, const entt::entity* oldParent)
	{
		TreeNode* node = &m_EntityTree;
		std::shared_ptr<NGN::GuiComponent::TreeNode> comp;

		if (oldParent)
		{
			NGN::List<entt::entity> path;
			GetNodePath(*oldParent, path);
			path.PushBack(*oldParent);

			comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(path[0])]);

			for (size_t i = 1; i < path.Size(); i++)
			{
				auto& entity = path[i];
				node = &node->Children[node->ChildMap.Get(entity)];
				comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(entity)]);
			}

			comp->m_Children.Erase(node->ChildMap.Get(entity));
		}
		else
		{
			Components.Erase(m_EntityTree.ChildMap.Get(entity));
		}

		node->Children.Erase(node->ChildMap.Get(entity));
		node->ChildMap.Erase(entity);

		AddEntity(entity);
	}

	void UpdateName(entt::entity entity)
	{
		auto& tag = NGN::Application::Get().GetRegistry().get<NGN::Component::Tag>(entity);

		NGN::List<entt::entity> path;
		GetNodePath(entity, path);

		if (path.Size() == 0)
		{
			Components[m_EntityTree.ChildMap.Get(entity)]->m_Name = tag.GetName();
			return;
		}

		TreeNode* node = &m_EntityTree;
		std::shared_ptr<NGN::GuiComponent::TreeNode> comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(path[0])]);

		for (size_t i = 1; i < path.Size(); i++)
		{
			auto& entity = path[i];
			node = &node->Children[node->ChildMap.Get(entity)];
			comp = std::dynamic_pointer_cast<NGN::GuiComponent::TreeNode>(Components[node->ChildMap.Get(entity)]);
		}

		comp->m_Children[node->ChildMap.Get(entity)]->m_Name = tag.GetName();
	}

private:
	struct TreeNode
	{
		entt::entity Entity;
		NGN::List<TreeNode> Children;
		NGN::HashMap<entt::entity, size_t> ChildMap;
	};

	TreeNode m_EntityTree;
};