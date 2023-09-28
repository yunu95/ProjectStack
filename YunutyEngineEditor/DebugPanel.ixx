/// ����� �α׸� ��� ���� ���
/// ������Ʈ�� ���� Ŭ��, ȣ���� �̺�Ʈ�� ���� ����� ���� �α׷� ���
/// 2023. 08. 23. ����ȭ

#include <vector>;
#include <string>;
#include <mutex>
import ui.EditorPanel;
export module ui.DebugPanel;

export class DebugPanel
    : public EditorPanel
{
public:
    static std::mutex logMutex;
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void OnUpdate(float ts) override;
    virtual void DrawPanel() override;

public:
    static void LogMessage(const char* text)
    {
        std::scoped_lock lock(logMutex);
        logs.push_back(text);
    }
    void ClearLogs() {
        logs.clear();
    }
private:
    static std::vector<std::string> logs;
};
