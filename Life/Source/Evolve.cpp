/******************************Module*Header*******************************\
* Module Name: evolve.cpp
*
* Evolve generation Screen saver
*
* Created: 6-Aug-1995
* Author : Jason Spofford
*				         ---FOX WORKED ON THIS WHOLE DOC---
*
\**************************************************************************/
#include "stdafx.h"
#include "Primordial Life.h"
#include <stdio.h>
#include <math.h>
#include "evolve.h"
#include "resource.h"

BOOL CALLBACK HabitatOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BiotOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void LoadComboBox(HWND hDlg, int id, int size, int choice, LISTENTRY* pEntry);


HWND TABW, CURRW;//The tab window handle & current window handle: Fox

CKeyRegistry  g_key;

char szTitle[]          = "Primordial Life 4.0";

BOOL bRegisterUp        = FALSE;

char szFile[MAX_PATH];

const int nagWidth  = 400;
const int nagHeight = 100;
const int interval  = 10;

const int REGENCOST_OPTIONS=5;
LISTENTRY regenCost[REGENCOST_OPTIONS] = {
  {"Nothing",  0},
  {"Cheap", 100},
  {"Normal", 200},
  {"Expensive", 300},
  {"Too Expensive", 400}
};

const int REGENTIME_OPTIONS=7;
LISTENTRY regenTime[REGENTIME_OPTIONS] = {
  {"Immediate",    0x0000},
  {"Very Quickly", 0x0001},
  {"Quickly",      0x0003},
  {"Normal",       0x0007},
  {"Slowly",       0x000F},
  {"Very Slow",    0x001F},
  {"Too  Slow",    0x003F},
};

const int BENEFIT_OPTIONS=5;
LISTENTRY benefit[BENEFIT_OPTIONS] = {
  {"Too Low", 2},
  {"Low", 3},
  {"Normal", 4},
  {"High", 5},
  {"High Noon", 6}
};

const int LIFESPAN_OPTIONS=6;
LISTENTRY lifeSpan[LIFESPAN_OPTIONS] = {
  {"Ephemeral", 3500},
  {"Transitory", 7000},
  {"Moderate", 9000},
  {"Long", 12000},
  {"Very Long", 15000},
  {"Immortal", 0}
};

const int POPULATION_OPTIONS=8;
LISTENTRY population[POPULATION_OPTIONS] = {
  {"One",       1},
  {"Five",      5},
  {"Ten",      10},
  {"Fifteen",  15},
  {"Twenty",   20},
  {"Twenty Five", 25},
  {"Thirty",   30},
  {"Forty",    40}
};

const int MUTATION_OPTIONS=8;
LISTENTRY mutation[MUTATION_OPTIONS] = {
  {"None",         0},
  {"Nearly None",  1},
  {"Very Slight",  3},
  {"Slight",       8},
  {"Moderate",    12},
  {"High",        24},
  {"Very High",   48},
  {"Too High",    80}
};


const int FRICTION_OPTIONS=5;
LISTENTRY friction[FRICTION_OPTIONS] = {
  { "None",      0},
  { "Slight",    2},
  { "Moderate",  5},
  { "High",      20},
  { "Rigid",     50}
};


CSock g_sock;
BOOL  g_bNT;

#ifndef NOT_SAVER
BOOL  bFirstTime = TRUE;
#endif


/******************************Public*Routine******************************\
*
* ScreenSaverProc
*
* Effects:  Creates a drawing thread.
*
* Warnings:
*
* History:
\**************************************************************************/

