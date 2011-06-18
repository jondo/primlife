    // sock.cpp
    //
    //
    //
    //
    /////////////////////////////////////////////////////////
    
    #include "stdafx.h"
	#include "Primordial Life.h"
    #include <winsock.h>
    #include "Evolve.h"
    #include "Connect.h"
    #include "Biots.h"
    #include "sock.h"
    #include "resource.h"
    
    char* CSock::szIniFile    = "control.ini";
    char* CSock::szKey        = "network";
    char* CSock::szbNetwork   = "bNetwork";
    char* CSock::szbListen    = "bListen";
    char* CSock::szLeftHost   = "LeftHost";
    char* CSock::szRightHost  = "RightHost";
    char* CSock::szTopHost    = "TopHost";
    char* CSock::szBottomHost = "BottomHost";
    char* CSock::szLeftPort   = "LeftPort";
    char* CSock::szRightPort  = "RightPort";
    char* CSock::szTopPort    = "TopPort";
    char* CSock::szBottomPort = "BottomPort";
    char* CSock::szListenPort = "ListenPort";
    char* CSock::szbLeft      = "bLeft";
    char* CSock::szbRight     = "bRight";
    char* CSock::szbTop       = "bTop";
    char* CSock::szbBottom    = "bBottom";
    char* CSock::szWorldName  = "name";
    
    
//////////////////////////////////////////////////////
// CSockSide Constructor and Destructor
//
//
CSockSide::CSockSide()
{
	oldside     = CSock::MAX_SOCKETS;
	state       = CSock::STATE_AVAILABLE;
	pBiot       = NULL;
	m_szHost[0] = '\0';
	Clear();
}
    
CSockSide::~CSockSide()
{
}
    
