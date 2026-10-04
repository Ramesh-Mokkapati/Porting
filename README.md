# Win32 → Win64 Porting Toolkit

A Visual Studio 2022 solution demonstrating two complementary approaches to porting legacy 32-bit Windows C++ code to 64-bit, together with a GUI tool that automates the most repetitive source-level transformations.

---

## Solution layout

```
Porting.sln
├── PortAssist/          — GUI porting-assistant tool  (x64 MFC dialog app)
├── Genie/               — Flex-based MBCS→Unicode migration tool  (Win32 MFC dialog app)
├── PortingLib/          — Shared static library: API tables and fix routines  (Win32 + x64)
└── 32to64/
    ├── Server/          — Legacy 32-bit DLL  (Win32)
    ├── Client/          — Original 32-bit client  (Win32 EXE)
    ├── COMServer/       — ATL COM out-of-process wrapper  (Win32 EXE)
    ├── COMServerPS/     — COM proxy/stub DLL  (Win32 DLL)
    └── ClientModified/  — Ported 64-bit client  (x64 EXE)
```

### Solution configurations

| Solution config | Typical use |
|---|---|
| `Debug\|x86` / `Release\|x86` | Build all projects for 32-bit |
| `Debug\|x64` / `Release\|x64` | Build all projects; `ClientModified` and `PortAssist` are x64, everything else is Win32 |

---

## Projects

### PortAssist  *(x64, MFC dialog application)*

An interactive GUI tool that scans a folder of C/C++ source files and performs in-place (or backed-up) identifier-level text transformations to help migrate legacy code.  The parser is comment- and string-literal-aware: replacements are never made inside `// line comments`, `/* block comments */`, `"string literals"`, or `'char literals'`.

Two transformation modes are available via the tab control.

---

#### Tab 1 — MBCS to Unicode

Replaces narrow-string (ANSI/MBCS) types, CRT functions, and Win32 helpers with their generic-text (`TCHAR` / `_t*` / `_tcs*`) equivalents, so the code compiles correctly under both the **Multi-Byte Character Set** and **Unicode** project settings.

**Data types**

| MBCS (before) | Generic-text (after) |
|---|---|
| `CHAR` | `TCHAR` |
| `char` | `TCHAR` *(not replaced when preceded by `unsigned` or `signed`)* |
| `LPSTR` | `LPTSTR` |
| `LPCSTR` | `LPCTSTR` |

**Constants and globals**

| Before | After |
|---|---|
| `EOF` | `_TEOF` |
| `_environ` | `_tenviron` |
| `_finddata_t` | `_tfinddata_t` |
| `TRACE0`, `TRACE2` | `TRACE` |

**Character classification**

| Before | After |
|---|---|
| `isalnum` | `_istalnum` |
| `isalpha` | `_istalpha` |
| `__isascii` | `_istascii` |
| `iscntrl` | `_istcntrl` |
| `isdigit` | `_istdigit` |
| `isgraph` | `_istgraph` |
| `islower` | `_istlower` |
| `isprint` | `_istprint` |
| `ispunct` | `_istpunct` |
| `isspace` | `_istspace` |
| `isupper` | `_istupper` |
| `isxdigit` | `_istxdigit` |
| `tolower` | `_totlower` |
| `toupper` | `_totupper` |

**String functions**

| Before | After |
|---|---|
| `strcat` | `_tcscat` |
| `strchr` | `_tcschr` |
| `strcmp` | `_tcscmp` |
| `strcoll` | `_tcscoll` |
| `strcpy` | `_tcscpy` |
| `strcspn` | `_tcscspn` |
| `strlen` | `_tcslen` |
| `strncat` | `_tcsnccat` |
| `strncmp` | `_tcsnccmp` |
| `strncpy` | `_tcsnccpy` |
| `strpbrk` | `_tcspbrk` |
| `strrchr` | `_tcsrchr` |
| `strspn` | `_tcsspn` |
| `strstr` | `_tcsstr` |
| `strtod` | `_tcstod` |
| `strtok` | `_tcstok` |
| `strtol` | `_tcstol` |
| `strtoul` | `_tcstoul` |
| `strxfrm` | `_tcsxfrm` |
| `_strdup` | `_tcsdup` |
| `_stricmp` / `stricmp` | `_tcsicmp` |
| `_stricoll` | `_tcsicoll` |
| `_strlwr` | `_tcslwr` |
| `_strnicmp` / `strnicmp` | `_tcsnicmp` |
| `_strnicoll` | `_tcsnicoll` |
| `_strrev` | `_tcsrev` |
| `_strset` | `_tcsset` |
| `strupr` / `_strupr` | `_tcsupr` |
| `strftime` | `_tcsftime` |
| `strtoi64` / `_strtoi64` | `_tcstoi64` |