/*LRESULT WINAPI ScreenSaverProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static Environment* env = NULL;
static int Cursor = 0;
#ifdef NOT_SAVER
static int nagTime = 80;
static int nagSet  = 80;
#else
static int nagTime = 130;
static int nagSet  = 130;
#endif
static HCURSOR hCursor  = NULL;
static BOOL bHelpCursor = FALSE;
static BOOL bPopUp = FALSE;
static int ntSaveTime = 0;


    if (env && env->sock.Message(message, wParam, lParam))
      return TRUE;

    switch(message)
    {
		case WM_CREATE:
		{
			// Lower our priority
			SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
			RegisterDialogClasses(hMainInstance);
			GetDataFileName(szFile);
			hCursor = LoadCursor(hMainInstance,MAKEINTRESOURCE(IDC_CURSOR1));
			SetTimer(hWnd, WM_TIMER, interval * 1000, NULL);
			g_bNT = IsNT();
 
			#ifdef NOT_SAVER
			SetCursor(hCursor);
			Cursor = 20;
			#endif

			// Check our size to see if we should nag
			RECT    rc;
			GetClientRect(hWnd, &rc);

			if ((rc.right - rc.left) >= 200 ||
                (rc.bottom - rc.top) >= 200)
				CreateNagScreen(hWnd, hMainInstance);
		}
		break;
 
        case WM_PAINT:
          if (env != NULL)
          {
            env->Lock();

            PAINTSTRUCT paint;
            HDC hDC = BeginPaint(hWnd, &paint);
            FillRect(hDC, &paint.rcPaint, (HBRUSH) GetStockObject(BLACK_BRUSH));
            env->Paint(hDC, &paint.rcPaint);
            EndPaint(hWnd, &paint);

            env->Unlock();
          }
          break;

        case WM_LBUTTONDOWN:
          if (env && env->bMouse)
          {
            if (bHelpCursor)
              env->BiotOperation(LOWORD(lParam), HIWORD(lParam), OPERATION_AID); 
            else
              env->BiotOperation(LOWORD(lParam), HIWORD(lParam), OPERATION_AGE); 
            return FALSE;
          }
          if (!env || env->bMouse)
            return FALSE;
          break;
          

        case WM_RBUTTONDOWN:
          if (env && env->bMouse && !env->bSmallWindow)
          {
            HMENU hMenu;

            hMenu = LoadMenu(hMainInstance, MAKEINTRESOURCE(IDR_MENU2));
            if (hMenu)
            {
              HMENU hPopup = GetSubMenu(hMenu, 0);
              if (hPopup)
              {
                POINT point;

                point.x = LOWORD(lParam);
                point.y = HIWORD(lParam);
                ClientToScreen(hWnd, &point);
                bPopUp = TRUE;

                if (bHelpCursor)
                  ModifyMenu(hMenu,	ID_POPUP_HELPBIOTS, MF_BYCOMMAND, ID_POPUP_HELPBIOTS, "Kill &Biots"); 
                else
                  ModifyMenu(hMenu,	ID_POPUP_HELPBIOTS, MF_BYCOMMAND, ID_POPUP_HELPBIOTS, "Feed &Biots"); 
                
                if (env->IsSuspended())
                  ModifyMenu(hMenu,	IDM_START, MF_BYCOMMAND, IDM_START, "Contin&ue"); 
                else
                  ModifyMenu(hMenu,	IDM_START, MF_BYCOMMAND, IDM_START, "Pa&use"); 

                Cursor = 20;
                TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
                SetCursor(hCursor);
                bPopUp = FALSE;
              }
              DestroyMenu(hMenu);
            }
            return FALSE;
          }
          break;

       
        case WM_COMMAND:
          switch(wParam)
          {
			case ID_POPUP_WORLDSTATUS:
				if (CheckActions(env, hWnd))
				{
					DialogBoxParam(hMainInstance, MAKEINTRESOURCE(IDD_STATUS),
						hWnd, (DLGPROC) StatusDialog, (LPARAM) env);
				}
				break;

          case ID_POPUP_HELPBIOTS:
            if (CheckActions(env, hWnd))
            {
              if (bHelpCursor)
                hCursor = LoadCursor(hMainInstance, MAKEINTRESOURCE(IDC_CURSOR1));
              else
                hCursor = LoadCursor(hMainInstance, MAKEINTRESOURCE(IDC_CURSOR2));
              
              bHelpCursor = !bHelpCursor ;
            }
            break;

#ifdef NOT_SAVER
            case IDM_SETTINGS:
            {
              Environment temp(hWnd, g_sock);
              if (env != NULL && !CheckActions(env, hWnd))
                break;

              if (env)
                env->Suspend();

              if (DialogBoxParam(hMainInstance, MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
                    hWnd, (DLGPROC) ScreenSaverConfigureDialog, (LPARAM)&temp))
              {
                if (env)
                  EnableMenuItem(GetMenu(hWnd), IDM_APPLY, MF_ENABLED);
              }

              if (env)
                env->Resume();
            }
            break;

            case IDM_APPLY:
              if (CheckActions(env, hWnd))
              {
                GetEnvironmentProfile(env);
                EnableMenuItem(GetMenu(hWnd), IDM_APPLY, MF_GRAYED);
              }
              break;

            case IDM_SAVE:
              if (CheckActions(env, hWnd))
              {
                if (Environment::GetLicenseInfo(env->key, env->name))
                {
                  if (!env->terminated)
                  {
                    env->Suspend();
                    env->Save();
                    env->Resume();
                  }
                }
                else
                  MessageBox(hWnd, "In the unregisted version, you cannot save your biots.",
                    szTitle, MB_OK | MB_ICONINFORMATION);
              }
              break;

			case ID_POPUP_STOP:
                if (env && CheckActions(env, hWnd))
                {
	                if (env && CheckActions(env, hWnd))
		            {
						ShutDown(env);
						env = NULL;
					}
					EnableMenuItem(GetMenu(hWnd), ID_POPUP_STOP, MF_GRAYED);
					EnableMenuItem(GetMenu(hWnd), ID_POPUP_WORLDSTATUS, MF_GRAYED);
                    EnableMenuItem(GetMenu(hWnd), IDM_SAVE,  MF_GRAYED);
                    EnableMenuItem(GetMenu(hWnd), IDM_APPLY, MF_GRAYED);
                    ModifyMenu(GetMenu(hWnd),	IDM_START,
                      MF_BYCOMMAND, IDM_START, "Pa&use"); 			
                    EnableMenuItem(GetMenu(hWnd), IDM_START, MF_GRAYED);
				}				
				break;

            case IDM_NEW_START:
            case IDM_NEW:
              if (IDM_NEW_START != wParam ||
                  DialogBoxParam(hMainInstance, MAKEINTRESOURCE(IDD_SEED),
                    hWnd, (DLGPROC) SeedDialog, 0))
              {
                if (env && CheckActions(env, hWnd))
                {
                  ShutDown(env);
					env = NULL;
                }
  
                if (!env)
                {
                  HDC hScreen = GetDC(hWnd);
                  StartUp(hWnd, &env, hScreen, (wParam == IDM_NEW_START));
                  ReleaseDC(hScreen, hWnd);
                  if (env)
                  {
					EnableMenuItem(GetMenu(hWnd), ID_POPUP_STOP, MF_ENABLED);
					EnableMenuItem(GetMenu(hWnd), ID_POPUP_WORLDSTATUS, MF_ENABLED);
                    EnableMenuItem(GetMenu(hWnd), IDM_APPLY, MF_GRAYED);
                    EnableMenuItem(GetMenu(hWnd), IDM_START, MF_ENABLED);
                    EnableMenuItem(GetMenu(hWnd), IDM_SAVE,  MF_ENABLED);
                    ModifyMenu(GetMenu(hWnd),	IDM_START,
                      MF_BYCOMMAND, IDM_START, "Pa&use"); 
                  }
                }
              }
              break;       
#else
            case IDM_NEW_START:
              if (CheckActions(env, hWnd) &&
                  DialogBoxParam(hMainInstance, MAKEINTRESOURCE(IDD_SEED),
                    hWnd, (DLGPROC) SeedDialog, 0))
              {
                ShutDown(env);
                env = NULL;
                HDC hScreen = GetDC(hWnd);
                StartUp(hWnd, &env, hScreen, (wParam == IDM_NEW_START));
                ReleaseDC(hWnd, hScreen);
              }
              break;       
#endif

            case ID_POPUP_SETTINGS:
              if (CheckActions(env, hWnd))
              {
                DialogBoxParam(hMainInstance, MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
                  hWnd, (DLGPROC) ScreenSaverConfigureDialog, (LPARAM)env);
              }
              break;

            case IDM_START:
              if (CheckActions(env, hWnd))
              {
                if (env->ToggleSuspend())
                {            
                  ModifyMenu(GetMenu(hWnd),	IDM_START,
                    MF_BYCOMMAND, IDM_START, "Contin&ue"); 
                }
                else
                {
                  ModifyMenu(GetMenu(hWnd),	IDM_START,
                    MF_BYCOMMAND, IDM_START, "Pa&use"); 
                }
              }
              break;

            case IDM_ABOUT:
            {
              bRegisterUp = TRUE;
              if (!env)
              {
                Environment *temp = new Environment(hWnd, g_sock);
                if (temp)
                {
                  GetEnvironmentProfile(temp);
                  DialogBoxParam(hMainInstance, MAKEINTRESOURCE(DLG_ABOUTHELP1),
                    hWnd, (DLGPROC) AboutDialog, (LPARAM)temp);
                  SetEnvironmentProfile(temp);
                  delete temp;
                }
              }
              else
              {
                GetEnvironmentProfile(env);
                DialogBoxParam(hMainInstance, MAKEINTRESOURCE(DLG_ABOUTHELP1),
                  hWnd, (DLGPROC) AboutDialog, (LPARAM)env);
                SetEnvironmentProfile(env);
              }
              bRegisterUp = FALSE;
            }
            break;

				case IDM_HELP:
					StartHelp(hWnd);
					break;

            case IDCANCEL:
            case IDCLOSE:
              if (env != NULL &&
                !env->terminated && !env->bSmallWindow)
              {
                env->Save();
              }
#ifdef NOT_SAVER
              DestroyWindow(hWnd);
#else
              PostMessage(hWnd, WM_KEYDOWN, 63, 63);
#endif
              break;
          }
          break;

        case WM_USER:
          // The NAG windows says get things going again
          if (env)
            env->Resume();
          break;

        case WM_TIMER:
          if (!bPopUp && env && !env->bSmallWindow)
          {
            Cursor -= interval;
            if (Cursor == 0)
              SetCursor(NULL);
          }
          else
            Cursor = 20;

#ifndef NOT_SAVER
          if (g_bNT && ntSaveTime++ > 120 && env)
          {
            if (!Environment::GetLicenseInfo(env->key, env->name) && !env->terminated)
            {
               env->Suspend();
               env->Save();
               env->Resume();
            }
            ntSaveTime = 0;
          }
#endif
          if (env && !env->bSmallWindow &&
              (!Environment::GetLicenseInfo(env->key, env->name) ||
				g_sock.ShouldNag()))
          {
            if (nagTime <= 0)
            {
              nagTime = nagSet;
              if (nagSet > (interval * 2))
                nagSet--;
              CreateNagScreen(hWnd, hMainInstance);
              env->Suspend();
            }
            nagTime -= interval;
          }
          break;

        case WM_MOUSEMOVE:
          if (env != NULL)
          {
            if (env->bMouse)
            {
              Cursor = 20;
              return TRUE;
            }
#ifndef NOT_SAVER
            if (!env->terminated && !env->bSmallWindow)
            {
              env->Save();
            }
#endif
          }
          break;      

#ifdef NOT_SAVER
        // Keeps the screen saver from starting
        case WM_SYSCOMMAND:
          if (wParam==SC_SCREENSAVE)
              return FALSE;
          break;

#else
        case WM_MBUTTONDOWN:
        case WM_KEYDOWN:
        case WM_KEYUP:
          if (env != NULL &&
              !env->terminated && !env->bSmallWindow)
          {
            env->Save();
          }
          break;      

        case WM_ERASEBKGND:
          StartUp(hWnd, &env, (HDC) wParam);
          return 1L;
#endif
        case WM_SETCURSOR:
          if (env && env->bMouse)
          {
            if (Cursor > 0)
              SetCursor(hCursor);

            return FALSE;
          }
          break;

        case WM_DESTROY:
          KillTimer(hWnd, WM_TIMER);
          ShutDown(env);
          env     = NULL;
		  g_sock.StopSession();
          PostQuitMessage(0);
          break;
    }

#ifdef NOT_SAVER
    return (DefWindowProc(hWnd,message,wParam,lParam));
#else
    return (DefScreenSaverProc(hWnd,message,wParam,lParam));
#endif              
} */


 //////////////////////////////////////////////////////////////
