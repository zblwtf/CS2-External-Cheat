#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include <iostream>
#include "Vector2.h"
#include "Vector3.h"
#include "mytool.h"
namespace AimControl
{
	inline int HotKey = VK_LBUTTON;
	inline float AimFov = 5;
	inline float Smooth = 0.7;
	inline Vec2 RCSScale = { 4.f,4.f };
	inline int RCSBullet = 1;
	inline std::vector<int> HotKeyList{VK_LBUTTON, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL};// added new button VK_LBUTTON

	inline void SetHotKey(int Index)
	{
		HotKey = HotKeyList.at(Index);
	}

	inline void AimBot(const CEntity& Local, Vec3 LocalPos, Vec3 AimPos)
	{


		float delta_x = AimPos.x - LocalPos.x;
		float delta_y = AimPos.y - LocalPos.y;
		float delta_z = AimPos.z - LocalPos.z;

		float distance = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));
		float new_pitch = 0.0, new_yaw = 0.0;

		new_yaw = atan2f(delta_y, delta_x) * (180 / 3.1415926535);
		new_pitch = -atan2f(delta_z, distance) * (180 / 3.1415926535);
		Vector3 new_view_angle = { new_pitch,new_yaw,0.0f };
		new_view_angle.Normalize();
		Vector3 punch_angle = { Local.Pawn.AimPunchAngle.x,Local.Pawn.AimPunchAngle.y ,0};
		Vector3 corr_angle = new_view_angle;



		//float Yaw, Pitch;
		//float Distance, Norm;
		//Vec3 OppPos;

		//OppPos = AimPos - LocalPos;

		//Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

		//Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.Pawn.ViewAngle.y;
		//Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.Pawn.ViewAngle.x;
		//Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));
		//if (Norm > AimFov)
		//	return;

		//Yaw = Yaw * (1 - Smooth) + Local.Pawn.ViewAngle.y;
		//Pitch = Pitch * (1 - Smooth) + Local.Pawn.ViewAngle.x;

		// Recoil control
		if (Local.Pawn.ShotsFired > RCSBullet)
		{
			//Vec2 PunchAngle = Local.Pawn.AimPunchAngle;
			//

			///*if (Local.Pawn.AimPunchCache.Count <= 0 && Local.Pawn.AimPunchCache.Count > 0xFFFF)
			//	return;
			//if (!ProcessMgr.ReadMemory<Vec2>(Local.Pawn.AimPunchCache.Data + (Local.Pawn.AimPunchCache.Count - 1) * sizeof(Vec3), PunchAngle))
			//	return;*/

			//Yaw = Yaw - PunchAngle.y * RCSScale.x;
			//Pitch = Pitch - PunchAngle.x * RCSScale.y;
			punch_angle = punch_angle * 2;
			corr_angle = corr_angle - punch_angle;
			corr_angle.Normalize();
		}
		gGame.SetViewAngle(corr_angle.y,corr_angle.x);
		
	}
	
}
