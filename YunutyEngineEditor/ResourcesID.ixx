module;
#include <string>
export module htkg.ResourcesID;
#define DebugMode
export namespace resourcesID
{
    // 일반 fbx를 불러오는건 시간이 오래 걸리기 때문에 debugPath를 따로 둔다.
//#ifdef DebugMode
    //inline const std::string FBXpath = "FBXFileDebug\\";
//#else
    inline const std::string FBXpath = "FBXFile\\";
    //#endif
    inline const std::string ASEpath = "ASEFile\\";

#ifdef DebugMode
    inline const std::string TexturePath = "Textures\\";
#else
    inline const std::string TexturePath = "Textures\\";
#endif
    //inline const std::string robotFBX = FBXpath + "Rob02.fbx";
    inline const std::string robotFBX = ASEpath + "Rob02.ase";
    inline const std::string buttonFBX = FBXpath + "button.fbx";
    inline const std::string buttonASE = ASEpath + "Cover_Medium.ase";
    inline const std::string batteryFBX = FBXpath + "battery.fbx";
    inline const std::string batteryASE = ASEpath + "Barrel.ase";
    //inline const std::string platformFBX = ASEpath + "Ground_Small_1.ase";
    inline const std::string platformFBX = FBXpath + "unitPlane.fbx";
    inline const std::string laserFBX = FBXpath + "laser.fbx";
    inline const std::string laserASE = ASEpath + "Pipe_Bend.ase";
    inline const std::string laserBeamFBX = ASEpath + "Pipe.ase";
    inline const std::string destinationFBX = FBXpath + "destination.fbx";
    inline const std::string destinationASE = ASEpath + "Cover_Medium.ase";

    inline const std::string redTexture = "Textures\\red.dds";
    inline const std::string orangeTexture = "Textures\\orange.dds";
    inline const std::string pinkTexture = "Textures\\pink.dds";
    inline const std::string purpleTexture = "Textures\\purple.dds";
    inline const std::string grayTexture = "Textures\\gray.dds";
    inline const std::string greenTexture = "Textures\\green.dds";
    inline const std::string cyanTexture = "Textures\\cyan.dds";
    inline const std::string scarletTexture = "Textures\\scarlet.dds";
    inline const std::string yellowTexture = "Textures\\yellow.dds";

    inline const std::string robotDiffuseTex = "Textures\\Rob02Blue_AlbedoTransparency_reverse.dds";
    inline const std::string robotNormalTex = "Textures\\Rob02_Normal_reverse.dds";
    inline const std::string buttonDiffuseTex = redTexture;
    inline const std::string batteryDiffuseTex = "Textures\\Walls_ALB_reverse.dds";
    inline const std::string batteryNormalTex = "Textures\\Walls_NM_reverse.dds";
    inline const std::string platformDiffuseTex = "Textures\\ground2.dds";
    inline const std::string platformNormalTex = "Textures\\ground2NM.dds";
    inline const std::string laserDiffuseTex = orangeTexture;
    inline const std::string laserNormalTex = "Textures\\Walls_NM_reverse.dds";
    inline const std::string destinationDiffuseTex = "Textures\\GameClearBoard.dds";
    inline const std::string destinationNormalTex = "Textures\\Walls_NM_reverse.dds";

    inline const std::string music_ingame_bgm = "sound\\bgm\\ingame_bgm.wav";
    inline const std::string music_main_bgm = "sound\\bgm\\main_bgm.wav";

    inline const std::string sound_button_effect = "sound\\effect\\button_effect.wav";
    inline const std::string sound_robot_down_effect = "sound\\effect\\robot_down_effect.wav";
    inline const std::string sound_stage_success_effect = "sound\\effect\\stage_success_effect.wav";
    inline const std::string sound_laser_off_effect = "sound\\effect\\laser_off_effect.wav";
    inline const std::string sound_robot_move_effect = "sound\\effect\\robot_move_effect.wav";
    inline const std::string sound_trigger_effect = "sound\\effect\\trigger_effect.wav";
    inline const std::string sound_laser_on_effect = "sound\\effect\\laser_on_effect.wav";
    inline const std::string sound_robot_oring_effect = "sound\\effect\\robot_oring_effect.wav";
    inline const std::string sound_turn_effect = "sound\\effect\\turn_effect.wav";
    inline const std::string sound_robot_charge_effect = "sound\\effect\\robot_charge_effect.wav";
    inline const std::string sound_stage_charge_effect = "sound\\effect\\stage_charge_effect.wav";
    inline const std::string sound_robot_click_effect = "sound\\effect\\robot_click_effect.wav";
    inline const std::string sound_stage_fail_effect = "sound\\effect\\stage_fail_effect.wav";

}