void CSockSide::Clear()
{
	side       = CSock::SIDE_AVAILABLE;
	saidHello  = FALSE;
	heardHello = FALSE;
	m_bNag     = FALSE;
	if (pBiot)
		delete pBiot;
	pBiot      = NULL;
	nSendAck   = 0;
	m_exited   = 0;
	m_entered  = 0;
	m_bounced  = 0;
	m_acksSent = 0;
	m_bConnected = FALSE;
	m_szWorldName[0] = '\0';
 	m_szAddress[0]   = '\0';
}
    
    
    //////////////////////////////////////////////////////
    // SetStatus
    //
    //
    void CSockSide::SetStatus(HWND hDlg, int nSide)
    {
    	char szText[128];
    
    	SetDlgItemInt(hDlg, IDC_LEFT_EXCHANGED + nSide, m_entered, FALSE);
    	SetDlgItemInt(hDlg, IDC_LEFT_BOUNCED + nSide, m_exited, FALSE);
    
    	switch (state)
    	{
    		case CSock::STATE_AVAILABLE:
    			lstrcpy(szText, "");
    			break;
    
    		case CSock::STATE_CONNECTED:
    			if (m_szWorldName[0] != '\0')
    			{
    				lstrcpy(szText, m_szWorldName);
    				lstrcat(szText, " c");
    			}
    			else
    			{
    				lstrcpy(szText, "C");
    			}
    			lstrcat(szText, "onnected from ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_DISCONNECTED:
    			lstrcpy(szText, "Disconnected from ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_ERROR_RESOLVING:
    			lstrcpy(szText, "Unable to resolve ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_ERROR_LISTENING:
    			lstrcpy(szText, "Unable to listen.");
    			break;
    
    		case CSock::STATE_ERROR_CONNECTING:
    			lstrcpy(szText, "Unable to connect to ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_CONNECTING:
    			lstrcpy(szText, "Attempting to connect to ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_SEND_ERROR:
    			lstrcpy(szText, "Error sending to ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		case CSock::STATE_RECEIVING_ERROR:
    			lstrcpy(szText, "Error receiving from ");
    			lstrcat(szText, m_szHost);
    			break;
    
    		default:
    			lstrcpy(szText, "Unknown State!");
    			break;
    	}
    	SetDlgItemText(hDlg, IDC_LEFT_WORLD + nSide, szText);
    }
    
    
    //////////////////////////////////////////////////////
    // Reserve
    //
    void CSockSide::Reserve()
    {
    	side       = CSock::SIDE_RESERVED;
    	saidHello  = FALSE;
    	heardHello = FALSE;
    	nSendAck   = 0;
    	if (pBiot)
    		delete pBiot;
    
    	pBiot      = NULL;
    }
    
    
    //////////////////////////////////////////////////////
    // SetHost
    //
    void CSockSide::SetHost(LPCSTR szHost)
    {
    	lstrcpy(m_szHost, szHost);
    }
    
    
//////////////////////////////////////////////////////
// SetHost
//
void CSockSide::SetHost(LPSOCKADDR_IN pAddr)
{
	 wsprintf(m_szHost, "%u.%u.%u.%u",
			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b1,
 			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b2,
 			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b3,
 			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b4);
}
 

//////////////////////////////////////////////////////
// SetAddress
//
void CSockSide::SetAddress(LPSOCKADDR_IN pAddr)
{
	 wsprintf(m_szAddress, "%u.%u.%u.%u",
			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b1,
			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b2,
			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b3,
			(UINT) pAddr->sin_addr.S_un.S_un_b.s_b4);
}
 
 
//////////////////////////////////////////////////////
// CSock Constructor and Destructor
//
//
CSock::CSock()
{
	InitializeCriticalSection(&criticalSection);
    
	m_hWinsockDll = NULL;
	m_lsock = INVALID_SOCKET;
}
    
CSock::~CSock()
{
	StopSession();
	DeleteCriticalSection(&criticalSection);
}
    
    
//////////////////////////////////////////////////////
// CriticalSection Lock and Unlock
//
//
void CSock::LockAll()
{
	EnterCriticalSection(&criticalSection);
}
    
void CSock::UnlockAll()
{
	LeaveCriticalSection(&criticalSection);
}
    
    
    //////////////////////////////////////////////////////
    // SetStatus
    //
    //
    void CSock::SetStatus(HWND hDlg)
    {
    	char szText[128];
    
    	for (int side = 0; side < MAX_SIDES; side++)
    	{
    		int i = FindOldSide(side);
    		if (i < MAX_SOCKETS)
    			m_side[i].SetStatus(hDlg, side);
    		else
    		{
    			SetDlgItemText(hDlg, IDC_LEFT_EXCHANGED + side, "");
    			SetDlgItemText(hDlg, IDC_LEFT_BOUNCED + side,   "");
    			if (!m_bNetwork)
    			{
    				SetDlgItemText(hDlg, IDC_LEFT_WORLD + side, "Not accepting connections");
    			}
    			else
    			{
    				if (m_bEnabled[side])
    				{
    					lstrcpy(szText, "Waiting to connect to ");
    					lstrcat(szText, m_sHost[side]);
    					SetDlgItemText(hDlg, IDC_LEFT_WORLD + side, szText);
    				}
    				else
    				{
    					if (m_bListen)
    					{
    						SetDlgItemText(hDlg, IDC_LEFT_WORLD + side, "Listening for connection");
    					}
    					else
    					{
    						SetDlgItemText(hDlg, IDC_LEFT_WORLD + side, "Disabled and not listening");
    					}
    				}	
    			}
    		}			
    	}
    
    	lstrcpy(szText, "Your World: ");
    	lstrcat(szText, m_szOurHost);
    	SetDlgItemText(hDlg, IDC_ADDRESS, szText);
    }
    
    
//////////////////////////////////////////////////////
// StartSession
//
//
BOOL CSock::StartSession(HWND hMessageWnd, Environment* pEnv)
{
	m_hWnd = AfxGetMainWnd()->GetSafeHwnd();
	m_pEnv = pEnv;
    
	if (m_hWinsockDll == NULL && m_bNetwork == TRUE)
	{ 
		CString sWinsock;
		LoadString("winsock", "wsock32.DLL", sWinsock, MAX_PATH);

		m_hWinsockDll = LoadLibrary(sWinsock);
  
		if (m_hWinsockDll != NULL)
		{
			m_htons					= (fhtons)					GetProcAddress(m_hWinsockDll, "htons");
			m_inet_addr				= (finet_addr)				GetProcAddress(m_hWinsockDll, "inet_addr");
			m_WSAStartup			= (fWSAStartup)				GetProcAddress(m_hWinsockDll, "WSAStartup");
			m_WSACleanup			= (fWSACleanup)				GetProcAddress(m_hWinsockDll, "WSACleanup");
			m_WSAAsyncSelect		= (fWSAAsyncSelect)			GetProcAddress(m_hWinsockDll, "WSAAsyncSelect");
			m_recv					= (frecv)					GetProcAddress(m_hWinsockDll, "recv");
			m_WSAGetLastError		= (fWSAGetLastError)		GetProcAddress(m_hWinsockDll, "WSAGetLastError");
			m_send					= (fsend)					GetProcAddress(m_hWinsockDll, "send");
			m_closesocket			= (fclosesocket)			GetProcAddress(m_hWinsockDll, "closesocket");
			m_socket				= (fsocket)					GetProcAddress(m_hWinsockDll, "socket");
			m_listen				= (flisten)					GetProcAddress(m_hWinsockDll, "listen");
			m_bind					= (fbind)					GetProcAddress(m_hWinsockDll, "bind");
			m_accept				= (faccept)					GetProcAddress(m_hWinsockDll, "accept");
			m_gethostbyname			= (fgethostbyname)			GetProcAddress(m_hWinsockDll, "gethostbyname");
			m_gethostname			= (fgethostname)			GetProcAddress(m_hWinsockDll, "gethostname");
			m_connect				= (fconnect)				GetProcAddress(m_hWinsockDll, "connect");
		}
  
		if (!(	m_hWinsockDll && m_htons && m_inet_addr && m_WSAStartup &&
				m_WSACleanup &&	m_WSAAsyncSelect && m_recv &&
				m_WSAGetLastError && m_send && m_closesocket &&
				m_socket && m_listen && m_bind && m_gethostbyname &&
				m_gethostname && m_accept && m_connect))
		{
			if (m_hWinsockDll)
			{
				FreeLibrary(m_hWinsockDll);
				m_hWinsockDll = NULL;
			}
    
			MessageBox(hMessageWnd,"Winsock is required for network operation.\n"
							" Unable to find and load 'wsock32.dll'",
							szTitle, MB_OK | MB_ICONEXCLAMATION);
			m_bNetwork = FALSE;
			return FALSE;
		}

		WSADATA WSAData;
		char szTemp[80];
		int status;
    
		if ((status = (*m_WSAStartup)(MAKEWORD(1,1), &WSAData)) != 0)
		{
			wsprintf(szTemp, "Unable to start Winsock support (%d).", status);
			MessageBox(hMessageWnd, szTemp, szTitle, MB_OK);
			StopSession();
			m_bNetwork = FALSE;
			return FALSE;
		}
    
		m_szAddress[0] = '\0';
		m_szOurHost[0]    = '\0';
		if ((*m_gethostname)(m_szOurHost, MAX_HOST_NAME) == 0)
		{
			PHOSTENT phe;

			phe = (*m_gethostbyname)(m_szOurHost);
			if (phe)
			{
				wsprintf(m_szAddress, "%u.%u.%u.%u",
					 (UINT) ((BYTE) phe->h_addr_list[0][0]),
					 (UINT) ((BYTE) phe->h_addr_list[0][1]),
					 (UINT) ((BYTE) phe->h_addr_list[0][2]),
					 (UINT) ((BYTE) phe->h_addr_list[0][3]));
			}
    			
		}
	}
	return TRUE;
}


void CSock::GetFullAddress(LPSTR szFullName)
{
	lstrcpy(szFullName, m_szOurHost);
	if (lstrlen(m_szAddress) > 0)
	{
		lstrcat(szFullName, " (");
		lstrcat(szFullName, m_szAddress);
		lstrcat(szFullName, ")");
	}
}
    
    
    //////////////////////////////////////////////////////
    // Disconnect
    //
    //
    void CSock::Disconnect()
    {
    		for (int i = 0; i < MAX_SOCKETS; i++)
    		{
    			if (m_side[i].side != SIDE_AVAILABLE)
    			{
    				(*m_closesocket)(m_side[i].socket);
    				m_side[i].Clear();
    				m_side[i].state = STATE_DISCONNECTED;
    			}
    		}
    		StopListening();
    }
    
    
    //////////////////////////////////////////////////////
    // StopSession
    //
    //
    void CSock::StopSession()
    {
    	if (m_hWinsockDll)
    	{
    		Disconnect();
    		(*m_WSACleanup)();
    
    		FreeLibrary(m_hWinsockDll);
    		m_hWinsockDll = NULL;
    	}
    }
    
    
    /////////////////////////////////////////////////////////////////////////////
    // FillAddr
    //
    // Retrieves the IP address and port number.
    //
    BOOL CSock::FillAddr(char* szAHost, PSOCKADDR_IN psin, BOOL bClient)
    {
    	PHOSTENT phe;
    
    	// Everyone is IP
    	psin->sin_family = AF_INET;
    
    	if (bClient)
    	{
    		psin->sin_addr.S_un.S_addr = (*m_inet_addr)(szAHost);
    
    		if (psin->sin_addr.S_un.S_addr == INADDR_NONE || lstrlen(szAHost) < 7)
    		{
    			phe = (*m_gethostbyname)(szAHost);
    			if (phe == NULL)
    			{
    				return FALSE;
    			}
    			memcpy((char FAR *)&(psin->sin_addr), phe->h_addr, phe->h_length);
    		}
       }
       else 
       { 
    	  // Address setup for listening
          psin->sin_addr.s_addr = INADDR_ANY;
    
    	  // Convert to network order
          psin->sin_port = (*m_htons)(m_listenPort);
       }
       return TRUE;
    }
    
    
    /////////////////////////////////////////////////////////////////////////////
    // Listen
    //
    // Listens if required.
    //
    BOOL CSock::Listen()
    {
    	SOCKADDR_IN local_sin;
    	int status;
    
    	// Are we already listening and should we stop?
    	if (m_lsock != INVALID_SOCKET)
    	{
    		if (!m_bListen || !m_bNetwork)
    			StopListening();
    	    return TRUE;
    	}
    
    	// Should we not listen?
    	if (!m_bListen || !m_bNetwork || m_hWinsockDll == NULL)
    		return TRUE;
    
    	m_lsock = (*m_socket)( AF_INET, SOCK_STREAM, 0 );
    	if (m_lsock == INVALID_SOCKET)
    	{
    		return FALSE;
    	}
      
    	// Retrieve the IP address and TCP Port number
    	if (!FillAddr(NULL, &local_sin, FALSE))
    	{
    		(*m_closesocket)( m_lsock );
    		m_lsock = INVALID_SOCKET;
    		return FALSE;
    	}
    
    	// Associate an address with a socket. (bind)
    	if ((*m_bind)( m_lsock, (struct sockaddr FAR *) &local_sin, sizeof(local_sin)) == SOCKET_ERROR)
    	{
    		(*m_closesocket)( m_lsock );
    		m_lsock = INVALID_SOCKET;
    		return FALSE;
    	}
    
    	// We will accept more connections that we can handle
    	if ((*m_listen)( m_lsock, 6 ) < 0)
    	{
    		(*m_closesocket)( m_lsock );
    		m_lsock = INVALID_SOCKET;
    		return FALSE;
    	}
    
    	// Send window a WSA_ACCEPT when something is trying to connect.
    	if ((status = (*m_WSAAsyncSelect)( m_lsock, m_hWnd, WSA_ACCEPT, FD_ACCEPT)) > 0)
    	{
    		(*m_closesocket)( m_lsock );
    		m_lsock = INVALID_SOCKET;
    		return FALSE;
    	}
    	return TRUE;
    }
    
    
/////////////////////////////////////////////////////////////////////////////
// Should we nag?
//
BOOL CSock::ShouldNag()
{
	for (int i = 0; i < MAX_SOCKETS; i++)
		if (m_side[i].m_bNag)
			return TRUE;

	return FALSE;
}
    
    
    //////////////////////////////////////////////////////////////////
    // StopListening
    //
    //
    void CSock::StopListening()
    {
    	if (m_lsock != INVALID_SOCKET)
    	{
    		(*m_closesocket)(m_lsock);
    		m_lsock = INVALID_SOCKET;
    	}
    }
    
    
    ///////////////////////////////////////////////////////////
    // ConnectAll
    //
    //
    void CSock::ConnectAll()
    {
    	if (m_hWinsockDll != NULL)
    	{
    		Connect(m_sHost[Side::LEFT], Side::LEFT);
    
    		Connect(m_sHost[Side::TOP], Side::TOP);
    
    		Connect(m_sHost[Side::BOTTOM], Side::BOTTOM);
    
    		Connect(m_sHost[Side::RIGHT], Side::RIGHT);
    	}
    }
    
    
    DWORD WINAPI ConnectWorld(LPVOID lpvParam)
    {
    int status;
    int i = (int) lpvParam;
    CSockSide& cSide = g_sock.m_side[i];
    
    	// Convert host name to ip address
    	SOCKADDR_IN dest_sin;  
    	if (!g_sock.FillAddr(cSide.m_szHost, &dest_sin, TRUE))
    	{
    		g_sock.LockAll();
    		cSide.Clear();
    		cSide.state = CSock::STATE_ERROR_RESOLVING;
    		g_sock.UnlockAll();
    		return FALSE;
    	}
    
    	// Open a socket
    	cSide.socket = (*g_sock.m_socket)( AF_INET, SOCK_STREAM, 0);
    	if (cSide.socket == INVALID_SOCKET)
    	{
    		g_sock.LockAll();
    		cSide.Clear();
    		cSide.state = CSock::STATE_ERROR_CONNECTING;
    		g_sock.UnlockAll();
    		return FALSE;
    	}
	cSide.SetAddress(&dest_sin);
    
    	// Get the right destination socket
    	switch (cSide.oldside)
    	{
    		case Side::LEFT:
    			dest_sin.sin_port = (*g_sock.m_htons)(g_sock.m_leftPort);
    			break;
    
    		case Side::RIGHT:
    			dest_sin.sin_port = (*g_sock.m_htons)(g_sock.m_rightPort);
    			break;
    
    		case Side::TOP:
    			dest_sin.sin_port = (*g_sock.m_htons)(g_sock.m_topPort);
    			break;
    
    		case Side::BOTTOM:
    			dest_sin.sin_port = (*g_sock.m_htons)(g_sock.m_bottomPort);
                break;
    	}
    
    	if ((*g_sock.m_connect)(cSide.socket, (PSOCKADDR) &dest_sin, sizeof( dest_sin)) < 0)
    	{
    		g_sock.LockAll();
    		(*g_sock.m_closesocket)( cSide.socket );
    		cSide.Clear();
    		cSide.state = CSock::STATE_ERROR_CONNECTING;
    		g_sock.UnlockAll();
    		return FALSE;
    	}
    
    	if ((status = (*g_sock.m_WSAAsyncSelect)( cSide.socket, g_sock.m_hWnd, WSA_CONNECTION + i, FD_CONNECT | FD_READ | FD_CLOSE )) > 0)
    	{
    		g_sock.LockAll();
    		(*g_sock.m_closesocket)( cSide.socket );
            cSide.Clear();
    		cSide.state = CSock::STATE_ERROR_CONNECTING;
    		g_sock.UnlockAll();
    		return FALSE;
    	}
    
    	g_sock.LockAll();
    	cSide.side = cSide.oldside;
    	cSide.state = CSock::STATE_CONNECTED;
    	cSide.m_bConnected = TRUE;
    	g_sock.UnlockAll();
    	return TRUE;
    }
    
    
    ///////////////////////////////////////////////////////////
    // Connect
    //
    //
    BOOL CSock::Connect(LPCSTR szHost, int side)
    {
    	int i = FindSide(side);
    
    	if (i < MAX_SOCKETS)
    	{
    		if ((!m_bEnabled[side] || !m_bNetwork || lstrcmp(m_side[i].m_szHost, szHost) != 0) 
    			&& m_side[i].m_bConnected)
    		{
    			LockAll();
    			m_pEnv->side[side]->SetSide(m_pEnv);
    			(*m_closesocket)(m_side[i].socket);
    			m_side[i].Clear();
    			m_side[i].state = STATE_DISCONNECTED;
    			i = MAX_SOCKETS;
    			UnlockAll();
    		}
    	}
    
    	if (!m_bEnabled[side] || i < MAX_SOCKETS || !m_bNetwork)
    		return TRUE;
    
    	// Is there anything to connect to?
    	if (lstrlen(szHost) == 0)
    		return FALSE;
    
    	// Find a map slot
    	i = FindMap();
    	if (i >= MAX_SOCKETS)
    		return FALSE;
    
    	LockAll();
    	// Reserve a map slot
    	m_side[i].Reserve();
    	m_side[i].SetHost(szHost);
    	m_side[i].state = STATE_CONNECTING;
    	m_side[i].oldside = side;
    	UnlockAll();
            
    
    	DWORD dwThreadId;
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectWorld, (LPVOID) i, STANDARD_RIGHTS_REQUIRED, &dwThreadId );
     	if (hThread)
    	{
            SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
    		CloseHandle(hThread);
    	}
    	return TRUE;
    }
    
    
//////////////////////////////////////////////////////////////////
// FindSide
//
//  Finds the m_side that represents a LEFT, RIGHT, TOP or BOTTOM.
//  Returns MAX_SOCKETS if a side cannot be found.
//
int  CSock::FindSide(int side)
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (m_side[i].side == side)
			return i;
	}
	return i;
}
    
    
//////////////////////////////////////////////////////////////////
// FindOldSide
//
//  Finds the m_side that represents a LEFT, RIGHT, TOP or BOTTOM.
//  Returns side MAX_SOCKETS if a side cannot be found.
//
int  CSock::FindOldSide(int side)
{
	int i;
    
	// Find the proper side
	for (i = 0; i < MAX_SOCKETS; i++)
	{
		if (m_side[i].side == side)
			return i;
	}
    
	// If we can't find proper, can we find the old
	for (i = 0; i < MAX_SOCKETS; i++)
	{
		if (m_side[i].oldside == side)
			return i;
	}
	return i;
}
    
    
////////////////////////////////////////////////////////////////
// ConnectionActive
//
// Is one connection active?
//
BOOL  CSock::ConnectionActive()
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (m_side[i].side >= Side::LEFT && m_side[i].side < Side::BOTTOM)
			return TRUE;
	}
	return FALSE;
}  
    
    
////////////////////////////////////////////////////////////////
// FindMap
//
// Finds an available side
//
int  CSock::FindMap()
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (m_side[i].side == SIDE_AVAILABLE)
			return i;
	}
	return i;
}
    
    
////////////////////////////////////////////////////////////////
// OppositeSide
//
// Takes one side value and returns the opposite side value.
//
int  CSock::OppositeSide(int side)
{
	if (side == Side::LEFT || side == Side::TOP)
		side++;
	else
		if (side == Side::RIGHT || side == Side::BOTTOM)
			side--;
    
	return side;
}
    
    
////////////////////////////////////////////////////////////////
// Message
//
//
LONG CSock::Message(UINT message, WPARAM wParam, LPARAM lParam)
    {
      switch (message)
      {  
        case WSA_ACCEPT:
          Accept(wParam, lParam);
          return TRUE;
    
        case WSA_CONNECTION_ONE:
          Process(0, (SOCKET) wParam, lParam);
          return TRUE;
    
        case WSA_CONNECTION_TWO:
          Process(1, (SOCKET) wParam, lParam);
          return TRUE;
    
        case WSA_CONNECTION_THREE:
          Process(2, (SOCKET) wParam, lParam);
          return TRUE;
    
        case WSA_CONNECTION_FOUR:
          Process(3, (SOCKET) wParam, lParam);
          return TRUE;
      }
      return FALSE;
    }
    
    
    ////////////////////////////////////////////////////////////////
    // Process
    //
    //
    void CSock::Process(int map, SOCKET sock, LPARAM lParam)
    {
      switch (WSAGETSELECTEVENT(lParam))
      {
      case FD_READ:
        Read(map, sock);
        break;
    
      case FD_CONNECT:
        break;
    
      case FD_CLOSE:
        LockAll();
    	(*m_closesocket)(m_side[map].socket);
    	if (m_side[map].side >= Side::LEFT &&
    		m_side[map].side < Side::BOTTOM)
    		m_pEnv->side[m_side[map].side]->SetSide(m_pEnv);
    	m_side[map].Clear();
    	m_side[map].state = STATE_DISCONNECTED;
        UnlockAll();
        break;
    
      default:
        break;
      }
    }
    
    
    ////////////////////////////////////////////////////////////////
    // WriteAll
    //
    //
    bool CSock::WriteAll()
    {
    	if (m_hWinsockDll != NULL)
     	{
    		for (int i = 0; i < MAX_SOCKETS; i++)
    			Write(i, m_side[i].socket);
			return true;
    	}
		return false;
    }
    
    
    ////////////////////////////////////////////////////////////////
    // Write
    //
    //
    void CSock::Write(int map, SOCKET sock)
    {
    	CSockSide& cSide = m_side[map];
    	PHELLO     p;
    	int        status;
    
		p.package.version = 31;
 
    	LockAll();
    	// We don't write if we don't know our side
    	if (cSide.side != SIDE_RESERVED && cSide.side != SIDE_AVAILABLE)
    	{
    		// If we haven't said hello
    		if (!cSide.saidHello)
    		{
    			cSide.saidHello = TRUE;
    			p.package.type = PACKAGE_HELLO;
    			p.package.size = sizeof (HELLO);
    			p.hello.side = cSide.side;
    
				p.hello.licenseFlags = 0;

				CKeyRegistry& key = AfxKeyRegistry();
   				if (!key.IsGoodKey())
				{
  					lstrcpy(p.hello.szUser, "Unregistered World");
				}
	  			else
				{
	  				lstrcpy(p.hello.szUser, key.m_sName);
					p.hello.licenseFlags |= SITE_REGISTERED;

					if (key.IsTemporaryKey())
						p.hello.licenseFlags |= SITE_TEMPORARY;

					if (key.IsSiteKey())
						p.hello.licenseFlags |= SITE_LICENSE;

					if (key.IsWorkAndHomeKey())
						p.hello.licenseFlags |= SITE_WORKANDHOME;
				}
    
    			if (p.hello.side < 2)
    				p.hello.sideSize = m_pEnv->Height();
    			else
    				p.hello.sideSize = m_pEnv->Width();
    
    			status = (*m_send)(sock, (char*) &p, sizeof(PHELLO), 0);
    		}
    		else
    		{
    			// Do we need to send an ack?
    			if (cSide.nSendAck)
    			{
				if (WriteAck(sock, cSide.nSendAck))
				{
    					cSide.nSendAck = 0;
    					cSide.m_acksSent++;
    				}
    				else
    				{
    					ASSERT(FALSE);
    					cSide.state = STATE_SEND_ERROR;
    					m_pEnv->side[cSide.side]->SetSide(m_pEnv);
    					(*m_closesocket)(cSide.socket);
    					cSide.Clear();
    				}	
    			}
    
    			// Time to send a biot?
    			if (cSide.pBiot == NULL) //bSendBiot)
    			{
    				cSide.pBiot = m_pEnv->side[cSide.side]->GetBiot();
    	
    				if (cSide.pBiot)
    				{
    					postData.ClearBuffer();
    					p.package.type = PACKAGE_BIOT;
    					p.package.size = 0;

						// Write the header in without size
    					postData.Write((LPBYTE) &p.package, sizeof(PACKAGE));

						// Write the biot in
//!						cSide.pBiot->Save(postData);

						// Set the true biot size
						p.package.size = postData.GetSize() - sizeof(PACKAGE);

						// Start over
    					postData.ClearBuffer();
    					postData.Write((LPBYTE) &p.package, sizeof(PACKAGE));
						// Set the true size
						postData.SetUsed(p.package.size + sizeof(PACKAGE));
      
    					if ((*m_send)(sock, (char*) postData.GetBuffer(), postData.GetSize(), 0) == SOCKET_ERROR)
    					{
    						ASSERT(FALSE);
    						cSide.state = STATE_SEND_ERROR;
    						m_pEnv->side[cSide.side]->SetSide(m_pEnv);
    						(*m_closesocket)(cSide.socket);
    						cSide.Clear();
    					}
    				}
    			}
    		}
    	}
    	UnlockAll();
    }
    
    
