#include <windows.h>
#include <stdio.h>
//#pragma comment(lib, "user32.lib")
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

void main()
{
   SYSTEM_INFO si;
 
   // Copy the hardware information to the SYSTEM_INFO structure. 

   ZeroMemory(&si, sizeof(SYSTEM_INFO));
   PGNSI pGnsi;
 
   pGnsi = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),TEXT("GetNativeSystemInfo"));
   if(pGnsi != NULL)
	   pGnsi(&si);
   GetSystemInfo(&si); 
 
   // Display the contents of the SYSTEM_INFO structure. 
   wprintf(L"hit");
   printf_s("Hardware information: \n");  
   printf_s("  OEM ID: %u\n", si.dwOemId);
   printf_s("  Number of processors: %u\n", si.dwNumberOfProcessors); 
   printf_s("  Page size: %u\n", si.dwPageSize); 
   printf_s("  Processor type: %u\n", si.dwProcessorType); 
   printf_s("  Minimum application address: %p \n", si.lpMinimumApplicationAddress); 
   printf_s("  Maximum application address: %p \n", si.lpMaximumApplicationAddress); 
   printf_s("  Active processor mask: %u\n", si.dwActiveProcessorMask); 
}
	
	
