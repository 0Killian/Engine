//
// Created by Killian on 06/12/2023.
//
#include "D3D11/Pipeline.h"
#include "D3D11/Renderer.h"

#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <fstream>
#include <ranges>
#include <optional>

#include "D3D11/Utils.h"
#include "ResourceManager.h"

namespace NGN::D3D11
{
    Pipeline::InputLayout::InputLayout(ID3DBlob* vertexShaderBlob)
    {
        Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;
        THROW_IF_FAILED(D3DReflect(
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            IID_ID3D11ShaderReflection,
            &shaderReflection));

        D3D11_SHADER_DESC shaderDesc;
        THROW_IF_FAILED(shaderReflection->GetDesc(&shaderDesc));

        D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
        THROW_IF_FAILED(shaderReflection->GetInputParameterDesc(0, &parameterDesc));

        size_t vertexStride = 0;
        size_t instanceStride = 0;
        LPCSTR semanticName = parameterDesc.SemanticName;
        std::optional<VertexStructureElement> element = std::nullopt;
        size_t layers = 0;
        D3D11_INPUT_ELEMENT_DESC elementDesc;

        for(size_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
			THROW_IF_FAILED(shaderReflection->GetInputParameterDesc(i, &parameterDesc));

            size_t& stride = String(parameterDesc.SemanticName).StartsWith("INSTANCE_") ? instanceStride : vertexStride;

            elementDesc = {
				.SemanticName = parameterDesc.SemanticName,
				.SemanticIndex = parameterDesc.SemanticIndex,
				.Format = DXGI_FORMAT_R32G32B32_FLOAT,
				.InputSlot = 0,
				.AlignedByteOffset = static_cast<UINT>(stride),
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			};

            if (String(parameterDesc.SemanticName).StartsWith("INSTANCE_"))
            {
                elementDesc.InputSlot = 1;
                elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
                elementDesc.InstanceDataStepRate = 1;
            }

            layers++;

            if (!element)
			{
				element = VertexStructureElement(VertexStructureElement::Float, 1);
			}

            if (strcmp(semanticName, parameterDesc.SemanticName) != 0)
            {
                layers--;

                if (layers == 4)
                {
                    if (element->m_Type == VertexStructureElement::Float4)
                    {
                        element->m_Type = VertexStructureElement::Mat4;
                    }
                    else
                    {
                        throw std::runtime_error("Unsupported format");
                    }
                }
                else if (layers != 1)
                {
                    throw std::runtime_error("Unsupported format");
                }

                String name = semanticName;
                if (name.StartsWith("INSTANCE_"))
                {
                    name = name.SubStr(9);
                    elementDesc.InputSlot = 1;
                    elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
                    elementDesc.InstanceDataStepRate = 1;

                    if (InstanceInputLayoutElements.ContainsKey(name))
                    {
                        throw std::runtime_error("Instance input layout element already exists");
                    }

                    InstanceInputLayoutElements[name] = *element;
                }
                else
                {
                    if (VertexInputLayoutElements.ContainsKey(name))
                    {
                        throw std::runtime_error("Vertex input layout element already exists");
                    }

                    VertexInputLayoutElements[name] = *element;
                }

                semanticName = parameterDesc.SemanticName;

                layers = 1;
            }

            if (parameterDesc.Mask == 1)
            {
                if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    elementDesc.Format = DXGI_FORMAT_R32_UINT;
                    throw std::runtime_error("Unsupported format");
                }
                else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    elementDesc.Format = DXGI_FORMAT_R32_SINT;
                    element->m_Type = VertexStructureElement::Int;
                }
                else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                {
                    elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                    element->m_Type = VertexStructureElement::Float;
                }

                stride += 4;
            }
			else if (parameterDesc.Mask <= 3)
            {
                if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
					throw std::runtime_error("Unsupported format");
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
					element->m_Type = VertexStructureElement::Int2;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
					element->m_Type = VertexStructureElement::Float2;
				}