**Formatted I/O**

| Before | After |
|---|---|
| `printf` | `_tprintf` |
| `fprintf` | `_ftprintf` |
| `sprintf` | `_stprintf` |
| `_snprintf` | `_sntprintf` |
| `scanf` | `_tscanf` |
| `fscanf` | `_ftscanf` |
| `sscanf` | `_stscanf` |
| `vprintf` | `_vtprintf` |
| `vfprintf` | `_vftprintf` |
| `vsprintf` | `_vstprintf` |
| `vsnprintf` / `_vsnprintf` | `_vsntprintf` |

**Character I/O**

| Before | After |
|---|---|
| `getc` | `_gettc` |
| `getchar` | `_gettchar` |
| `gets` | `_getts` |
| `putc` | `_puttc` |
| `putchar` | `_puttchar` |
| `puts` | `_putts` |
| `fgetc` | `_fgettc` |
| `fgetchar` | `_fgettchar` |
| `fgets` | `_fgetts` |
| `fputc` | `_fputtc` |
| `fputchar` | `_fputtchar` |
| `fputs` | `_fputts` |
| `ungetc` | `_ungettc` |

**Numeric conversion**

| Before | After |
|---|---|
| `atoi` | `_ttoi` |
| `atoi64` | `_ttoi64` |
| `atol` | `_ttol` |
| `itoa` / `_itoa` | `_itot` |
| `ltoa` / `_ltoa` | `_ltot` |
| `ultoa` / `_ultoa` | `_ultot` |
| `_i64toa` | `_i64tot` |
| `_ui64toa` | `_ui64tot` |

**File and path operations**

| Before | After |
|---|---|
| `fopen` | `_tfopen` |
| `freopen` | `_tfreopen` |
| `fdopen` / `_fdopen` | `_tfdopen` |
| `_fsopen` | `_tfsopen` |
| `_fullpath` | `_tfullpath` |
| `_makepath` | `_tmakepath` |
| `_splitpath` | `_tsplitpath` |
| `_stat` | `_tstat` |
| `_tempnam` | `_ttempnam` |
| `tmpnam` | `_ttmpnam` |
| `_mktemp` | `_tmktemp` |
| `rename` | `_trename` |
| `_searchenv` | `_tsearchenv` |

**Directory and process operations**

| Before | After |
|---|---|
| `mkdir` / `_mkdir` | `_tmkdir` |
| `rmdir` / `_rmdir` | `_trmdir` |
| `chdir` / `_chdir` | `_tchdir` |
| `getcwd` / `_getcwd` | `_tgetcwd` |
| `access` / `_access` | `_taccess` |
| `chmod` / `_chmod` | `_tchmod` |
| `creat` / `_creat` | `_tcreat` |
| `_open` | `_topen` |
| `_popen` | `_tpopen` |
| `_sopen` | `_tsopen` |
| `getenv` | `_tgetenv` |
| `_putenv` | `_tputenv` |
| `setlocale` | `_tsetlocale` |
| `system` | `_tsystem` |
| `perror` | `_tperror` |

**File search**

| Before | After |
|---|---|
| `_findfirst` | `_tfindfirst` |
| `_findnext` | `_tfindnext` |

**Time functions**

| Before | After |
|---|---|
| `asctime` | `_tasctime` |
| `ctime` | `_tctime` |
| `_strdate` | `_tstrdate` |
| `_strtime` | `_tstrtime` |
| `_utime` | `_tutime` |

**Spawn and exec**

