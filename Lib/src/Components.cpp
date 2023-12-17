#include "Components.h"
#include "Application.h"

namespace NGN::Component
{
	void OnMeshConstruct(entt::registry& registry, entt::entity entity)
	{
		Transform& transform = registry.get<Transform>(entity);
		Mesh& mesh = registry.get<Mesh>(entity);
		NGN::Mesh& ngnMesh = ResourceManager::Get().GetMesh(mesh.MeshName);

		auto buffer = InstanceBufferBuilder()
			.AddElement("MODEL", VertexStructureElement::Mat4Row, 1)
			.Build();

		buffer.SetElement("MODEL", transform.GetMatrix());

		mesh.InstanceID = ngnMesh.AddInstance(buffer.Merge(mesh.GetBuffer()));
	}

	void OnMeshReplace(entt::registry& registry, entt::entity entity)
	{
		Transform& transform = registry.get<Transform>(entity);
		Mesh& mesh = registry.get<Mesh>(entity);
		NGN::Mesh& ngnMesh = ResourceManager::Get().GetMesh(mesh.MeshName);

		auto buffer = InstanceBufferBuilder()
			.AddElement("MODEL", VertexStructureElement::Mat4Row, 1)
			.Build();

		buffer.SetElement("MODEL", transform.GetMatrix());

		mesh.InstanceID = ngnMesh.AddInstance(buffer.Merge(mesh.GetBuffer()));
	}

	void OnEntityCreated(entt::registry& registry, entt::entity entity)
	{
		registry.emplace<Tag>(entity, "New Entity", entity);
		registry.emplace<Transform>(entity, entity);

		EventData data;
		data.Entity = entity;

		EventManager::Get().TriggerEvent(EventType::ENTITY_CREATED, data);
	}

	void OnEntityDestroyed(entt::registry& registry, entt::entity entity)
	{
		EventData data;
		data.Entity = entity;

		EventManager::Get().TriggerEvent(EventType::ENTITY_DESTROYED, data);
	}

	void InitComponents()
	{
		entt::registry& registry = Application::Get().GetRegistry();

		registry.on_construct<Mesh>().connect<&OnMeshConstruct>();
		registry.on_update<Mesh>().connect<&OnMeshReplace>();
		registry.on_construct<entt::entity>().connect<&OnEntityCreated>();
		registry.on_destroy<entt::entity>().connect<&OnEntityDestroyed>();
	}
}