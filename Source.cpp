#include "Windows.h"
#include "Direct.h"
#include "iostream"

using namespace std;


bool InstanceCheck(LPCTSTR game) 
{
	HANDLE handler = CreateMutex(NULL, TRUE, game);

	if (GetLastError() != ERROR_SUCCESS) 
	{
		HWND h = FindWindow(game, NULL);

		if (h) 
		{
			ShowWindow(h, SW_SHOWNORMAL);
			SetFocus(h);
			SetForegroundWindow(h);
			SetActiveWindow(h);

			cout << "Failed Instance Check - There is no game running." << endl;

			return false;
		}
	}

	cout << " Successful Instance Check - A game is running." << endl; 

	return true;
}

//Checking for enough storage space space 

bool StorageCheck(const DWORDLONG diskSpaceNeeded) 
{
	
	int const storage = _getdrive();
	struct _diskfree_t gotStorage;
	_getdiskfree(storage, &gotStorage);
	unsigned __int64 const spaceNeeded = diskSpaceNeeded / (gotStorage.sectors_per_cluster * gotStorage.bytes_per_sector);
	if (gotStorage.avail_clusters  <  spaceNeeded) 
	{
		// If the player doesn't have enough storage space
		cout << "Failed Storage Check - Not enough storage. Please check your storage and try again" << endl;
		
		return false;
	}
	return true;
}

//Checking for Memory 

bool MemoryCheck() 
{
	MEMORYSTATUSEX status;

	status.dwLength = sizeof(status);

	GlobalMemoryStatusEx(&status);
		
		cout << "You have " << status.ullAvailPhys << " bytes in Physical Memory Available." << endl;
		
		cout << "You have " << status.ullAvailVirtual << " bytes in Virtual Memory Available." << endl;
		
		return false;
		
}

//CPU Speed check

DWORD SpeedCheck() 
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		reinterpret_cast<LPCTSTR>("\\DESCRIPTION\\System\\CentralProcessor\\0"), 0,
		KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) {
		// query the key:
		RegQueryValueEx(hKey, reinterpret_cast<LPCTSTR>("MHz"), NULL, &type, (LPBYTE)
			&dwMHz, &BufSize);
	}
	return dwMHz;
}

int main()
{
	InstanceCheck("startGame");

	StorageCheck(300);

	MemoryCheck();

	cout << "Your CPU Speed is " << SpeedCheck() << endl;

	system("pause");

	return 0;

}