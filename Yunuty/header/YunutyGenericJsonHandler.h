#pragma once
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
#include "RapidJson/rapidjson.h"
#include "RapidJson/prettywriter.h"
#include "RapidJson/reader.h"
#include "RapidJson/document.h"
#pragma comment(lib,"Rpcrt4.lib")
namespace yunutyEngine
{
    // yutility �Լ� ����, Ȥ�� functor�� ����.
    namespace yutility
    {
        using namespace rapidjson;
        using namespace std;
        template<typename T>
        concept PairInstantiation = std::is_same_v<T, std::pair<typename T::first_type, typename T::second_type>>;

        template <typename JsonStruct>
        class GenericJsonStructHandler
        {
        public:
            template<typename KeyPair, typename... KeyPairs>
                requires PairInstantiation<KeyPair>
            GenericJsonStructHandler(KeyPair keyPair, KeyPairs... keypairs) : GenericJsonStructHandler(keypairs...)
            {
                RegisterKeyValue(boolMap, keyPair);
                RegisterKeyValue(intMap, keyPair);
                RegisterKeyValue(uintMap, keyPair);
                RegisterKeyValue(int64Map, keyPair);
                RegisterKeyValue(uint64Map, keyPair);
                RegisterKeyValue(doubleMap, keyPair);
                //RegisterKeyValue(rawNumberMap, keyPair);
                RegisterKeyValue(stringMap, keyPair);
            }
            template<typename KeyPair>
                requires PairInstantiation<KeyPair>
            GenericJsonStructHandler(KeyPair keyPair)
            {
                RegisterKeyValue(boolMap, keyPair);
                RegisterKeyValue(intMap, keyPair);
                RegisterKeyValue(uintMap, keyPair);
                RegisterKeyValue(int64Map, keyPair);
                RegisterKeyValue(uint64Map, keyPair);
                RegisterKeyValue(doubleMap, keyPair);
                //RegisterKeyValue(rawNumberMap, keyPair);
                RegisterKeyValue(stringMap, keyPair);
            }
            JsonStruct ReadData(const WCHAR* jsonStr, JsonStruct jsonStruct = JsonStruct())
            {
                rapidjson::GenericReader<rapidjson::UTF8<WCHAR>, rapidjson::UTF8<WCHAR>> reader;
                rapidjson::GenericStringStream<rapidjson::UTF8<WCHAR>> ss(jsonStr);
                targetStruct = &jsonStruct;
                JsonReadHandler readHandler{ this };
                reader.Parse(ss, readHandler);
                targetStruct = nullptr;

                return jsonStruct;
            }
            ///�����͸� ����ȭ���ִ� �Լ�.
            ///PrettyWriter ������ �����ؼ� serializeList�� �ִ� �͵��� ��� write�Ѵ�.
            ///
            wstring SerializeData(JsonStruct& target)
            {
                WStringBuffer stringBuffer;
                PrettyWWriter writer{ stringBuffer };
                targetStruct = &target;
                writer.StartObject();
                //for (decltype(serializeList)::reverse_iterator each : serializeList)
                //for (auto itr = serializeList.rbegin(); itr != serializeList.rend(); itr++)
                    //(*itr)(writer);
                for (auto each : serializeList)
                    each(writer);
                writer.EndObject();

                return stringBuffer.GetString();
            }

            /// <summary>
            /// �⺻���� �ڷ��� ���� ��ȯ���ִ� ����ü. ���� bool������ ��ȯ�Ѵ�.
            /// </summary>
            struct JsonReadHandler
            {
                JsonReadHandler(GenericJsonStructHandler<JsonStruct>* parent) : parent(parent) {}
                bool Null() { return true; }
                bool Bool(bool b) { parent->CallMappedFunction(parent->boolMap, b); return true; }
                bool Int(int i) { parent->CallMappedFunction(parent->intMap, i); return true; }
                bool Uint(unsigned u) { parent->CallMappedFunction(parent->uintMap, u); return true; }
                bool Int64(int64_t i) { parent->CallMappedFunction(parent->int64Map, i); return true; }
                bool Uint64(uint64_t u) { parent->CallMappedFunction(parent->uint64Map, u); return true; }
                bool Double(double d) { parent->CallMappedFunction(parent->doubleMap, d); return true; }
                bool RawNumber(const WCHAR* str, SizeType length, bool copy) { parent->CallMappedFunction(parent->rawNumberMap, wstring(str)); return true; }
                bool String(const WCHAR* str, SizeType length, bool copy) { parent->CallMappedFunction(parent->stringMap, wstring(str)); return true; }
                bool StartObject() { return true; }
                bool Key(const WCHAR* str, SizeType length, bool copy) { parent->key = wstring(str); return true; }
                bool EndObject(SizeType memberCount) { return true; }
                bool StartArray() { return true; }
                bool EndArray(SizeType elementCount) { return true; }
            private:
                GenericJsonStructHandler<JsonStruct>* parent;
            };
        private:
            /// <summary>
            ///serialization�ϴµ��� �ʿ��� ������ �⺻������ Key������ ����Ʈ�� �����س��� �͵��̴�.
            /// </summary>
            using WStringBuffer = GenericStringBuffer<rapidjson::UTF8<WCHAR>>;
            using PrettyWWriter = PrettyWriter<WStringBuffer, rapidjson::UTF8<WCHAR>, rapidjson::UTF8<WCHAR>>;
            std::map<wstring, function<void(bool)>> boolMap;
            std::map<wstring, function<void(int i)>> intMap;
            std::map<wstring, function<void(unsigned u)>> uintMap;
            std::map<wstring, function<void(int64_t i)>> int64Map;
            std::map<wstring, function<void(uint64_t u)>> uint64Map;
            std::map<wstring, function<void(double d)>> doubleMap;
            std::map<wstring, function<void(wstring)>> rawNumberMap;
            std::map<wstring, function<void(wstring)>> stringMap;
            std::vector<function<void(PrettyWWriter&)>> serializeList;