////////////////////////////////////////////////////////////////
// WriteAck
//
//
BOOL CSock::WriteAck(SOCKET sock, int type)
{
	PACKAGE p;
    
	p.type = type;
	p.size = 0;
	p.version = 31;
	return ((*m_send)(sock, (char*) &p, sizeof(PACKAGE), 0) != SOCKET_ERROR);
}
    
    
////////////////////////////////////////////////////////////////
// WriteInformation
//
//
BOOL CSock::WriteInformation(SOCKET sock)
{
	PACKAGE p;
	char  szLine[800];
	char* pLine;

	p.type    = PACKAGE_INFO_RESPONSE;
	p.version = 31;
	
	pLine = szLine;

	wsprintf(pLine, "\nNAME=%s\n", AfxKeyRegistry().GetName());
	pLine += lstrlen(pLine);
	wsprintf(pLine, "HOST=%s\n", m_szOurHost);
	pLine += lstrlen(pLine);
	wsprintf(pLine, "ADDRESS=%s\n", m_szAddress);
	pLine += lstrlen(pLine);
	wsprintf(pLine, "POPULATION=%lu\n", m_pEnv->GetPopulation());
	pLine += lstrlen(pLine);
	wsprintf(pLine, "TICKS=%lu\n", m_pEnv->options.m_generation);
	pLine += lstrlen(pLine);
//	wsprintf(pLine, "DEATH_RATE=%lu\n", m_pEnv->deathRate);
//	pLine += lstrlen(pLine);
//	wsprintf(pLine, "BIRTH_RATE=%lu\n", m_pEnv->birthRate);
//	pLine += lstrlen(pLine);

	for (int side = 0; side < MAX_SIDES; side++)
	{
		int realSide = FindSide(side);
		if (realSide != MAX_SOCKETS)
		{
			wsprintf(pLine, "SIDE_%d_NAME=%s\n", side, m_side[realSide].m_szWorldName);
			pLine += lstrlen(pLine);
			wsprintf(pLine, "SIDE_%d_HOST=%s\n", side, m_side[realSide].m_szHost);
			pLine += lstrlen(pLine);
			wsprintf(pLine, "SIDE_%d_ADDRESS=%s\n", side, m_side[realSide].m_szAddress);
			pLine += lstrlen(pLine);
			wsprintf(pLine, "SIDE_%d_ENTERED=%lu\n", side, m_side[realSide].m_entered);
			pLine += lstrlen(pLine);
			wsprintf(pLine, "SIDE_%d_EXITED=%lu\n", side, m_side[realSide].m_exited);
			pLine += lstrlen(pLine);
		}
	}
	p.size    = lstrlen(szLine) + 1;

	if ((*m_send)(sock, (char*) &p, sizeof(PACKAGE), 0) != SOCKET_ERROR)
		return ((*m_send)(sock, (char*) szLine, p.size, 0) != SOCKET_ERROR);
	else
		return FALSE;
}


