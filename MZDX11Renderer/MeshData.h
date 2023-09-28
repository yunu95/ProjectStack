#pragma once
#include <vector>
#include <map>
#include <string>
#include <SimpleMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <array>
#include "DX11Define.h"
#include "Vertex.h"
#include "DeferredRenderer.h"
#include "Skeleton.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// ResourceManager�� Mesh Data���� �����ϱ� ���� ����ü��.
/// �����͵鿡 �������⼭ �����ؾ� �Ǳ� ������ 
/// �⺻������ public���� �����Ǿ� �ִ� ����ü�� �������.
/// 2023.07.27 MJKIM

namespace MZMeshData
{
    struct Vertex
    {
        Vertex();

        // ��ǥ���� ��ġ��
        Vector3 m_pos;

        // �븻��
        Vector3 m_normal;

        // �ؽ��� ��ǥ
        float u, v;

        /// Skinned Mesh ���� �߰�
        UINT m_skinnedBoneCount{ 0 };
        std::array<float, 4> m_bone_blenging_weights;
        std::array<UINT, 4> m_bone_blending_indices;

        // �ε���
        int m_indices;

    };

    struct Face
    {
        Face();

        // �� Face�� �̷�� ���ؽ��� �ε���
        int				m_vertexindex[3];
        // Face�� Normal��
        Vector3			m_normal;
        // Vertex�� Normal��
        Vector3			m_normalvertex[3];

        // Texture Coordinate
        int				m_TFace[3];

        // UV ��ǥ, diffuse map�� ���� ��ǥ�ϼ���, normal map�� ���� ��ǥ�ϼ��� ����.
        // n��° �ε����� �ش��ϴ� ���ؽ��� m��° �ؽ�ó�� �����Ǵ� UV ��ǥ�� UVs[n][m]�̴�.
        vector<Vector2> UVs[3];

    };

    struct TextureVertex
    {
        TextureVertex();

        float m_u, m_v, m_w;
    };

    struct Mesh
    {
        Mesh();
        ~Mesh();
        template<typename VertexStructType>
        void SplitAndGenerateVertexBuffer();
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_staticMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_staticMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_skinnedMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_skinnedMesh{nullptr};

        ///----------------------------------
        /// *MESH
        ///----------------------------------
        float m_timevalue;			// ���� ������ ���������� ���δ�. ��ư �ð���.
        int	m_numbone;			// ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
        int	m_mesh_numvertex;		// (���ؽ��� ��ǥ������)
        int	m_mesh_numfaces;		// (���ؽ��� �ε���)

        /// *MESH_VERTEX_LIST
        vector<Vertex*>	m_meshvertex;	// ���ؽ�
        /// *MESH_FACE_LIST
        vector<Face*> m_meshface;		// �ε���

        // �ؽ��Ŀ� �����͵�
        /// *MESH_NUMTVERTEX
        vector<TextureVertex*> m_mesh_tvertex;		// �ؽ��Ŀ� ���ؽ�
        int	m_mesh_numtvertex;		// �ؽ��Ŀ� ���ؽ� ����
        int	m_mesh_tvfaces;			// �ؽ��Ŀ� ���̽� ����
        vector<std::string> m_texture_sets; // �ؽ�ó�� ������ ���� �ĺ��ڵ� ex) normal, diffuse...

        ///----------------------------------
        /// *NODE_NAME
        ///---------------------------------
        std::string	m_nodename;
        std::string	m_nodeparent;
        bool m_isparentexist;

        ///----------------------------------
        /// *NODE_TM (Transform Matrix)
        ///----------------------------------
        Vector3	m_inherit_pos;
        Vector3	m_inherit_rot;
        Vector3	m_inherit_scl;
        Vector3	m_tm_row0;
        Vector3	m_tm_row1;
        Vector3	m_tm_row2;
        Vector3	m_tm_row3;
        Vector3	m_tm_pos;
        Vector3	m_tm_rotaxis;
        float m_tm_rotangle;
        Vector3	m_tm_scale;
        Vector3	m_tm_scaleaxis;
        float m_tm_scaleaxisang;

        ///----------------------------------
        /// Animation
        ///----------------------------------
        vector<pair<int, Vector3>> pos_sample;
        vector<pair<int, Vector4>> rot_sample;
    };

    struct MeshData
    {
        MeshData() {};
        ~MeshData();

        vector<Mesh*> m_MeshList;
        std::map<string, int> m_boneList;
        unique_ptr<Skeleton> skeletonData;

        Mesh* GetMesh(UINT index)
        {
            return m_MeshList[index];
        }
    };
}

//namespace MZFBXMeshData
//{
//	struct Vertex
//	{
//		//Vertex();
//
//		Vector3 pos;
//		Vector3 normal;
//		Vector2 uvCoord;
//	};
//	
//	struct Mesh
//	{
//		std::vector<Vertex> m_vertexList;
//		std::vector<UINT> m_indicesList;
//	};
//
//	// Mesh�� ���� �� ���� �� �ִµ�, �� Mesh���� ���ͷ� �����ϰ� �ִ�.
//	struct MeshData
//	{
//		MeshData() {};
//		~MeshData();	
//
//		vector<Mesh*> m_MeshList;
//
//		Mesh* GetMesh(UINT index)
//		{
//			return m_MeshList[index];
//		}
//	};
//}

