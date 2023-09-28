// �̹��� ��ƼŬ�� �Ϲ� ��ƼŬ�� �����ϱ� ���� ��ũ��
#define PT_EMITTER 0
#define PT_FLARE 1

// �� ������ ���ŵǴ� ��� ����
cbuffer cbPerFrame
{
	float3 gEyePosW;
	
	float3 gEmitPosW;
	float3 gEmitDirW;

	float gGameTime;
	float gTimeStep;
	
	float4x4 gViewProj;
	
	float gParticleInterval;
	float gParticleSize;
	float gParticleLife;
	
	float gRandomVectorIntensity;
	float gRandomVectorRange;
	
	float gAccelIntensity;
};

// ���� ���� ���� ��� ����
cbuffer cbFixed
{
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };
	
	float2 gQuadTexC[4] =
	{
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f),
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f)
	};
};

// ��ƼŬ �ؽ���
Texture2D gTexArray;

// ���÷� ������Ʈ
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// SO ���� ��ũ�п��� ���� ���۸� ��Ȱ��ȭ�ϱ� ���� DSS
DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

// �׸��� ��ũ�п��� ���� ���۸� Ȱ��ȭ�ϱ� ���� DSS
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS;
};

// ���� ������ ���� ���� ������Ʈ
BlendState AdditiveBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	BlendOpAlpha = SUBTRACT;
	RenderTargetWriteMask[0] = 0x0F;
};

// ���� ���� �Լ�
float random(float2 p)
{
	float2 K1 = float2(23.14069263277926, 2.665144142690225);
	return frac(cos(dot(p, K1)) * 12345.6789);
}


//////////////////////////////////////////////////////////////////////
/// ��Ʈ�� ��� ���� ��ũ��
//////////////////////////////////////////////////////////////////////

struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW : SIZE;
	float Age : AGE;
	uint Type : TYPE;
};

Particle StreamOutVS(Particle vin)
{
	return vin;
}

[maxvertexcount(2)]
void StreamOutGS(point Particle gin[1], inout PointStream<Particle> ptStream)
{
	gin[0].Age += gTimeStep; // TimeStep?
	
	// �̹��� ��ƼŬ�� ���
	if(gin[0].Type == PT_EMITTER)
	{
		if(gin[0].Age > gParticleInterval) // ��ƼŬ�� �����Ǵ� �ֱ�
		{
			// ���� ���ν�Ƽ�� �ֱ� ���� ������ 3���� ���͸� ����
			float2 s1 = float2(gGameTime + 0.1f, gGameTime - 0.1f);
			float2 s2 = float2(gGameTime - 0.1f, gGameTime + 0.1f);
			float2 s3 = float2(gGameTime + 0.5f, gGameTime + 0.5f);
			
			float3 vRandom = float3(random(s1) * 2 - 1, random(s2), random(s3));
			vRandom.x *= gRandomVectorRange;
			vRandom.z *= gRandomVectorRange;

			// ��ƼŬ�� ����� ��Ʈ�� ��� �ܰ�� �ѱ��
			Particle p;
			p.InitialPosW = gEmitPosW.xyz;
			p.InitialVelW = gRandomVectorIntensity * vRandom + gEmitDirW; // ��ƼŬ�� ���������� ����
			p.SizeW = float2(gParticleSize, gParticleSize); // ��ƼŬ ũ��
			p.Age = 0.0f;
			p.Type = PT_FLARE;
			
			// ��Ʈ�� ��� �ܰ踦 ����� ��ƼŬ���� SO�� VB�� ����
			ptStream.Append(p);
			
			// ��ƼŬ�� ������ �� Emitter ��ƼŬ�� Age �ʱ�ȭ
			gin[0].Age = 0.0f;
		}
		ptStream.Append(gin[0]);
	}
	// �Ϲ� ��ƼŬ�� ���
	else
	{
		// ��ƼŬ�� ����ִ� ���� ����Ѵ� (SO�� ���� VB�� �ѱ��)
		if(gin[0].Age <= gParticleLife) // ��ƼŬ ���� �ð�
		{
			ptStream.Append(gin[0]);
		}
	}
}

// ��Ʈ�� ������� ����ϱ� ���� ������Ʈ�� ���̴��� �����
GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, StreamOutGS()),
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x"
);

// ��Ʈ�� ��� ��ũ��
technique11 StreamOutTech
{
	pass P0
	{
		// ���ؽ� ���̴��� ������Ʈ�� ���̴��� �����Ѵ�
		SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
		SetGeometryShader(gsStreamOut);
		
		// ��Ʈ�� ����� ���� �ȼ� ���̴��� ���� ���۸� ��Ȱ��ȭ
		SetPixelShader(NULL);
		SetDepthStencilState(DisableDepth, 0);
	}
}

//////////////////////////////////////////////////////////////////////
/// �׸��� ��ũ��
//////////////////////////////////////////////////////////////////////

struct VertexOut
{
	float3 PosW : POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint Type : TYPE;
};

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;
	
	// ��ƼŬ�� ������ �� ���� �ð� (Age)
	float t = vin.Age;
	
	// ��ƼŬ�� �ʱ� ��ġ, �̵� �ӵ�, ����
	vout.PosW = gAccelIntensity * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW; 
	
	// ��ƼŬ�� �ð��� ���� ����������
	float opacity = 1.0f - (smoothstep(0.0f, 1.0f, t * 0.4f)) * 1.0f; // ����
	vout.Color = float4(1.0f, 1.0f, 1.0f, opacity);
	
	vout.SizeW = vin.SizeW;
	vout.Type = vin.Type;
	
	return vout;
}

struct GeoOut
{
	float4 posH : SV_Position;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{
	if(gin[0].Type != PT_EMITTER)
	{
		// ī�޶� ���ϴ� �����带 ����� ���� ���� ���
		float3 look = normalize(gEyePosW.xyz - gin[0].PosW); // ��ƼŬ�� ��ǥ���� ī�޶�� ���ϴ� ����
		float3 right = normalize(cross(float3(0, 1, 0), look)); // local up�� look�� ����
		float3 up = cross(look, right); // world up
		
		float halfWidth = 0.5f * gin[0].SizeW.x;
		float halfHeight = 0.5f * gin[0].SizeW.y;
		
		// �簢���� �����ϴ� ���ؽ� ���
		float4 v[4];
		v[0] = float4(gin[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
		v[1] = float4(gin[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
		v[2] = float4(gin[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
		v[3] = float4(gin[0].PosW - halfWidth * right + halfHeight * up, 1.0f);

		// point���� quad�� Ȯ��
		GeoOut gout;
		[unroll]
		for(int i = 0; i < 4; ++i)
		{
			gout.posH = mul(v[i], gViewProj);
			gout.Tex = gQuadTexC[i];
			gout.Color = gin[0].Color;
			triStream.Append(gout);
		}

	}
}

float4 DrawPS(GeoOut pin) : SV_Target
{
	// ������� quad�� �ؽ��ĸ� ���� ����Ѵ�
	return gTexArray.Sample(samLinear, pin.Tex) * pin.Color * 0.5f;
	//return pin.Color * 0.5f * float4(1.0f, 1.0f, 1.0f, 0.015f);
	//return float4(1.0f, 0.2f, 0.0f, 0.5f);

}
technique11 DrawTech
{
	pass P0
	{
		// ���̴� ����
		SetVertexShader(CompileShader(vs_5_0, DrawVS()));
		SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
		SetPixelShader(CompileShader(ps_5_0, DrawPS()));
		
		// ���� ������ ���� BlendState ����
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		
		// DSS ����
		SetDepthStencilState(EnableDepth, 0);
	}
}