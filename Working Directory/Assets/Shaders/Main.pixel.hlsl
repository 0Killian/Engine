struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

PSOutput main(PSInput input)
{
    PSOutput output;
    output.Color = float4(input.Color, 1.0);
    return output;
}