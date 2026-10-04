// ClientModified.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"

#ifdef _DEBUG
#import "..\\..\\Binaries\\Win32\\Debug\\COMServer.exe" no_namespace
#else
#import "..\\..\\Binaries\\Win32\\Release\\COMServer.exe" no_namespace
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);
	IServerWrapperPtr ptr;
	HRESULT hr = ptr.CreateInstance(__uuidof(ServerWrapper));
	if (SUCCEEDED(hr))
	{
		printf("Temperature %d\n",ptr->GetTemperature());
	}
	else
	{
		printf("Error on instantiation: 0x%x\n", hr);
	}
	::CoUninitialize();
	return 0;
}

