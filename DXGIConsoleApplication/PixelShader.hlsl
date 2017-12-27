// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//----------------------------------------------------------------------

Texture2D tx : register( t0 );
SamplerState samLinear : register( s0 );

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
	//float4 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 tar;
    tar = tx.Sample( samLinear, input.Tex );
	int a = tar.r * 255;
	int b = tar.g * 255;
	int c = tar.b * 255;
	float y = ((66 * a + 129 * b + 25 * c + 128) / 256.0) + 16;
	float u = ((-38 * a - 74 * b + 112 * c + 128) / 256.0) + 128;
	float v = ((112 * a - 94 * b - 18 * c + 128) / 256.0) + 128;
	tar = ( y / 255.0 , u / 255.0, v / 255.0);
	return tar;
}