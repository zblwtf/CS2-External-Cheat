#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <Tlhelp32.h>
#include <atlconv.h>
#include "../driver.hpp"
#define _is_invalid(v) if(v==NULL) return false
#define _is_invalid(v,n) if(v==NULL) return n

/*
	@Liv github.com/TKazer
*/

/// <summary>
/// ����״̬��
/// </summary>
enum StatusCode
{
	SUCCEED,
	FAILE_PROCESSID,
	FAILE_HPROCESS,
	FAILE_MODULE,
};

/// <summary>
/// ���̹���
/// </summary>
class ProcessManager 
{
private:

	bool   Attached = false;
	
public:
	HANDLE driver_handle;
	HANDLE hProcess = 0;
	DWORD  ProcessID = 0;
	DWORD64  ModuleAddress = 0;

public:
	~ProcessManager()
	{
		//if (hProcess)
			//CloseHandle(hProcess);
	}

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="ProcessName">������</param>
	/// <returns>����״̬��</returns>
	StatusCode Attach(std::string ProcessName)
	{
		ProcessID = this->GetProcessID(ProcessName);
		_is_invalid(ProcessID, FAILE_PROCESSID);

		
		driver_handle = CreateFileW(L"\\\\.\\lilyweeDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);
		if (driver_handle == INVALID_HANDLE_VALUE) {
			std::cout << "Failed to create our driver handle.\n";
			std::cin.get();
			return FAILE_HPROCESS;
		}
		else {
			std::cout << "[+]create our driver success.\n";
		}

		if (driver::attach_to_process(driver_handle, ProcessID) == false)
		{
			return FAILE_HPROCESS;
		}


		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, ProcessID);
		_is_invalid(hProcess, FAILE_HPROCESS);

		ModuleAddress = reinterpret_cast<DWORD64>(this->GetProcessModuleHandle(ProcessName));
		_is_invalid(ModuleAddress, FAILE_MODULE);

		Attached = true;

		return SUCCEED;
	}

	/// <summary>
	/// ȡ������
	/// </summary>
	void Detach()
	{
		if (hProcess)
			CloseHandle(hProcess);
		hProcess = 0;
		ProcessID = 0;
		ModuleAddress = 0;
		Attached = false;
	}

	/// <summary>
	/// �жϽ����Ƿ񼤻�״̬
	/// </summary>
	/// <returns>�Ƿ񼤻�״̬</returns>
	bool IsActive()
	{
		if (!Attached)
			return false;
		return true;
		/*DWORD ExitCode{};
		GetExitCodeProcess(hProcess, &ExitCode);
		return ExitCode == STILL_ACTIVE;*/
	}

	/// <summary>
	/// ��ȡ�����ڴ�
	/// </summary>
	/// <typeparam name="ReadType">��ȡ����</typeparam>
	/// <param name="Address">��ȡ��ַ</param>
	/// <param name="Value">��������</param>
	/// <param name="Size">��ȡ��С</param>
	/// <returns>�Ƿ��ȡ�ɹ�</returns>
	template <typename ReadType>
	bool ReadMemory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid(hProcess,false);
		_is_invalid(ProcessID, false);

		driver::read_memory<ReadType>(this->driver_handle, Address,Value,Size);
		/*if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, Size, 0))
			return true;*/
		return true;
	}

	template <typename ReadType>
	bool ReadMemory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		Value = driver::read_memory<ReadType>(this->driver_handle, Address);
		return true;
		/*if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, sizeof(ReadType), 0))
			return true;
		return false;*/
	}

	/// <summary>
	/// д������ڴ�
	/// </summary>
	/// <typeparam name="ReadType">д������</typeparam>
	/// <param name="Address">д���ַ</param>
	/// <param name="Value">д������</param>
	/// <param name="Size">д���С</param>
	/// <returns>�Ƿ�д��ɹ�</returns>
	template <typename ReadType>
	bool WriteMemory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		driver::write_memory(this->driver_handle, Address, Value,Size);
		/*if (WriteProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, Size, 0))
			return true;
		return false;*/
	}

	template <typename ReadType>
	bool WriteMemory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid(hProcess, false);
		_is_invalid(ProcessID, false);

		driver::write_memory(this->driver_handle, Address, Value);
		/*if (WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(Address), &Value, sizeof(ReadType), 0))
			return true;
		return false;*/
	}



	DWORD64 TraceAddress(DWORD64 BaseAddress, std::vector<DWORD> Offsets)
	{
		_is_invalid(hProcess,0);
		_is_invalid(ProcessID,0);
		DWORD64 Address = 0;

		if (Offsets.size() == 0)
			return BaseAddress;

		if (!ReadMemory<DWORD64>(BaseAddress, Address))
			return 0;
	
		for (int i = 0; i < Offsets.size() - 1; i++)
		{
			if (!ReadMemory<DWORD64>(Address + Offsets[i], Address))
				return 0;
		}
		return Address == 0 ? 0 : Address + Offsets[Offsets.size() - 1];
	}

public:

	DWORD GetProcessID(std::string ProcessName)
	{
		PROCESSENTRY32 ProcessInfoPE;
		ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(15, 0);
		Process32First(hSnapshot, &ProcessInfoPE);
		USES_CONVERSION;
		do {
			if (strcmp(W2A(ProcessInfoPE.szExeFile), ProcessName.c_str()) == 0)
			{
				CloseHandle(hSnapshot);
				return ProcessInfoPE.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &ProcessInfoPE));
		CloseHandle(hSnapshot);
		return 0;
	}

	HMODULE GetProcessModuleHandle(std::string ModuleName)
	{
		MODULEENTRY32 ModuleInfoPE;
		ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->ProcessID);
		Module32First(hSnapshot, &ModuleInfoPE);
		USES_CONVERSION;
		do {
			if (strcmp(W2A(ModuleInfoPE.szModule), ModuleName.c_str()) == 0)
			{
				CloseHandle(hSnapshot);
				return ModuleInfoPE.hModule;
			}
		} while (Module32Next(hSnapshot, &ModuleInfoPE));
		CloseHandle(hSnapshot);
		return 0;
	}

};

inline ProcessManager ProcessMgr;