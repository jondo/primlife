    // sock.h
    //
    //
    //
    //
    /////////////////////////////////////////////////////////
#ifndef sock_h
#define sock_h
    
#include "KeyRegistry.h"
#include "Winsock.h"
    
    //Winsock prototype functions
    typedef u_short (PASCAL FAR *fhtons) (u_short hostshort);
    typedef unsigned long (PASCAL FAR* finet_addr) (const char FAR * cp);
    typedef int (PASCAL FAR* fWSAStartup) (WORD wVersionRequired, LPWSADATA lpWSAData);
    typedef int (PASCAL FAR* fWSACleanup) (void);
    typedef int (PASCAL FAR* fWSAAsyncSelect) (SOCKET s, HWND hWnd, u_int wMsg, long lEvent);
    typedef int (PASCAL FAR* frecv) (SOCKET s, char FAR * buf, int len, int flags);
    typedef int (PASCAL FAR* fWSAGetLastError) (void);
    typedef int (PASCAL FAR* fsend) (SOCKET s, const char FAR * buf, int len, int flags);
    typedef int (PASCAL FAR* fclosesocket) (SOCKET s);
    typedef SOCKET (PASCAL FAR* fsocket) (int af, int type, int protocol);
    typedef int (PASCAL FAR* flisten) (SOCKET s, int backlog);
    typedef int (PASCAL FAR* fbind) (SOCKET s, const struct sockaddr FAR *addr, int namelen);
    typedef struct hostent FAR * (PASCAL FAR* fgethostbyname) (const char FAR * name);
    typedef int (PASCAL FAR* fgethostname) (char FAR * name, int namelen);
    typedef SOCKET (PASCAL FAR* faccept) (SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
    typedef int (PASCAL FAR* fconnect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
    
    
    class Environment;
    class Biot;
    
    #define WSA_CONNECTION        WM_USER + 400
    #define WSA_CONNECTION_ONE    WSA_CONNECTION
    #define WSA_CONNECTION_TWO    WSA_CONNECTION + 1
    #define WSA_CONNECTION_THREE  WSA_CONNECTION + 2
    #define WSA_CONNECTION_FOUR   WSA_CONNECTION + 3
    #define WSA_ACCEPT            WM_USER + 395
    
    
    enum {
        MAX_HOST_NAME  = 48,
        MAX_WORLD_NAME = 35
    };
    
    
    class CSockSide
    {
    public:
    	CSockSide();
    	~CSockSide();
    
    	SOCKET socket;
    	int    side;
    	int    state;
    	int    oldside;
    	BOOL   saidHello;
    	BOOL   heardHello;
    	int    nSendAck;
    	Biot*  pBiot;
    	DWORD  m_exited;
    	DWORD  m_entered;
    	DWORD  m_bounced;
    	DWORD  m_acksSent;
    
    	void Clear();
    	void Reserve();
    	void SetHost(LPCSTR szHost);
    	void SetStatus(HWND hDlg, int nSide);
 	void SetHost(LPSOCKADDR_IN pAddr);
 	void SetAddress(LPSOCKADDR_IN pAddr);
    
    	char m_szWorldName[MAX_WORLD_NAME];
    	char m_szHost[MAX_HOST_NAME];
 	char m_szAddress[MAX_HOST_NAME];
    	BOOL m_bNag;
    	BOOL m_bConnected;
    };
    
    
class CSock
{
public:
	CSock();
	~CSock();
    
	void LockAll();
	void UnlockAll();
    
	BOOL StartSession(HWND hMessageWnd, Environment* pEnv);
	void StopSession();  
	void Disconnect();
    
	BOOL Listen();
	void StopListening();
    
	BOOL FillAddr(char* szHost, PSOCKADDR_IN psin, BOOL bClient);
	LONG Message(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Connect(LPCSTR szHost, int side);
	void ConnectAll();
    
	void GetDefaultSettings();
	void SetDefaultSettings();
	void Serialize(CArchive& ar);
    
	int  LoadInteger(char* szValue, int defValue);
	WORD LoadWord(char* szValue, WORD defValue);
	void SaveInteger(char* szValue, int nValue);
	void LoadString(char* szValue, char* defString, CString& sString, int maxSize);
	void SaveString(char* szValue, LPCSTR szString);
    
	BOOL ConnectionActive();
	int  FindSide(int side);
	int  FindOldSide(int side);
	int  FindMap();
	int  OppositeSide(int side);
	BOOL WriteInformation(SOCKET sock);
    
	void Accept(WPARAM wParam, LPARAM lParam);
	void Process(int map, SOCKET sock, LPARAM lParam);
	void Read(int map, SOCKET sock);
	void Write(int map, SOCKET sock);
	bool WriteAll();
	BOOL WriteAck(SOCKET sock, int type);
	BOOL WriteAck(int map, SOCKET sock, int type);
    
	void SetStatus(HWND hDlg);
	BOOL ShouldNag();
    
	void GetFullAddress(LPSTR szFullName);
    
public:
	enum {
		SIDE_AVAILABLE = 4,
		SIDE_RESERVED  = -1,
		MAX_SOCKETS    = 5,
		MAX_SIDES      = 4
	};

	enum {
		SITE_LICENSE     = 0x01,
		SITE_WORKANDHOME = 0x02,
		SITE_TEMPORARY   = 0x04,
		SITE_REGISTERED  = 0x08
	};

	enum {
		STATE_AVAILABLE,
		STATE_CONNECTED,
		STATE_DISCONNECTED,
		STATE_ERROR_RESOLVING,
		STATE_ERROR_LISTENING,
		STATE_ERROR_CONNECTING,
		STATE_CONNECTING,
		STATE_SEND_ERROR,
		STATE_RECEIVING_ERROR
	};
    
	CPostData postData;
    
	char    m_szStatus[80];  
	BOOL    m_bNetwork;
	BOOL    m_bListen;
	char    m_szOurHost[MAX_HOST_NAME];
	char    m_szAddress[MAX_HOST_NAME];
	CString m_sHost[4];
	u_short m_leftPort;
	u_short m_rightPort;
	u_short m_topPort;
	u_short m_bottomPort;
	u_short m_listenPort;
	BOOL    m_bEnabled[4];
   
	CRITICAL_SECTION criticalSection;
    
	static char* szIniFile;
	static char* szKey;
	static char* szbNetwork;
	static char* szbListen;
	static char* szLeftHost;
	static char* szRightHost;
	static char* szTopHost;
	static char* szBottomHost;
	static char* szLeftPort;
	static char* szRightPort;
	static char* szTopPort;
	static char* szBottomPort;
    static char* szListenPort;
    static char* szbLeft;
      static char* szbRight;
      static char* szbTop;
      static char* szbBottom;
      static char* szWorldName;
    
      HINSTANCE    m_hWinsockDll;
      Environment* m_pEnv;
    
      HWND   m_hWnd;
    
      CSockSide m_side[MAX_SOCKETS];
    
      SOCKET m_lsock;
    
      // Winsock variables
      fWSAGetLastError m_WSAGetLastError;
      fWSAAsyncSelect  m_WSAAsyncSelect;
      fWSAStartup      m_WSAStartup;
      fWSACleanup      m_WSACleanup;
      fgethostbyname   m_gethostbyname;
      fgethostname     m_gethostname;
      fclosesocket     m_closesocket;
      finet_addr       m_inet_addr;
      fhtons           m_htons;
      frecv            m_recv;
      fsend            m_send;
      fsocket          m_socket;
      flisten          m_listen;
      fbind            m_bind;
      faccept          m_accept;
      fconnect         m_connect;
    };
    
    enum {
	PACKAGE_HELLO = 0,
	PACKAGE_BIOT  = 1,
	PACKAGE_ACK   = 2,
	PACKAGE_NACK  = 3,
	PACKAGE_INFO_REQUEST  = 4,
	PACKAGE_INFO_RESPONSE = 5
    };
    
typedef struct {
 	BYTE  type;
	BYTE  version;
 	WORD  size;
} PACKAGE, *LPPACKAGE;
    
typedef struct {
	char   szUser[CKeyRegistry::MAX_NAME_SIZE + 1];
	WORD   sideSize;
	BYTE   licenseFlags;
	BYTE   side;
} HELLO, *LPHELLO;
    
typedef struct {
	PACKAGE package;
	HELLO   hello;
} PHELLO;

#endif
