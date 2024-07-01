struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
};

struct Camera
{
    float32_t4x4 view;
    float32_t4x4 projection;
    
    float32_t4x4 sview;
    float32_t4x4 sprojection;
    
    float32_t3 worldPosition;
};

ConstantBuffer<Camera> gCamera : register(b1);