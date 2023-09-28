/// 2023. 07. 09. �����
/// GameViewPanel���� ����� ī�޶� Ŭ����
/// (Game �� Main Camera)
/// 
/// �ش� �гο����� �ϳ��� ī�޶��� ����� ���̹Ƿ�,
/// �̱��� �������� ������
import graphics.Camera;
export module graphics.GameViewCamera;

export namespace graphics {
    class GameViewCamera : public Camera
    {
    public:
        static GameViewCamera* SingleInstance();
        static GameViewCamera& Get() { return *instance; }

        ~GameViewCamera();

        virtual void Init() override;
        virtual void Shutdown() override;
        virtual void OnUpdate(float ts) override;

    private:
        GameViewCamera();
        GameViewCamera(const GameViewCamera&) = delete;
        GameViewCamera& operator=(const GameViewCamera&) = delete;

        static GameViewCamera* instance;
    };
}
