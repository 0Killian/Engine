#pragma once

#include <stdint.h>
#include <optional>
#include <functional>

#include "Math/Vec3.h"
#include "Containers/List.h"
#include "Containers/String.h"
#include "Containers/HashMap.h"
#include <any>

struct ImGuiContext;
typedef unsigned int ImGuiID;

namespace NGN
{
	class GUI
	{
	public:
		enum class Side
		{
			Left,
			Right,
			Top,
			Bottom,
			Tab,
			Viewport
		};

		class Component
		{
		public:
			Component(
				const String& name,
				bool open,
				bool dragDropSource, 
				bool dragDropTarget,
				std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> dragDropPayload,
				std::function<void()> m_OnClick,
				std::function<void()> m_OnDoubleClick,
				std::function<void(const String& type, std::any data)> m_OnDragDrop)
				: m_Name(name)
				, m_Open(open)
				, m_DragDropSource(dragDropSource)
				, m_DragDropTarget(dragDropTarget)
				, m_DragDropPayload(dragDropPayload)
				, m_OnClick(m_OnClick)
				, m_OnDoubleClick(m_OnDoubleClick)
				, m_OnDragDrop(m_OnDragDrop)
			{}

			virtual ~Component() = default;

			virtual void OnClick()
			{
				if (m_OnClick)
					m_OnClick();
			}

			virtual void OnDoubleClick()
			{
				if (m_OnDoubleClick)
					m_OnDoubleClick();
			}

			virtual void OnDragDrop(const String& type, std::any data)
			{
				if (m_OnDragDrop)
					m_OnDragDrop(type, data);
			}

			virtual void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) = 0;

			String m_Name;
			bool m_Open = true;
			bool m_DragDropSource = false;
			bool m_DragDropTarget = false;
			std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> m_DragDropPayload;
			std::function<void()> m_OnClick = nullptr;
			std::function<void()> m_OnDoubleClick = nullptr;
			std::function<void(const String& type, std::any data)> m_OnDragDrop = nullptr;

		protected:
			struct DragDropData
			{
				const std::any* Data;
				const char* Type;
			};
		};

		class Window
		{
		public:
			enum _Flags
			{
				None = 0,
				DockSpace = 1 << 0,
				NoDecoration = 1 << 1,
				NoMove = 1 << 2,
				NoResize = 1 << 3,
				NoBringToFrontOnFocus = 1 << 4,
				NoNavFocus = 1 << 5,
				NoBackground = 1 << 6,
			};

			struct Parameters
			{
				bool Open = true;
				List<std::shared_ptr<Component>> Components = {};
				_Flags Flags = None;
				std::optional<float> X = std::nullopt;
				std::optional<float> Y = std::nullopt;
				std::optional<float> Width = std::nullopt;
				std::optional<float> Height = std::nullopt;
				std::optional<Side> DockSide = std::nullopt;
			};

			Window(const Parameters& parameters)
				: Open(parameters.Open)
				, Components(parameters.Components)
				, Flags(parameters.Flags)
				, X(parameters.X)
				, Y(parameters.Y)
				, Width(parameters.Width)
				, Height(parameters.Height)
				, DockSide(parameters.DockSide)
			{}
			virtual ~Window() = default;

			float ComputedX = 0.0f;
			float ComputedY = 0.0f;
			float ComputedWidth = 0.0f;
			float ComputedHeight = 0.0f;

		protected:
			bool Open = true;
			List<std::shared_ptr<Component>> Components = {};
			_Flags Flags = None;
			std::optional<float> X = std::nullopt;
			std::optional<float> Y = std::nullopt;
			std::optional<float> Width = std::nullopt;
			std::optional<float> Height = std::nullopt;
			std::optional<Side> DockSide = std::nullopt;

			friend class GUI;
		};

		static GUI& Get();

		GUI();

		void Init(const HashMap<String, std::shared_ptr<Window>>& windows);
		void BeginFrame();
		void Render(HashMap<String, std::shared_ptr<Window>>& popups);

		Window& GetWindow(const String& name) { return *m_Windows[name]; }

		void EndFrame();

	private:
		ImGuiContext* m_Context;
		bool m_DockSpaceInitialized = false;
		size_t m_NextID = 0;
		HashMap<String, std::shared_ptr<Window>> m_Windows = {};

		void RenderComponent(const Component& component, bool canDragDrop = true);
		void RenderWindow(const String& name, Window& window);
		void HandleComponentEvents(const Component& component, bool canDragDrop = true);
	};
}