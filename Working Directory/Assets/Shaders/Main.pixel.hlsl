struct PSInput
{
    float4 Position : SV_POSITION;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

PSOutput main(PSInput input)
{
    PSOutput output;
    output.Color = float4(input.Position.xyz, 1.0);
    return output;
}