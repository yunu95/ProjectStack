#pragma once
#include "../Include/rapidjson/prettywriter.h" // for stringify JSON
#include "../Include/rapidjson/reader.h" // for stringify JSON
#include "../Include/rapidjson/document.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <vector>
#include <rpc.h>
#include <rpcdce.h>
#include <iostream>
#include <functional>
#include <map>
#include <concepts>
#include <utility>
#include <fstream>
#include <unordered_map>
#pragma comment(lib,"Rpcrt4.lib")

using namespace rapidjson;
using namespace std;

/// <summary>
/// �������� Serialize �ʰԳ��� �۾� ����
/// 2023.08.17
/// </summary>
namespace Bjson
{
	class Serialize
	{
	public:
		Serialize();
		~Serialize();

		//���� �����Ǿ��� �� ����ȭ �� �Լ�
		void SceneSerialize(const std::string& name);

		//������Ʈ�� �����Ǿ��� �� ����ȭ �� �Լ�
		void ObjectSerialize(Document& doc, const std::string& name);

		void LoadSerializeDataToFile(StringBuffer& buffer, const char* filePath);

	private:
		/// <summary>
		/// ������ �����͸� ���� �������� �������
		/// </summary> 
		std::unordered_map<Value*, std::string> sceneDataList;
		std::vector<Value*> objectDataList;
	};
}