// ServerWrapper.cpp: Implementierung von CServerWrapper

#include "stdafx.h"
#include "ServerWrapper.h"

#include "..\\Server\\Server.h"


// CServerWrapper


STDMETHODIMP CServerWrapper::GetTemperature(SHORT* temperature)
{
	*temperature = ::GetTemperature();

	return S_OK;
}
