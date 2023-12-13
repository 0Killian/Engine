#include "Renderer.h"
#include "Application.h"

namespace NGN
{
	Renderer& Renderer::Get()
	{
		return Application::Get().GetRenderer();
	}
}