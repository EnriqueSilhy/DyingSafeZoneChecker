#define CURL_STATICLIB 
#include <iostream>
#include <Windows.h>
#include "curl/curl.h"
//#include <windows.h>
//#include <vector>
//#include <stdio.h>
//#include <stdlib.h>
#include <TlHelp32.h>
#include <Psapi.h> // GET RID OF THESE WHEN DISABLING ENUMs OR COMMENTING
#include <tchar.h>
//#include <ntstatus.h>
//#include <winternl.h>
//#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_a_debug.lib" )
#else
#pragma comment(lib, "curl/libcurl_a.lib" )
#endif

#pragma comment(lib, "Normaliz.lib" )
#pragma comment(lib, "Ws2_32.lib" )
#pragma comment(lib, "Wldap32.lib" )
#pragma comment(lib, "Crypt32.lib" )
#pragma comment(lib, "advapi32.lib" )
#pragma comment(lib, "winmm.lib")

using namespace std;


int main() {
    int y = 0;
    int x = 0;
RETRY:DWORD ProcessID;
    system("cls");

    HWND hwnd = FindWindowA(NULL, "Dying Light (inactive)");
    GetWindowThreadProcessId(hwnd, &ProcessID);

    //HANDLE handler = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ProcessID);
    HANDLE handler = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

    system("COLOR A");
    if (!hwnd) {
        while (1 == 1) {
            if (y != 6) {
                cout << "Program Not Found..." << endl;
                cout << "Retrying In " << y++;
                Sleep(1000);
                system("cls");
                goto RETRY;
            }
            else if (y == 5) {
                std::cout << "Error Code: " << GetLastError() << std::endl;
                cout << "Program Not Found" << endl;
                return 404;
            }
        }
    }
    else {


        cout << "Program Found!\n";

        cout << "Loading Libraries...\n";
        //This Section Will
        HMODULE moduleList[1024];
        DWORD moduleCount;
        if (EnumProcessModules(handler, moduleList, sizeof(moduleList), &moduleCount)) {
            for (int i = 0; i < moduleCount / sizeof(HMODULE); i++) {
                TCHAR moduleName[MAX_PATH];
                if (GetModuleBaseName(handler, moduleList[i], moduleName, sizeof(moduleName))) {
                    std::wcout << "Module name: " << moduleName << std::endl;
                }
            }
        }
        //This section will read all the ker/libs it must include
            //in more simple terms it will ready all the .DLLs that the porgram in this case Dying Light uses to load.
        cout << endl << endl << endl;
        cout << "Library \"gamedll_x64_rwdi.dll\" Found!\n";

        cout << "Everything Seems To Be In Order!\n";

        cout << "Attaching To Module...!\n";

        Sleep(3000);
        system("cls");
        //Due to some more advanced game were not gonna use the Normal "GetWindowThreadProcessId" Function, Instead We Will Use The "CreateToolhelp32Snapshot"
        //Since This Will Make It Easier For A x64 Bit App Like of this nature to attach
        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

        HANDLE moduleSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
        if (moduleSnapshotHandle == INVALID_HANDLE_VALUE)
        {
            std::cout << "Error creating snapshot: " << GetLastError() << std::endl;
            return 1;
        }

        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        if (!Module32First(moduleSnapshotHandle, &moduleEntry))
        {
            std::cout << "Error getting first module: " << GetLastError() << std::endl;
            CloseHandle(moduleSnapshotHandle);
            return 1;
        }
        //We are going to create a handle to a module for if the pointer we are trying to find is not the in Main.dll but in hidden in of these (see line 69 for context)
        HMODULE hModule = NULL;
        do
        {
            if (_tcsicmp(moduleEntry.szModule, L"gamedll_x64_rwdi.dll") == 0)
            {
                hModule = moduleEntry.hModule;
                break;
            }
        } while (Module32Next(moduleSnapshotHandle, &moduleEntry));

        if (hModule == NULL)
        {
            std::cout << "Module not found." << std::endl;
            CloseHandle(moduleSnapshotHandle);
            return 1;
        }


        //std::cout << "Module found: " << moduleEntry.szModule << std::endl;
        DWORD_PTR baseAddressTrueOrFalse = (DWORD_PTR)hModule + 0x01BAFA68;
        DWORD_PTR baseAddressIfInsideSafeZone = (DWORD_PTR)hModule + 0x01C15440;

        CloseHandle(moduleSnapshotHandle);
        CloseHandle(processHandle);

        DWORD_PTR baseAddress = (DWORD_PTR)hModule + 0xBE043D;
        cout << endl;
        cout << endl;

        DWORD_PTR buffer1;
        DWORD_PTR buffer2;
        DWORD_PTR safeZoneDisplay = 0;
        DWORD_PTR isInsideSafeZone;
        DWORD_PTR isInsideSafeZoneBuffer;
        int x = 0;
        int wannaContinue = 0;
//Values reverse engineering Checked and WORKING in dying light verison 1.49.00
        //Pointer That Check Safezone
            // When Pointer Is Set To == 1 The Game Understands Youre In A Safezone and when != 1 Youre Outside 
        ReadProcessMemory(handler, (LPCVOID)(baseAddressIfInsideSafeZone), &buffer1, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xA00), &buffer2, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x30), &buffer1, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xAA0), &buffer2, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x10), &buffer1, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xE00), &buffer2, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x0), &buffer1, sizeof(DWORD_PTR), NULL);
        ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x9A8), &safeZoneDisplay, sizeof(DWORD_PTR), NULL);

        while (0 == 0) {
            if ((DWORD)safeZoneDisplay == 0) {
                isInsideSafeZone = 0;
                ReadProcessMemory(handler, (LPCVOID)(baseAddressIfInsideSafeZone), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xA00), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x30), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xAA0), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x10), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xE00), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x0), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x9A8), &safeZoneDisplay, sizeof(DWORD_PTR), NULL);

                isInsideSafeZoneBuffer = 0;
            }
            else if ((DWORD)safeZoneDisplay == 1) {
                isInsideSafeZone = 1;
                ReadProcessMemory(handler, (LPCVOID)(baseAddressIfInsideSafeZone), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xA00), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x30), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xAA0), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x10), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xE00), &buffer2, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x0), &buffer1, sizeof(DWORD_PTR), NULL);
                ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x9A8), &safeZoneDisplay, sizeof(DWORD_PTR), NULL);
                isInsideSafeZoneBuffer = 1;
            }
            else {
                cout << (DWORD)safeZoneDisplay << endl;
                cout << "Critcal Error " << endl;
                return 404;
            }
            //if ((DWORD)isInsideSafeZoneBuffer != (DWORD)safeZoneDisplay) {

            //    if ((DWORD)safeZoneDisplay == 1) {
            //        PlaySound(TEXT("C:\\Users\\iHasPunchyFace\\source\\repos\\HTTP Request C++.\\EnteringSafezone.wav"), NULL, SND_FILENAME | SND_ASYNC);
            //        cout << "Currently Inside A Safe Zone" << endl;
            //        system("cls");

            //    }
            //    else if ((DWORD)safeZoneDisplay == 0) {
            //        PlaySound(TEXT("C:\\Users\\iHasPunchyFace\\source\\repos\\HTTP Request C++.\\LeavingSafezone.wav"), NULL, SND_FILENAME | SND_ASYNC);
            //        cout << "Currently Outside A Safe Zone" << endl;
            //        system("cls");


            //    }


                curl_global_init(CURL_GLOBAL_ALL);
                CURL* curl = nullptr;
                CURLcode res;

                curl = curl_easy_init();
                if (curl) {
                    std::string url = /*Enter URL HERE -> */"exampleURLThatTriggersLightBulb.com";//Enter a URL for example a light address so whenever you enter safezone the url can turn on a light.
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK)
                        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                    curl_easy_cleanup(curl);
                }

            }

        }



        //cout << "Done! Check Your Game!\n";
    }
