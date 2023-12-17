#pragma once

#include "GUI.h"
#include "GuiComponent\Image.h"
#include "Renderer.h"

class Viewport : public NGN::GUI::Window
{
public:
	Viewport(size_t width, size_t height)
		: Window({
			.Components = {
				std::make_shared<NGN::GuiComponent::Image>(NGN::GuiComponent::Image::Parameters {
					.Name = "Viewport",
					.Width = width,
					.Height = height,
					.NativeTexture = NGN::Renderer::Get().GetNativeRenderTexture(),
				})
			},
			.Flags = static_cast<NGN::GUI::Window::_Flags>(NGN::GUI::Window::NoBackground | NGN::GUI::Window::NoMove | NGN::GUI::Window::NoResize | NGN::GUI::Window::NoDecoration | NGN::GUI::Window::NoBringToFrontOnFocus | NGN::GUI::Window::NoNavFocus),
			.DockSide = NGN::GUI::Side::Viewport,
		})
	{}

	void Update()
	{
		NGN::GuiComponent::Image& image = *std::dynamic_pointer_cast<NGN::GuiComponent::Image>(Components[0]);
		image.m_NativeTexture = NGN::Renderer::Get().GetNativeRenderTexture();
		image.m_Width = ComputedWidth;
		image.m_Height = ComputedHeight;
	}
};