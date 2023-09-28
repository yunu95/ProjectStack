#include "Serialize.h"

Bjson::Serialize::Serialize()
{
}

Bjson::Serialize::~Serialize()
{
}

/// <summary>
/// ���⼭ �Ű������� �׳� �ܼ� string���� �ƴ϶�
/// Scene�� �����ϴ� �̸��� �޾ƿ��� ���� ������...?
/// </summary>
/// <param name="name"></param>
void Bjson::Serialize::SceneSerialize(const std::string& name)
{
	//�� ������ ����
	Document doc(kArrayType);
	doc.SetArray();

	Value sceneVal(kObjectType);

	Value sceneName;
	sceneName.SetString(name.c_str(), strlen(name.c_str()), doc.GetAllocator());
	sceneVal.AddMember(StringRef("Scene Name"), sceneName, doc.GetAllocator());

	for (auto iter : objectDataList)
	{
	}
	///�� ������ �ȿ� ������Ʈ�� �ְ� ������....
	ObjectSerialize(doc, "Object1");
	
	doc.PushBack(sceneVal, doc.GetAllocator());
	
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	doc.Accept(writer);

	cout << buffer.GetString() << endl;
}

/// <summary>
/// �̰͵� ���������� ������Ʈ�� �̸��� �Ű������� �޾ƿ��� �� �� ������...
/// </summary>
/// <param name="name"></param>
void Bjson::Serialize::ObjectSerialize(Document& doc, const std::string& name)
{
	Value objectVal(kObjectType);

	Value objName;
	objName.SetString(name.c_str(), strlen(name.c_str()), doc.GetAllocator());

	objectVal.AddMember(StringRef("Object Name"), objName, doc.GetAllocator());

	doc.PushBack(objectVal, doc.GetAllocator());
}

///Serialize�� �����͸� ���Ϸ� ����ϴ� �Լ�
void Bjson::Serialize::LoadSerializeDataToFile(StringBuffer& buffer, const char* filePath)
{
	std::ofstream outputFile(filePath);

	if (outputFile.is_open())
	{
		outputFile << buffer.GetString();
		outputFile.close();
	}
	else
	{
		assert(outputFile.is_open());
	}
}