//
//    Sleep(2000);
//    return 0;
//}









//Extra Sectioon Of Possible Pointers That Also Point To That Section
//-------------------------------------------------------------------------------------------------
// 
// 
//     *Please Note I Discarded These Pointers Since The Current Ones At Line 160+ Have Proven To Be Flawless and Super Stable At Game Ver 1.49.00*
// 
// 
//-------------------------------------------------------------------------------------------------








//HMODULE hModule = GetModuleHandle(L"engine_x64_rwdi.dll");
////HMODULE hModule = LoadLibrary(L"gameoverlayrenderer64.dll");
//if (hModule == NULL) {

//    std::cout << "Error: " << GetLastError() << std::endl;



//    return 404;

                //ReadProcessMemory(handler, (LPCVOID)(baseAddressIfInsideSafeZone), &bufferI, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0xA00), &bufferII, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x30), &bufferI, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0xAA0), &bufferII, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x10), &bufferI, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0xE00), &bufferII, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x0), &bufferI, sizeof(DWORD_PTR), NULL);
                //WriteProcessMemory(handler, (LPVOID)(bufferI + 0x9A8), &insideSafeZone, sizeof(DWORD_PTR), NULL);

    //ReadProcessMemory(handler, (LPCVOID)(baseAddressTrueOrFalse), &buffer1, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xCE0), &buffer2, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x0), &buffer1, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x770), &buffer2, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x8), &buffer1, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x28), &buffer2, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x20), &buffer1, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xA0), &safeZoneDisplay, sizeof(DWORD_PTR), NULL);

    //ReadProcessMemory(handler, (LPCVOID)(baseAddressIfInsideSafeZone), &bufferI, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0x3A0), &bufferII, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x48), &bufferI, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0xD8), &bufferII, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x188), &bufferI, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferI + 0x158), &bufferII, sizeof(DWORD_PTR), NULL);
    //ReadProcessMemory(handler, (LPCVOID)(bufferII + 0x20), &bufferI, sizeof(DWORD_PTR), NULL);
    //WriteProcessMemory(handler, (LPVOID)(bufferI + 0x950), &insideSafeZone, sizeof(DWORD_PTR), NULL);


 //ReadProcessMemory(handler, (LPCVOID)(baseAddressTrueOrFalse), &buffer1, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xCE0), &buffer2, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x0), &buffer1, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x770), &buffer2, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x8), &buffer1, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0x28), &buffer2, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer2 + 0x20), &buffer1, sizeof(DWORD_PTR), NULL);
                //ReadProcessMemory(handler, (LPCVOID)(buffer1 + 0xA0), &safeZoneDisplay, sizeof(DWORD_PTR), NULL);
                //WriteProcessMemory(handler, (LPVOID)(baseAddress), "\x44\x8B\x83\xA8\x09\x00\x00", 7, NULL);