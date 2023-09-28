#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_set>
#include <functional>
#include <memory>
#include <string>
#include "YunuGraphicsInterface.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace yunuGI
{
    class I3DRenderer;
}
namespace yunutyEngine
{
    class YunutyCycle;
    namespace graphics
    {
        class _YunuGIObjects;
        template<typename T>
        class Renderable;
    }
}
using namespace std;
namespace yunutyEngine
{
    namespace graphics
    {
        class YUNUTY_API Renderer
        {
        public:
            static Renderer& SingleInstance();
            // �׷��Ƚ� ���� ���̺귯���� �ڵ带 �����ɴϴ�. 
            // ����� �Ǵ� �׷��� ������ YunuGraphicsInterface �������̽��� �����ؾ� �մϴ�.
            void LoadGraphicsDll(const wstring& dllFileName);

            // �׷��� ����� ����� â�� �����մϴ�. �� â���� ���ӿ����� ����� �׷��Ƚ� �������� ������ �־�� �մϴ�. 
            // ���� â�� ���� �׷��Ƚ� ������ �������� �õ��� ���, ���� �ַλ����� �߻��ϰ� �� ���Դϴ�.
            void SetOutputWindow(const HWND& hwnd);

            // �ش� ������ ���� ���� ���ο� �ִ� ��� �׷��Ƚ� ���ϵ��� �ε��մϴ�.
            void LoadFiles(const char* rootPath);
            // ������ �ε��մϴ�. ������ �׷��Ƚ� ���Ϸ� ������ �� ���� ���, �ش� ������ �����մϴ�.
            void LoadFile(const char* fileName);
            // �������� ���� ���� ȣ���� �ݹ��Լ��� �����մϴ�.
            void SetAfterRenderAction(const function<void()>& action);

            // �׷��� ����� ������ �ؽ�ó ��ü�� �ڵ��� ��û�մϴ�. Null�� ��ȯ�� ���, SetOutputWindow �Լ����� �����ؾ� �մϴ�.
            void* QuerySharedOutputHandle();

            // �������� �׷��Ƚ� ������ ������ ������ �����ŵ�ϴ�.
            void ManualRender() { Render(); }

            // �������� �׷��Ƚ� ������ ������Ʈ �Լ��� �����ŵ�ϴ�.
            void ManualUpdate(float delta) { Update(delta); }

            
            // �׷��Ƚ� ������ ClickEvent�� �߻���ŵ�ϴ�. x,y ��ǥ�� 0���� 1 ������ ������ ����ȭ�Ǿ�� �ϸ�,
            // (0,0)�� ��ǥ�� ���� �� ������ �ش��մϴ�.
            // �� �Լ��� �׷��Ƚ� ��ü�� �����Ǵ� EventHandler ��ü�� �̺�Ʈ �Լ��� ȣ���մϴ�.
            void InvokeClickEvent(float x, float y);
            void InvokeHoverEvent(float x, float y);

            // �׷��Ƚ� ������ ClickEvent�� �߻���ŵ�ϴ�. x,y ��ǥ�� 0���� 1 ������ ������ ����ȭ�Ǿ�� �ϸ�,
            // (0,0)�� ��ǥ�� ���� �� ������ �ش��մϴ�.
            // �� �Լ��� �׷��Ƚ� ��ü�� �����Ǵ� Yunuty Renderable Component�� ��ȯ�ϸ�, �ΰ��� �̺�Ʈ�� �������� �ʽ��ϴ�.
            Renderable<yunuGI::IRenderable>* ClickRenderable(float x, float y);
            const yunuGI::IResourceManager* GetResourceManager();

            // �׷��Ƚ� �������� Ŭ���̺�Ʈ�� �߻����� ��, �̸� ó������ �ݹ� �Լ��� ����մϴ�.
            //void RegisterClickEventHandler(function<void(yunuGI::IRenderable*)> callback) 
            //{ clickEventCallback = callback; };
        private:
            //static void CallClickEventCallback(yunuGI::IRenderable* arg) { instance->clickEventCallback(arg); }
            void Render();
            void Update(float delta);
        private:
            static unique_ptr<Renderer> instance;
            function<void()> afterRenderAction{ []() {} };
            //function<void(yunuGI::IRenderable*)> clickEventCallback{ [](yunuGI::IRenderable*) {} };
            friend yunutyEngine::YunutyCycle;
            friend yunutyEngine::graphics::_YunuGIObjects;
        };
    }
}
