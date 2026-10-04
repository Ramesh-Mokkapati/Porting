========================================================================
    AKTIVE VORLAGENBIBLIOTHEK: COMServer-Projektübersicht
========================================================================

Der Anwendungs-Assistent hat dieses COMServer-Projekt als Ausgangspunkt
zum Schreiben der ausführbaren Datei (EXE) erstellt.


Die Datei enthält eine Zusammenfassung des Inhalts der Dateien für
das Projekt.

COMServer.vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die mit dem Anwendungs-Assistenten generiert werden. 
    Sie enthält Informationen zu der Version von Visual C++, mit der die Datei generiert wurde, 
    sowie Informationen zu Plattformen, Konfigurationen und Projektfeatures, die mit dem
    Anwendungs-Assistenten ausgewählt wurden.

COMServer.idl
    Diese Datei enthält die IDL-Definitionen der Typbibliothek, die Schnittstellen
    und Co-Klassen, die im Projekt definiert sind.
    Diese Datei wird vom MIDL-Compiler verarbeitet, um Folgendes zu generieren:
        C++-Schnittstellendefinitionen und GUID-Deklarationen (COMServer.h)
        GUID-Definitionen                                (COMServer_i.c)
        Eine Typbibliothek                                   (COMServer.tlb)
        Marshallingcode                                 (COMServer_p.c and dlldata.c)

COMServer.h
    Diese Datei enthält die C++-Schnittstellendefinitionen und GUID-Deklarationen der
    in .idl definierten Elemente. Sie wird von MIDL während der Kompilierung erneut generiert.

COMServer.cpp
    Diese Datei enthält die Objekttabelle und die Implementierung von WinMain.

COMServer.rc
    Hierbei handelt es sich um eine Auflistung aller Ressourcen von Microsoft Windows, die
    vom Programm verwendet werden.


/////////////////////////////////////////////////////////////////////////////
Weitere Standarddateien:

StdAfx.h, StdAfx.cpp
    Mit diesen Dateien werden vorkompilierte Headerdateien (PCH)
    mit der Bezeichnung COMServer.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj erstellt.

Resource.h
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.

/////////////////////////////////////////////////////////////////////////////
Proxy/Stub-DLL-Projekt und Moduldefinitionsdatei:

COMServerps.vcproj
    Dies ist die Projektdatei zum Erstellen einer Proxy/Stub-DLL.
	Die IDL-Datei im Hauptprojekt muss mindestens eine Schnittstelle enthalten. Die IDL-Datei muss 
	vor dem Erstellen der Proxy/Stub-DLL kompiliert werden.	In diesem Prozess werden
	dlldata.c, COMServer_i.c und COMServer_p.c generiert, die erforderlich sind, um
	die Proxy/Stub-DLL zu generieren.

COMServerps.def
    Die Moduldefinitionsdatei enthält die Linkerinformationen zu den Exporten, 
    die für den Proxy/Stub erforderlich sind.

/////////////////////////////////////////////////////////////////////////////