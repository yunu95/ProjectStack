#pragma once

// FBX���� �̾Ƴ� Material������ ���� ����ü
struct FbxMaterialInfo
{
	Vec4			diffuse;
	Vec4			ambient;
	Vec4			specular;
	wstring			name;
	wstring			diffuseTexName;
	wstring			normalTexName;
	wstring			specularTexName;
};

// ��Ű�� �ִϸ��̼��� �� �� �ʿ��� �� �ε���, ����ġ�� �����ϴ� ����ü
struct BoneWeight
{
	// �ε���, ����ġ
	using Pair = pair<int32, double>;
	vector<Pair> boneWeights;

	void AddWeights(uint32 index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) { return p.second < weight; });

		if (findIt != boneWeights.end())
			boneWeights.insert(findIt, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		// ����ġ�� �ִ� 4��
		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

// FBX���� �ε��� Mesh ���� 
struct FbxMeshInfo
{
	wstring								name;			// �޽� �̸�
	vector<Vertex>						vertices;		// ���ؽ� �迭
	vector<vector<uint32>>				indices;		// ���� �ε��� �迭
	vector<FbxMaterialInfo>				materials;		// ���� �޽��� ����� ���׸��� ����
	vector<BoneWeight>					boneWeights;	// �� ����ġ
	bool								hasAnimation;	// �ִϸ��̼��� �ִ� �޽�����
	float								boundRadius;	// �ٿ�� ������
};

// FBX���� �ε��� �ִϸ��̼� ������ ����
struct FbxKeyFrameInfo
{
	FbxAMatrix  matTransform;	// ���� �������� WTM
	double		time;			// �� ����������
};

// FBX���� �ε��� �� ����
struct FbxBoneInfo
{
	wstring					boneName;		// �� �̸�
	int32					parentIndex;	// �θ� ���� �ε���
	FbxAMatrix				matOffset;		// Offset WTM
};

// FBX���� �ε��� �ִϸ��̼� Ŭ�� ����
struct FbxAnimClipInfo
{
	wstring			name;							// �ִϸ��̼� �̸�
	FbxTime			startTime;						// ���� �ð�
	FbxTime			endTime;						// ������ �ð�
	FbxTime::EMode	mode;		

	// Bone0	Bone1	Bone2...
	// Mat		Mat		Mat...
	// Mat		Mat		Mat...
	vector<vector<FbxKeyFrameInfo>>	keyFrames;		// ���������� ���� �迭
};

class Device;
class ConstantBuffer;
class GraphicsDescriptorHeap;
class ComputeDescriptorHeap;
class ComputeCommandQueue;


class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

public:
	void Init(Device* p_device, vector<ConstantBuffer*>* p_constantBuffers, GraphicsDescriptorHeap* p_graphicsDescriptorHeap, ComputeDescriptorHeap* p_computeDescriptorHeap, ComputeCommandQueue* p_computeCommandQueue);

	void LoadFbx(const wstring& path, const wstring& p_shaderPath);

public:
	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	const FbxMeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }
	vector<FbxBoneInfo*>& GetBones() { return _bones; }
	vector<FbxAnimClipInfo*>& GetAnimClip() { return _animClips; }

private:
	// ����Ʈ �� �� FBXManager, Scene, Texture���, Importer �����.
	void Import(const wstring& path);

	// ���ڷ� ���� ��忡 ���Ѱ� �Ľ��Ѵ�.
	// ���⼭ Mesh�� Material�� �ε��Ѵ�.
	void ParseNode(FbxNode* root);

	// ParseNode���� ���̴� �Լ� 
	// ���⼭�� ���ؽ��� Position, Normal, UV, �ε��������� �ε��Ѵ�.
	void LoadMesh(FbxMesh* mesh);

	// ���⼭ Material������ �ε��Ѵ�.
	void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	void		GetNormal(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	void		GetTangent(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	void		GetUV(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	Vec4		GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	wstring		GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);

	void CreateTextures();
	void CreateMaterials();

	// Animation
	void LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	void LoadBones(FbxNode* node, int32 idx, int32 parentIdx);
	void LoadAnimationInfo(FbxMesh* mesh);

	void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
	void LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo);
	void LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* container);

	int32 FindBoneIndex(string name);
	FbxAMatrix GetTransform(FbxNode* node);

	void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

private:
	FbxManager*		_manager = nullptr;
	FbxScene*		_scene = nullptr;
	FbxImporter*	_importer = nullptr;
	wstring			_resourceDirectory;

	vector<FbxMeshInfo>					_meshes;
	vector<FbxBoneInfo*>				_bones;
	vector<FbxAnimClipInfo*>			_animClips;
	FbxArray<FbxString*>				_animNames;
	map<int, vector<Vertex>>			_originVertexIndex;

private:
	// ������ ������ 
	wstring m_shaderPath;
	Device* m_device;
	vector<ConstantBuffer*>* m_constantBuffers;
	GraphicsDescriptorHeap* m_graphicsDescriptorHeap;
	ComputeDescriptorHeap* m_computeDescriptorHeap;
	ComputeCommandQueue* m_computeCommandQueue;
};

