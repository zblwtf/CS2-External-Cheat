#pragma once
#include <Windows.h>
#include "Utils/ProcessManager.hpp"
#include "offsets.hpp"
#include "client.dll.hpp"
#include "buttons.hpp"
using namespace cs2_dumper::offsets;
using namespace cs2_dumper::schemas::client_dll;
// From: https://github.com/a2x/cs2-dumper/blob/main/generated/client.dll.hpp
namespace Offset
{
	inline DWORD EntityList = client_dll::dwEntityList;
	inline DWORD Matrix = client_dll::dwViewMatrix;
	inline DWORD ViewAngle = client_dll::dwViewAngles;
	inline DWORD LocalPlayerController = client_dll::dwLocalPlayerController;
	inline DWORD LocalPlayerPawn = client_dll::dwLocalPlayerPawn;
	inline DWORD ForceJump = cs2_dumper::buttons::jump;
	inline DWORD GlobalVars = client_dll::dwGlobalVars;

	struct
	{
		DWORD Health = C_BaseEntity::m_iHealth;
		DWORD TeamID = C_BaseEntity::m_iTeamNum;
		DWORD IsAlive = CCSPlayerController::m_bPawnIsAlive;
		DWORD PlayerPawn = CBasePlayerController::m_hPawn;
		DWORD iszPlayerName = CBasePlayerController::m_iszPlayerName;
	}Entity;

	struct
	{
		DWORD Pos = C_BasePlayerPawn::m_vOldOrigin;
		DWORD MaxHealth = C_BaseEntity::m_iMaxHealth;
		DWORD CurrentHealth = C_BaseEntity::m_iHealth;
		DWORD GameSceneNode = C_BaseEntity::m_pGameSceneNode;
		DWORD m_modelState = CSkeletonInstance::m_modelState;
		DWORD BoneArray = m_modelState+0x80;
		DWORD angEyeAngles = C_CSPlayerPawnBase::m_angEyeAngles;
		DWORD vecLastClipCameraPos = C_CSPlayerPawnBase::m_vecLastClipCameraPos;
		DWORD pClippingWeapon = C_CSPlayerPawnBase::m_pClippingWeapon;
		DWORD iShotsFired = C_CSPlayerPawn::m_iShotsFired;
		DWORD flFlashDuration = C_CSPlayerPawnBase::m_flFlashDuration;
		DWORD aimPunchAngle = C_CSPlayerPawn::m_aimPunchAngle;
		DWORD aimPunchCache = C_CSPlayerPawn::m_aimPunchCache;
		DWORD iIDEntIndex = C_CSPlayerPawnBase::m_iIDEntIndex;
		DWORD iTeamNum = C_BaseEntity::m_iTeamNum;
		DWORD CameraServices = C_BasePlayerPawn::m_pCameraServices;
		DWORD iFovStart = CCSPlayerBase_CameraServices::m_iFOVStart;
		DWORD fFlags = C_BaseEntity::m_fFlags;
		DWORD bSpottedByMask = C_CSPlayerPawn::m_entitySpottedState + EntitySpottedState_t::m_bSpottedByMask; // entitySpottedState + bSpottedByMask
	}Pawn;

	struct
	{
		DWORD RealTime = 0x00;
		DWORD FrameCount = 0x04;
		DWORD MaxClients = 0x10;
		DWORD IntervalPerTick = 0x14;
		DWORD CurrentTime = 0x2C;
		DWORD CurrentTime2 = 0x30;
		DWORD TickCount = 0x40;
		DWORD IntervalPerTick2 = 0x44;
		DWORD CurrentNetchan = 0x0048;
		DWORD CurrentMap = 0x0180;
		DWORD CurrentMapName = 0x0188;
	} GlobalVar;

	

}
