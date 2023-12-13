struct VSInput
{
    float3 position : POSITION;
    
    float4x4 model : INSTANCE_MODEL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 View;
    float4x4 Projection;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(mul(float4(input.position, 1.0f), input.model), mul(View, Projection));
    return output;
}
