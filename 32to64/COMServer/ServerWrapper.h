// ServerWrapper.h: Deklaration von CServerWrapper

#pragma once
#include "resource.h"       // Hauptsymbole

#include "COMServer.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Singlethread-COM-Objekte werden auf der Windows CE-Plattform nicht vollständig unterstützt. Windows Mobile-Plattformen bieten beispielsweise keine vollständige DCOM-Unterstützung. Definieren Sie _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, um ATL zu zwingen, die Erstellung von Singlethread-COM-Objekten zu unterstützen und die Verwendung eigener Singlethread-COM-Objektimplementierungen zu erlauben. Das Threadmodell in der RGS-Datei wurde auf 'Free' festgelegt, da dies das einzige Threadmodell ist, das auf Windows CE-Plattformen ohne DCOM unterstützt wird."
#endif



// CServerWrapper

class ATL_NO_VTABLE CServerWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CServerWrapper, &CLSID_ServerWrapper>,
	public IDispatchImpl<IServerWrapper, &IID_IServerWrapper, &LIBID_COMServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CServerWrapper()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SERVERWRAPPER)


BEGIN_COM_MAP(CServerWrapper)
	COM_INTERFACE_ENTRY(IServerWrapper)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

public:
	STDMETHOD(GetTemperature)(SHORT* temperature);
};

OBJECT_ENTRY_AUTO(__uuidof(ServerWrapper), CServerWrapper)