// 
// GetDataFileName
//
//
/*void GetDataFileName(char* szFile)
{
  szFile[0] = '\0';

#ifndef NOT_SAVER
  GetSystemDirectory(szFile, MAX_PATH);
  lstrcat(szFile, "\\prim32.dat");
#else
  lstrcpy(szFile, "prim32.dat");
#endif
}*/



/*void StartUp(HWND hWnd, Environment** pEnv, HDC hScreen, int startNew)   
{
  if (*pEnv == NULL)
  {
    *pEnv = new Environment(hWnd);
    if (*pEnv != NULL)
    {
      RECT   rc;
      DWORD  dwThreadId;

      // Wipe out the main screen
      GetClientRect(hWnd, &rc);
      FillRect(hScreen, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
          
      // Set Environmental variables from control.ini
      (*pEnv)->GetDefaultSettings();

      if (startNew < 2 && startNew >= 0)
        (*pEnv)->startNew = startNew;

      // Bring our environment to life
      (*pEnv)->hThread = CreateThread(NULL, 0,
              (LPTHREAD_START_ROUTINE)StartEvolving,
               *pEnv,
               STANDARD_RIGHTS_REQUIRED,
               &dwThreadId );

      // Set priority of environment less than this window
      if ((*pEnv)->hThread)
      {
        SetThreadPriority((*pEnv)->hThread, THREAD_PRIORITY_IDLE);//THREAD_PRIORITY_LOWEST);
      }
      else
      {
        // If we can't create the thread, just leave screen black.
        delete *pEnv;
        *pEnv = NULL;
      } 
    }
  }
}
             

void ShutDown(Environment* env)
{
  if (env != NULL)
  {
    // Signal the environment thread to shutdown
    InterlockedIncrement(&env->terminated);

    // Wait Ten seconds for thread to complete
    if (env->hThread != NULL)
    {
      env->Resume();
      if (WaitForSingleObject(env->hThread, 5000) == WAIT_TIMEOUT)
      {
        // Force it to shutdown (if necessary)
		    TerminateThread(env->hThread, (DWORD)0L);
      }
      CloseHandle(env->hThread);
    } 
    delete env;
  }
}*/
                 

/******************************Public*Routine******************************\
*
* NagScreenProc
*
* Effects:  Creates a drawing thread.
*
* Warnings:
*
* History:
\**************************************************************************/

