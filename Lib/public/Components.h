#pragma once
#include <optional>

#include "Application.h"
#include "entt.h"
#include "Containers/String.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"

namespace NGN::Component
{
	void InitComponents();

	struct Transform
	{
		Transform(entt::entity entity)
			: m_Entity(entity)
		{}

		Transform(entt::entity entity, const Math::Vec3<float>& position, const Math::Vec3<float>& rotation, const Math::Vec3<float>& scale)
			: m_Entity(entity)
			, m_Position(position)
			, m_Rotation(rotation)
			, m_Scale(scale)
		{}

		Math::Mat4<float, Math::ROW_MAJOR> GetMatrix()
		{
			if(m_matrixDirty)
			{
				if (m_Parent)
					m_matrix = Application::Get().GetRegistry().get<Transform>(*m_Parent).GetMatrix();
				else
					m_matrix = Math::Mat4<float, Math::ROW_MAJOR>::Identity();

				auto local = Math::Mat4<float, Math::ROW_MAJOR>::Translation(m_Position);
				local *= Math::Mat4<float, Math::ROW_MAJOR>::Rotation(m_Rotation);
				local *= Math::Mat4<float, Math::ROW_MAJOR>::Scale(m_Scale);

				m_matrix = m_matrix * local;

				m_matrixDirty = false;
			}

			return m_matrix;
		}

		Math::Vec3<float> GetPosition() const { return m_Position; }
		Math::Vec3<float> GetRotation() const { return m_Rotation; }
		Math::Vec3<float> GetScale() const { return m_Scale; }

		void SetPosition(const Math::Vec3<float>& position)
		{
			m_Position = position;
			m_matrixDirty = true;
		}

		void SetRotation(const Math::Vec3<float>& rotation)
		{
			m_Rotation = rotation;
			m_matrixDirty = true;
		}

		void SetScale(const Math::Vec3<float>& scale)
		{
			m_Scale = scale;
			m_matrixDirty = true;
		}

		void SetParent(const std::optional<entt::entity>& parent)
		{
			entt::entity p;
			if (m_Parent)
				p = m_Parent.value();

			EventData data;
			data.ParentChanged.Entity = m_Entity;
			data.ParentChanged.OldParent = m_Parent ? &p : nullptr;

			m_Parent = parent;
			m_matrixDirty = true;

			EventManager::Get().TriggerEvent(EventType::PARENT_CHANGED, data);
		}

		void SetShouldUpdate(bool shouldUpdate) { m_matrixDirty = shouldUpdate; }

		bool ShouldUpdate() const { return m_matrixDirty; }

		std::optional<entt::entity> GetParent() const { return m_Parent; }

	private:
		Math::Mat4<float, Math::ROW_MAJOR> m_matrix = Math::Mat4<float, Math::ROW_MAJOR>::Identity();
		bool m_matrixDirty = true;

		Math::Vec3<float> m_Position{};
		Math::Vec3<float> m_Rotation{};
		Math::Vec3<float> m_Scale{ 1.0f, 1.0f, 1.0f };

		std::optional<entt::entity> m_Parent;
		entt::entity m_Entity;
	};

	struct Mesh
	{
		String MeshName;
		size_t InstanceID;
		
		Mesh(const String& meshName, InstanceBuffer buffer, entt::entity entity)
			: MeshName(meshName), InstanceID(static_cast<size_t>(-1)), m_Buffer(buffer), m_Entity(entity)
		{
		}

		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;

		Mesh(Mesh&& other) noexcept
			: MeshName(std::move(other.MeshName)), InstanceID(other.InstanceID), m_Buffer(std::move(other.m_Buffer)), m_Entity(other.m_Entity)
		{
			other.InstanceID = static_cast<size_t>(-1);
		}

		Mesh& operator=(Mesh&& other) noexcept
		{
			if (this == &other)
				return *this;

			NGN::Mesh& mesh = ResourceManager::Get().GetMesh(MeshName);
			mesh.RemoveInstance(InstanceID);

			MeshName = std::move(other.MeshName);
			InstanceID = other.InstanceID;
			m_Buffer = std::move(other.m_Buffer);
			m_Entity = other.m_Entity;

			other.InstanceID = static_cast<size_t>(-1);

			return *this;
		}

		~Mesh()
		{
			if (InstanceID == static_cast<size_t>(-1))
				return;

			NGN::Mesh& mesh = ResourceManager::Get().GetMesh(MeshName);
			mesh.RemoveInstance(InstanceID);
			InstanceID = 0;
		}

		const InstanceBuffer& GetBuffer() const { return m_Buffer; }
		void SetBuffer(const InstanceBuffer& buffer)
		{
			m_Buffer = buffer;
			auto& transform = Application::Get().GetRegistry().get<Transform>(m_Entity);
			auto& mesh = ResourceManager::Get().GetMesh(MeshName);

			m_Buffer.SetElement("MODEL", transform.GetMatrix());
			mesh.UpdateInstance(InstanceID, m_Buffer);
		}

	private:
		InstanceBuffer m_Buffer;
		entt::entity m_Entity;
	};

	struct Tag
	{
		Tag(const String& name, entt::entity entity)
			: m_Name(name)
			, m_Entity(entity)
		{
		}

		const String& GetName() const { return m_Name; }
		void SetName(const String& name)
		{
			m_Name = name;

			EventData data;
			data.Entity = m_Entity;

			EventManager::Get().TriggerEvent(EventType::TAG_CHANGED, data);
		}

	private:
		String m_Name;
		entt::entity m_Entity;
	};
}