| Before | After |
|---|---|
| `_execl` … `_execvpe` (8 variants) | `_texecl` … `_texecvpe` |
| `_spawnl` … `_spawnvpe` (8 variants) | `_tspawnl` … `_tspawnvpe` |

**Entry point**

| Before | After |
|---|---|
| `WinMain` | `_tWinMain` |

**Special-case fix**

After applying all replacements, the tool reverses any `TCHAR`-ification of the MFC debug macro pattern:

```cpp
// before (incorrect after bulk replacement):
static TCHAR THIS_FILE[] = __FILE__;

// restored to:
static char THIS_FILE[] = __FILE__;
```

`__FILE__` always expands to a narrow string literal in MSVC; `THIS_FILE` must therefore remain `char`.

**Identifiers flagged for manual review**

The following identifiers are not replaced automatically but are flagged with a warning because they require human judgement:

| Identifier | Why manual review is needed |
|---|---|
| `MultiByteToWideChar` | Explicit ANSI↔Unicode conversion — may need to be removed or restructured |
| `WideCharToMultiByte` | Same as above |
| `GetProcAddress` | Function-name string argument may itself need an `A`/`W` suffix correction |
| `sizeof` | Result used as a byte count may silently truncate on pointer-sized types |

Additionally, any identifier that ends in a literal `A` or `W` suffix and contains lowercase letters (e.g. `CreateFileA`, `CharNextW`) is flagged, because it is an explicit narrow/wide Win32 API call that the generic-text macros should replace.

---

#### Tab 2 — x64 Porting (32-bit → 64-bit)

Replaces Win32 integer types that are **not pointer-safe** on a 64-bit build with their `_PTR`-suffixed equivalents, which are 32 bits wide on Win32 and 64 bits wide on Win64.

| 32-bit type (before) | Pointer-safe type (after) |
|---|---|
| `DWORD` | `DWORD_PTR` |
| `UINT` | `UINT_PTR` |
| `ULONG` | `ULONG_PTR` |

These replacements prevent silent pointer truncation when values derived from or compared with pointer arithmetic are stored in plain 32-bit integers.

The same warning flags from the Unicode mode also apply here: `sizeof`, `GetProcAddress`, and explicit `A`/`W`-suffixed APIs are flagged for manual review.

---

#### Controls

| Control | Description |
|---|---|
| **Select Folder** | Root directory of the source tree to transform; recurses into all subdirectories |
| **File patterns** | Extensions to process (`.cpp`, `.h`, `.hpp`, `.c`) |
| **Unicode Porting / x64 Porting** | Selects which transformation table to apply |
| **Overwrite** | Writes changes directly back to each source file |
| **Save backup** | Copies the original to `<filename>.bak` before writing |

After processing, a dialog reports how many files were modified and how many failed.

Output binary is written to `Binaries\x64\Debug\` or `Binaries\x64\Release\`.

---

### Genie  *(Win32, MFC dialog application)*

Genie is a second-generation MBCS-to-Unicode migration tool that performs the same identifier replacements as PortAssist but drives them through a **flex-generated lexer** (`MiniC.l` / `win_flex`) instead of a custom text scanner.  Because the lexer tokenises the source file according to C/C++ grammar rules, replacements are strictly token-accurate: a match only fires on a complete identifier token, never inside a larger word or across a token boundary.

Genie additionally **wraps bare string and character literals in `_T()`**, which PortAssist does not do.  Already-wrapped literals (`L"..."`, `_T("...")`, `TEXT("...")`) are passed through unchanged.

#### How the flex lexer processes each file

The lexer rules in `MiniC.l` handle every significant token class:

| Token / pattern | Action |
|---|---|
| `#include <…>` / `#include "…"` / `extern "C"` | Passed through unchanged (`ECHO`) |
| String literal `"…"` or char literal `'…'` not already inside `L`, `_T`, or `TEXT` | Wrapped: `"hello"` → `_T("hello")` |
| Already-wrapped literal `L"…"`, `_T("…")`, `TEXT("…")` | Passed through unchanged |
| Identifier matched in `BadAPIs[]` table | Replaced with generic-text equivalent (see table below) |
| Identifier in `WarnAPIs[]` or ending with `A`/`W` suffix | Emitted unchanged; warning written to `stderr` |
| `/* … */` block comment | Passed through unchanged |
| `// … \n` line comment | Passed through unchanged |
| All other characters | Passed through unchanged |