/*LRESULT WINAPI NagScreenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT par;
RECT nag;

    switch(message)
    {
        case WM_CREATE:
          if (GetWindowRect(GetParent(hWnd), &par))
            if (GetWindowRect(hWnd, &nag))
              MoveWindow(hWnd, ((par.right + par.left) / 2) - ((nag.right - nag.left) / 2),
                 ((par.bottom + par.top) / 2) - ((nag.bottom - nag.top) / 2),
                 (nag.right - nag.left), (nag.bottom - nag.top), FALSE);

          if (SetTimer(hWnd, WM_TIMER, 3800, NULL) == 0)
            DestroyWindow(hWnd);
          break;
 
        case WM_TIMER:
          PostMessage(GetParent(hWnd), WM_USER, 1, 0);
          DestroyWindow(hWnd);
          break;

        case WM_LBUTTONDOWN:
          if (!bRegisterUp)
          {
            PostMessage(GetParent(hWnd), WM_COMMAND, IDM_ABOUT, 0);
          }
          break;

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC hDC = BeginPaint(hWnd, &paint);
            HDC hMemoryDC = CreateCompatibleDC(hDC);
            if (hMemoryDC)
            {
				HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, "NAG");
				if (hBitmap)
				{
					hBitmap = (HBITMAP) SelectObject(hMemoryDC, hBitmap);
					BitBlt(hDC, 0, 0, nagWidth, nagHeight, hMemoryDC, 0, 0, SRCCOPY);
					DeleteObject(SelectObject(hMemoryDC, hBitmap));
				}

				char bigName[MAX_PATH];
				char name[CKeyRegistry::MAX_NAME_SIZE];
				int licenses;
				BOOL bSite;
				CKeyRegistry& rKey = AfxKeyRegistry();

				if (!rKey.GetLicenseInfo(rKey.GetKey(), rKey.GetName(name), &bSite, &licenses))
					lstrcpy(bigName, "Please register so I might evolve...");
              else
              {
                lstrcpy(bigName, "Registered To:\r\n");
                lstrcat(bigName, name);
				lstrcat(bigName, "\r\n");
				if (bSite)
					lstrcat(bigName, "Site License");
				else
				{
					lstrcat(bigName, "Single user license for ");
					wsprintf(name, "%d computer", licenses);
					lstrcat(bigName, name);
					if (licenses > 1)
						lstrcat(bigName, "s");
				}
              }

              SetTextColor(hDC, RGB(255, 0, 0));
              SetBkColor(hDC, RGB(192, 192, 192));
              RECT rect = { 94, 40, 388, 94 };

              DrawText(hDC, bigName, lstrlen(bigName), &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
                
              DeleteDC(hMemoryDC);
            }
            EndPaint(hWnd, &paint);
        }
        break;

        case WM_DESTROY:
          KillTimer(hWnd, WM_TIMER);
          break;
    }
    return (DefWindowProc(hWnd,message,wParam,lParam));
}


void CreateNagScreen(HWND hWnd, HINSTANCE hInstance)
{
RECT rc;
HWND hNagWnd;

   GetClientRect(hWnd, &rc);

   hNagWnd = CreateWindow("NagScreen", "",  WS_POPUP,
              ((rc.right - rc.left) - nagWidth) / 2, ((rc.bottom - rc.top) - nagHeight) / 2,
              nagWidth, nagHeight,
              hWnd, NULL, hInstance, NULL);
   
   if (hNagWnd)
   {
     ShowWindow(hNagWnd, SW_SHOW);
     UpdateWindow(hNagWnd);
   }
}

*/
/******************************Public*Routine******************************\
*
* RegisterDialogClasses
*
* Effects: Register window classes used by the configuration dialog
*
* Warnings: Return TRUE even if no class needed to be registered.
*
\**************************************************************************/

//BOOL RegisterDialogClasses(HINSTANCE hInst)
//{
//   WNDCLASS wndClass;


 //   /*
 //   ** Set the common wndClass information.  This is common to all windows
 //   ** of this application.
 //   */
//    wndClass.style         = CS_NOCLOSE | CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT;
//    wndClass.cbClsExtra    = 0;
//    wndClass.cbWndExtra    = sizeof(LONG);
//    wndClass.hCursor       = LoadCursor(NULL,IDC_ARROW);
//    wndClass.hInstance     = hInst;
                                             

 //   /*
 //   ** Register the main top-level window (frame).
 //   */
//    wndClass.lpfnWndProc   = NagScreenProc;
 //   wndClass.hIcon         = NULL;
//    wndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
//    wndClass.lpszMenuName  = NULL;
//    wndClass.lpszClassName = "NagScreen";

//    if(!RegisterClass(&wndClass))
//        return(FALSE);

//    return TRUE;
//}


/**************************************************************************\
*
* StartEvolving
*
* Effects:   Constantly runs the environment.
*
* Warnings:
*
* History:
\**************************************************************************/

/*DWORD WINAPI StartEvolving(LPVOID lpvParam)
{
Environment* env = (Environment*) lpvParam;
  
  if (env != NULL && env->terminated)
  {
    env->thread();
  }
  return 0;
}*/


/******************************Public*Routine******************************\
*
* ScreenSaverConfigureDialog
*
* Effects:
*
* Warnings:
*
\**************************************************************************/
//Fox BEGIN
static CSettings HOLDER; //Keeps the tabbed windows' data in
						   //tact until the user hits OK

