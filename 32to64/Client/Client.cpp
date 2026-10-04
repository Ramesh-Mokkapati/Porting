// Client.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "..\\Server\\Server.h"


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Temperatire %d\n",GetTemperature());
	return 0;
}


