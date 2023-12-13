//
// Created by Killian on 06/12/2023.
//
#pragma once
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl.h>
#include <optional>

#include "win.h"
#include "Configuration.h"
#include "Logger.h"
#include "Containers/HashMap.h"
#include "Containers/VertexBuffer.h"

namespace NGN
{
    class Mesh;
}

namespace NGN::D3D11
{
    class Renderer;

    /**
     * \brief A pipeline is a combination of multiple shaders into a single program.
     *
     * This class also contains the resources associated with the pipeline, like vertex buffers, textures, etc.
     * It is the main class used to render multiple meshes with the same shader. It implements indirect rendering.
     */
    class Pipeline
    {
    public:
        Pipeline(const String& vertexSource, const String& pixelSource);
        Pipeline(const List<uint8_t>& compiledVertex, const List<uint8_t>& compiledPixel);

        void UpdateMesh(size_t meshId, String name, Mesh& mesh);
        uint32_t AddInstance(size_t meshId, InstanceBuffer buffer);
        void UpdateInstance(size_t meshId, size_t instanceId, InstanceBuffer buffer);
        void UpdateConstantBuffer(InstanceBuffer buffer);

        void Render();

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_CommandBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_StagingBuffer;

        struct InstanceReference
        {
            size_t Index; // From start of mesh instance window
            InstanceBuffer Data;
        };

        struct MeshReference
        {
            uint32_t VertexOffset{};
            uint32_t IndexOffset{};
            uint32_t InstanceCount{};
            uint32_t FirstInstance{};
            uint32_t CommandIndex{};

            String Name;

            HashMap<size_t, InstanceReference> Instances;
            size_t NextInstanceID = 0;
        };

        size_t m_NextIndex = 0;
        size_t m_NextVertex = 0;
        size_t m_NextCommand = 0;
        size_t m_NextInstance = 0;
        size_t m_LastInstanceMeshId = 0;

        HashMap<size_t, MeshReference> m_MeshReferences;

        static constexpr UINT DefaultVertexBufferSize = 1024 * 1024;
        static constexpr UINT DefaultIndexBufferSize = 1024 * 1024;
        static constexpr UINT DefaultCommandBufferSize = 1024 * 1024;
        static constexpr UINT DefaultInstanceBufferSize = 1024 * 1024;
        static constexpr UINT DefaultStagingBufferSize = 16384;

        struct InputLayout
        {
            HashMap<String, VertexStructureElement> VertexInputLayoutElements;
            HashMap<String, VertexStructureElement> InstanceInputLayoutElements;
            HashMap<String, VertexStructureElement> ConstantInputLayoutElements;

            List<D3D11_INPUT_ELEMENT_DESC> Layout;

            InputLayout(ID3DBlob* vertexShaderBlob);
        };

        std::unique_ptr<BufferLayout> m_VertexBufferLayout;
        std::unique_ptr<BufferLayout> m_InstanceBufferLayout;
        std::unique_ptr<BufferLayout> m_ConstantBufferLayout;

        static Microsoft::WRL::ComPtr<ID3DBlob> CompileVertexShader(const String& source);
        static Microsoft::WRL::ComPtr<ID3DBlob> CompilePixelShader(const String& source);
        static Microsoft::WRL::ComPtr<ID3DBlob> PackVertexShader(const List<uint8_t>& compiled);
        static Microsoft::WRL::ComPtr<ID3DBlob> PackPixelShader(const List<uint8_t>& compiled);

        Pipeline(Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob, Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob);

        void UpdateBufferWithStaging(ID3D11Device* device, ID3D11DeviceContext* context, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, size_t start, const void* data, size_t size);
        void UpdateBuffer(ID3D11Device* device, ID3D11DeviceContext* context, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, size_t start, const void* data, size_t size);
    };
}