            ///Key���� ����Ʈ�� �����ϴ� �Լ� ���ø�.
            ///���ڷ� ���� ���� �ڷ����� �Ǻ��Ͽ� �׿� �´� ����Ʈ�� �����Ѵ�.
            template<typename ValueType, typename KeyPair>
            void RegisterKeyValue(std::map<wstring, function<void(ValueType)>>& funcMap, const KeyPair& keyPair)
            {
                //static_assert(is_same_v<decltype(keyPair.first), wstring>, "Yunu : json key value type should be wstring");

                /// ����� �ι�° ������ ��� ���� �������϶�
                if constexpr (is_same_v<decltype(keyPair.second), ValueType JsonStruct::*>)
                {
                    funcMap[keyPair.first] = [=](ValueType arg) {this->targetStruct->*keyPair.second = arg; };
                    serializeList.push_back(
                        [=](PrettyWWriter& writer)
                        {
                            ///�ڷ��� ���� Ÿ���� �ԷµǾ��� �� �׿� �´� writer�� ȣ���Ѵ�.
                            writer.String(keyPair.first);
                            if constexpr (is_same_v<bool, ValueType>) { writer.Bool(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<int, ValueType>) { writer.Int(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<unsigned, ValueType>) { writer.Uint(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<int64_t, ValueType>) { writer.Int64(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<uint64_t, ValueType>) { writer.Uint64(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<double, ValueType>) { writer.Double(this->targetStruct->*(keyPair.second)); };
                            if constexpr (is_same_v<wstring, ValueType>) { writer.String((this->targetStruct->*(keyPair.second)).c_str()); };
                        }
                    );
                }
                // ����� �ι�° ������ ��� �Լ� ������ �� Setter�϶�
                if constexpr (is_same_v<decltype(keyPair.second), void (JsonStruct::*)(ValueType)>)
                {
                    funcMap[keyPair.first] = [=](ValueType arg) {(this->targetStruct->*(keyPair.second))(arg); };
                }
                // ����� �ι�° ������ ��� �Լ� ������ �� Getter�϶�
                if constexpr (is_same_v<decltype(keyPair.second), ValueType(JsonStruct::*)()>)
                {
                    serializeList.push_back(
                        [=](PrettyWWriter& writer)
                        {
                            ///�ڷ��� ���� Ÿ���� �ԷµǾ��� �� �׿� �´� writer�� ȣ���Ѵ�.
                            writer.String(keyPair.first);
                            if constexpr (is_same_v<bool, ValueType>) { writer.Bool(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<int, ValueType>) { writer.Int(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<unsigned, ValueType>) { writer.Uint(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<int64_t, ValueType>) { writer.Int64(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<uint64_t, ValueType>) { writer.Uint64(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<double, ValueType>) { writer.Double(this->targetStruct->*(keyPair.second)()); };
                            if constexpr (is_same_v<wstring, ValueType>) { writer.String((this->targetStruct->*(keyPair.second)()).c_str()); };
                        }
                    );
                    funcMap[keyPair.first] = [=](ValueType arg) {(this->targetStruct->*(keyPair.second)())(arg); };
                }
            };
        private:
            template<typename T>
            void CallMappedFunction(std::map<wstring, function<void(T)>>& funcMap, const T& value)
            {
                assert(funcMap.find(key) != funcMap.end(), "yunu : json key string mismatch while reading.");
                funcMap[key](value);
            }
            JsonStruct* targetStruct;
            wstring key;
        };
    }
}