BOOL CALLBACK ConfigureDialog (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
static Environment* env = NULL;
TCITEM tabs;	 //The tab control
NMHDR *LP;

	switch(message)
	{
		case WM_INITDIALOG:
//Fox BEGIN
			env = (Environment*) lParam;

			if (env == NULL)
	            EndDialog(hDlg, IDCANCEL);

            lstrcpy(szFile, szTitle);
            lstrcat(szFile, " Settings");
            SetWindowText(hDlg, szFile);

			//sets up the tab control
			TABW = CreateWindow(WC_TABCONTROL, "", WS_VISIBLE | WS_TABSTOP |
			        WS_CHILD, 216, 3, 386, 410, hDlg, NULL,
					AfxGetApp()->m_hInstance, NULL);
	

			tabs.mask = TCIF_TEXT;
			tabs.iImage = -1;

			//{  creates the tabs themselves
			tabs.pszText = "Biot";
			TabCtrl_InsertItem(TABW, 0, &tabs);
			tabs.pszText = "Habitat";
			TabCtrl_InsertItem(TABW, 1, &tabs);
			//}

			//call up the first tab window
			HOLDER = env->options;
			CURRW = (HWND)CreateDialogParam(AfxGetApp()->m_hInstance,
			    MAKEINTRESOURCE(BIOT_OPTIONS),
                TABW, (DLGPROC)BiotOptions, (LPARAM)env);
			
			//if(CURRW == NULL)   // THIS WAS A DEBUGGER, NOW OBSOLETE
			//  MessageBox(NULL, "NULL", "NULL", MB_OK);
//Fox END
//          Set Environmental variables from control.ini
            InitDlgItems(hDlg, env);			

			char string[10];
			AfxCommonRegistry().GetString("dialup", string, 10);
			if (atol(string))
				EnableWindow(GetDlgItem(hDlg, IDC_NETWORK), FALSE);

			EnableWindow(GetDlgItem(hDlg, IDC_MOUSE), !AfxIsNT() || AfxGetPLife().IsSaver());
			return TRUE;
//Fox BEGIN
		case WM_NOTIFY:	  //message handler for the tab control
		  {
		   LP = (LPNMHDR)lParam;

		   if(LP->code == TCN_SELCHANGE) //is user making new selection?
			 {
			  if(CURRW)
				DestroyWindow(CURRW); //kill the current window

			  switch(TabCtrl_GetCurSel((HWND)LP->hwndFrom)) //what's the user
				{											//selecting?
				 case 0:				
				   CURRW = (HWND)CreateDialogParam(AfxGetApp()->m_hInstance,
			               MAKEINTRESOURCE(BIOT_OPTIONS),
                           TABW, (DLGPROC)BiotOptions, (LPARAM)env);

				   break;

				 case 1:
				   CURRW = (HWND)CreateDialogParam(AfxGetApp()->m_hInstance,
			               MAKEINTRESOURCE(HABITAT_OPTIONS),
                           TABW, (DLGPROC)HabitatOptions, (LPARAM)env);
				}
			 }

		   break;
		  }
//Fox END        
        case WM_COMMAND:
          switch(LOWORD(wParam))
          {
            case IDC_NETWORK:
              DialogBoxParam(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDD_NETWORK),
                hDlg, (DLGPROC) NetworkDialog, (LPARAM)env);
              break;

            case IDC_ABOUT://????
				if (AfxGetPLife().IsSaver())
				{
					GetDlgItems(hDlg, env);
					env->SetDefaultSettings();
//					env->Save();
				}
				AfxStartHelp(hDlg);
				break;

            case IDC_ABOUT2:
              DialogBoxParam(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(DLG_ABOUTHELP1),
                hDlg, (DLGPROC) AboutDialog, (LPARAM)env);

              if (!AfxKeyRegistry().IsGoodKey())
              {
                EnableWindow(GetDlgItem(hDlg, IDC_OLDPOPULATION), FALSE);
                CheckRadioButton(hDlg, IDC_NEWPOPULATION, IDC_OLDPOPULATION, IDC_NEWPOPULATION);
              }
              else
              {
                EnableWindow(GetDlgItem(hDlg, IDC_OLDPOPULATION), TRUE);
                CheckRadioButton(hDlg, IDC_NEWPOPULATION, IDC_OLDPOPULATION,
                  (env->options.startNew?IDC_NEWPOPULATION:IDC_OLDPOPULATION));
              }
              break;

            case IDOK:
              GetDlgItems(hDlg, env);

            case IDCANCEL:
              EndDialog(hDlg, 0/*IDOK == wParam*/);
              return TRUE;

            case IDC_ERASE:
              if (!remove(szFile))
                MessageBox(hDlg, "The current population has been erased.",
                  szTitle, MB_OK | MB_ICONINFORMATION);
              else
                MessageBox(hDlg, "Population already erased.",
                  szTitle, MB_OK | MB_ICONEXCLAMATION);
              return TRUE;
              
            default:
              break;
          }
          break;

        default:
            break;
    }
    return FALSE;
}
//Fox END
//-----------------------------------------
//Fox BEGIN
BOOL CALLBACK BiotOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
Environment *env = (Environment*)lParam;

switch(message)
  {
   case WM_INITDIALOG:
	 {	  
	  CheckDlgButton(hDlg, IDC_ASEXUAL, (HOLDER.nSexual == 2?1:0));
      CheckDlgButton(hDlg, IDC_SEXUAL,  (HOLDER.nSexual == 1?1:0));
      CheckDlgButton(hDlg, IDC_BOTHSEXUAL,  (HOLDER.nSexual == 3?1:0));      
      CheckDlgButton(hDlg, IDC_PARENTATTACK, (HOLDER.bParentAttack?1:0));
      CheckDlgButton(hDlg, IDC_SIBLINGSATTACK, (HOLDER.bSiblingsAttack?1:0));      

      LoadComboBox(hDlg, IDC_MUTATION, MUTATION_OPTIONS, HOLDER.chance, mutation);
      LoadComboBox(hDlg, IDC_REGENCOST, REGENCOST_OPTIONS, HOLDER.regenCost, regenCost);
      LoadComboBox(hDlg, IDC_REGENTIME, REGENTIME_OPTIONS, HOLDER.regenTime, regenTime);      
	  break;
	 }
   case WM_COMMAND:
   case WM_DESTROY:
	 {
	  if (IsDlgButtonChecked(hDlg, IDC_PARENTATTACK))
        HOLDER.bParentAttack = TRUE;
      else
        HOLDER.bParentAttack = FALSE;

      if (IsDlgButtonChecked(hDlg, IDC_SIBLINGSATTACK))
        HOLDER.bSiblingsAttack = TRUE;
      else
        HOLDER.bSiblingsAttack = FALSE;

      if (IsDlgButtonChecked(hDlg, IDC_SEXUAL))
        HOLDER.nSexual = 1;
      else if (IsDlgButtonChecked(hDlg, IDC_ASEXUAL))
        HOLDER.nSexual = 2;
      else
        HOLDER.nSexual = 3;
	          

      HOLDER.chance     = GetComboBox(hDlg, IDC_MUTATION, MUTATION_OPTIONS, mutation);   
      HOLDER.regenCost  = GetComboBox(hDlg, IDC_REGENCOST, REGENCOST_OPTIONS, regenCost);
      HOLDER.regenTime  = GetComboBox(hDlg, IDC_REGENTIME, REGENTIME_OPTIONS, regenTime);
	  break;
	 }
  }

return(0);
}
//Fox END
//-------------------------------------------------
//Fox BEGIN
BOOL CALLBACK HabitatOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
Environment *env = (Environment*)lParam;