#### Transformation table (MBCS → Unicode)

The `BadAPIs[]` array in `PortingLib/FixAPI.cpp` is **sorted at startup** and looked up with `bsearch`, giving O(log *n*) per-token lookup.  The entries cover the same categories as PortAssist:

- **Data types** — `CHAR`→`TCHAR`, `char`→`TCHAR`, `LPSTR`→`LPTSTR`, `LPCSTR`→`LPCTSTR`
- **Constants / globals** — `EOF`→`_TEOF`, `_environ`→`_tenviron`, `_finddata_t`→`_tfinddata_t`, `TRACE0`/`TRACE2`→`TRACE`
- **Character classification** — `isalpha`→`_istalpha`, `isdigit`→`_istdigit`, `tolower`→`_totlower`, etc.
- **String functions** — `strlen`→`_tcslen`, `strcpy`→`_tcscpy`, `strcat`→`_tcscat`, `strcmp`→`_tcscmp`, `strtok`→`_tcstok`, etc.
- **Formatted I/O** — `printf`→`_tprintf`, `fprintf`→`_ftprintf`, `sprintf`→`_stprintf`, `scanf`→`_tscanf`, etc.
- **Character I/O** — `getc`→`_gettc`, `fgets`→`_fgetts`, `puts`→`_putts`, `ungetc`→`_ungettc`, etc.
- **Numeric conversion** — `atoi`→`_ttoi`, `atol`→`_ttol`, `itoa`→`_itot`, `ltoa`→`_ltot`, etc.
- **File / path / directory** — `fopen`→`_tfopen`, `_mkdir`→`_tmkdir`, `_getcwd`→`_tgetcwd`, `rename`→`_trename`, etc.
- **Process / exec / spawn** — `_execl`…`_execvpe` (8 variants), `_spawnl`…`_spawnvpe` (8 variants)
- **Time** — `asctime`→`_tasctime`, `ctime`→`_tctime`, `_strdate`→`_tstrdate`, etc.
- **Entry point** — `WinMain`→`_tWinMain`

#### Identifiers flagged for manual review

| Identifier | Reason |
|---|---|
| `MultiByteToWideChar` | Explicit ANSI↔Unicode conversion — may need restructuring |
| `WideCharToMultiByte` | Same as above |
| `GetProcAddress` | Function-name string argument may need an `A`/`W` suffix correction |
| `sizeof` | Result used as a byte count may truncate on pointer-sized types |
| Any identifier ending in `A` or `W` with mixed case (e.g. `CreateFileA`, `CharNextW`) | Explicit narrow/wide Win32 API call |

Warnings are emitted to `stderr` in Visual Studio's error-list format (`file(line) : warning ToUnicode: …`) so they appear directly in the Output window.

#### Comparison with PortAssist

| | PortAssist | Genie |
|---|---|---|
| Parser engine | Custom C++ text scanner | Flex-generated lexer (`MiniC.l`) |
| String / char literals | Not modified | Wrapped in `_T()` |
| Comment handling | Manual scan (skip `//` and `/* */`) | Flex token rules |
| Identifier lookup | Linear search | Binary search on sorted table |
| x64 type replacements | Yes (second tab) | Not included |
| Output platform | x64 | Win32 only |

#### Build-time dependency — win_flex

