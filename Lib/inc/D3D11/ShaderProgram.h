//
// Created by Killian on 06/12/2023.
//
#pragma once
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl.h>

#include "Configuration.h"
#include "Logger.h"

namespace NGN::D3D11
{
    class Renderer;

    class ShaderProgram
    {
    public:
        ShaderProgram(const String& shaderName, Logger& logger, D3D11::Renderer& renderer, const Configuration& config);

        [[nodiscard]] inline ID3DBlob* GetVertexShaderBlob() const { return m_VertexShaderBlob.Get(); }

        void Bind(D3D11::Renderer& renderer);

    private:
        Microsoft::WRL::ComPtr<ID3DBlob> m_VertexShaderBlob;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

    };
}
