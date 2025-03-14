#pragma once

class Memory
{

private:
	DWORD pId;
	DWORD modulBaseAddr;
	HANDLE pHandleapp;
	HWND hwnd;

public:
	void GetpHande()
	{
		pHandleapp = OpenProcess( PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, pId );

		// if ( pHandle == ERROR_INVALID_PARAMETER ) { throw std::runtime_error("No Find Proccess aka pHandle"); }

		// if ( pHandle == ERROR_ACCESS_DENIED ) { throw std::runtime_error("Access denied"); }

		if ( pHandleapp == NULL ) { throw std::runtime_error("Error pHandle"); }
	}

	void GetPid( const char *name )
	{
		hwnd = FindWindow(NULL, (TCHAR*)name );

		if ( hwnd == 0 ) { throw std::runtime_error("No Find Proccess aka pId"); }

		GetWindowThreadProcessId(hwnd, &pId);
	}

	void GetModuleBaseAddress( const char* lpszModuleName ) {
		HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pId ); 
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof( MODULEENTRY32 );

		if (Module32First(hSnapshot, &ModuleEntry32))
		{
			do 
			{
				if (_tcscmp(ModuleEntry32.szModule, (TCHAR*)lpszModuleName) == 0) 
				{
					modulBaseAddr = (DWORD_PTR)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32)); 

		}
		CloseHandle(hSnapshot);
	}

	DWORD GetPointerAddress(DWORD address, std::vector<DWORD> offsets)
	{
		DWORD pointeraddress = 0;
		
		ReadProcessMemory(pHandleapp, (LPVOID*)( modulBaseAddr + address ), &pointeraddress, sizeof(pointeraddress), 0);

		for (int i = 0; i < offsets.size() - 1; i++) { ReadProcessMemory(pHandleapp, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0); }

		return pointeraddress += offsets.at(offsets.size() - 1);
	}

	int ReadMemory( DWORD adress )
	{
		int value;
		ReadProcessMemory(pHandleapp, (LPVOID *)adress,  &value, sizeof( value ), 0);
		return value;
	}

	template<typename T>
	void WriteMemory( DWORD adress, T value )
	{
		WriteProcessMemory(pHandleapp, (LPVOID *)adress,  &value, sizeof( value ), 0);
	}	

	bool autoRun( const char* name, const char* namefull )
	{
		GetPid( name );
		GetpHande();
		GetModuleBaseAddress( namefull );

		return true;
	}

};
