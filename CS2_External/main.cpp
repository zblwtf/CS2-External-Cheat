#include "Offsets.h"
#include "Cheats.h"
#include "Utils/Format.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <thread>
extern CEntity tempLocalEntity;
namespace fs = std::filesystem;
bool rcs_ative;
void RCS()
{
	auto clientModule = gGame.GetClientDLLAddress();


	
	Vector3 oPunch{ 0,0,0 };
	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			break;

		//uintptr_t local_player_pawn = *(uintptr_t*)(clientModule + Offset::LocalPlayerPawn);
		uintptr_t local_player_pawn;
		ProcessMgr.ReadMemory<uintptr_t>(clientModule + Offset::LocalPlayerPawn, local_player_pawn);

		if (local_player_pawn == NULL && !rcs_ative )
			continue;

		uintptr_t pviewAngles = (clientModule + Offset::ViewAngle);
		uintptr_t pShotFired = (local_player_pawn + Offset::Pawn.iShotsFired);
		uintptr_t paimPunchAgnle (local_player_pawn + Offset::Pawn.aimPunchAngle);

		Vector3 aimPunchAgnle_a;
		Vector3 viewAngles_a;
		DWORD ShotFired = 0;
		Vector3 PunchAngle;
		

	

		ProcessMgr.ReadMemory<DWORD>(pShotFired, ShotFired);
		ProcessMgr.ReadMemory<Vector3>(paimPunchAgnle, aimPunchAgnle_a);
		ProcessMgr.ReadMemory<Vector3>(pviewAngles, viewAngles_a);
		
		Vector3 punchAngle = aimPunchAgnle_a * 2;

		if (ShotFired > 1)
		{
			Vector3 newAngle = viewAngles_a + oPunch - punchAngle;
			newAngle.Normalize();
			//*pviewAngles = newAngle;
			ProcessMgr.WriteMemory<Vector3>(pviewAngles, newAngle);
			//tool::SetViewAngle(newAngle, 70, viewAngles_a);
		}
		oPunch = punchAngle;
	}

	

}
int main()
{
	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");
	
	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK) {
		std::cerr << "Failed to get the Documents folder path." << std::endl;
		goto END;
	}
	MenuConfig::path = documentsPath;
	MenuConfig::path += "/CS2_External";

	if (ProcessStatus != StatusCode::SUCCEED)
	{
		std::cout << "[ERROR] Failed to attach process, StatusCode:" << ProcessStatus << std::endl;
		goto END;
	}

	

	if (!gGame.InitAddress())
	{
		std::cout << "[ERROR] Failed to call InitAddress()."<< std::endl;
		goto END;
	}


	std::cout << Format("[Game] Pid:%d\n", ProcessMgr.ProcessID);
	std::cout << Format("[Game] Client:%llX\n", gGame.GetClientDLLAddress());

	std::cout << "Offset:" << std::endl;
	std::cout << Format("--EntityList:%llX\n", Offset::EntityList);
	std::cout << Format("--Matrix:%llX\n", Offset::Matrix);
	std::cout << Format("--LocalPlayerController:%llX\n", Offset::LocalPlayerController);
	std::cout << Format("--ViewAngles:%llX\n", Offset::ViewAngle);
	std::cout << Format("--LocalPlayerPawn:%llX\n", Offset::LocalPlayerPawn);
	std::cout << Format("--ForceJump:%llX\n", Offset::ForceJump);


	if (fs::exists(MenuConfig::path))
		std::cout << "Config folder connected: "<< MenuConfig::path << std::endl;
	else
	{
		if (fs::create_directory(MenuConfig::path))
			std::cout << "Config folder created: " << MenuConfig::path << std::endl;
		else 
		{
			std::cerr << "Failed to create the config directory." << std::endl;
			goto END;
		}
	}

	std::cout << "Runing..." << std::endl;

	try
	{
		std::thread thd(RCS);
		thd.detach();
		Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
		
	}
	catch (OSImGui::OSException& e)
	{
		std::cout << e.what() << std::endl;
	}

END:
	system("pause");
	return 0;
}
