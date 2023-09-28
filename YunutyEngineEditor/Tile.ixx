module;
#include "YunutyEngine.h"
#include <math.h>
export module htkg.InGame.Tile;
import htkg.InGame.Robot;
import htkg.InGame.Laser;
import htkg.InGame.Battery;
import htkg.InGame.Button;
import htkg.InGame.Destination;

// ��⿡���� ���漱�� export �ؾߵ�
export class Stage;

// Serialize �����
export class Tile : public yunutyEngine::Component
{
public:
    //const float TileHeightFromPivot = 1;
    void ApplyChanges(bool reset = false)
    {
        if (reset)
        {
            lastTileType = tileType;
            lastLaserDirection = LaserDirection;
        }
        // Ÿ��Ÿ���� ������ �Ӽ��� �� �ٲ� ���
        if (lastTileType == tileType || reset)
        {
            if (tileType == TileType::Robot)
                _robot->energy = energy;
            // ������ ���� ������
            if (tileType == TileType::Laser)
            {
                _laser->GetTransform()->SetWorldRotation(Vector3d(0, (int)LaserDirection * 90, 0));
            }
            lastLaserDirection = LaserDirection;
            if (reset == false)
                return;
        }
        // Ÿ��Ÿ�� ��ü�� �ٲ� ��� ���� Ÿ���� ������ �����.
        switch (lastTileType)
        {
        case Tile::TileType::None:
            break;
        case Tile::TileType::Blocked:
            tileMesh->SetActive(true);
            break;
        case Tile::TileType::Robot:
            _robot->ClearArrow();
            Scene::getCurrentScene()->DestroyGameObject(_robot->GetGameObject());
            _robot = nullptr;
            break;
        case Tile::TileType::Battery:
            Scene::getCurrentScene()->DestroyGameObject(_battery->GetGameObject());
            Scene::getCurrentScene()->DestroyGameObject(_battery->energyLeftText->GetGameObject());
            _battery = nullptr;
            break;
        case Tile::TileType::Laser:
            Scene::getCurrentScene()->DestroyGameObject(_laser->GetGameObject());
            _laser = nullptr;
            break;
        case Tile::TileType::Button:
            Scene::getCurrentScene()->DestroyGameObject(_button->GetGameObject());
            _button = nullptr;
            break;
        case Tile::TileType::Destination:
            Scene::getCurrentScene()->DestroyGameObject(_destination->GetGameObject());
            _destination = nullptr;
            break;
        default:
            break;
        }
        // Ÿ��Ÿ�� ��ü�� �ٲ� ��� �� Ÿ�Ͽ� �°� ��ü�� �����Ѵ�.
        switch (tileType)
        {
        case Tile::TileType::None:
            break;
        case Tile::TileType::Blocked:
            tileMesh->SetActive(false);
            break;
        case Tile::TileType::Robot:
            _robot = GetGameObject()->AddGameObject()->AddComponent<Robot>();
            _robot->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            _robot->energy = this->energy;
            _robot->currentTile = this;
            break;
        case Tile::TileType::Battery:
            _battery = GetGameObject()->AddGameObject()->AddComponent<Battery>();
            _battery->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            break;
        case Tile::TileType::Laser:
            _laser = GetGameObject()->AddGameObject()->AddComponent<Laser>();
            _laser->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            _laser->GetTransform()->SetWorldRotation(Vector3d(0, (int)LaserDirection * 90, 0));
            break;
        case Tile::TileType::Button:
            _button = GetGameObject()->AddGameObject()->AddComponent<Button>();
            _button->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            break;
        case Tile::TileType::Destination:
            _destination = GetGameObject()->AddGameObject()->AddComponent<Destination>();
            //_destination->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
            break;
        default:
            break;
        }
        lastTileType = tileType;
    }
    bool isAdjacent(const Tile* tile)
    {
        return abs(tile->_row - _row) + abs(tile->_col - _col) == 1;
    }

    enum class TileType
    {
        None, Blocked, Robot, Battery, Laser, Button, Destination
    };
    enum class Direction
    {
        Up = 0, Right, Down, Left,
    };
    // Serialize �����
    TileType tileType{ TileType::None };
    int energy{ 0 };
    int mechanismIndex{ 0 };
    Direction LaserDirection{ Direction::Up };

    //constexpr static float standardScale = 0.0065;
    //constexpr static float standardScaleInv = 1/0.0065;
    // Serialize ����� �ƴ�.
    int _row{ 0 }, _col{ 0 };
    graphics::StaticMesh* tileMesh{ nullptr };
    Stage* _stage{ nullptr };
    bool _isDangerous{ false };
    Robot* _robot{ nullptr };
    Battery* _battery{ nullptr };
    Button* _button{ nullptr };
    Destination* _destination{ nullptr };
    Laser* _laser{ nullptr };
private:
    TileType lastTileType{ TileType::None };
    Direction lastLaserDirection{ Direction::Up };
};