////////////////////////////////////////////////////////////////
// Read
//
//
    void CSock::Read(int map, SOCKET sock)
    {
    	PHELLO p;
    	CSockSide& cSide = m_side[map];
    
    	ASSERT(sock == cSide.socket);
    
    	int status = (*m_recv)(sock, (char*) &p.package, sizeof(PACKAGE), 0);
    
    	// Did we read the package header?
    	if (status == sizeof(PACKAGE))
    	{
			if (p.package.version != 31)
			{
				(*m_closesocket)(sock);
				cSide.Clear();
				return;
			}

    		switch (p.package.type)
    		{
		case PACKAGE_INFO_RESPONSE:
			if (p.package.size > 0)
			{
				char szDummy[32];
				
				// Throw it away.
				while (p.package.size > 0 && status > 0)
				{
					status = (*m_recv)(sock, (char*) szDummy, (p.package.size > sizeof(szDummy))?sizeof(szDummy):p.package.size, 0);
					p.package.size -= status;
				}
			}
			break;

		case PACKAGE_INFO_REQUEST:
			// It should send no information
			if (p.package.size == 0)
			{
				if (!WriteInformation(sock))
				{
					(*m_closesocket)(sock);
					cSide.Clear();
				}
			}
			break;

    		case PACKAGE_HELLO:
    			if (p.package.size == sizeof(HELLO))
    			{
    				// Read in the hello
    				status = (*m_recv)(sock, (char*) &p.hello, sizeof(HELLO), 0);
    				if (status == sizeof (HELLO))
    				{
    					LockAll();
    					// Did we receive a hello yet?
    					if (!cSide.heardHello)
    					{
    						// Do we know what side we are yet?
    						if (cSide.side == SIDE_RESERVED)
    						{
    							p.hello.side = OppositeSide(p.hello.side);
    
    							// Is this side taken?
    							if (FindSide(p.hello.side) == MAX_SOCKETS)
    							{
    								int sideSize;
    								if (p.hello.side < 2)
    									sideSize = m_pEnv->Height();
    								else
    									sideSize = m_pEnv->Width();
    
    	
    								// Open the side in the environment
    								m_pEnv->side[p.hello.side]->SetConnect(p.hello.sideSize);
    								cSide.state = STATE_CONNECTED;
    								lstrcpy(cSide.m_szWorldName, p.hello.szUser);
    
    
    								p.package.type = PACKAGE_HELLO;
    								p.package.size = sizeof (HELLO);
    								p.hello.sideSize = sideSize;
	 								p.package.version = 31;
    
									p.hello.licenseFlags = 0;

									CKeyRegistry& key = AfxKeyRegistry();
					   				if (!key.IsGoodKey())
									{
					  					lstrcpy(p.hello.szUser, "Unregistered World");
									}
						  			else
									{
						  				lstrcpy(p.hello.szUser, key.m_sName);
										p.hello.licenseFlags |= SITE_REGISTERED;
	
										if (key.IsTemporaryKey())
											p.hello.licenseFlags |= SITE_TEMPORARY;

										if (key.IsSiteKey())
											p.hello.licenseFlags |= SITE_LICENSE;

										if (key.IsWorkAndHomeKey())
											p.hello.licenseFlags |= SITE_WORKANDHOME;
									}

    								status = (*m_send)(sock, (char*) &p, sizeof(PHELLO), 0);
    
    								// Now we need to send an hello back
    								cSide.side       = p.hello.side;
    								cSide.oldside    = p.hello.side;
    								cSide.heardHello = TRUE;
    								cSide.saidHello  = TRUE;
    								cSide.pBiot      = NULL;
    								//cSide.bSendBiot  = TRUE;
    								cSide.nSendAck   = 0;
    							}
    							else
    							{
    								(*m_closesocket)(sock);
    								cSide.Clear();
    							}
    						}
    						else
    						{
    							// We already sent our hello, we just needed one back
    							// The connector sends the first hello
    							// and now receives a hello back.
    							cSide.heardHello = TRUE; 
    							//cSide.bSendBiot  = TRUE;
    							cSide.pBiot = NULL;
    							// Open the side in the environment
    							m_pEnv->side[cSide.side]->SetConnect(p.hello.sideSize);
    							lstrcpy(cSide.m_szWorldName, p.hello.szUser);
    							cSide.state = STATE_CONNECTED;
    						}
    					}
    					UnlockAll();
    				}
    			}
    			break;
    
    		case PACKAGE_BIOT:
    			if (p.package.size > 512 && p.package.size < 4096)
    			{
    				CPostData* pData = new CPostData(p.package.size);
    				if (pData && pData->GetBufferSize() > 0)
    				{
    					status = (*m_recv)(sock, (char*) pData->GetBuffer(), p.package.size, 0);
    
    					LockAll();
    					cSide.nSendAck = PACKAGE_NACK;
    
    					if (status == p.package.size)
    					{
    						pData->SetUsed(status);
    
    						// Accept it
    						if (cSide.heardHello)
    						{
    							if (!m_pEnv->side[cSide.side]->RecvBiot(pData))
    							{
    								cSide.m_bounced++;
    								delete pData;
    							}
    							else
    							{
    								cSide.nSendAck = PACKAGE_ACK;
    								cSide.m_entered++;
    							}
    						}
    						else
    						{
    							ASSERT(FALSE);
    							delete pData;
    						}
    					}
    					else
    					{
    						ASSERT(FALSE);
    						delete pData;
    					}
    				}
    				else
    				{
    					if (pData)
    						delete pData;
    				}
    				UnlockAll();
    			}
    			else
    			{
    				ASSERT(FALSE);
    			}
    			break;
    
    		case PACKAGE_ACK:
    		{
    			LockAll();
    			if (cSide.pBiot != NULL)
    			{
    				delete cSide.pBiot;
    				cSide.pBiot = NULL;
    				cSide.m_exited++;
    			}
    			else
    			{
    				ASSERT(FALSE);
    			}
    
    			UnlockAll();
    			break;
    		}
    
    		case PACKAGE_NACK:
    		{
    			LockAll();
    			if (cSide.pBiot != NULL)
    			{
    				CPostData* pData = new CPostData;
    				if (pData)
    				{
    					cSide.pBiot->Reject(cSide.side);
//!    					cSide.pBiot->Save(*pData);
    					if (!m_pEnv->side[cSide.side]->RecvBiot(pData))
    						delete pData;
    				}
    				delete cSide.pBiot;
    				cSide.pBiot = NULL;
    				cSide.m_bounced++;
     			}
    			else
    			{
    				ASSERT(FALSE);
    			}
    			UnlockAll();
    			break;
    		}
    
    		default:
    		{
    			cSide.state = STATE_RECEIVING_ERROR;
     			m_pEnv->side[cSide.side]->SetSide(m_pEnv);
    			(*m_closesocket)(cSide.socket);
    			cSide.Clear();
    			break;
    		}
    		}
    	}
    	else
    	{
    //		ASSERT(FALSE);
    	}
    }
    
    