switch(message)
  {
   case WM_INITDIALOG:
	 {	  
      CheckDlgButton(hDlg, IDC_MUTE, (HOLDER.bSoundOn?0:1));

      if (!AfxIsNT() || !AfxGetPLife().IsSaver())
   	    CheckDlgButton(hDlg, IDC_MOUSE, (HOLDER.bMouse?1:0));

   
      LoadComboBox(hDlg, IDC_POPULATION, POPULATION_OPTIONS, HOLDER.m_initialPopulation, population);
      LoadComboBox(hDlg, IDC_FRICTION, FRICTION_OPTIONS, HOLDER.GetFriction(), friction);
	  LoadComboBox(hDlg, IDC_GREEN, BENEFIT_OPTIONS, HOLDER.m_leafEnergy, benefit);
	  break;
	 }
   case WM_COMMAND:
   case WM_DESTROY:
	 {	  
      if (IsDlgButtonChecked(hDlg, IDC_NEWPOPULATION))
        HOLDER.startNew = 1;
      else
        HOLDER.startNew = 0;

      if (IsDlgButtonChecked(hDlg, IDC_MUTE))
        HOLDER.bSoundOn = FALSE;
      else
        HOLDER.bSoundOn = TRUE;

      if (IsDlgButtonChecked(hDlg, IDC_MOUSE))
        HOLDER.bMouse = TRUE;
      else
        HOLDER.bMouse = FALSE;
           
   
      HOLDER.m_initialPopulation    = GetComboBox(hDlg, IDC_POPULATION, POPULATION_OPTIONS, population);   
      HOLDER.m_leafEnergy         = GetComboBox(hDlg, IDC_GREEN, BENEFIT_OPTIONS, benefit);
      HOLDER.SetFriction(GetComboBox(hDlg, IDC_FRICTION, FRICTION_OPTIONS, friction));
	  break;
	 }
  }

return(0);
}
//Fox END
//-----------------------------------------

//Fox BEGIN
void LoadComboBox(HWND hDlg, int id, int size, int choice, LISTENTRY* pEntry)
{
WPARAM select = 0;

  for (int i = 0; i < size; i++)
  {
    SendDlgItemMessage(hDlg, id, CB_ADDSTRING, 0, (LPARAM) pEntry[i].listStr);
    if (choice == pEntry[i].listValue)
      select = i;
  }
  SendDlgItemMessage(hDlg, id, CB_SETCURSEL, select, i / 2);
}
//Fox END

/******************************Public*Routine******************************\
*
* GetComboBox
*
* Effects: Reads the current item selected.
*
* Warnings:
*
\**************************************************************************/
//Fox BEGIN
int GetComboBox(HWND hDlg, int id, int size, LISTENTRY* pEntry)
{
int select = SendDlgItemMessage(hDlg, id, CB_GETCURSEL, 0, 0);

  if (select < size)
    return pEntry[select].listValue;
  else
    return 0;
}


void InitDlgItems(HWND hDlg, Environment* env)
{
   if (!AfxKeyRegistry().IsGoodKey())
   {
     EnableWindow(GetDlgItem(hDlg, IDC_OLDPOPULATION), FALSE);
     CheckRadioButton(hDlg, IDC_NEWPOPULATION, IDC_OLDPOPULATION, IDC_NEWPOPULATION);
   }
   else
   {
     CheckRadioButton(hDlg, IDC_NEWPOPULATION, IDC_OLDPOPULATION,
       (env->options.startNew?IDC_NEWPOPULATION:IDC_OLDPOPULATION));
   }
}
//Fox END
//Fox BEGIN
void GetDlgItems(HWND hDlg, Environment* env)
{

//*env = HOLDER;

env->options.bSiblingsAttack    = HOLDER.bSiblingsAttack;      
env->options.bParentAttack      = HOLDER.bParentAttack;
env->options.nSexual            = HOLDER.nSexual;

env->options.chance             = HOLDER.chance;
env->options.regenCost          = HOLDER.regenCost;
env->options.regenTime          = HOLDER.regenTime;
 
env->options.startNew           = HOLDER.startNew;
env->options.bSoundOn           = HOLDER.bSoundOn;
      
env->options.bMouse             = HOLDER.bMouse;
           
env->options.m_initialPopulation = HOLDER.m_initialPopulation;
env->options.m_leafEnergy       = HOLDER.m_leafEnergy;
env->options.friction           = HOLDER.friction;


}
//Fox END


/******************************Public*Routine******************************\
*
* AboutDialog
*
* Effects:
*
* Warnings:
*
\**************************************************************************/

BOOL CALLBACK AboutDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/)
{
static char reg[] = 
  "Primordial Life 4.0\015\012"
  "Copyright (C) 1996 Jason Spofford. All Rights Reserved\015\012\015\012"             
  "Primordial Life was written to visually explore the concepts of evolution.\015\012\015\012"
  "See http://www.io.com/~spofford for information and updates.  Send me email at "
  "spofford@io.com"
  "\015\012\015\012"
  "Shareware Notice:\015\012\015\012"
  "Primordial Life runs as a screen saver or in a window. "
  "Please keep this program evolving by registering. Thank you!"
  "\015\012\015\012"
  "To register, "
  "press the \"I want to buy it!\" button. A single user license costs as little as $12 (US) "
  "and there are several payment options.  "
  "After you register, I will send you a shareware key that will fully enable the program. "
  "When you receive your shareware key, just enter it below along with your full name.  After registering, "
  "you will be able to save the current biots between screen saver sessions, the nag screen "
  "will go away and you will have unrestricted use of the menu inside the screen saver.  "
  "If you buy this version, the next version I create is yours to download free."
  "\015\012\015\012"
  "If you want me to mail you a 3.5\" diskette, please indicate this.  "
  "Otherwise I will assume Internet delivery is acceptable."
  "\015\012\015\012"
  "If you have any questions or comments, you can reach me at:\015\012\015\012"            
  "Jason Spofford\015\012"
  "11040 Crossland Drive\015\012"
  "Austin, Texas  78726"
  "\015\012\015\012"
  "email: spofford@io.com";

static int key;
//static char szName[CKeyRegistry::MAX_NAME_SIZE];

  switch(message)
  {
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, IDC_TEXT, reg);
			key = AfxKeyRegistry().GetKey();
			if (key > 0)
				SetDlgItemInt(hDlg, IDC_EDIT_KEY, key, FALSE);
         
			SendDlgItemMessage(hDlg, IDC_EDIT_NAME, EM_LIMITTEXT, CKeyRegistry::MAX_NAME_SIZE - 1, 0);
			SetDlgItemText(hDlg, IDC_EDIT_NAME, AfxKeyRegistry().GetName());
			SetWindowText(hDlg, szTitle);
			return TRUE;
        
      case WM_COMMAND:
        switch(LOWORD(wParam))
        {
          case IDC_BUY:
          {
            PROCESS_INFORMATION p;
            STARTUPINFO s;
            char dest[MAX_PATH];
            char dir[MAX_PATH];

			AfxCommonRegistry().GetString("directory", dest, MAX_PATH);
            lstrcat(dest, "\\register.exe");

			AfxCommonRegistry().GetString("directory", dir, MAX_PATH);

            ZeroMemory(&s, sizeof(s));
            s.wShowWindow = SW_MAXIMIZE;

            if (CreateProcess(
             NULL,	// pointer to name of executable module 
             dest,	// pointer to command line string
             NULL,	// pointer to process security attributes 
             NULL,	// pointer to thread security attributes 
             TRUE,	// handle inheritance flag 
             0,	// creation flags 
             NULL,	// pointer to new environment block 
             dir,	// pointer to current directory name 
             &s,	// pointer to STARTUPINFO 
             &p 	// pointer to PROCESS_INFORMATION  
           )) {
             CloseHandle(p.hThread);
             CloseHandle(p.hProcess);

             if (AfxGetPLife().GetView() == CPrimCmdLine::SHOW_SAVER_WINDOW)
               PostQuitMessage(0);
           }
           else
             MessageBox(hDlg, "Sorry, cannot find register.exe to assist in purchasing Primordial Life. Please contact spofford@io.com.",
               szTitle, MB_OK | MB_ICONEXCLAMATION);
          }
          break;


          case IDOK:
			char name[CKeyRegistry::MAX_NAME_SIZE + 1];
				int  key;

				GetDlgItemText(hDlg, IDC_EDIT_NAME, name, CKeyRegistry::MAX_NAME_SIZE);
				key = GetDlgItemInt(hDlg, IDC_EDIT_KEY, NULL, FALSE);

				AfxKeyRegistry().SetName(name);
				AfxKeyRegistry().SetKey(key);

				if (!AfxKeyRegistry().IsGoodKey())
					MessageBox(hDlg, "Your shareware key is invalid.", szTitle,
						MB_OK | MB_ICONINFORMATION);
				else
					MessageBox(hDlg, "Thank you for registering!", szTitle,
						MB_OK | MB_ICONEXCLAMATION);
          
          case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;

          default:
            break;
        }
        break;

        default:
            break;
    }
    return FALSE;
}


