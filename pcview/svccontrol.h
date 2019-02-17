#ifndef SVCCONTROL_H
#define SVCCONTROL_H
#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")
VOID __stdcall DoStartSvc(LPCWSTR szSvcName);
VOID MyDoStopSvc(LPCWSTR szSvcName);
BOOL StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager);
#endif // SVCCONTROL_H
