#include "mytool.h"
#include <math.h>
#include <Windows.h>
#include "Timer.hpp"
#include "Entity.h"
#include <fstream>
#include "driver.hpp"
#include "Utils/ProcessManager.hpp"
#include <iostream>
CEntity tempLocalEntity;
HWND temp_window;
double AnglePerPixel= 0.07;
int timer_value = 1;
uintptr_t x_address = 0;
uintptr_t y_address = 0;

typedef struct window_info
{
	int width;
	int height;
};
window_info get_window_info()
{
	RECT rect;
	int width, height;
	GetClientRect(temp_window, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	return { width,height };
}
CViewMatrix tool::GetMatrixViewport()
{
	RECT rect;
	unsigned int width, height;
	GetClientRect(temp_window, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	return
	{ width * 0.5f, 0, 0, 0,
		0, -(height * 0.5f), 0, 0,
		0, 0, 1, 0,
		width * 0.5f, height * 0.5f, 0, 1
	};
}
Vector3 tool::ViewportTransform(CViewMatrix m, Vector3 v)
{
	float w = 1.0 / (m.Matrix[4 - 1] * v.x + m.Matrix[2 * 4 - 1] * v.y + m.Matrix[3 * 4 - 1] * v.z + m.Matrix[4 * 4 - 1]);
	auto x_ = m.Matrix[0] * v.x + m.Matrix[0 + 4] * v.y + m.Matrix[0 + 8] * v.z + m.Matrix[8 + 4] * w;
	auto y_ = m.Matrix[1] * v.x + m.Matrix[1 + 4] * v.y + m.Matrix[1 + 8] * v.z + m.Matrix[8 + 4 + 1] * w;
	auto z_ = m.Matrix[2] * v.x + m.Matrix[2 + 4] * v.y + m.Matrix[2 + 8] * v.z + m.Matrix[8 + 4 + 2] * w;
	return { x_, y_, z_ };
}
Vector3 tool::AngleToPixels(Vector3 angle, int fov, int width, int height)
{
	float aspectRatio = (float)width / height;
	float fov_Y = fov * (3.1415926535 / 180);
	float fov_X = fov_Y * aspectRatio;

	
	auto fovRatio = 90.0 / fov;
	float Yaw_pixel = roundf(angle.x / AnglePerPixel * fovRatio);
	float Pitch_pixel = roundf(angle.y / AnglePerPixel * fovRatio);
	return{ Pitch_pixel + fov_X,-Yaw_pixel + fov_Y,0 };
}





extern  ProcessManager ProcessMgr;
void tool::MoveMouse(Vector2 point)
{
	
	mouse_event(MOUSEEVENTF_MOVE, (INT)point.x, (INT)point.y, 0, NULL);
	/*if (x_address > 0 && y_address > 0)
	{
		driver::write_memory(ProcessMgr.driver_handle, x_address, point.x);
		driver::write_memory(ProcessMgr.driver_handle, y_address, point.y);
	}*/
}
void tool::SetViewAngle(Vector3 dstangle, int fov, Vector3 sourceAngle)
{
	static bool hhh = true;
	static Timer m_zoomTimer;
	if (hhh)
	{
		m_zoomTimer.start(timer_value);
		hhh = false;
	}
	else
	{
		if (!m_zoomTimer.isElapsed())
		{
			return;
		}
		else
		{
			
			static window_info  wino = get_window_info();
			
			auto angle = sourceAngle - dstangle;
			angle.clamp();
			auto screen_pixels = tool::AngleToPixels(angle, fov, wino.width, wino.height);
			tool::MoveMouse({ screen_pixels.x,screen_pixels.y });

			hhh = true;
		}
	}

}

Vector3 ViewportTransform(CViewMatrix m, Vector3 v)
{
	float w = 1.0 / (m.Matrix[4 - 1] * v.x + m.Matrix[2 * 4 - 1] * v.y + m.Matrix[3 * 4 - 1] * v.z + m.Matrix[4 * 4 - 1]);
	auto x_ = m.Matrix[0] * v.x + m.Matrix[0 + 4] * v.y + m.Matrix[0 + 8] * v.z + m.Matrix[8 + 4] * w;
	auto y_ = m.Matrix[1] * v.x + m.Matrix[1 + 4] * v.y + m.Matrix[1 + 8] * v.z + m.Matrix[8 + 4 + 1] * w;
	auto z_ = m.Matrix[2] * v.x + m.Matrix[2 + 4] * v.y + m.Matrix[2 + 8] * v.z + m.Matrix[8 + 4 + 2] * w;
	return { x_, y_, z_ };
}

double DegreeToRadian(double degree) {
	return degree * 3.1415926575 / 180.0;
}

Vector3 GetPositionScreen(int fov,Vector2 AimPunchAngle)
{
	static window_info  wino = get_window_info();
	static CViewMatrix playViewMatrix = tool::GetMatrixViewport();

	auto spectRatio = (double)wino.width / wino.height;



	auto fovY = DegreeToRadian(fov);
	auto fovX = fovY * spectRatio;
	auto punchX =DegreeToRadian((double)AimPunchAngle.x * 2.0);
	auto punchY =DegreeToRadian((double)AimPunchAngle.y * 2.0);
	auto pointClip =  Vector3
	(
		(float)(-punchY / fovX),
		(float)(-punchX / fovY),
		0
	);
	
	return ViewportTransform(playViewMatrix,pointClip);
}