/******************************Public*Routine******************************\
*
* SeedDialog
*
* Effects:
*
* Warnings:
*
\**************************************************************************/

BOOL CALLBACK SeedDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/)
{
static Randomizer rand;
int seed;

  switch(message)
  {
     case WM_INITDIALOG:
       SetDlgItemInt(hDlg, IDC_SEED, 123456789, FALSE);
       return TRUE;
        
      case WM_COMMAND:
        switch(LOWORD(wParam))
        {
          case IDC_OLDSEED:
            seed = GetDlgItemInt(hDlg, IDC_SEED, NULL, FALSE);
            rand.RandSeed(seed);
            EndDialog(hDlg, TRUE);
            return TRUE;

          case IDC_NEWSEED:
            rand.RandSeed(GetTickCount());
            EndDialog(hDlg, TRUE);
            return TRUE;

          case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return TRUE;

          default:
            break;
        }
        break;

        default:

        break;
    }
    return FALSE;
}


/*********************************************************\
* SetEnabledState
*
*
*
\*********************************************************/

void SetEnableState(HWND hDlg, CSock& sSock)
{
  EnableWindow(GetDlgItem(hDlg, IDC_LISTEN_CHECK), sSock.m_bNetwork);
  EnableWindow(GetDlgItem(hDlg, IDC_STATIC1), sSock.m_bNetwork);
  EnableWindow(GetDlgItem(hDlg, IDC_LEFT_ENABLE), sSock.m_bNetwork);
  EnableWindow(GetDlgItem(hDlg, IDC_RIGHT_ENABLE), sSock.m_bNetwork);
  EnableWindow(GetDlgItem(hDlg, IDC_TOP_ENABLE), sSock.m_bNetwork);
  EnableWindow(GetDlgItem(hDlg, IDC_BOTTOM_ENABLE), sSock.m_bNetwork);

  EnableWindow(GetDlgItem(hDlg, IDC_LEFT_ADDRESS), sSock.m_bNetwork && sSock.m_bEnabled[Side::LEFT]);
  EnableWindow(GetDlgItem(hDlg, IDC_RIGHT_ADDRESS), sSock.m_bNetwork && sSock.m_bEnabled[Side::RIGHT]);
  EnableWindow(GetDlgItem(hDlg, IDC_TOP_ADDRESS), sSock.m_bNetwork && sSock.m_bEnabled[Side::TOP]);
  EnableWindow(GetDlgItem(hDlg, IDC_BOTTOM_ADDRESS), sSock.m_bNetwork && sSock.m_bEnabled[Side::BOTTOM]);
}

void SetNetworkDlg(HWND hDlg, CSock& sSock)
{
  CheckDlgButton(hDlg, IDC_CONNECT_CHECK, sSock.m_bNetwork?BST_CHECKED:BST_UNCHECKED);

  CheckDlgButton(hDlg, IDC_LISTEN_CHECK, sSock.m_bListen?BST_CHECKED:BST_UNCHECKED);

  SetDlgItemText(hDlg, IDC_LEFT_ADDRESS, sSock.m_sHost[Side::LEFT]);
  SendDlgItemMessage(hDlg, IDC_LEFT_ADDRESS, EM_LIMITTEXT, MAX_HOST_NAME - 1, 0);
  CheckDlgButton(hDlg, IDC_LEFT_ENABLE, sSock.m_bEnabled[Side::LEFT]?BST_CHECKED:BST_UNCHECKED);

  SetDlgItemText(hDlg, IDC_RIGHT_ADDRESS, sSock.m_sHost[Side::RIGHT]);
  SendDlgItemMessage(hDlg, IDC_RIGHT_ADDRESS, EM_LIMITTEXT, MAX_HOST_NAME - 1, 0);
  CheckDlgButton(hDlg, IDC_RIGHT_ENABLE, sSock.m_bEnabled[Side::RIGHT]?BST_CHECKED:BST_UNCHECKED);

  SetDlgItemText(hDlg, IDC_TOP_ADDRESS, sSock.m_sHost[Side::TOP]);
  SendDlgItemMessage(hDlg, IDC_TOP_ADDRESS, EM_LIMITTEXT, MAX_HOST_NAME - 1, 0);
  CheckDlgButton(hDlg, IDC_TOP_ENABLE, sSock.m_bEnabled[Side::TOP]?BST_CHECKED:BST_UNCHECKED);

  SetDlgItemText(hDlg, IDC_BOTTOM_ADDRESS, sSock.m_sHost[Side::BOTTOM]);
  SendDlgItemMessage(hDlg, IDC_BOTTOM_ADDRESS, EM_LIMITTEXT, MAX_HOST_NAME - 1, 0);
  CheckDlgButton(hDlg, IDC_BOTTOM_ENABLE, sSock.m_bEnabled[Side::BOTTOM]?BST_CHECKED:BST_UNCHECKED);

  SetEnableState(hDlg, sSock);
}

