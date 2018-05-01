/************* Resources *************/

cbuffer CBufferPerFrame
{
    float4x4 View : VIEW;
    float4x4 Projection : PROJECTION;

}

/************* Data Structures *************/

struct Particle
{
    int vertexStart : VERSTART;
    int vertexNum : VERNUM;
    float4 color :COLOR;
    row_major float4x4 world : WORLD;
};
struct GeoOut
{
    float4 postion : SV_Position;
    float4 color : COLOR;
};

Buffer<float3> Vertexes;
Texture2D<float4> OffScreen;


RasterizerState DisableCulling
{
    CullMode = NONE;
};

DepthStencilState DisableDetpth
{
    DepthEnable = false;
    StencilEnable = false;

};

BlendState Light
{
    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = ZERO;
    DestBlendAlpha[0] = ZERO;
    BlendOpAlpha[0] = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};
/************* Vertex Shader *************/

Particle vertex_shader(Particle IN)
{    
    return IN;
}


/************* Geometry Shader *************/
[maxvertexcount(32)]
void geo_shader(point Particle gin[1],inout TriangleStream<GeoOut> triStream)
{

    Particle particle = gin[0];
    
    float4x4 wv = mul(particle.world, View);
    
    
    for (uint i = 0; i < particle.vertexNum ; i+=3)
    {
        GeoOut vertex;
        vertex.color = particle.color;
        int index = i + particle.vertexStart;
        float4 viewPts[3];
        [unroll]
        for (int j = 0; j < 3;j++)
        {
            float4 position = float4(Vertexes[index+j], 1.0);
            viewPts[j] = mul(position, wv);
        }
        float3 v1 = viewPts[1].xyz - viewPts[0].xyz;   //w = 1,so no need to div
        float3 v2 = viewPts[2].xyz - viewPts[0].xyz;
        float3 cv = normalize(cross(v1, v2));
        float3 center = normalize(viewPts[0].xyz + viewPts[1].xyz + viewPts[2].xyz);
        float d = abs(dot(cv, center));
        vertex.color.a *= d;
        //vertex.color.a = d;
        triStream.RestartStrip();
        [unroll]
        for (uint j = 0; j < 3;j++)
        {
            vertex.postion = mul(viewPts[j], Projection);
            triStream.Append(vertex);
        }
        
            
        
    }
}
/************* Pixel Shader *************/

float4 pixel_shader(GeoOut In) : SV_Target
{
    float4 color = In.color;
    
    //color.rgb *= color.a;
    //color.a = 1.0;
    return color;
}

float4 post_vs(float2 position:POSITION):SV_Position
{
    return float4(position, 0.5, 1);
}

float4 post_ps(float4 pos : SV_Position) : SV_Target
{
    uint2 index = uint2(pos.xy);
    return 1.0f-OffScreen[index];
}


/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(CompileShader(gs_5_0, geo_shader()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
        SetRasterizerState(DisableCulling);
        SetDepthStencilState(DisableDetpth, 0);
        SetBlendState(Light, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
    }

    pass p1
    {
        SetVertexShader(CompileShader(vs_5_0, post_vs()));
        SetPixelShader(CompileShader(ps_5_0, post_ps()));
    }
}