//
// Created by Killian on 28/11/2023.
//
#include "Application.h"

#include "Configuration.h"
#include "Components.h"

namespace NGN
{
    struct ApplicationInner
    {
    };

    Application* Application::s_Instance;

    Application::Application(const List<String>& args)
        : m_Configuration(args) // TODO: Read from config file if it exists
        , m_Logger(m_Configuration.LogLevel)
        , m_EventManager(new EventManager())
        , m_Window(Window::Specification{
            .Title = m_Configuration.WindowTitle,
            .Width = m_Configuration.WindowWidth,
            .Height = m_Configuration.WindowHeight,
            .API = RenderAPI::D3D11,
        })
    {}

    Application::~Application()
    {
        delete static_cast<ApplicationInner*>(m_Inner);
        delete m_EventManager;
    }

    void Application::Init()
    {
        m_Input = std::make_unique<Input>();
        m_Window.EnableEvents();
        m_Window.CreateRenderer();

        InitInner();
    }

    void Application::Run()
    {
        Logger::Info() << "Application running" << Logger::EndLine;

        float angle = 0.0f;

        while(!m_ShouldExit)
        {
            angle += 0.01f;
            m_Window.PollEvents();
            OnUpdate();
            
            FrameData frameData = {
                .constantBuffer = InstanceBufferBuilder()
                    .AddElement("View", VertexStructureElement::Mat4, 1)
                    .AddElement("Projection", VertexStructureElement::Mat4, 1)
                    .Build(),
            };

            Math::Mat4<float, Math::ROW_MAJOR> view = Math::Mat4<float, Math::ROW_MAJOR>::LookAt(
				Math::Vec3<float>(cos(angle) * 4.0f, 0.0f, sin(angle) * 4.0f),
				Math::Vec3<float>(0.0f, 0.0f, 0.0f),
				Math::Vec3<float>(0.0f, 1.0f, 0.0f)
			);
            
            Math::Mat4<float, Math::ROW_MAJOR> projection = Math::Mat4<float, Math::ROW_MAJOR>::Perspective(
                90.0f,
                1280.0f / 720.0f,
                0.1f,
                100.0f
            );

            frameData.constantBuffer.SetElement("View", view);
            frameData.constantBuffer.SetElement("Projection", projection);

            FramePacket packet = GetRenderer().StartFrame(frameData);

            auto buffer = InstanceBufferBuilder()
                .AddElement("MODEL", VertexStructureElement::Mat4, 1)
                .Build();

            for(auto&& [entity, transform, mesh] : m_Registry.view<Component::Transform, Component::Mesh>().each())
			{
				if(transform.ShouldUpdate())
                {
                    NGN::Mesh& ngnMesh = ResourceManager::Get().GetMesh(mesh.MeshName);

                    buffer.SetElement("MODEL", transform.GetMatrix());

                    mesh.InstanceID = ngnMesh.AddInstance(buffer);
				}
			}

            GetRenderer().EndFrame(packet);
        }
    }

    void Application::Exit()
    {
        m_ShouldExit = true;
    }

}
