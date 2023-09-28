#include "CASEParser.h"


CASEParser* CreateASEParser()
{
	return new CASEParser();
}

void ReleaseASEParser(CASEParser* parser)
{
	delete parser;
	parser = nullptr;
}


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}


CASEParser::~CASEParser()
{
	delete m_lexer;
}


bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}

//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------

bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}


/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}


bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
	unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face�������� ���ؽ��� �븻�� �־��ش�.
			// (���ÿ�)
			_nowVertex->m_normal.x = 0;
			_nowVertex->m_normal.y = 0;
			_nowVertex->m_normal.z = 0;
		}
	}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}


ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

//----------------------------------------------------------------
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;
		static int currentVertexIndex;
		static int weightIndex;
		static int currentBoneIndex;

		switch (nowtoken)
		{
			case TOKEND_BLOCK_START:

				Parsing_DivergeRecursiveALL(depth++);
				break;

				//--------------------
				// 3DSMAX_ASCIIEXPORT
				//--------------------

			case TOKENR_3DSMAX_ASCIIEXPORT:
				m_data_asciiexport = Parsing_NumberLong();
				break;

				//--------------------
				// COMMENT
				//--------------------

			case TOKENR_COMMENT:
				Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
				break;
			case TOKENR_SCENE_FIRSTFRAME:
				m_scenedata.m_firstframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_LASTFRAME:
				m_scenedata.m_lastframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_FRAMESPEED:
				m_scenedata.m_framespeed = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_TICKSPERFRAME:
				m_scenedata.m_ticksperframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_MESHFRAMESTEP:
				m_scenedata.m_meshframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_KEYFRAMESTEP:
				m_scenedata.m_keyframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_BACKGROUND_STATIC:
				m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
				break;
			case TOKENR_SCENE_AMBIENT_STATIC:
				m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
				break;

			case TOKENR_SCENE_ENVMAP:
			{
				// �׳� ���� ������ �о������ }�� ���ö�����
				while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
				}
			}
			break;

			//--------------------
			// MATERIAL_LIST
			//--------------------
			case TOKENR_MATERIAL_LIST:
				break;
			case TOKENR_MATERIAL_COUNT:
				m_materialcount = Parsing_NumberInt();
				break;
			case TOKENR_MATERIAL:
			{
				Create_materialdata_to_list();
				m_list_materialdata.back()->m_materialnumber = Parsing_NumberInt();
			}
			break;
			case TOKENR_MAP_DIFFUSE:
			{
				Parsing_String(); // MAP_DIFFUSE
				Parsing_String(); // MAP_NAME
				Parsing_String(); // " "
				Parsing_String(); // MAP_CLASS
				Parsing_String(); // " "
				Parsing_String(); // MAP_SUBNO
				Parsing_NumberInt();
				Parsing_String(); // MAP_AMOUNT
				Parsing_NumberFloat();
				Parsing_String(); // BITMAP
				m_list_materialdata.back()->m_map_diffuse = new MaterialMap();
				m_list_materialdata.back()->m_map_diffuse->m_bitmap = Parsing_String();
				Parsing_String(); // MAP_TYPE
				Parsing_String(); // Screen
				Parsing_String(); // Screen
				Parsing_String(); // UVW_U_OFFSET
				Parsing_NumberFloat();
				Parsing_String(); // UVW_V_OFFSET
				Parsing_NumberFloat();
				Parsing_String(); // UVW_U_TILING
				Parsing_NumberFloat();
				Parsing_String(); // UVW_V_TILING
				Parsing_NumberFloat();
				Parsing_String(); // UVW_ANGLE
				Parsing_NumberFloat();
				Parsing_String(); // UVW_BLUR
				Parsing_NumberFloat();
				Parsing_String(); // UVW_BLUR_OFFSET
				Parsing_NumberFloat();
				Parsing_String(); // UVW_NOUSE_AMT
				Parsing_NumberFloat();
				Parsing_String(); // UVW_NOISE_SIZE
				Parsing_NumberFloat();
				Parsing_String(); // UVW_NOISE_LEVEL
				Parsing_NumberInt();
				Parsing_String(); // UVW_NOISE_PHASE
				Parsing_NumberFloat();
				Parsing_String(); // BITMAP_FILTER
				Parsing_String();
				//Parsing_String();
			}
			break;

			//--------------------
			// GEOMOBJECT
			//--------------------

			case TOKENR_GROUP:
				//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
				break;

			case TOKENR_HELPEROBJECT:
				// �ϴ� �����ϰ�
				// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
				Create_onemesh_to_list();

				break;

			case TOKENR_GEOMOBJECT:
				/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?			
				Create_onemesh_to_list();
				break;

			case TOKENR_SHAPEOBJECT:
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
				// ��忡 ���� �־�� �� ���� �ٸ���.			
				m_OneMesh->m_nodename = Parsing_String();
				/*if (m_boneList.find(m_OneMesh->m_nodename) != m_boneList.end())
				{
					m_OneMesh->m_bone = new Bone;
					m_OneMesh->m_bone->m_bone_name = m_OneMesh->m_nodename;
				}*/
				break;

			case TOKENR_NODE_PARENT:
				// �� ����� �θ� ����� ����.
				// �ϴ� �Է��� �ϰ�, ���߿� ��������.
				m_OneMesh->m_nodeparent = Parsing_String();
				m_OneMesh->m_isparentexist = true;
				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				//m_parsingmode	=	eGeomobject;
				// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
				// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

				/// �Դٰ�,
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
				// ��ͱ����� �������̴�....				


				break;

			case TOKENR_INHERIT_POS:
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_ROT:
				m_OneMesh->m_inherit_rot.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_SCL:
				m_OneMesh->m_inherit_scl.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW0:
				m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW1:
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW2:
				m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW3:
				m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_POS:
				m_OneMesh->m_tm_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTAXIS:
				m_OneMesh->m_tm_rotaxis.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTANGLE:
				m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALE:
				m_OneMesh->m_tm_scale = Parsing_NumberVector3();
				break;
			case TOKENR_TM_SCALEAXIS:
				m_OneMesh->m_tm_scaleaxis = Parsing_NumberVector3();
				break;
			case TOKENR_TM_SCALEAXISANG:
				// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
				m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
				break;

				/// MESH
			case TOKENR_MESH:
				break;
			case TOKENR_TIMEVALUE:
				m_OneMesh->m_timevalue = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMBONE:
				// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
				m_OneMesh->m_numbone = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMSKINWEIGHT:
				break;
			case TOKENR_MESH_NUMVERTEX:
				m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMFACES:
				m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
				break;

				/// MESH_VERTEX_LIST

			case TOKENR_MESH_VERTEX_LIST:
				//
				// ���ؽ��� ������ ����־�� �ϴµ�
				// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
				break;
			case TOKENR_MESH_VERTEX:
			{
				// ������ �Է�
				Create_onevertex_to_list();
				listcount = Parsing_NumberInt();
				m_OneMesh->m_meshvertex[listcount]->m_indices = listcount;
				m_OneMesh->m_meshvertex[listcount]->m_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_meshvertex[listcount]->m_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_meshvertex[listcount]->m_pos.y = Parsing_NumberFloat();
			}
			break;

			/// Bone

			case TOKENR_SKIN_INITTM:
				break;
			case TOKENR_BONE_LIST:
				break;
			case TOKENR_BONE:
			{
				/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
				// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
				// Bone�� �ѹ��� �о� ����
				currentBoneIndex = Parsing_NumberInt();
			}
			break;
			//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
			case TOKENR_BONE_NAME:
			{
				string boneName = Parsing_String();
				m_boneList.emplace(boneName, currentBoneIndex);
			}
				break;
			case TOKENR_BONE_PROPERTY:
				// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
				break;
				// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

			case TOKENR_MESH_WVERTEXS:
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
				currentVertexIndex = Parsing_NumberInt();
				weightIndex = 0;
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				m_OneMesh->m_meshvertex[currentVertexIndex]->m_bone_blenging_indices[weightIndex] = Parsing_NumberInt();
				m_OneMesh->m_meshvertex[currentVertexIndex]->m_bone_blenging_weights[weightIndex] = Parsing_NumberFloat();
				weightIndex++;
				if (weightIndex >= 4)
					weightIndex = 0;
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				//
				break;
			case TOKENR_MESH_FACE:
			{
				Create_oneface_to_list();
				// Face�� ��ȣ�ε�...
				listcount = Parsing_NumberInt();
				// A:�� �а�
				Parsing_String();
				m_OneMesh->m_meshface[listcount]->m_vertexindex[0] = Parsing_NumberInt();
				// B:
				Parsing_String();
				m_OneMesh->m_meshface[listcount]->m_vertexindex[2] = Parsing_NumberInt();
				// C:
				Parsing_String();
				m_OneMesh->m_meshface[listcount]->m_vertexindex[1] = Parsing_NumberInt();

				/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
				/// ......

				// ���Ϳ� �־��ش�.
			}
			break;

			case TOKENR_MESH_MAPPINGCHANNEL:
				meshTVertFlag = false;
				break;

			case TOKENR_MESH_NUMTVERTEX:
				m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
				break;

				/// MESH_TVERTLIST
			case TOKENR_MESH_TVERTLIST:
				//				
				break;
			case TOKENR_MESH_TVERT:
			{
				// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
				// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
				if (meshTVertFlag)
				{
					Create_oneTVertex_to_list();
					listcount = Parsing_NumberInt();
					m_OneMesh->m_mesh_tvertex[listcount]->m_u = Parsing_NumberFloat();
					m_OneMesh->m_mesh_tvertex[listcount]->m_v = Parsing_NumberFloat();
					m_OneMesh->m_mesh_tvertex[listcount]->m_w = Parsing_NumberFloat();
				}

			}
			break;

			/// MESH_TFACELIST
			case TOKENR_MESH_NUMTVFACES:
				m_OneMesh->m_mesh_tvfaces = Parsing_NumberInt();
				break;
			case TOKENR_MESH_TFACELIST:
				break;
			case TOKENR_MESH_TFACE:
			{
				listcount = Parsing_NumberInt();
				m_OneMesh->m_meshface[listcount]->m_TFace[0] = Parsing_NumberInt();
				m_OneMesh->m_meshface[listcount]->m_TFace[2] = Parsing_NumberInt();
				m_OneMesh->m_meshface[listcount]->m_TFace[1] = Parsing_NumberInt();
			}
			break;

			/// MESH NORMALS
			case TOKENR_MESH_NORMALS:
				break;
			case TOKENR_MESH_FACENORMAL:
			{
				listcount = Parsing_NumberInt();
				m_OneMesh->m_meshface[listcount]->m_normal.x = Parsing_NumberFloat();
				m_OneMesh->m_meshface[listcount]->m_normal.z = Parsing_NumberFloat();
				m_OneMesh->m_meshface[listcount]->m_normal.y = Parsing_NumberFloat();

				for (int i = 0; i < 3; ++i)
				{
					Parsing_String();
					Parsing_NumberInt();
					m_OneMesh->m_meshface[listcount]->m_normalvertex[i].x = Parsing_NumberFloat();
					m_OneMesh->m_meshface[listcount]->m_normalvertex[i].z = Parsing_NumberFloat();
					m_OneMesh->m_meshface[listcount]->m_normalvertex[i].y = Parsing_NumberFloat();
				}

			}
			break;
			case TOKENR_MESH_VERTEXNORMAL:
				// TOKENR_MESH_FACENORMAL���� �Ѳ����� ó����.
				break;

				/// TM_ANIMATION
			case TOKENR_TM_ANIMATION:
				break;

			case TOKENR_CONTROL_POS_TRACK:
				break;

			case TOKENR_CONTROL_POS_SAMPLE:
			{
				int keyframe = Parsing_NumberInt();
				float tempX = Parsing_NumberFloat();
				float tempZ = Parsing_NumberFloat();
				float tempY = Parsing_NumberFloat();
				Vector3 tempVec(tempX, tempY, tempZ);
				m_OneMesh->pos_sample.push_back(make_pair(keyframe, tempVec));
			}
			break;

			case TOKENR_CONTROL_ROT_TRACK:
				break;

			case TOKENR_CONTROL_ROT_SAMPLE:
			{
				int keyframe = Parsing_NumberInt();
				float tempX = Parsing_NumberFloat();
				float tempZ = Parsing_NumberFloat();
				float tempY = Parsing_NumberFloat();
				float tempW = Parsing_NumberFloat();
				Vector4 tempVec(tempX, tempY, tempZ, tempW);
				m_OneMesh->rot_sample.push_back(make_pair(keyframe, tempVec));
			}
			break;

			case TOKEND_END:
				// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

				//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
				TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
				return;

				break;

				/// ���� �ƹ��͵� �ش����� ������
			default:
				// �ƹ��͵� ���� �ʴ´�.
				break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

void CASEParser::Create_oneface_to_list()
{
	Face* temp = new Face;
	m_OneMesh->m_meshface.push_back(temp);
}

void CASEParser::Create_oneTVertex_to_list()
{
	COneTVertex* temp = new COneTVertex;
	m_OneMesh->m_mesh_tvertex.push_back(temp);
}
