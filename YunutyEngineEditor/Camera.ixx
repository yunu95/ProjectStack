/// 2023. 07. 09. �����
/// �����Ϳ��� ����� ī�޶� Ŭ����
/// ���������� ����ϴ� ī�޶�� ����� ���� ��
/// 
/// �������̽��� �ƴ�����,
/// �ش� ī�޶� ��ӹ޾Ƽ� ��üȭ�� ī�޶� ���� �� ����
export module graphics.Camera;

export namespace graphics
{
    class Camera
    {
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void OnUpdate(float ts) = 0;

    private:
        // 4x4 ��Ʈ���� �ʿ��� ��
    };
}