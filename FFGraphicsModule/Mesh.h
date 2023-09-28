#pragma once

#include "IResource.h"
#include "FBXLoader.h"

class Device;
class GraphicsCommandQueue;
class GraphicsDescriptorHeap;
class InstancingBuffer;
class StructuredBuffer;

struct FbxMeshInfo;

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		buffer;
	D3D12_INDEX_BUFFER_VIEW		bufferView;
	DXGI_FORMAT					format;
	uint32						count;
};

struct KeyFrameInfo
{
	double	time;
	int32	frame;
	Vec3	scale;
	Vec4	rotation;
	Vec3	translate;
};

struct BoneInfo
{
	wstring					boneName;
	int32					parentIdx;
	Matrix					matOffset;
};

struct AnimClipInfo
{
	wstring			animName;
	int32			frameCount;
	double			duration;
	vector<vector<KeyFrameInfo>>	keyFrames;
};

/// <summary>
/// ������Ʈ�� �Ž� �����͸� ������ �ִ� Ŭ����
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
class Mesh : public IResource
{
public:
	Mesh(Device* p_device, GraphicsCommandQueue* p_graphicsCommandQueue, GraphicsDescriptorHeap* p_graphicsDescriptorHeap);
	virtual ~Mesh();

public:
	// ���ؽ� ���ۿ� �ε��� ���۸� �����.
	void Init(vector<Vertex>& p_vertexBuffer, const vector<uint32>& p_indexBuffer);

	// �޽��� �׸���.
	void Render(bool p_isFinal = false, uint32 p_instanceCount = 1, uint32 idx = 0);
	void Render(InstancingBuffer* p_buffer, uint32 idx = 0);

	void CreateFromFBX(const FbxMeshInfo* meshInfo, FBXLoader& p_loader);

	void SetName(const wstring& p_name) { m_name = p_name; }
	wstring& GetName() { return m_name; }

	void SetBoundRadius(const float p_boundRadius) { m_boundRadius = p_boundRadius; }
	float GetBoundRadius() { return m_boundRadius; }

public:
	uint32 GetSubsetCount() { return static_cast<uint32>(m_vecIndexInfo.size()); }
	bool IsAnimMesh() { return !_animClips.empty(); }
	vector<AnimClipInfo>& GetAnimClipInfo() { return _animClips; }

	StructuredBuffer* GetBoneFrameDataBuffer(int32 index = 0) { return _frameBuffer[index]; } // ��ü �� ������ ����
	StructuredBuffer* GetBoneOffsetBuffer() { return  _offsetBuffer; }
	vector<BoneInfo>& GetBoneInfo() { return _bones; }

private:
	void CreateVertexBuffer(const vector<Vertex>& p_buffer);
	void CreateIndexBuffer(const vector<uint32>& p_buffer);

	void CreateBonesAndAnimations(class FBXLoader& loader);
	Matrix GetMatrix(FbxAMatrix& matrix);
	

private:
	ComPtr<ID3D12Resource>		m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView = {};
	uint32 m_vertexCount = 0;

	vector<IndexBufferInfo>		m_vecIndexInfo;

	wstring m_name;

	// �ִϸ��̼� ����
	// Animation
	vector<AnimClipInfo>			_animClips;
	vector<BoneInfo>				_bones;

	StructuredBuffer*	_offsetBuffer; // �� ���� offset ���
	vector<StructuredBuffer*> _frameBuffer; // ��ü �� ������ ����

	//DirectX::BoundingSphere bound;
	float m_boundRadius;

private:
	Device* m_device;
	GraphicsCommandQueue* m_graphicsCommandQueue;
	GraphicsDescriptorHeap* m_graphicsDescriptorHeap;
};