                stride += 8;
			}
            else if (parameterDesc.Mask <= 7)
            {
				if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
					throw std::runtime_error("Unsupported format");
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
					element->m_Type = VertexStructureElement::Int3;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
					element->m_Type = VertexStructureElement::Float3;
				}

                stride += 12;
            }
            else if (parameterDesc.Mask <= 15)
			{
                if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
                    elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
					throw std::runtime_error("Unsupported format");
                }
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
					element->m_Type = VertexStructureElement::Int4;
                }
                else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    element->m_Type = VertexStructureElement::Float4;
                }

                stride += 16;
            }

			Layout.PushBack(elementDesc);
		}

        if (layers > 0)
        {
            if (layers == 4)
            {
                if (element->m_Type == VertexStructureElement::Float4)
                {
                    element->m_Type = VertexStructureElement::Mat4;
                }
                else
                {
                    throw std::runtime_error("Unsupported format");
                }
            }
            else if (layers != 1)
            {
                throw std::runtime_error("Unsupported format");
            }

            String name = semanticName;
            if (name.StartsWith("INSTANCE_"))
            {
                name = name.SubStr(9);
                elementDesc.InputSlot = 1;
                elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
                elementDesc.InstanceDataStepRate = 1;

                if (InstanceInputLayoutElements.ContainsKey(name))
                {
                    throw std::runtime_error("Instance input layout element already exists");
                }

                InstanceInputLayoutElements[name] = *element;
            }
            else
            {
                if (VertexInputLayoutElements.ContainsKey(name))
                {
                    throw std::runtime_error("Vertex input layout element already exists");
                }

                VertexInputLayoutElements[name] = *element;
            }
        }

        // Constant buffer
        if (shaderDesc.ConstantBuffers > 0)
        {
            if(shaderDesc.ConstantBuffers > 1)
                throw std::runtime_error("Multiple constant buffers are not supported");

            ID3D11ShaderReflectionConstantBuffer* constantBufferReflection = shaderReflection->GetConstantBufferByIndex(0);
            D3D11_SHADER_BUFFER_DESC constantBufferDesc;
            THROW_IF_FAILED(constantBufferReflection->GetDesc(&constantBufferDesc));

            if(constantBufferDesc.Type != D3D_CT_CBUFFER)
                throw std::runtime_error("Constant buffer is not a cbuffer");

            for (size_t i = 0; i < constantBufferDesc.Variables; i++)
            {
                ID3D11ShaderReflectionVariable* variableReflection = constantBufferReflection->GetVariableByIndex(i);
                D3D11_SHADER_VARIABLE_DESC variableDesc;
                THROW_IF_FAILED(variableReflection->GetDesc(&variableDesc));

                ID3D11ShaderReflectionType* typeReflection = variableReflection->GetType();
                D3D11_SHADER_TYPE_DESC typeDesc;
                THROW_IF_FAILED(typeReflection->GetDesc(&typeDesc));

                VertexStructureElement element;

                if (typeDesc.Type == D3D_SVT_FLOAT && typeDesc.Class == D3D_SVC_SCALAR)
                {
                    element.m_Type = VertexStructureElement::Float;
                }
                else if (typeDesc.Type == D3D_SVT_INT && typeDesc.Class == D3D_SVC_SCALAR)
                {
                    element.m_Type = VertexStructureElement::Int;
                }
                else if (typeDesc.Type == D3D_SVT_FLOAT && typeDesc.Class == D3D_SVC_VECTOR)
                {
                    if (typeDesc.Columns == 2)
                    {
                        element.m_Type = VertexStructureElement::Float2;
                    }
                    else if (typeDesc.Columns == 3)
                    {
                        element.m_Type = VertexStructureElement::Float3;
                    }
                    else if (typeDesc.Columns == 4)
                    {
                        element.m_Type = VertexStructureElement::Float4;
                    }
                    else
                    {
                        throw std::runtime_error("Unsupported constant buffer type");
                    }
                }
                else if (typeDesc.Type == D3D_SVT_INT && typeDesc.Class == D3D_SVC_VECTOR)
                {
                    if (typeDesc.Columns == 2)
                    {
                        element.m_Type = VertexStructureElement::Int2;
                    }
                    else if (typeDesc.Columns == 3)
                    {
                        element.m_Type = VertexStructureElement::Int3;
                    }
                    else if (typeDesc.Columns == 4)
                    {
                        element.m_Type = VertexStructureElement::Int4;
                    }
                    else
                    {
                        throw std::runtime_error("Unsupported constant buffer type");
                    }
                }
                else if (typeDesc.Type == D3D_SVT_FLOAT && typeDesc.Class == D3D_SVC_MATRIX_COLUMNS)
                {
                    element.m_Type = VertexStructureElement::Mat4;
                }
                else
                {
                    throw std::runtime_error("Unsupported constant buffer type");
                }

                element.m_Count = typeDesc.Elements != 0 ? typeDesc.Elements : 1;

                ConstantInputLayoutElements[variableDesc.Name] = element;
            }
        }
    }