The `MiniC.l` grammar is processed at build time by `win_flex.exe`, which regenerates `MiniC.h` (the scanner implementation).  The `win_flex.exe` binary and the MSBuild integration files are committed under `custom_build_rules\` at the solution root.  No separate installation is required.

#### Controls

| Control | Description |
|---|---|
| **Select Folder** | Root directory of the source tree to transform (recurses into subdirectories) |
| **File patterns** | Extensions to process (`.cpp`, `.h`, `.c`) |
| **Overwrite** | Writes the transformed file directly back in place |
| **Save backup** | Copies the original to `<filename>.bak` before writing |

Output binary is written to `Binaries\Win32\Debug\` or `Binaries\Win32\Release\`.

---

### 32to64 demo — calling a 32-bit DLL from a 64-bit process via COM

This sub-solution demonstrates the standard Windows technique for allowing a 64-bit application to consume a 32-bit DLL without recompiling the DLL: wrap the DLL in a 32-bit COM **out-of-process** EXE server. COM handles all cross-bitness marshaling transparently. See [Accessing 32-bit DLLs from 64-bit code](https://blog.mattmags.com/2007/06/30/accessing-32-bit-dlls-from-64-bit-code/).

#### Architecture

```
┌──────────────────────────┐        COM / DCOM (out-of-process)
│  ClientModified.exe      │ ──────────────────────────────────────►  ┌──────────────────────┐
│  (64-bit)                │                                          │  COMServer.exe       │
│                          │  IServerWrapper::GetTemperature()        │  (32-bit ATL EXE)    │
│  Uses #import to obtain  │ ◄──────────────────────────────────────  │                      │
│  a type library and      │                                          │  ServerWrapper       │
│  create the COM object   │                                          │  calls directly:     │
└──────────────────────────┘                                          │  ::GetTemperature()  │
                                                                      └──────────┬───────────┘
                                                                                 │ LoadLibrary
                                                                                 ▼
                                                                      ┌──────────────────────┐
                                                                      │  Server.dll          │
                                                                      │  (32-bit)            │
                                                                      │  GetTemperature()→42 │
                                                                      └──────────────────────┘
```

#### Project descriptions

**Server** *(Win32 DLL)*  
The original legacy DLL. Exports a single function:
```cpp
SERVER_API int GetTemperature(void);   // returns 42
```
Cannot be loaded into a 64-bit process directly.

---

**Client** *(Win32 EXE)*  
The original 32-bit client. Links `Server.dll` directly and calls `GetTemperature()`. Demonstrates the pre-porting baseline.

---

**COMServer** *(Win32 ATL out-of-process EXE COM server)*  
Wraps `Server.dll` behind a COM interface. Because it is a **32-bit EXE**, it can load `Server.dll` and Windows COM can activate it from a 64-bit client in a separate process.

Key files:
- `COMServer.idl` — defines `IServerWrapper` with `GetTemperature([out] SHORT* temperature)`.
- `ServerWrapper.cpp` — implements `IServerWrapper`, calls `::GetTemperature()` from `Server.dll`.
- `COMServer.cpp` — ATL `CAtlExeModuleT` host; supports `/RegServer` and `/UnregServer`.

The post-build step calls `/RegServer` to register the object in `HKCR`. `Server.dll` and `COMServer.exe` share the same output directory (`Binaries\Win32\$(Configuration)\`), so no copy step is needed.

> **Note:** `/RegServer` writes to `HKEY_CLASSES_ROOT` and requires administrator privileges. If the build post-step fails due to UAC the binaries are still produced correctly; register manually with `COMServer.exe /RegServer` from an elevated prompt.

---

**COMServerPS** *(Win32 proxy/stub DLL)*  
Standard ATL-generated proxy/stub DLL for the `IServerWrapper` interface. Required when the COM client and server run in different apartments or processes. Built from the MIDL-generated `COMServer_i.c`, `COMServer_p.c`, and `dlldata.c`.

---

**ClientModified** *(x64 EXE)*  
The ported 64-bit client. Uses `#import` to consume the type library embedded in `COMServer.exe` and calls `GetTemperature()` through COM:

```cpp
IServerWrapperPtr ptr;
ptr.CreateInstance(__uuidof(ServerWrapper));
printf("Temperature %d\n", ptr->GetTemperature());
```

COM activates `COMServer.exe` as a 32-bit out-of-process server automatically. The 64-bit client never touches `Server.dll` directly.

---

## Prerequisites

| Requirement | Version |
|---|---|
| Visual Studio | 2022 (v143 toolset) |
| Windows SDK | 10.0.26100.0 or later |
| ATL | Included with VS (Desktop development with C++ workload) |
| MFC | Included with VS (Desktop development with C++ workload) |

