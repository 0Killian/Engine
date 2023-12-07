//
// Created by Killian on 06/12/2023.
//
#include "D3D11/ShaderProgram.h"
#include "D3D11/Renderer.h"

#include <d3dcompiler.h>
#include <fstream>

#include "D3D11/Utils.h"

namespace NGN::D3D11
{
    ShaderProgram::ShaderProgram(const String& shaderName, Logger& logger, D3D11::Renderer& renderer, const Configuration& config)
    {
#ifdef NGN_DEBUG
        constexpr auto compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS;
#else
        constexpr auto compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

        Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        std::ifstream vertexShaderFile((config.BaseAssetFolder + "/Shaders/" + shaderName + ".vert.hlsl").GetData());
        if(!vertexShaderFile.is_open())
        {
            throw std::runtime_error((String("Failed to open vertex shader ") + shaderName).GetData());
        }
        std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());

        if(FAILED(D3DCompile2(
            vertexShaderSource.c_str(),
            vertexShaderSource.size(),
            (shaderName + ".vert.hlsl").GetData(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "main",
            "vs_5_0",
            compileFlags,
            0,
            0,
            nullptr,
            0,
            &m_VertexShaderBlob,
            &errorBlob)))
        {
            if(errorBlob)
            {
                throw std::runtime_error((String("Failed to compile vertex shader ") + shaderName + ":\n" + static_cast<char*>(errorBlob->GetBufferPointer())).GetData());
            }
            else
            {
                throw std::runtime_error((String("Failed to compile vertex shader ") + shaderName).GetData());
            }
        }

        std::ifstream pixelShaderFile((config.BaseAssetFolder + "/Shaders/" + shaderName + ".pixel.hlsl").GetData());
        if(!pixelShaderFile.is_open())
        {
            throw std::runtime_error((String("Failed to open pixel shader ") + shaderName).GetData());
        }
        std::string pixelShaderSource((std::istreambuf_iterator<char>(pixelShaderFile)), std::istreambuf_iterator<char>());

        logger.Info() << (config.BaseAssetFolder + "/Shaders/" + shaderName + ".pixel.hlsl").GetData() << Logger::EndLine;
        logger.Info() << pixelShaderSource.c_str() << Logger::EndLine;

        if(FAILED(D3DCompile2(
            pixelShaderSource.c_str(),
            pixelShaderSource.size(),
            (shaderName + ".pixel.hlsl").GetData(),
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
            if(errorBlob)
            {
                throw std::runtime_error((String("Failed to compile pixel shader ") + shaderName + ":\n" + static_cast<char*>(errorBlob->GetBufferPointer())).GetData());
            }
            else
            {
                throw std::runtime_error((String("Failed to compile pixel shader ") + shaderName).GetData());
            }
        }

        THROW_IF_FAILED(renderer.GetDevice()->CreateVertexShader(
            m_VertexShaderBlob->GetBufferPointer(),
            m_VertexShaderBlob->GetBufferSize(),
            nullptr,
            &m_VertexShader));

        THROW_IF_FAILED(renderer.GetDevice()->CreatePixelShader(
            pixelShaderBlob->GetBufferPointer(),
            pixelShaderBlob->GetBufferSize(),
            nullptr,
            &m_PixelShader));
    }

    void ShaderProgram::Bind(D3D11::Renderer& renderer)
    {
        renderer.GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
        renderer.GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    }
}
