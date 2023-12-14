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
		Transform() = default;
		Transform(const Math::Vec3<float>& position, const Math::Vec3<float>& rotation, const Math::Vec3<float>& scale)
			: m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{
		}

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
			m_Parent = parent;
			m_matrixDirty = true;
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
	};

	struct Mesh
	{
		String MeshName;
		size_t InstanceID;

		Mesh(const String& meshName)
			: MeshName(meshName), InstanceID(0)
		{
		}
	};

	struct Tag
	{
		String Name;

		Tag(const String& name)
			: Name(name)
		{
		}
	};
}