---

## Building

Open `Porting.sln` in Visual Studio 2022 and choose **Build → Rebuild Solution**.  
Select `Release|x64` for the normal full build.

Build outputs:

| Project | Output location |
|---|---|
| PortAssist | `Binaries\x64\Release\PortAssist.exe` |
| Genie | `Binaries\Win32\Release\Genie.exe` |
| Server.dll | `Binaries\Win32\Release\Server.dll` + `Server.lib` |
| Client.exe | `Binaries\Win32\Release\Client.exe` |
| COMServer.exe | `Binaries\Win32\Release\COMServer.exe` |
| COMServerPS.dll | `Binaries\Win32\Release\COMServerPS.dll` |
| ClientModified.exe | `Binaries\x64\Release\ClientModified.exe` |

---

## Running the 32-to-64 demo

1. Build the solution in `Release|x64`.
2. From an **elevated** command prompt, register the COM server and proxy/stub:
   ```
   Binaries\Win32\Release\COMServer.exe /RegServer
   regsvr32 Binaries\Win32\Release\COMServerPS.dll
   ```
3. Run the original 32-bit client (calls `Server.dll` directly):
   ```
   Binaries\Win32\Release\Client.exe
   ```
4. Run the ported 64-bit client (calls `Server.dll` via COM):
   ```
   Binaries\x64\Release\ClientModified.exe
   ```
Both should print `Temperature 42`.

---

## Project dependency graph

```
PortingLib
 ├── PortAssist      (links PortingLib.lib — x64 variant)
 └── Genie           (links PortingLib.lib — Win32 variant, built via pre-build event)

Server
 ├── Client          (links Server.lib)
 └── COMServer       (links Server.lib)
      ├── COMServerPS   (compiles MIDL output from COMServer)
      └── ClientModified (#imports type library from COMServer.exe)
```

---

## Notable build fixes applied during modernisation

The projects originated from a Visual Studio 2005 solution and required the following fixes to build cleanly with VS 2022 and Windows SDK 10.0.26100.0:

- **`rpcndr.lib` removed** from Windows SDK 10.0.26100.0 — removed from `COMServerPS` linker inputs (its symbols are now part of `rpcrt4.lib`).
- **MIDL `-no_robust` / `-target` conflict** — removed the deprecated `<ValidateAllParameters>false</ValidateAllParameters>` MIDL property from `COMServer.vcxproj`; it emits `-no_robust` which conflicts with the `-target` flag in the VS 2022 MIDL compiler.
- **Proxy/stub Windows Vista minimum** — `COMServer_p.c` (MIDL-generated) uses Vista-era marshaling; raised `_WIN32_WINNT` from `0x0500` to `0x0600` in `COMServerPS.vcxproj`.
- **`Server.dll` not found at registration** — initially fixed with an `xcopy` post-build step; superseded by the unified output directory change below, which co-locates the binaries automatically without any copy step.
- **Library path updates** — all `AdditionalDependencies` paths updated to `$(SolutionDir)Binaries\$(Platform)\$(Configuration)\Server.lib` to match the unified output layout.
- **Unified output directory** — all projects write to `$(SolutionDir)Binaries\$(Platform)\$(Configuration)\`; `Server.dll` and `COMServer.exe` therefore co-locate automatically.
- **PortingLib static library** — `FixAPI.cpp` / `FixAPI.h` (API replacement tables and lookup routines) extracted from both `PortAssist` and `Genie` into a shared `PortingLib` static library project.  PortAssist links the x64 variant; Genie always links the Win32 variant, which its pre-build event builds explicitly when the solution is configured for x64.
- **Project build-order dependencies** — added explicit `ProjectDependencies` in `Porting.sln` (Client→Server, COMServer→Server, ClientModified→COMServer, COMServerPS→COMServer) so parallel builds do not race on shared outputs.
- **`COMServer` solution-config mapping** — corrected `Release|x64` solution config to build `COMServer` as `Release|Win32` (the server must run as 32-bit to load the 32-bit `Server.dll`; `Debug|x64` already mapped to `Debug|Win32`).
