#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>
// Skeleton�� �о�� ������ ���������� ������ �ʱ� Ʈ�������� �����Ѵ�.
using namespace std;
namespace fbxsdk
{
    class FbxNode;
}
class ResourceManager;
class Skeleton
{
public:
    struct Bone
    {
        std::string name;
        DirectX::XMMATRIX initialTM;
        DirectX::XMMATRIX initialTM_inverse;
        vector<int> childIndices;
    };
    vector<Bone>& GetBones() { return bones; }
    unordered_map<string, unsigned int> boneIndexMap;
private:
    vector<Bone> bones;
    vector<fbxsdk::FbxNode*> fbxNodes;
    friend ResourceManager;
};