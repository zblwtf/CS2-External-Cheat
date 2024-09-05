#include "Game.h"
#include "Entity.h"
extern CEntity tempLocalEntity;
bool CGame::InitAddress()
{
	this->Address.ClientDLL = reinterpret_cast<DWORD64>(ProcessMgr.GetProcessModuleHandle("client.dll"));
	
	this->Address.EntityList = GetClientDLLAddress() + Offset::EntityList;
	this->Address.Matrix = GetClientDLLAddress() + Offset::Matrix;
	this->Address.ViewAngle = GetClientDLLAddress() + Offset::ViewAngle;
	this->Address.LocalController = GetClientDLLAddress() + Offset::LocalPlayerController;
	this->Address.LocalPawn = GetClientDLLAddress() + Offset::LocalPlayerPawn;
	this->Address.ForceJump = GetClientDLLAddress() + Offset::ForceJump;
	this->Address.GlobalVars = GetClientDLLAddress() + Offset::GlobalVars;

	return this->Address.ClientDLL != 0;
}

DWORD64 CGame::GetClientDLLAddress()
{
	return this->Address.ClientDLL;
}

DWORD64 CGame::GetEntityListAddress()
{
	return this->Address.EntityList;
}

DWORD64 CGame::GetMatrixAddress()
{
	return this->Address.Matrix;
}

DWORD64 CGame::GetViewAngleAddress() 
{
	return this->Address.ViewAngle;
}

DWORD64 CGame::GetEntityListEntry()
{
	return this->Address.EntityListEntry;
}

DWORD64 CGame::GetLocalControllerAddress()
{
	return this->Address.LocalController;
}

DWORD64 CGame::GetLocalPawnAddress()
{
	return this->Address.LocalPawn;
}

DWORD64 CGame::GetGlobalVarsAddress()
{
	return this->Address.GlobalVars;
}

bool CGame::UpdateEntityListEntry()
{
	DWORD64 EntityListEntry = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityListEntry))
		return false;
	if (!ProcessMgr.ReadMemory<DWORD64>(EntityListEntry + 0x10, EntityListEntry))
		return false;

	this->Address.EntityListEntry = EntityListEntry;

	return this->Address.EntityListEntry != 0;
}

bool CGame::SetForceJump(int value)
{
	if (!ProcessMgr.WriteMemory<int>(this->Address.ForceJump, value))
		return false;

	return true;
}
#include "mytool.h"
bool CGame::SetViewAngle(float Yaw, float Pitch)
{
	Vector3 dstangle,srcangle;
	Vec2 Angle{ Pitch,Yaw };
	
	
	Vec2 sourceAngle;
	ProcessMgr.ReadMemory<Vec2>(this->Address.ViewAngle, sourceAngle);
	srcangle.x = sourceAngle.x;
	srcangle.y = sourceAngle.y;
	dstangle.x = Pitch;
	dstangle.y = Yaw;

	
	if (!ProcessMgr.WriteMemory<Vec2>(this->Address.ViewAngle, Angle))
		return false;
	/*tool::SetViewAngle(dstangle, 70, srcangle);*/
	return true;
}