////////////////////////////////////////////////////////////////
// Accept
//
// Accepts an incoming connection.
//
void CSock::Accept(WPARAM /*wParam*/, LPARAM lParam)
{
	SOCKET sock;
	SOCKADDR_IN acc_sin;    /* Accept socket address - internet style */
	int acc_sin_len;        /* Accept socket address length */
	int status;
    
	if (WSAGETSELECTERROR( lParam ) == 0)
	{
		// Accept the incoming connection.
		acc_sin_len = sizeof( acc_sin );
		sock = (*m_accept)( m_lsock, (struct sockaddr FAR *) &acc_sin, (int FAR *) &acc_sin_len);
    
		if (sock == INVALID_SOCKET)
			return;

		int i = FindMap();
    
		if (i >= MAX_SOCKETS)
		{
			(*m_closesocket)(sock);
			return;
		}
    
		LockAll();
		m_side[i].Clear();
		m_side[i].side   = SIDE_RESERVED;
		m_side[i].SetHost(&acc_sin);
		m_side[i].SetAddress(&acc_sin);
		m_side[i].state = STATE_CONNECTING;
		UnlockAll();

		// Send main window a WSA_READ when either data is pending on
		// the socket (FD_READ) or the connection is closed (FD_CLOSE)
		if ((status = (*m_WSAAsyncSelect)( sock, m_hWnd, WSA_CONNECTION + i, FD_WRITE | FD_READ | FD_CLOSE )) > 0)
		{
			LockAll();
			(*m_closesocket)(sock);
			m_side[i].Clear();
			m_side[i].state = STATE_ERROR_LISTENING;
			UnlockAll();
			return;
		}
    
		// Initialize 
		LockAll();
		m_side[i].socket     = sock;
		m_side[i].saidHello  = FALSE;
		m_side[i].heardHello = FALSE;
		m_side[i].pBiot      = NULL;
		m_side[i].state = STATE_CONNECTED;
		UnlockAll();
	}
	else
	{
		//Cancel any further notifications.
		StopListening();
	}
}
    
    
////////////////////////////////////////////////////////////////
// GetDefaultSettings
//
//
void CSock::GetDefaultSettings()
{
	m_bNetwork = LoadInteger(szbNetwork, FALSE);
	m_bListen  = LoadInteger(szbListen,  FALSE);
      
	m_leftPort   = LoadWord(szLeftPort,   613);
	m_rightPort  = LoadWord(szRightPort,  613);
	m_topPort    = LoadWord(szTopPort,    613);
	m_bottomPort = LoadWord(szBottomPort, 613);
	m_listenPort = LoadWord(szListenPort, 613);
    
	LoadString(szLeftHost,   "", m_sHost[Side::LEFT],   MAX_HOST_NAME);
	LoadString(szRightHost,  "", m_sHost[Side::RIGHT],  MAX_HOST_NAME);
	LoadString(szTopHost,    "", m_sHost[Side::TOP],    MAX_HOST_NAME);
	LoadString(szBottomHost, "", m_sHost[Side::BOTTOM], MAX_HOST_NAME);
    
	m_bEnabled[Side::RIGHT] = LoadInteger(szbRight, FALSE);
	m_bEnabled[Side::TOP]   = LoadInteger(szbTop,   FALSE);
	m_bEnabled[Side::BOTTOM]= LoadInteger(szbBottom,FALSE);
	m_bEnabled[Side::LEFT]  = LoadInteger(szbLeft,  FALSE);
}     
    