void GetNetworkDlg(HWND hDlg, CSock& sSock)
{
	char szHost[MAX_HOST_NAME];

	GetDlgItemText(hDlg, IDC_LEFT_ADDRESS, szHost, MAX_HOST_NAME);
	sSock.m_sHost[Side::LEFT] = szHost;

	GetDlgItemText(hDlg, IDC_RIGHT_ADDRESS, szHost, MAX_HOST_NAME);
	sSock.m_sHost[Side::RIGHT] = szHost;

	GetDlgItemText(hDlg, IDC_TOP_ADDRESS, szHost, MAX_HOST_NAME);
	sSock.m_sHost[Side::TOP] = szHost;

	GetDlgItemText(hDlg, IDC_BOTTOM_ADDRESS, szHost, MAX_HOST_NAME);
	sSock.m_sHost[Side::BOTTOM] = szHost;
}

void GetEnableState(HWND hDlg, CSock& sSock)
{
  if (IsDlgButtonChecked(hDlg, IDC_CONNECT_CHECK) == BST_CHECKED)
	    sSock.m_bNetwork = TRUE;
  else
    sSock.m_bNetwork = FALSE;

  if (IsDlgButtonChecked(hDlg, IDC_LISTEN_CHECK) == BST_CHECKED)
    sSock.m_bListen = TRUE;
  else
    sSock.m_bListen = FALSE;

	for (int side = 0; side < 4; side++)
	{
		if (IsDlgButtonChecked(hDlg, IDC_LEFT_ENABLE + side) == BST_CHECKED)
			sSock.m_bEnabled[side] = TRUE;
		else
			sSock.m_bEnabled[side] = FALSE;
	}
}


/******************************Public*Routine******************************\
*
* NetworkDialog
*
* Effects:
*
* Warnings:
*
\**************************************************************************/

BOOL WINAPI NetworkDialog (HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/)
{
static CSock* pSock;
BOOL bTemp;
 
	switch(message)
	{
		case WM_INITDIALOG:
			pSock = &g_sock;
			bTemp = pSock->m_bNetwork;
			pSock->m_bNetwork = TRUE;
			if (pSock->m_hWinsockDll == NULL)
			{
				if (!pSock->StartSession(hDlg, NULL))
				{
					pSock->m_bNetwork = FALSE;
					EndDialog(hDlg, FALSE);
				}
				else
				{	
					char szFullName[128];
					pSock->GetFullAddress(szFullName);
					SetDlgItemText(hDlg, IDC_ADDRESS, szFullName);
					pSock->StopSession();
				}
			}
			else
			{
				char szFullName[128];
				pSock->GetFullAddress(szFullName);
				SetDlgItemText(hDlg, IDC_ADDRESS, szFullName);
			}

			pSock->m_bNetwork = bTemp;
			SetNetworkDlg(hDlg, *pSock);
			SetEnableState(hDlg, *pSock);
			return TRUE;
        
        case WM_COMMAND:
          switch(LOWORD(wParam))
          {
            case IDC_LEFT_ENABLE:
            case IDC_RIGHT_ENABLE:
            case IDC_TOP_ENABLE:
            case IDC_BOTTOM_ENABLE:
            case IDC_CONNECT_CHECK:
              GetEnableState(hDlg, *pSock);
              SetEnableState(hDlg, *pSock);
              break;

            case IDC_HELP:
				AfxStartHelp(hDlg);
				break;

            case IDOK:
              GetNetworkDlg(hDlg, *pSock);
              GetEnableState(hDlg, *pSock);
			  if (pSock->m_pEnv)
				pSock->m_pEnv->bNetworkSettingsChange = TRUE;


            case IDCANCEL:
              EndDialog(hDlg, IDOK == wParam);
              return TRUE;

            default:
              break;
          }
          break;

        case WM_DESTROY:
          break;

        default:
            break;
    }
    return FALSE;
}

void SetStatus(HWND hDlg, Environment& env)
{
	g_sock.SetStatus(hDlg);

//!	SetDlgItemInt(hDlg, IDC_POPULATION, m_biotList.GetSize(), FALSE);
//	SetDlgItemInt(hDlg, IDC_TIME, env.generation, FALSE);
//!	SetDlgItemInt(hDlg, IDC_DIED, env.deathRate, FALSE);
//!	SetDlgItemInt(hDlg, IDC_BORN, env.birthRate, FALSE);
}


/******************************Public*Routine******************************\
*
* StatusDialog
*
* Effects:
*
* Warnings:
*
\**************************************************************************/

BOOL WINAPI StatusDialog (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Environment* pEnv;
 
	switch(message)
	{
		case WM_INITDIALOG:
			pEnv =  (Environment* ) lParam;

			if (!pEnv)
				EndDialog(hDlg, IDCANCEL);
			else
			{
				SetStatus(hDlg, *pEnv);
				SetTimer(hDlg, WM_TIMER, 2000, NULL);

				char string[10];
				AfxCommonRegistry().GetString("dialup", string, 10);
				if (atol(string))
					EnableWindow(GetDlgItem(hDlg, IDC_NETWORK), FALSE);
			}
			return TRUE;

		case WM_TIMER:
			SetStatus(hDlg, *pEnv);
			break;
        
        case WM_COMMAND:
          switch(LOWORD(wParam))
          {
			case IDC_NETWORK:
				DialogBoxParam(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDD_NETWORK),
		            hDlg, (DLGPROC) NetworkDialog, (LPARAM)pEnv);
				break;
			
            case IDC_HELP:
				AfxStartHelp(hDlg);
				break;

            case IDOK:
            case IDCANCEL:
              EndDialog(hDlg, IDOK == wParam);
              return TRUE;

            default:
              break;
          }
          break;

        case WM_DESTROY:
          KillTimer(hDlg, WM_TIMER);
          break;

        default:
            break;
    }
    return FALSE;
}

