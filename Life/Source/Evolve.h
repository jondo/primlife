
#ifndef evolve_h
#define evolve_h

#include "Registry.h"
#include "Environ.h"

typedef struct {
  char* listStr;
  int   listValue;
} LISTENTRY;

//Fox BEGIN
LONG  APIENTRY ScreenSaverProc(HWND, UINT, WPARAM, LPARAM);
BOOL  CALLBACK ConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL  CALLBACK AboutDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL  CALLBACK SeedDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI   StartEvolving(LPVOID lpvParam);
//Fox END

BOOL  RegisterDialogClasses(HINSTANCE hInst);

//Fox BEGIN
void InitDlgItems(HWND hDlg, Environment* env);
void GetDlgItems(HWND hDlg, Environment* env);
int  GetComboBox(HWND hDlg, int id, int size, LISTENTRY* pEntry);
void LoadComboBox(HWND hDlg, int id, int size, int choice, LISTENTRY* pEntry);
//Fox END
void CreateNagScreen(HWND hWnd, HINSTANCE hInstance);
void ShutDown(Environment* env);
void StartUp(HWND hWnd, Environment** pEnv, HDC hScreen, int startNew = 2);
void GetDataFileName(char* szFile);
BOOL WINAPI NetworkDialog (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK SeedDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL WINAPI StatusDialog (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CheckActions(Environment* env, HWND hWnd);

// External resources
//extern CPrimRegistry g_registry;

extern int g_nagTime;
extern CSock g_sock;

extern char szTitle[];

#ifndef NOT_SAVER
extern BOOL bFirstTime;
#endif

#endif