////////////////////////////////////////////////////////////////
// LoadInteger
//
//
int CSock::LoadInteger(char* szValue, int defValue)
{
	return AfxUserRegistry().GetValue(szValue, defValue);
}


////////////////////////////////////////////////////////////////
// LoadWord
//
//
WORD CSock::LoadWord(char* szValue, WORD defValue)
{
	int value = (int) defValue;
	
	return (WORD) AfxUserRegistry().GetValue(szValue, value);
}


////////////////////////////////////////////////////////////////
// LoadString
//
//
void CSock::LoadString(char* szValue, char* defString, CString& sString, int maxSize)
{
	LPSTR szString = sString.GetBuffer(maxSize + 1);
	AfxUserRegistry().GetDefaultString(szValue, szString, maxSize, defString);
	sString.ReleaseBuffer();
}
    
    
////////////////////////////////////////////////////////////////
// SetDefaultSettings
//
//
void CSock::SetDefaultSettings()
{
	SaveInteger(szbNetwork, m_bNetwork);
	SaveInteger(szbListen,  m_bListen);
      
	SaveInteger(szLeftPort, m_leftPort);
	SaveInteger(szRightPort, m_rightPort);
	SaveInteger(szTopPort, m_topPort);
	SaveInteger(szBottomPort, m_bottomPort);
	SaveInteger(szListenPort, m_listenPort);
   
	SaveString(szLeftHost, m_sHost[Side::LEFT]);
	SaveString(szRightHost, m_sHost[Side::RIGHT]);
	SaveString(szTopHost, m_sHost[Side::TOP]);
	SaveString(szBottomHost, m_sHost[Side::BOTTOM]);
    
	SaveInteger(szbLeft, m_bEnabled[Side::LEFT]);
	SaveInteger(szbRight, m_bEnabled[Side::RIGHT]);
	SaveInteger(szbTop, m_bEnabled[Side::TOP]);
	SaveInteger(szbBottom, m_bEnabled[Side::BOTTOM]);
}

    
////////////////////////////////////////////////////////////////
// SaveInteger
//
//
void CSock::SaveInteger(char* szValue, int nValue)
{
	AfxUserRegistry().SetValue(szValue, nValue);
}
    

