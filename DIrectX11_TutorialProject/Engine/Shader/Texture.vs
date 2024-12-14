
// Globals //
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// 컬러 쉐이더와 다르게 색을 조정하지 않고 텍스처의 좌표를 사용하여 그림

// TYPEDEFS
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0; // 텍스처의 좌표
};

// Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    
	// 컬러쉐이더 떄와 동일
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// 차이
	output.tex = input.tex;

	return output;
}
