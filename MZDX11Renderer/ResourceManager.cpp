#include "ResourceManager.h"
#include "CASEParser.h"
#include "MeshData.h"
#include <stdexcept>
#include <queue>
#include <stack>
#include <numeric>
#include <algorithm>
//#include "FbxLoader.h"
#include "mathConversion.h"
#include "DDSTextureLoader.h"
//#include <fbxsdk/>
//#include "fbx"

MZRenderer::LazyObjects<ResourceManager> ResourceManager::Instance;


ResourceManager::ResourceManager()
{
    //fbxsdk::FbxManager* a;
    mFbxManager = fbxsdk::FbxManager::Create();
}
ResourceManager::~ResourceManager()
{
    for (auto it = m_meshObjectList.begin(); it != m_meshObjectList.end(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }

    if (mFbxManager) mFbxManager->Destroy();
}

MZMeshData::MeshData* ResourceManager::LoadMeshData(LPCSTR fileName)
{
    auto meshObjectListIter = m_meshObjectList.find(fileName);
    if (meshObjectListIter != m_meshObjectList.end())
    {
        return meshObjectListIter->second;
    }
    return nullptr;
}

void ResourceManager::LoadFile(LPCSTR fileName)
{
    std::string_view fileNameView{ fileName };
    std::string_view extension{ fileNameView.substr(fileNameView.find_last_of(".")) };

    if (extension == ".ase" || extension == ".ASE")
    {
        auto meshObjectListIter = m_meshObjectList.find(fileName);
        if (meshObjectListIter != m_meshObjectList.end())
            return;

        LoadASE(fileName);
    }
    else if (extension == ".fbx" || extension == ".FBX")
    {
        auto meshObjectListIter = m_meshObjectList.find(fileName);
        if (meshObjectListIter != m_meshObjectList.end())
            return;

        LoadFBX(fileName);
    }
    else if (extension == ".dds" || extension == ".DDS")
    {
        auto itr = m_textureSRVMap.find(fileName);
        if (itr != m_textureSRVMap.end())
            return;

        LoadDDS(fileName);
    }
    else
        return;
}
void ResourceManager::LoadDDS(LPCSTR fileName)
{
    ID3D11Resource* texResource = nullptr;

    int wstrSize = MultiByteToWideChar(CP_UTF8, 0, fileName, -1, nullptr, 0);
    std::wstring wstr(wstrSize, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, fileName, -1, &wstr[0], wstrSize);

    m_textureSRVMap[fileName] = nullptr;
    HR(CreateDDSTextureFromFile(DeferredRenderer::Instance.Get().GetDevice(),
        wstr.c_str(), &texResource, m_textureSRVMap[fileName].GetAddressOf()));
    SafeRelease(texResource);	// view saves reference
}
MZMeshData::MeshData* ResourceManager::GetMeshData(LPCSTR key)
{
    auto itr = m_meshObjectList.find(key);
    return itr == m_meshObjectList.end() ? nullptr : itr->second;
}
vector<YunuAnimation*> ResourceManager::GetYunuAnimations(FbxScene* scene, Skeleton* targetSkeleton)
{
    vector<YunuAnimation*> animations;
    //FbxArray<FbxString*> animStackNames;
    //scene->FillAnimStackNameArray(animStackNames);
    //string temp = animStackNames[0]->Buffer();
    auto animStackCount{ scene->GetMemberCount<FbxAnimStack>() };
    animations.resize(animStackCount);
    for (auto i = 0; i < animStackCount; i++)
    {
        YunuAnimation* animation = animations[i] = new YunuAnimation(targetSkeleton);
        auto animStack = scene->GetMember<FbxAnimStack>(i);
        animation->animationName = animStack->GetName();
        auto animLayer = animStack->GetMember<FbxAnimLayer>();
        if (animLayer)
        {
            // Get the start and end times of the animation
            FbxTime start = animStack->LocalStart;
            FbxTime end = animStack->LocalStop;
            animation->duration = static_cast<float>((end - start).GetSecondDouble());

            // Define the interval between keyframes (e.g., every 30 frames)
            FbxTime interval = FbxTime::GetOneFrameValue(FbxTime::eFrames30);

            // Evaluate the animation at keyframes within the specified interval
            FbxTime frameTime;
            auto root = scene->GetRootNode();
            for (frameTime = start; frameTime <= end; frameTime += interval)
            {
                // Go through all nodes and get their transform at the current keyframe
                for (int i = 0; i < targetSkeleton->fbxNodes.size(); i++)
                {
                    FbxAMatrix transformMatrix = targetSkeleton->fbxNodes[i]->EvaluateGlobalTransform(frameTime);
                    transformMatrix.GetQ();
                    YunuAnimation::BoneKeyFrame boneKeyFrame
                    {
                        .time = static_cast<float>(frameTime.GetSecondDouble()),
                            .localPosition = mathConversion::ToXMVector(transformMatrix.GetT().Buffer()),
                            .localRotation = mathConversion::ToXMVector(transformMatrix.GetQ().Buffer()),
                            .localScale = mathConversion::ToXMVector(transformMatrix.GetS().Buffer())
                    };
                    animation->bonesKeyFrames[i].push_back(boneKeyFrame);
                }
            }
        }
    }

    return animations;
}
Skeleton* ResourceManager::GetSkeletonData(LPCSTR key)
{
    auto itr = m_skeletonMap.find(key);
    return itr == m_skeletonMap.end() ? nullptr : itr->second.get();
}
ID3D11ShaderResourceView* ResourceManager::GetTextureSRV(LPCSTR key)
{
    auto itr = m_textureSRVMap.find(key);
    return itr == m_textureSRVMap.end() ? nullptr : itr->second.Get();
}
YunuAnimation* ResourceManager::GetYunuAnimation(LPCSTR key)
{
    auto itr = m_animationMap.find(key);
    return itr == m_animationMap.end() ? nullptr : itr->second.get();
}
void ResourceManager::LoadASE(LPCSTR fileName)
{
    string sFileName = fileName;
    CASEParser* _pASEParser = CreateASEParser();
    _pASEParser->Init();
    _pASEParser->Load(&sFileName[0]);

    //FbxLoader* _pFBXLoader = new FbxLoader();

    MZMeshData::MeshData* _meshData = new MZMeshData::MeshData;

    for (auto it = _pASEParser->m_boneList.begin(); it != _pASEParser->m_boneList.end(); it++)
    {
        _meshData->m_boneList.emplace(it->first, it->second);
    }

    int index = _pASEParser->m_MeshList.size();
    for (UINT i = 0; i < index; ++i)
    {
        MZMeshData::Mesh* mesh = new MZMeshData::Mesh;
        ASEParser::Mesh* _parserMesh = _pASEParser->GetMesh(i);

        mesh->m_timevalue = _parserMesh->m_timevalue;
        mesh->m_numbone = _parserMesh->m_numbone;
        mesh->m_mesh_numvertex = _parserMesh->m_mesh_numvertex;
        mesh->m_mesh_numfaces = _parserMesh->m_mesh_numfaces;
        mesh->m_mesh_numtvertex = _parserMesh->m_mesh_numtvertex;
        mesh->m_mesh_tvfaces = _parserMesh->m_mesh_tvfaces;
        mesh->m_nodename = _parserMesh->m_nodename;
        mesh->m_nodeparent = _parserMesh->m_nodeparent;
        mesh->m_isparentexist = _parserMesh->m_isparentexist;
        mesh->m_inherit_pos = _parserMesh->m_inherit_pos;
        mesh->m_inherit_rot = _parserMesh->m_inherit_rot;
        mesh->m_inherit_scl = _parserMesh->m_inherit_scl;
        mesh->m_tm_row0 = _parserMesh->m_tm_row0;
        mesh->m_tm_row1 = _parserMesh->m_tm_row1;
        mesh->m_tm_row2 = _parserMesh->m_tm_row2;
        mesh->m_tm_row3 = _parserMesh->m_tm_row3;
        mesh->m_tm_pos = _parserMesh->m_tm_pos;
        mesh->m_tm_rotaxis = _parserMesh->m_tm_rotaxis;
        mesh->m_tm_rotangle = _parserMesh->m_tm_rotangle;
        mesh->m_tm_scale = _parserMesh->m_tm_scale;
        mesh->m_tm_scaleaxis = _parserMesh->m_tm_scaleaxis;
        mesh->m_tm_scaleaxisang = _parserMesh->m_tm_scaleaxisang;

        for (UINT j = 0; j < _parserMesh->m_meshvertex.size(); ++j)
        {
            MZMeshData::Vertex* vertex = new MZMeshData::Vertex;
            vertex->m_pos = _parserMesh->m_meshvertex[j]->m_pos;
            vertex->m_normal = _parserMesh->m_meshvertex[j]->m_normal;
            vertex->u = _parserMesh->m_meshvertex[j]->u;
            vertex->v = _parserMesh->m_meshvertex[j]->v;
            vertex->m_bone_blenging_weights[0] = _parserMesh->m_meshvertex[j]->m_bone_blenging_weights[0];
            vertex->m_bone_blenging_weights[1] = _parserMesh->m_meshvertex[j]->m_bone_blenging_weights[1];
            vertex->m_bone_blenging_weights[2] = _parserMesh->m_meshvertex[j]->m_bone_blenging_weights[2];
            vertex->m_bone_blenging_weights[3] = _parserMesh->m_meshvertex[j]->m_bone_blenging_weights[3];
            vertex->m_bone_blending_indices[0] = _parserMesh->m_meshvertex[j]->m_bone_blenging_indices[0];
            vertex->m_bone_blending_indices[1] = _parserMesh->m_meshvertex[j]->m_bone_blenging_indices[1];
            vertex->m_bone_blending_indices[2] = _parserMesh->m_meshvertex[j]->m_bone_blenging_indices[2];
            vertex->m_bone_blending_indices[3] = _parserMesh->m_meshvertex[j]->m_bone_blenging_indices[3];
            vertex->m_indices = _parserMesh->m_meshvertex[j]->m_indices;
            mesh->m_meshvertex.emplace_back(vertex);
        }

        for (UINT j = 0; j < _parserMesh->m_meshface.size(); ++j)
        {
            MZMeshData::Face* face = new MZMeshData::Face;
            face->m_vertexindex[0] = _parserMesh->m_meshface[j]->m_vertexindex[0];
            face->m_vertexindex[1] = _parserMesh->m_meshface[j]->m_vertexindex[1];
            face->m_vertexindex[2] = _parserMesh->m_meshface[j]->m_vertexindex[2];
            face->m_normal = _parserMesh->m_meshface[j]->m_normal;
            face->m_normalvertex[0] = _parserMesh->m_meshface[j]->m_normalvertex[0];
            face->m_normalvertex[1] = _parserMesh->m_meshface[j]->m_normalvertex[1];
            face->m_normalvertex[2] = _parserMesh->m_meshface[j]->m_normalvertex[2];
            face->m_TFace[0] = _parserMesh->m_meshface[j]->m_TFace[0];
            face->m_TFace[1] = _parserMesh->m_meshface[j]->m_TFace[1];
            face->m_TFace[2] = _parserMesh->m_meshface[j]->m_TFace[2];
            mesh->m_meshface.emplace_back(face);
        }

        for (UINT j = 0; j < _parserMesh->m_mesh_tvertex.size(); ++j)
        {
            MZMeshData::TextureVertex* tvertex = new MZMeshData::TextureVertex;
            tvertex->m_u = _parserMesh->m_mesh_tvertex[j]->m_u;
            tvertex->m_v = _parserMesh->m_mesh_tvertex[j]->m_v;
            tvertex->m_w = _parserMesh->m_mesh_tvertex[j]->m_w;
            mesh->m_mesh_tvertex.emplace_back(tvertex);
        }

        for (UINT j = 0; j < _parserMesh->pos_sample.size(); ++j)
        {
            int _frame = _parserMesh->pos_sample[j].first;
            Vector3 _pos = _parserMesh->pos_sample[j].second;
            mesh->pos_sample.emplace_back(_frame, _pos);
        }

        for (UINT j = 0; j < _parserMesh->rot_sample.size(); ++j)
        {
            int _frame = _parserMesh->rot_sample[j].first;
            Vector4 _rot = _parserMesh->rot_sample[j].second;
            mesh->rot_sample.emplace_back(_frame, _rot);
        }

        _meshData->m_MeshList.emplace_back(mesh);
    }

    for (auto each : _meshData->m_MeshList)
        each->SplitAndGenerateVertexBuffer<VertexStruct::NormalMap>();

    m_meshObjectList.emplace(fileName, _meshData);
    ReleaseASEParser(_pASEParser);
    RegisterMeshData(fileName, _meshData);
}

void ResourceManager::LoadFBX(LPCSTR fileName)
{
    // ���̷��� �����͸� ���Ѵ�.
    // ���ؽ����� �����ϴ� �� ������ �ε����� �̹� ������ �ִٸ�,
    // �̹� ������ ��� �ε����� �°� ���̷��� �����͸� �����ؾ��Ѵ�.

    FbxImporter* mFbxImporter = FbxImporter::Create(mFbxManager, "");
    FbxScene* scene = nullptr;
    scene = FbxScene::Create(mFbxManager, "");
    mFbxImporter->Initialize(fileName, -1, mFbxManager->GetIOSettings());
    mFbxImporter->Import(scene);
    auto rootNode = scene->GetRootNode();
    // �ϳ��� FBX ���Ͽ��� �ִ� �� �ϳ��� �޽�, �ϳ��� ���̷���, �ϳ��� �ִϸ��̼Ǹ� ����ִٰ� �����Ѵ�.
    // Ű���� �����̸��̱� ������ ��¿���� ����.
    // �ϳ��� ���Ͽ� �� ������ ���ҽ��� guid�� �ο��ϰ�, ��Ÿ ������ ����� 
    // ���α׷� �ε嶧���� ���ҽ����� �ϰ��� gui�� ������ �����ϴ� ���� �� �̷��� �̾߱��̴�.
    // �޽ÿ� ���̷����� ��� ��Ʈ ����� �ڽ����� ����ִٰ� �����Ѵ�. ������ 2 �̻��� ���� �볳�� �� ����.
    bool meshLoaded{ false };
    bool skeletonLoaded{ false };
    bool animationLoaded{ false };

    int childCount = rootNode->GetChildCount();
    for (int i = 0; i < childCount; i++)
    {
        auto child = rootNode->GetChild(i);
        switch (child->GetNodeAttribute()->GetAttributeType())
        {
        case FbxNodeAttribute::eMesh:
            if (!meshLoaded) RegisterMeshData(fileName, GetMeshData(fileName, child));
            break;
            //case FbxNodeAttribute::eSkeleton:
                //if (!skeletonLoaded) RegisterSkeletonData(fileName, GetSkeletonData(child));
                //break;
        default:
            break;
        }
    }
    scene->Destroy();
    mFbxImporter->Destroy();
}
Skeleton* ResourceManager::GetSkeletonData(FbxNode* skeletalRoot)
{
    Skeleton* retData = new Skeleton();
    stack<FbxNode*> boneNodeStack;
    boneNodeStack.push(skeletalRoot);
    while (!boneNodeStack.empty())
    {
        auto top = boneNodeStack.top();
        if (top->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            auto mat = mathConversion::ToXMMatrix((top->EvaluateGlobalTransform().Double44()));
            Skeleton::Bone bone
            {
                .name = top->GetName(),
                    .initialTM = mat,
                    .initialTM_inverse = XMMatrixInverse(nullptr, mat),
                    .childIndices{ top->GetChildCount()},
            };
            // n��° ���� m���� �ڽ� ���� ������ ���� ���, n+1���� n+m������ �ε����� n��° ���� �ڽ� �ε����� �ȴ�.
            std::iota(bone.childIndices.begin(), bone.childIndices.end(), retData->bones.size() + 1);
            retData->boneIndexMap[bone.name] = retData->bones.size();
            retData->bones.push_back(bone);
            retData->fbxNodes.push_back(top);
        }
        // ť�� �ڽ� ���� �ִ´�.
        boneNodeStack.pop();
        for (auto i = top->GetChildCount() - 1; i >= 0; i--)
            boneNodeStack.push(top->GetChild(i));
    }
    return retData;
}
MZMeshData::MeshData* ResourceManager::GetMeshData(LPCSTR fileName, FbxNode* meshRoot)
{
    MZMeshData::MeshData* meshData{ new MZMeshData::MeshData() };
    stack<FbxNode*> nodes;
    nodes.push(meshRoot);
    bool isSkinnedData = false;
    Skeleton* loadedSkeleton = nullptr;
    vector<FbxNode*> bones;
    map<FbxNode*, int> boneIndexMap;

    while (!nodes.empty())
    {
        auto node = nodes.top();
        nodes.pop();
        for (int i = 0; i < node->GetChildCount(); i++)
            if (node->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
                nodes.push(node->GetChild(i));

        FbxMesh* mesh = node->GetMesh();
        if (mesh == nullptr)
            continue;
        fbxsdk::FbxVector4* vertices = mesh->GetControlPoints();

        // mesh ��� �ϳ��� mzMesh �ϳ��� �����ȴ�.
        auto mzMesh = new MZMeshData::Mesh();
        meshData->m_MeshList.push_back(mzMesh);

        // mesh�� ���ؽ� ����Ʈ ����
        mzMesh->m_meshvertex.resize(mesh->GetControlPointsCount(), nullptr);
        // ���ؽ� ��ǥ ����
        auto controlPoints = mesh->GetControlPoints();
        for (int i = 0; i < mesh->GetControlPointsCount(); i++)
        {
            mzMesh->m_meshvertex[i] = new MZMeshData::Vertex();
            mzMesh->m_meshvertex[i]->m_pos = DirectX::SimpleMath::Vector3(
                static_cast<float>(controlPoints[i].mData[0]),
                static_cast<float>(controlPoints[i].mData[1]),
                static_cast<float>(controlPoints[i].mData[2]));
        }

        // ���ؽ� ���̽��鿡 ���� ���� ����
        mzMesh->m_meshface.resize(mesh->GetPolygonCount(), nullptr);

        // ���̽��� ���ؽ� �ε��� ����
        auto faceIndices = mesh->GetPolygonVertices();

        for (int i = 0; i < mesh->GetPolygonCount(); i++)
            mzMesh->m_meshface[i] = new MZMeshData::Face();
        for (int i = 0; i < mesh->GetPolygonCount() * 3; i++)
        {
            mzMesh->m_meshface[i / 3]->m_vertexindex[i % 3] = faceIndices[i];
            //assert(mesh->GetPolygonSize(i / 3) == 3 && "�������� ���ؽ� ������ 3���� ��츸 �����ؼ� ���� fbx�δ� ����ε�, �������� ���ؽ� ����� 3���� ũ��.");
        }

        // ���̽��� ��� ����
        FbxArray<fbxsdk::FbxVector4> faceNormals;
        mesh->GetPolygonVertexNormals(faceNormals);
        for (int i = 0; i < mesh->GetPolygonCount() * 3; i++)
            mzMesh->m_meshface[i / 3]->m_normalvertex[i % 3] = DirectX::SimpleMath::Vector3(faceNormals[i].mData[0], faceNormals[i].mData[1], faceNormals[i].mData[2]);

        // ���̽��� UV Ÿ�� �̸�. normal map�̳� diffuse map ������ �ؽ�ó Ÿ�� �ĺ���
        // ���̽��� UV Ÿ�Կ� ���� UV ��ǥ ����.
        fbxsdk::FbxStringList UVSetNames;
        mesh->GetUVSetNames(UVSetNames);
        for (int i = 0; i < UVSetNames.GetCount(); i++)
        {
            mzMesh->m_texture_sets.push_back(UVSetNames.GetItemAt(i)->mString.Buffer());
            fbxsdk::FbxArray<fbxsdk::FbxVector2> uvs;
            mesh->GetPolygonVertexUVs(UVSetNames.GetItemAt(i)->mString.Buffer(), uvs);
            // UVSetNames�� ������ ���� �� �������� UVs�� ��ǥ ������ �����ȴ�.
            for (int j = 0; j < mzMesh->m_meshface.size()*3; j++)
                mzMesh->m_meshface[j / 3]->UVs[j % 3].push_back({
                static_cast<float>(uvs[j].mData[0]) ,
                static_cast<float>(uvs[j].mData[1]) });
        }

        // ��Ű�� ����
        // skinned mesh�Ͻ� then, static mesh�Ͻ� else�� ������.
        if (mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
        {
            FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));

            if (skin == nullptr)
                continue;

            int clusterCount = skin->GetClusterCount();
            if (clusterCount == 0)
                continue;

            isSkinnedData = true;

            if (loadedSkeleton == nullptr)
            {
                auto rootBone = skin->GetCluster(0)->GetLink();
                while (rootBone->GetParent()->GetNodeAttribute() && rootBone->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
                    rootBone = rootBone->GetParent();

                loadedSkeleton = GetSkeletonData(rootBone);
                meshData->skeletonData = unique_ptr<Skeleton>(loadedSkeleton);
                auto anims = GetYunuAnimations(meshRoot->GetScene(), loadedSkeleton);
                RegisterSkeletonData(fileName,loadedSkeleton);
                for (auto eachAnim : anims)
                    RegisterYunuAnimationData((string(fileName) +'-' + string(eachAnim->animationName)).c_str(), eachAnim);
            }

            // Cluster�� skinned mesh���� �ϳ��� ���� �����Ǵ� ���ؽ� ���̴�.
            // Cluster�� �ϳ��� ���̱� ������, n���� Ŭ�����Ͱ� vertex 0�� ����Ű�� ������
            // vertex 0�� n���� ���� �����Ͽ� �����δ�.
            for (int i = 0; i < clusterCount; ++i)
            {
                FbxCluster* cluster = skin->GetCluster(i);
                FbxNode* bone = cluster->GetLink();

                int controlPointIndicesCount = cluster->GetControlPointIndicesCount();
                int* controlPointIndices = cluster->GetControlPointIndices();
                double* controlPointWeights = cluster->GetControlPointWeights();

                for (int j = 0; j < controlPointIndicesCount; j++)
                {
                    int vertexIndex = controlPointIndices[j];
                    double weight = controlPointWeights[j];

                    // ���⼭ �ʿ��� ���� �����͸� �� ���� �� ������,
                    // ���� Ʈ�������� �̿��� ���ؽ��� ���� ���� ��ǥ�� ���� �� �ִ�.
                    UINT& boneCount = mzMesh->m_meshvertex[vertexIndex]->m_skinnedBoneCount;
                    assert("��Ű�� ���ؽ��� �������� ���� ������ ���α׷����� ���ǵ� ��Ű�� �� �ε��� �뷮�� �Ѿ�ϴ�!"
                        && boneCount < mzMesh->m_meshvertex[vertexIndex]->m_bone_blending_indices.size());

                    mzMesh->m_meshvertex[vertexIndex]->m_bone_blending_indices[boneCount] =
                        meshData->skeletonData->boneIndexMap[bone->GetName()];
                    mzMesh->m_meshvertex[vertexIndex]->m_bone_blenging_weights[boneCount] =
                        static_cast<float>(weight);
                    boneCount++;
                }
            }
        }
        else
        {

        }
    }

    if (isSkinnedData)
    {
        for (auto eachMesh : meshData->m_MeshList)
            eachMesh->SplitAndGenerateVertexBuffer<VertexStruct::PosNormalTexTanSkinned>();
    }
    else
    {
        for (auto eachMesh : meshData->m_MeshList)
            eachMesh->SplitAndGenerateVertexBuffer<VertexStruct::NormalMap>();
    }
    return meshData;
}