////////////////////////////////////////////////////////////////
// SaveString
//
//
void CSock::SaveString(char* szValue, LPCSTR szString)
{
	AfxUserRegistry().SetString(szValue, szString);
}
    

////////////////////////////////////////////////////////////////
// Serialize
//
//
void CSock::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bNetwork;
		ar << m_bListen;
      
		ar << m_leftPort;
		ar << m_rightPort;
		ar << m_topPort;
		ar << m_bottomPort;
		ar << m_listenPort;
    
		ar << m_sHost[Side::LEFT];
		ar << m_sHost[Side::RIGHT];
		ar << m_sHost[Side::TOP];
		ar << m_sHost[Side::BOTTOM];
    
		ar << m_bEnabled[Side::LEFT];
		ar << m_bEnabled[Side::RIGHT];
		ar << m_bEnabled[Side::TOP];
		ar << m_bEnabled[Side::BOTTOM];
	}
	else
	{
		ar >> m_bNetwork;
		ar >> m_bListen;
      
		ar >> m_leftPort;
		ar >> m_rightPort;
		ar >> m_topPort;
		ar >> m_bottomPort;
		ar >> m_listenPort;
    
		ar >> m_sHost[Side::LEFT];
		ar >> m_sHost[Side::RIGHT];
		ar >> m_sHost[Side::TOP];
		ar >> m_sHost[Side::BOTTOM];
    
		ar >> m_bEnabled[Side::LEFT];
		ar >> m_bEnabled[Side::RIGHT];
		ar >> m_bEnabled[Side::TOP];
		ar >> m_bEnabled[Side::BOTTOM];
	}
}
