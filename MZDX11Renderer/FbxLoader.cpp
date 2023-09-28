#include "FbxLoader.h"

FbxLoader* CreateFBXLoader()
{
	return new FbxLoader();
}

void ReleaseFBXLoader(FbxLoader* loader)
{
	delete loader;
	loader = nullptr;
}

FbxLoader::FbxLoader()
	: m_pManager(nullptr), m_pScene(nullptr)
{
	Initialize();
}

FbxLoader::~FbxLoader()
{
	Finalize();
}

void FbxLoader::Load(const char* fileName)
{
	FbxImporter* pImporter = FbxImporter::Create(m_pManager, "");

	pImporter->Initialize(fileName, -1, m_pManager->GetIOSettings());
	pImporter->Import(m_pScene);

	pImporter->Destroy();

	SetGeometryBuffers();
}

std::vector<MyVertex> FbxLoader::GetVertexList()
{
	return m_VertextList;
}

std::vector<unsigned int> FbxLoader::GetIndexList()
{
	return m_IndexList;
}

void FbxLoader::Initialize()
{
	// �Ŵ��� �����
	m_pManager = FbxManager::Create();

	// IOS �����ϱ�
	FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	pIOsettings->SetBoolProp(IMP_FBX_MATERIAL, true);
	m_pManager->SetIOSettings(pIOsettings);

	m_pScene = FbxScene::Create(m_pManager, "");
}

void FbxLoader::SetGeometryBuffers()
{
	FbxNode* pFbxRootNode = m_pScene->GetRootNode();
	auto rootName = pFbxRootNode->GetName();
	if (pFbxRootNode)
	{
		// ��� �ڽ� �޽����� ��ȸ (pMesh->GetPolygonCount()), ����� ù��° �ڽ� �޽��� ��ȸ
		//unsigned int nodeCount = pFbxRootNode->GetChildCount();
		unsigned int nodeCount = 1;
		for (unsigned int i = 0; i < nodeCount; i++)
		{
			// �ڽ� �޽��� ����Ŵ
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
			auto nodeName = pFbxChildNode->GetName();
			// ?
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
			// pMesh = ����Ű���ִ� �ڽ� �޽�
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			// �ڽ� �޽��� ��� Vertex���� ����ִ� ������
			FbxVector4* pVertexList = pMesh->GetControlPoints();

			// �븻 ������ ����
			FbxArray<FbxVector4> normals;
			pMesh->GetPolygonVertexNormals(normals);

			// UV ������ ����
			FbxStringList uvNames;
			pMesh->GetUVSetNames(uvNames);
			const char* uvSetName = uvNames.GetStringAt(0);
			FbxArray<FbxVector2> uv;
			pMesh->GetPolygonVertexUVs(uvSetName, uv);

			// pMesh->GetPolygonCount() -> ���̽� ���ڸ�ŭ ��ȸ
			int polygonCount = pMesh->GetPolygonCount();
			for (int j = 0; j < polygonCount; j++)
			{
				// pMesh->GetPolygonSize(j) -> j�� ���̽��� �̷���� ������ ���� (�ﰢ�� �������̹Ƿ� 3)
				int polygonTypeCount = pMesh->GetPolygonSize(j);
				for (int k = 0; k < polygonTypeCount; k++)
				{
					MyVertex vertex;
					m_VertextList.push_back(vertex);

					// pMesh->GetPolygonVertex(j, k) -> j�� ���̽��� �̷�� k��° ���ؽ��� �ε���
					int nowVertexIndex = pMesh->GetPolygonVertex(j, k);

					// �� ���̽��� �̷�� �ִ� ���ؽ��� ���� ��ġ ���� ���� ���� ���� ���ؽ� ����Ʈ ���Ϳ� �����
					m_VertextList[j * 3 + k].pos[0] = (float)pVertexList[nowVertexIndex].mData[0];
					m_VertextList[j * 3 + k].pos[1] = (float)pVertexList[nowVertexIndex].mData[1];
					m_VertextList[j * 3 + k].pos[2] = (float)pVertexList[nowVertexIndex].mData[2];
				
					m_VertextList[j * 3 + k].normal[0] = normals[nowVertexIndex][0];
					m_VertextList[j * 3 + k].normal[1] = normals[nowVertexIndex][1];
					m_VertextList[j * 3 + k].normal[2] = normals[nowVertexIndex][2];

					if (uv != nullptr)
					{
						m_VertextList[j * 3 + k].uv[0] = uv[j * 3 + k].mData[0];
						m_VertextList[j * 3 + k].uv[1] = uv[j * 3 + k].mData[1];
						isTexCoordExist = true;

					}
				}
			}

			// �ڽ� �޽��� ��� index���� ����ִ� ������
			//int* indexList = pMesh->GetPolygonVertices();
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				// ��� �ε��� ������ ������� ���� ���� �ε��� ����Ʈ ���Ϳ� �־���
				/*m_IndexList.push_back(indexList[3 * j]);
				m_IndexList.push_back(indexList[3 * j + 1]);
				m_IndexList.push_back(indexList[3 * j + 2]);*/
				m_IndexList.push_back(3 * j + 0);
				m_IndexList.push_back(3 * j + 1);
				m_IndexList.push_back(3 * j + 2);
			}
		}
	}
}

void FbxLoader::Finalize()
{
	std::vector<unsigned int> tempIntVec;
	m_IndexList.swap(tempIntVec);

	std::vector<MyVertex> tempVerTexVec;
	m_VertextList.swap(tempVerTexVec);

	m_pScene->Destroy();
	m_pManager->Destroy();
}


