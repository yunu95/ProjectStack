#include <string>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <filesystem>
#include "MetaData.h"
#include "MetafileManager.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/ostreamwrapper.h"

using namespace yunutyEngine::resource;

const map<wstring, ResourceType> yunutyEngine::resource::MetafileManager::ResourceTypeByExtension
{
    make_pair(L"ase",ResourceType::Model)
    ,make_pair(L"dds",ResourceType::Texture)
    ,make_pair(L"bmp",ResourceType::Texture)
};
const unordered_set<wstring> yunutyEngine::resource::MetafileManager::resourceExtensions
{
    L"ase"
    ,L"dds"
    ,L"bmp"
};
//unique_ptr<MetafileManager> YunutyEngine::Resource::MetafileManager::instance = nullptr;

//MetafileManager& YunutyEngine::Resource::MetafileManager::GetInstance()
//{
//    if (instance.get())
//        instance = make_unique<MetafileManager>();
//    return *instance.get();
//};
void yunutyEngine::resource::MetafileManager::GetOrCreateMetaFiles(const WCHAR* rootPath)
{
    loadedMetaDatas.clear();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath))
    {
        if (entry.is_regular_file())
        {
            const std::wstring& filePath = entry.path().wstring();

            // Check if the file has one of the specified extensions
            for (const auto& extensionPair : ResourceTypeByExtension)
            {
                const auto& extension = extensionPair.first;
                if (filePath.length() >= extension.length() && filePath.compare(filePath.length() - extension.length(), extension.length(), extension) == 0)
                {
                    // resource ���Ͽ� �����Ǵ� meta ������ �����ϴ��� Ȯ���Ѵ�.
                    MetaData metaData{ 0 };
                    metaData.filePath = entry.path().wstring() + L".meta";
                    if (std::filesystem::exists(metaData.filePath))
                    {
                        // meta ���Ϸκ��� ������ �ҷ��´�.
                        metaData = LoadMetaFile(metaData.filePath.c_str());

                        // meta ������ �ֽ��� �ƴ϶�� meta ������ ���� �����Ѵ�.
                        if (metaData.metaFileVersion != MetafileVersion)
                            metaData = GenerateMetaFile(metaData.filePath.c_str());
                    }
                    else
                        // meta ������ �������� �ʴ´ٸ� meta ������ ���� �����Ѵ�.
                    {
                        metaData = GenerateMetaFile(metaData.filePath.c_str());
                    }
                    loadedMetaDatas.push_back(metaData);
                    // meta ������ ��·�� �ݵ�� �߰��ȴ�.

                    break;
                }
            }
        }
    }
}
MetaData yunutyEngine::resource::MetafileManager::GenerateMetaFile(const WCHAR* fileName)
{
    MetaData metaData;
    std::wofstream file{
        wstring(fileName)
        ,std::ios::out | std::ios::trunc
    };
    assert(file.is_open());
    file.close();
    return metaData;
}
MetaData yunutyEngine::resource::MetafileManager::LoadMetaFile(const WCHAR* fileName)
{
    MetaData metaData;

    return metaData;
}