#ifdef NGN_DEBUG
    constexpr auto compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS;
#else
    constexpr auto compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> Pipeline::CompileVertexShader(const String& source)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        if (FAILED(D3DCompile2(
            source.GetData(),
            source.Size(),
            "VertexShader",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "main",
            "vs_5_0",
            compileFlags,
            0,
            0,
            nullptr,
            0,
            &vertexShaderBlob,
            &errorBlob)))
        {
            if (errorBlob)
            {
                throw std::runtime_error((String("Failed to compile vertex shader :\n") + static_cast<char*>(errorBlob->GetBufferPointer())).GetData());
            }

            throw std::runtime_error("Failed to compile vertex shader ");
        }

        return vertexShaderBlob;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> Pipeline::CompilePixelShader(const String& source)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        if (FAILED(D3DCompile2(
            source.GetData(),
            source.Size(),
            "PixelShader",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "main",
            "ps_5_0",
            compileFlags,
            0,
            0,
            nullptr,
            0,
            &pixelShaderBlob,
            &errorBlob)))
        {
            if (errorBlob)
            {
                throw std::runtime_error((String("Failed to compile pixel shader :\n") + static_cast<char*>(errorBlob->GetBufferPointer())).GetData());
            }
            throw std::runtime_error("Failed to compile pixel shader ");
        }

        return pixelShaderBlob;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> Pipeline::PackVertexShader(const List<uint8_t>& compiled)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;

        D3DCreateBlob(compiled.Size(), &vertexShaderBlob);
        memcpy(vertexShaderBlob->GetBufferPointer(), compiled.GetData(), compiled.Size());

        return vertexShaderBlob;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> Pipeline::PackPixelShader(const List<uint8_t>& compiled)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;

        D3DCreateBlob(compiled.Size(), &pixelShaderBlob);
        memcpy(pixelShaderBlob->GetBufferPointer(), compiled.GetData(), compiled.Size());

        return pixelShaderBlob;
    }

    Pipeline::Pipeline(const String& vertexSource, const String& pixelSource)
        : Pipeline(CompileVertexShader(vertexSource), CompilePixelShader(pixelSource))
    {}

    Pipeline::Pipeline(const List<uint8_t>& compiledVertex, const List<uint8_t>& compiledPixel)
        : Pipeline(PackVertexShader(compiledVertex), PackPixelShader(compiledPixel))
    {}

    Pipeline::Pipeline(Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob, Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob)
    {
        Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

        THROW_IF_FAILED(renderer.GetDevice()->CreateVertexShader(
            vertexBlob->GetBufferPointer(),
            vertexBlob->GetBufferSize(),
            nullptr,
            &m_VertexShader));

        THROW_IF_FAILED(renderer.GetDevice()->CreatePixelShader(
            pixelBlob->GetBufferPointer(),
            pixelBlob->GetBufferSize(),
            nullptr,
            &m_PixelShader));

        InputLayout layout = InputLayout(vertexBlob.Get());
        m_VertexBufferLayout = std::make_unique<BufferLayout>(layout.VertexInputLayoutElements);
        m_InstanceBufferLayout = std::make_unique<BufferLayout>(layout.InstanceInputLayoutElements);
        m_ConstantBufferLayout = std::make_unique<BufferLayout>(layout.ConstantInputLayoutElements);

        THROW_IF_FAILED(renderer.GetDevice()->CreateInputLayout(
            layout.Layout.GetData(),
            layout.Layout.Size(),
            vertexBlob->GetBufferPointer(),
            vertexBlob->GetBufferSize(),
            &m_InputLayout));

        D3D11_BUFFER_DESC bufferDesc = {
            .ByteWidth = static_cast<UINT>(DefaultVertexBufferSize * m_VertexBufferLayout->GetStride()),
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_VERTEX_BUFFER,
            .CPUAccessFlags = 0,
            .MiscFlags = 0,
            .StructureByteStride = 0
        };

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_VertexBuffer));

        bufferDesc.ByteWidth = DefaultInstanceBufferSize * m_InstanceBufferLayout->GetStride();
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        if (bufferDesc.ByteWidth == 0)
        {
            bufferDesc.ByteWidth = 1;
        }

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_InstanceBuffer));

        bufferDesc.ByteWidth = m_ConstantBufferLayout->GetStride();
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        if (bufferDesc.ByteWidth == 0)
        {
            bufferDesc.ByteWidth = 1;
        }

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_ConstantBuffer));

        bufferDesc.ByteWidth = DefaultIndexBufferSize * sizeof(uint32_t);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_IndexBuffer));

        bufferDesc.ByteWidth = DefaultStagingBufferSize;
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        bufferDesc.BindFlags = 0;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_StagingBuffer));

        bufferDesc.ByteWidth = DefaultCommandBufferSize * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS);
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.StructureByteStride = sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS);

        THROW_IF_FAILED(renderer.GetDevice()->CreateBuffer(
            &bufferDesc,
            nullptr,
            &m_CommandBuffer));
    }

    void Pipeline::Render()
    {
        Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

        const auto context = renderer.GetContext();
        ID3D11Buffer* buffers[] = {
            m_VertexBuffer.Get(),
            m_InstanceBuffer.Get()
        };

        const UINT strides[] = { static_cast<UINT>(m_VertexBufferLayout->GetStride()), static_cast<UINT>(m_InstanceBufferLayout->GetStride()) };
        constexpr UINT offsets[] = { 0, 0 };

        context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
        context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
        context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
        context->IASetInputLayout(m_InputLayout.Get());
        context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
        
        for(const auto& meshReference : std::ranges::views::values(m_MeshReferences))
            context->DrawIndexedInstancedIndirect(m_CommandBuffer.Get(), meshReference.CommandIndex * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS));
    }

    void Pipeline::UpdateMesh(size_t meshId, String name, Mesh& mesh)
    {
        Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

        const auto device = renderer.GetDevice();
        const auto context = renderer.GetContext();
        
        if(!m_MeshReferences.ContainsKey(meshId))
        {
            const VertexBuffer vertices = mesh.GetVertices().GenerateCompatible(*m_VertexBufferLayout);
            
            // We need to add the mesh to the pipeline, this means that we do not need to regenerate the command buffer,
            // we can just append the new mesh to the end of the buffer.
            const MeshReference meshReference = {
                .VertexOffset = static_cast<uint32_t>(m_NextVertex),
                .IndexOffset = static_cast<uint32_t>(m_NextIndex),
                .InstanceCount = 0,
                .FirstInstance = 0,
                .CommandIndex = static_cast<uint32_t>(m_NextCommand),
                .Name = name
            };

            const D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS command = {
                .IndexCountPerInstance = static_cast<UINT>(mesh.GetIndices().Size()),
                .InstanceCount = meshReference.InstanceCount,
                .StartIndexLocation = meshReference.IndexOffset,
                .BaseVertexLocation = static_cast<INT>(meshReference.VertexOffset),
                .StartInstanceLocation = meshReference.FirstInstance
            };

            UpdateBufferWithStaging(device, context, m_VertexBuffer, meshReference.VertexOffset * vertices.GetStride(), vertices.GetData().GetData(), vertices.GetData().Size());
            UpdateBufferWithStaging(device, context, m_IndexBuffer, meshReference.IndexOffset * sizeof(uint32_t), mesh.GetIndices().GetData(), mesh.GetIndices().Size() * sizeof(uint32_t));
            UpdateBufferWithStaging(device, context, m_CommandBuffer, m_NextCommand * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS), &command, sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS));

            m_NextIndex += mesh.GetIndices().Size();
            m_NextVertex += vertices.GetData().Size() / vertices.GetStride();
            m_NextCommand++;

            m_MeshReferences[meshId] = meshReference;
        }
        else
        {
            // We need to update the mesh in the pipeline, this means that we need to regenerate the buffers.
            m_NextIndex = 0;
            m_NextVertex = 0;
            m_NextCommand = 0;

            for (auto& meshReference : std::ranges::views::values(m_MeshReferences))
            {
                const auto& meshData = meshReference.Name == name ? mesh : ResourceManager::Get().GetMesh(meshReference.Name);
                const VertexBuffer vertices = mesh.GetVertices().GenerateCompatible(*m_VertexBufferLayout);

                meshReference.IndexOffset = static_cast<uint32_t>(m_NextIndex);
                meshReference.VertexOffset = static_cast<uint32_t>(m_NextVertex);
                meshReference.CommandIndex = static_cast<uint32_t>(m_NextCommand);

                D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS command = {
                    .IndexCountPerInstance = static_cast<UINT>(meshData.GetIndices().Size()),
                    .InstanceCount = meshReference.InstanceCount,
                    .StartIndexLocation = meshReference.IndexOffset,
                    .BaseVertexLocation = static_cast<INT>(meshReference.VertexOffset),
                    .StartInstanceLocation = meshReference.FirstInstance
                };

                UpdateBufferWithStaging(device, context, m_VertexBuffer, meshReference.VertexOffset * vertices.GetStride(), vertices.GetData().GetData(), vertices.GetData().Size());
                UpdateBufferWithStaging(device, context, m_IndexBuffer, meshReference.IndexOffset * sizeof(uint32_t), meshData.GetIndices().GetData(), meshData.GetIndices().Size() * sizeof(uint32_t));
                UpdateBufferWithStaging(device, context, m_CommandBuffer, (m_NextCommand - 1) * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS), &command, sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS));

                m_NextIndex += meshData.GetIndices().Size();
                m_NextVertex += vertices.GetData().Size() / vertices.GetStride();
                m_NextCommand++;
            }
        }
    }

    uint32_t Pipeline::AddInstance(size_t meshId, InstanceBuffer buffer)
    {
        Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

        if (!m_MeshReferences.ContainsKey(meshId))
        {
            throw std::runtime_error("Mesh does not exist");
        }

        MeshReference& meshRef = m_MeshReferences[meshId];

        meshRef.InstanceCount++;

        
        const InstanceReference reference = {
            .Index = meshRef.InstanceCount - 1,
            .Data = buffer.GenerateCompatible(*m_InstanceBufferLayout)
		};

        meshRef.Instances.Insert(meshRef.NextInstanceID++, reference);

        if (meshRef.InstanceCount == 1)
        {
            meshRef.FirstInstance = static_cast<uint32_t>(m_NextInstance);
        }

        const auto& mesh = ResourceManager::Get().GetMesh(meshRef.Name);

        D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS command = {
			.IndexCountPerInstance = static_cast<UINT>(mesh.GetIndices().Size()),
			.InstanceCount = meshRef.InstanceCount,
			.StartIndexLocation = meshRef.IndexOffset,
			.BaseVertexLocation = static_cast<INT>(meshRef.VertexOffset),
			.StartInstanceLocation = meshRef.FirstInstance
		};

        UpdateBufferWithStaging(
            renderer.GetDevice(),
            renderer.GetContext(),
            m_CommandBuffer,
            meshRef.CommandIndex * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS),
            &command,
            sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS));

        if(meshId == m_LastInstanceMeshId || meshRef.InstanceCount == 1)
        {
            // We can just append the new instance to the end of the buffer.
            UpdateBuffer(
                renderer.GetDevice(),
                renderer.GetContext(),
                m_InstanceBuffer,
                m_NextInstance++ * reference.Data.GetStride(),
                reference.Data.GetData().GetData(),
                reference.Data.GetStride());

            m_LastInstanceMeshId = meshId;
        }
        else
        {
            m_NextInstance = 0;
            // We need to regenerate the instance buffer, and update the command buffer accordingly.
            for (auto& meshReference : std::ranges::views::values(m_MeshReferences))
            {
                List<uint8_t> instances ;
                instances.Resize(meshReference.InstanceCount * m_InstanceBufferLayout->GetStride());

                for (auto& instanceReference : std::ranges::views::values(meshReference.Instances))
				{
					memcpy(&instances[instanceReference.Index * instanceReference.Data.GetStride()], instanceReference.Data.GetData().GetData(), instanceReference.Data.GetStride());
                    instanceReference.Index = instances.Size() - 1;
				}

                meshReference.FirstInstance = static_cast<uint32_t>(m_NextInstance);

                UpdateBuffer(
					renderer.GetDevice(),
					renderer.GetContext(),
					m_InstanceBuffer,
					meshReference.FirstInstance * m_InstanceBufferLayout->GetStride(),
					instances.GetData(),
					instances.Size());

				D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS command = {
					.IndexCountPerInstance = static_cast<UINT>(mesh.GetIndices().Size()),
					.InstanceCount = meshReference.InstanceCount,
					.StartIndexLocation = meshReference.IndexOffset,
					.BaseVertexLocation = static_cast<INT>(meshReference.VertexOffset),
					.StartInstanceLocation = meshReference.FirstInstance
				};

				UpdateBufferWithStaging(
					renderer.GetDevice(),
					renderer.GetContext(),
					m_CommandBuffer,
					meshReference.CommandIndex * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS),
					&command,
					sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS));

				m_NextInstance += meshReference.InstanceCount;
                m_LastInstanceMeshId = meshId;
            }
        }

        return static_cast<uint32_t>(meshRef.NextInstanceID) - 1;
    }

    void Pipeline::UpdateInstance(size_t meshId, size_t instanceId, InstanceBuffer buffer)
    {
        Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

		if (!m_MeshReferences.ContainsKey(meshId))
		{
			throw std::runtime_error("Mesh does not exist");
		}

		MeshReference& mesh = m_MeshReferences[meshId];

		if (!mesh.Instances.ContainsKey(instanceId))
		{
			throw std::runtime_error("Instance does not exist");
		}

		mesh.Instances.Get(instanceId).Data = buffer.GenerateCompatible(*m_InstanceBufferLayout);

		UpdateBufferWithStaging(
			renderer.GetDevice(),
			renderer.GetContext(),
			m_InstanceBuffer,
			(mesh.FirstInstance + mesh.Instances.Get(instanceId).Index) * m_InstanceBufferLayout->GetStride() ,
			buffer.GetData().GetData(),
			m_InstanceBufferLayout->GetStride());
    }

    void Pipeline::UpdateConstantBuffer(InstanceBuffer buffer)
    {
		Renderer& renderer = static_cast<Renderer&>(Renderer::Get());

        InstanceBuffer compatible = buffer.GenerateCompatible(*m_ConstantBufferLayout);

        UpdateBufferWithStaging(
			renderer.GetDevice(),
			renderer.GetContext(),
			m_ConstantBuffer,
			0,
            compatible.GetData().GetData(),
            compatible.GetStride());
	}

    void Pipeline::UpdateBufferWithStaging(ID3D11Device* device, ID3D11DeviceContext* context, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, size_t start, const void* data, size_t size)
    {
        D3D11_BUFFER_DESC desc;
        buffer->GetDesc(&desc);

        if(desc.ByteWidth < start + size)
        {
            desc.ByteWidth = static_cast<UINT>(start + size);
            Microsoft::WRL::ComPtr<ID3D11Buffer> newBuffer;
            THROW_IF_FAILED(device->CreateBuffer(&desc, nullptr, &newBuffer));

            context->CopySubresourceRegion(newBuffer.Get(), 0, 0, 0, 0, buffer.Get(), 0, nullptr);

            buffer = newBuffer;
        }

        m_StagingBuffer->GetDesc(&desc);
        if(desc.ByteWidth < size)
        {
            D3D11_SUBRESOURCE_DATA subresourceData = {};
            subresourceData.pSysMem = data;

            desc.ByteWidth = static_cast<UINT>(size);
            THROW_IF_FAILED(device->CreateBuffer(&desc, &subresourceData, &m_StagingBuffer));
        }
        else
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            THROW_IF_FAILED(context->Map(m_StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &mappedSubresource));
            memcpy(mappedSubresource.pData, data, size);
            context->Unmap(m_StagingBuffer.Get(), 0);
        }

        const D3D11_BOX box = {
			.left = 0,
			.top = 0,
			.front = 0,
			.right = static_cast<UINT>(size),
			.bottom = 1,
			.back = 1
		};

        context->CopySubresourceRegion(buffer.Get(), 0, static_cast<UINT>(start), 0, 0, m_StagingBuffer.Get(), 0, &box);
    }

    void Pipeline::UpdateBuffer(ID3D11Device* device, ID3D11DeviceContext* context, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, size_t start, const void* data, size_t size)
    {
        D3D11_BUFFER_DESC desc;
        buffer->GetDesc(&desc);

        if (desc.ByteWidth < start + size)
        {
            desc.ByteWidth = static_cast<UINT>(start + size);
            Microsoft::WRL::ComPtr<ID3D11Buffer> newBuffer;
            THROW_IF_FAILED(device->CreateBuffer(&desc, nullptr, &newBuffer));

            context->CopySubresourceRegion(newBuffer.Get(), 0, 0, 0, 0, buffer.Get(), 0, nullptr);

            buffer = newBuffer;
        }

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        THROW_IF_FAILED(context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedSubresource));
        memcpy(&static_cast<uint8_t*>(mappedSubresource.pData)[start], data, size);
        context->Unmap(buffer.Get(), 0);
    }
}
