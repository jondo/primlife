#ifdef NOT_SAVER

#include <windows.h>
#include <scrnsave.h>
#include "resource.h"


#define APPCLASS "Evolve"
#define APPNAME  "Primordial Life 3.2"
HANDLE hMainInstance;

BOOL WINAPI RegisterDialogClasses(HANDLE hInst);
                      
/*---------------------------------------------------------------------------*\
| REGISTER APPLICATION CLASS
|   This routine registers all classes necessary for the application.
\*---------------------------------------------------------------------------*/
BOOL FAR RegisterAppClass(HANDLE hInstance)
{
    WNDCLASS wndClass;

    /*
    ** Set the common wndClass information.  This is common to all windows
    ** of this application.
    */
    //CS_HREDRAW | CS_VREDRAW | CS_OWNDC | 
    wndClass.style         = CS_CLASSDC | CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT | CS_OWNDC;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = sizeof(LONG);
    wndClass.hCursor       = NULL; 
    wndClass.hInstance     = hInstance;


    /*
    ** Register the main top-level window (frame).
    */
    wndClass.lpfnWndProc   = ScreenSaverProc;
    wndClass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(ID_APP));
    wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
    wndClass.lpszClassName = APPCLASS;

    if(!RegisterClass(&wndClass))
        return(FALSE);

    return RegisterDialogClasses(hInstance);
}


/*---------------------------------------------------------------------------*\
| UNREGISTER APPLICATION CLASS
|   This routine unregisters all classes registered for the application.
|   It is typically called upon termination of the application.
\*---------------------------------------------------------------------------*/
VOID FAR UnregisterAppClass(HANDLE hInstance)
{
    UnregisterClass(APPCLASS   ,hInstance);

    return;
}


/*---------------------------------------------------------------------------*\
| CREATE APPLICATION WINDOW
|   This routine creates the main top-level window.
\*---------------------------------------------------------------------------*/
HWND FAR CreateAppWindow(HANDLE hInstance)
{
    /*
    ** Upon creation of the window, initialize the extra object information.
    */
    return(CreateWindow(APPCLASS,APPNAME,WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
            NULL,NULL,hInstance,NULL));
}




int PASCAL WinMain(HANDLE hInst, HANDLE hPrevInst, LPSTR lpszLine, int nShow)
{
    HWND hWndFrame;
    MSG  msg;

    lpszLine      = lpszLine;
    hMainInstance = hInst;
    /*
    ** If there's a previous instance of this application, then we do not need
    ** to register it again.
    */
    if(!hPrevInst)
        if(!RegisterAppClass(hInst))
            return(1);


    /*
    ** Enter the application message-polling loop.  This is the anchor for
    ** the application.
    */
    msg.wParam = 1;
    if(hWndFrame = CreateAppWindow(hInst))
    {
        ShowWindow(hWndFrame,nShow);
        UpdateWindow(hWndFrame);

        while(GetMessage(&msg,NULL,0,0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnregisterAppClass(hInst);

    return(msg.wParam);
}
#endif