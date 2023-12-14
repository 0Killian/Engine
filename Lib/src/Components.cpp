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

		mesh.InstanceID = ngnMesh.AddInstance(buffer);
	}

	void InitComponents()
	{
		entt::registry& registry = Application::Get().GetRegistry();

		registry.on_construct<Mesh>().connect<&OnMeshConstruct>();
	}
}