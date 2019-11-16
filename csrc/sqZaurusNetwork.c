/*
  Network support code for Zaurus and ICRUISE.
  Written by OHSHIMA Yoshiki (ohshima@is.titech.ac.jp)
  based on the UNIX version by Ian Piumarta (ian.piumarta@inria.fr)

  * DAPI_SUPPORT added by T.Ishitani (1999.11.27)
        this code is still experimental.
  */

#include <zheaders.h>
#include <pimmgr.h>
#include <pppfunc.h>
#include <wtii_res.h>
#include <wtii.h>
#include <crystal.h>
#include "sq.h"
#include "wtii_C_usr.h"

char *printf(char *fmt, ...);
static int dapi_IsConnected();

#undef DEBUG 

/* The maximum number of sockets Squeak/Zaurus handles */
#define SOCKETSNUM 4
#define INITFILENAME "F0:WTIIPPP.INF"

#define MAXHOSTNAMELEN 256

static int nameToAddr(char *hostName);
static void addrToName(int inetAddress);

/*** Socket types ***/

#define TCPSocketType	 	0
#define UDPSocketType	 	1


/*** Resolver states ***/

#define ResolverUninitialized	0
#define ResolverSuccess		1
#define ResolverBusy		2
#define ResolverError		3


/*** TCP Socket states ***/

#define Unconnected		0x00
#define WaitingForConnection	0x01
#define Connected		0x02
#define OtherEndClosed		0x03
#define ThisEndClosed		0x04

static int thisNetSession = 0;

static char   localHostName[MAXHOSTNAMELEN];
static u_long localHostAddress;
      /* WARNING: GROSS IPv4 ASSUMPTION!!! */

typedef struct privateSocketStruct {
  int s;		/* Zaurus socket */
  int sema;		/* io notification semaphore */
  int sockState;	/* connection + data state */
  int sockError;	/* errno after socket error */
  int socketsIndex;     /* index for "sockets" array */
  int nselIndex;        /* index for "nsel" array */
  
} privateSocketStruct;

/*** Accessors for private socket members from
  a Squeak socket pointer ***/

#define PSP(S)	((privateSocketStruct *)((S)->privateSocketPtr))

#define SOCKET(S)	(PSP(S)->s)
#define SOCKETSEMA(S)	(PSP(S)->sema)
#define SOCKETSTATE(S)	(PSP(S)->sockState)
#define SOCKETERROR(S)	(PSP(S)->sockError)
#define SOCKETPEER(S)	(PSP(S)->peer)

#define SIGNAL(PSS)	signalSemaphoreWithIndex((PSS)->sema)

/*** Resolver state ***/

static char lastName[MAXHOSTNAMELEN+1];
static int  lastAddr= 0;
static int  lastError= 0;

static int  resolverSema;

int localAddress;

typedef void (*AioHandler)(privateSocketStruct *pss, int errorFlag);

static void nullHandler(privateSocketStruct *, int);
static void acceptHandler(privateSocketStruct *, int);
static void connectHandler(privateSocketStruct *, int);
static void dataHandler(privateSocketStruct *, int);
static void closeHandler(privateSocketStruct *, int);

privateSocketStruct sockets[SOCKETSNUM];
AioHandler handler[SOCKETSNUM];
sel nsel[SOCKETSNUM];
int nselMap[SOCKETSNUM];
int nselMax;


static int useDapiConnect = 0;

static int
dapi_IsConnected()
/* external ppp connection program support. */
/* if already connected return 1, else return 0 */
{
   /* すでにPPPConnect されているかを知るInterface があれば、いいのだが、
    * 残念ながら、用意されていないらしい。
    * 仕方がないので、非常に怪しい裏技で調べることにする。
    * i.e. dapi用のキューが生成されているか == connect されていると考えてみよう
    */

  if (queue_npeek(str_to_name("APPQ")) < 0) {
    /* queue error */
    return 0;
  }
  else
    return 1;
}

static char *
handlerName(AioHandler h)
{
  if (h == nullHandler)    return "nullHandler";
  if (h == acceptHandler)  return "acceptHandler";
  if (h == connectHandler) return "connectHandler";
  if (h == dataHandler)    return "dataHandler";
  if (h == closeHandler)   return "closeHandler";
  return "***unknownHandler***";
}

/* internal functions for manipulating nsel struct */

static void
nselCompact()
{
  int i;
  int nextFree = -1;
  int plusOne;

  for (i = 0; i < SOCKETSNUM; i++) {
    if (nsel[i].se_fd == 0) {
      nextFree = i;
      break;
    }
  }

  plusOne = i + 1;
  while(plusOne < SOCKETSNUM && nsel[plusOne].se_fd != 0) {
    nsel[nextFree].se_fd = nsel[plusOne].se_fd;
    nsel[nextFree].se_inflags = nsel[plusOne].se_inflags;
    nsel[nextFree].se_outflags = nsel[plusOne].se_outflags;
    nsel[plusOne].se_fd = 0;
    sockets[nselMap[nextFree]].nselIndex = nextFree;
    nextFree++;
    plusOne++;
  }
  nselMax = nextFree;
}

static int
nselInstall(privateSocketStruct *pss)
{
  if (nselMax >= SOCKETSNUM) {
    return -1;
  }

  nsel[nselMax].se_fd = pss->s;
  nsel[nselMax].se_inflags = 0;
  nsel[nselMax].se_outflags = 0;

  pss->nselIndex = nselMax;
  nselMap[nselMax] = pss->socketsIndex;

  return nselMax++;
}

static int
socketsFindNextFree()
{
  int i;
  
  for (i = 0; i < SOCKETSNUM; i++) {
    if (sockets[i].s == 0) {
      return i;
    }
  }
  return -1;
}

/*  asynchronous I/O functions */

void
aioPoll()
{
  int result;
  int err;
  int wait = 0;
  int i;

  /* static int aioPollCount = 0; */
  /*  printf("aioPoll: %d\n", aioPollCount++); */
  
  if (nselMax == 0) {
    return;
  }

  result = nselect(nsel, nselMax, &wait, 0, 0, &err);

  if (result == 0) {
    /* do nothing */
  } else if (result == -1) {
    printf("aioPoll error\n");
    /* ioExit(); */
  } else {
    for (i = 0; result > 0 && i < nselMax; i++) {
      if (nsel[i].se_outflags & (CONNECT_NOTIFY|READ_NOTIFY|WRITE_NOTIFY|ACCEPT_NOTIFY|EXCEPT_NOTIFY)) {
	handler[nselMap[i]](&sockets[nselMap[i]], (nsel[i].se_outflags & EXCEPT_NOTIFY));
	result--;
      }
    }
  }
  return;
}

void
aioSuspend(privateSocketStruct *pss)
{
  int nselIndex = pss->nselIndex;

  nsel[nselIndex].se_inflags = 0;
  nsel[nselIndex].se_outflags = 0;
  handler[pss->socketsIndex] = nullHandler;
}

void
aioDisable(privateSocketStruct *pss)
{
  int nselIndex = pss->nselIndex;

  nsel[nselIndex].se_fd = 0;
  nsel[nselIndex].se_inflags = 0;
  nsel[nselIndex].se_outflags = 0;
  handler[pss->socketsIndex] = nullHandler;

  nselCompact();
}

/* install/change the handler for a socket */
static void
aioHandle(privateSocketStruct *pss, AioHandler handlerFn,
	  int nselFlags)
{
  int fd = pss->s;
  int index = pss->socketsIndex;

#ifdef DEBUG
  printf("aioHandle(%d,%s)\n",
	 fd, handlerName(handlerFn));
#endif

  if (&sockets[index] != pss) {
    printf("aioHandle error: bad match\n");
    /* ioExit(); */
  }
  handler[index] = handlerFn;

  nsel[pss->nselIndex].se_inflags = nselFlags|EXCEPT_NOTIFY;
}

/* initialise asynchronous i/o handlers */
static void
aioInit()
{
  int i;

  for (i = 0; i < SOCKETSNUM; i++) {
    sockets[i].s = 0;
    handler[i] = NULL;
    nselMap[i] = -1;
    nsel[i].se_fd = 0;
  }
}


/* disable handlers and close all sockets */
static void
aioShutdown()
{
  int i;

  for (i= 0; i < SOCKETSNUM; i++) {
    if (sockets[i].s != 0) {
      aioDisable(&sockets[i]);
      close(sockets[i].s);
      sockets[i].s = 0;
    }
  }
}


/* answer whether the given socket is valid
   in this net session */
static int
socketValid(SocketPtr s)
{
  if ((s != NULL) && (SOCKET(s) != 0)
      && (s->sessionID == thisNetSession)) {
    return true;
  }
  success(false);
  return false;
}

/* answer whether the socket can be read
   (or accept()ed) without blocking */
static int
socketReadable(int s)
{
  struct sel nsel;
  int err;
  int wait = 0;

  nsel.se_fd = s;
  nsel.se_inflags = READ_NOTIFY;
  
  return nselect(&nsel, 1, &wait, 0, 0, &err) > 0;
}

/* answer whether the socket
   can be written without blocking */
static int
socketWritable(int s)
{
  struct sel nsel;
  int err;
  int wait = 0;

  nsel.se_fd = s;
  nsel.se_inflags = WRITE_NOTIFY;
  
  return nselect(&nsel, 1, &wait, 0, 0, &err) > 0;
}

/* --- aio handlers --- */

void
nullHandler(privateSocketStruct *pss, int errFlag)
{
#ifdef DEBUG
  printf("nullHandler(%d,%d)\n", pss->s, errFlag);
#endif
  aioSuspend(pss);
}

void
acceptHandler(privateSocketStruct *pss, int errFlag)
{
  int oldSock = pss->s;

#ifdef DEBUG
  printf("acceptHandler(%d,%d)\n", pss->s, errFlag);
#endif

  aioSuspend(pss);
  if (errFlag) {
    /* error during listen() */
    nonblocking(oldSock);
    close(oldSock);
    pss->sockState = Unconnected;
    pss->sockError = EBADF;	/* educated guess */
  } else {
    /* accept() is ready */
    int err;
    int newSock = accept(oldSock, 0, 0, &err);
    if (newSock < 0) {
      /* error during accept() */
      aioDisable(pss);
      nonblocking(oldSock);
      close(oldSock);
      pss->sockError = err;
      pss->sockState = Unconnected;
      /* printf("acceptHandler"); */
    } else {
      /* connection accepted */
      nsel[pss->nselIndex].se_fd = newSock;
      pss->s = newSock;
      pss->sockState = Connected;
      nonblocking(oldSock);
      close(oldSock);
    }
  }
  SIGNAL(pss);	/* operation complete */
}

/* read or write data transfer is now possible for the socket */
void
dataHandler(privateSocketStruct *pss, int errFlag)
{

#ifdef DEBUG
  printf("dataHandler(%d,%d)\n", pss->s, errFlag);
#endif
  
  aioSuspend(pss);
  if (errFlag)
    /* error: almost certainly "connection closed by peer" */
    pss->sockState = OtherEndClosed;
  SIGNAL(pss);	/* operation complete */
}

/* a non-blocking close() has completed -- finish tidying up */
void
closeHandler(privateSocketStruct *pss, int errFlag)
{
#ifdef DEBUG
  printf("closeHandler(%d,%d)\n", pss->s, errFlag);
#endif

  aioDisable(pss);
  pss->sockState = Unconnected;
  pss->s = 0;
  SIGNAL(pss);	/* operation complete */
}

/* connect() has completed: check errors,
   leaving the socket unhandled */
void
connectHandler(privateSocketStruct *pss, int errFlag)
{
#ifdef DEBUG
  printf("connectHandler(%d,%d)\n", pss->s, errFlag);
#endif

  aioSuspend(pss);
  if (errFlag) {
    close(pss->s);
    pss->sockError = 1;
    pss->sockState = Unconnected;
  } else {
    /* connect() has completed */
    pss->sockState = Connected;
  }
  SIGNAL(pss);	/* operation complete */
}

/*** Squeak network functions ***/

/* start a new network session */

int
sqNetworkInit(int resolverSemaIndex)
{
  int ret;
  int i;

  if (thisNetSession != 0) {
    return 0;
  }

  if (dapi_IsConnected()) {
    /* already connected by external program. */
    useDapiConnect = 1;
  } else {
    ret = PPPconnect(PPP_OPEN, NULL, INITFILENAME);
    if (ret != NOERR) {
      printf("PPPconnect error: %d\n", ret);
      success(false);
      return false;
    }
  }

  aioInit();
  
  thisNetSession = ioSeconds();
  resolverSema = resolverSemaIndex;

  if (useDapiConnect)
    localAddress = 0;
  else
    localAddress = ntohl(get_myipaddr());

  return 0;
}

void
sqNetworkShutdown()
{
  int ret;
  int i;

  aioShutdown();

  if (thisNetSession == 0)
    return;

  if (useDapiConnect) {
    useDapiConnect = 0;
    thisNetSession = 0;
    resolverSema = 0;
    return;
  }

  ret = PPPconnect(PPP_CLOSE, NULL, INITFILENAME);
  if (ret != NOERR) {
    printf("PPPconnect close error\n");
  }

  ret = PPPconnect(PPP_CLEANUP, NULL, INITFILENAME);
  if (ret != NOERR) {
    printf("PPPconnect cleanup error\n");
  }

  thisNetSession = 0;
  resolverSema = 0;
  
  return;
}

/*** Squeak Generic Socket Functions ***/

/* create a new socket */
void
sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaID(
        SocketPtr s, int netType, int socketType,
        int recvBufSize, int sendBufSize, int semaIndex)
{
  int err;
  int sock;
  int test = 0;
  int next;
  privateSocketStruct *pss;

  s->sessionID = 0;

  if (socketType != TCPSocketType) {
    printf("not TCP\n");
    success(false);
    return;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0, &err);
  if (sock < 0) {
    printf("socket error\n");
    success(false);
    return;
  }

  next = socketsFindNextFree();

  if (next < 0) {
    printf("too many sockets\n");
    success(false);
    return;
  }

  pss = &sockets[next];
  pss->s = sock;
  pss->sema = semaIndex;
  pss->sockState = Unconnected;
  pss->sockError = 0;
  pss->socketsIndex = next;
  nselInstall(pss);

  s->sessionID = thisNetSession;
  s->socketType = socketType;
  s->privateSocketPtr = pss;

  return;
}

/* TCP => start listening for incoming connections.
 */
void
sqSocketListenOnPort(SocketPtr s, int port)
{
  struct sockaddr_in saddr;
  int sock;
  int ret;

  if (!socketValid(s)) return;

  sock = SOCKET(s);

#ifdef DEBUG
  printf("listenOnPort(%d)\n", sock);
#endif

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons((short)port);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
    success(false);
    return;
  }
  
  if (TCPSocketType == s->socketType) {
    /* --- TCP --- */
    /* set backlog to 1, since Squeak server sockets only
       ever accept a single connection.
       (This is unforgivable, but that's the way things are.) */
    listen(sock, 1);
    SOCKETSTATE(s) = WaitingForConnection;
    /* aioEnable(PSP(s)); */
    aioHandle(PSP(s), acceptHandler, ACCEPT_NOTIFY);
    /* accept() possible */
  } else {
    /* --- UDP --- */
    success(false);
    return;
  }
}

/* TCP => open a connection.
 */
void
sqSocketConnectToPort(SocketPtr s, int addr, int port)
{
  int err;
  struct sockaddr_in sin;
  int sock;

  /* long addr = inet_addr("131.112.49.130");
  int test = 0;*/
  
  if (!socketValid(s)) {
    return;
  }
  
  sin.sin_family = AF_INET;
  sin.sin_port   = htons((short)port);
  sin.sin_addr.s_addr = ntohl(addr);

  sock = SOCKET(s);

  nonblocking(sock);
  err = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
  if (err == EWOULDBLOCK || err == EINPROGRESS) {
    SOCKETSTATE(s) = WaitingForConnection;
    aioHandle(PSP(s), connectHandler, CONNECT_NOTIFY);
    return;
  }
  if (err == 0) {
    /* synchronous connect */
    /* printf("synchronous connect: %d", sock); */
    SOCKETSTATE(s) = Connected;
    SIGNAL(PSP(s));	/* operation complete */
    return;
  }
  
  printf("connect error\n");
  success(false);
  return;
}

/* close the socket */
void
sqSocketCloseConnection(SocketPtr s)
{
  int result;
  int sock;
  
  if (!socketValid(s)) {
    return;
  }

  sock = SOCKET(s);

  aioSuspend(PSP(s));
  SOCKETSTATE(s) = ThisEndClosed;

  /* nonblocking(sock); */
  result = close(sock);

  if (result == -1) {
    /* error */
    aioDisable(PSP(s));
    SOCKETSTATE(s) = Unconnected;
    SOCKETERROR(s) = 1;
    SIGNAL(PSP(s));	/* operation complete */
  } else if (result == 0) {
    /* close completed synchronously */
    aioDisable(PSP(s));
    SOCKETSTATE(s) = Unconnected;
    SOCKET(s) = 0;
    SIGNAL(PSP(s));	/* operation complete */
  } else {
    /* asynchronous close in progress */
    SOCKETSTATE(s) = ThisEndClosed;
    /* => close() done */
    aioHandle(PSP(s), closeHandler, CLOSE_NOTIFY);
  }
}

/* close the socket without lingering */
void
sqSocketAbortConnection(SocketPtr s)
{
  int val = 0;

  if (!socketValid(s)) {
    return;
  }

#ifdef DEBUG
  printf("abortConnection(%d)\n", SOCKET(s));
#endif

  setsopt(SOCKET(s), SOL_SOCKET, SO_DONTLINGER,
	     (char*)&val, sizeof(int));
  sqSocketCloseConnection(s);
}

/* Release the resources associated with this socket. 
   If a connection is open, abort it. */
void
sqSocketDestroy(SocketPtr s)
{
  if (!socketValid(s)) return;

#ifdef DEBUG
  printf("destroy(%d)\n", SOCKET(s));
#endif

  /* close if necessary */
  if (SOCKET(s)) {
    sqSocketAbortConnection(s);
  }
  s->privateSocketPtr = NULL;
}


/* answer the OS error code for the last socket operation */
int
sqSocketError(SocketPtr s)
{
  if (!socketValid(s))
    return -1;

  return SOCKETERROR(s);
}

/* return the state of a socket */
int
sqSocketConnectionStatus(SocketPtr s)
{
  if (!socketValid(s)) return -1;

  return SOCKETSTATE(s);
}


/* return the local IP address bound to a socket */
int
sqSocketLocalAddress(SocketPtr s)
{
  if (!socketValid(s)) return -1;

  if (SOCKETSTATE(s) != Unconnected) {
    return sqResolverLocalAddress();
  }

  return 0;
}

/* return the peer's IP address */
int
sqSocketRemoteAddress(SocketPtr s)
{
  struct sockaddr_in saddr;
  int saddrSize;

  if (!socketValid(s)) return -1;

  if (TCPSocketType == s->socketType) {
    /* --- TCP --- */
    saddrSize= sizeof(saddr);
    if (getpeername(SOCKET(s), (struct sockaddr *)&saddr, &saddrSize) ||
	AF_INET != saddr.sin_family)
      return 0;
    return ntohl(saddr.sin_addr.s_addr);
  }
}

/* return the local port number of a socket */
int
sqSocketLocalPort(SocketPtr s)
{
  struct sockaddr_in saddr;
  int saddrSize = sizeof(saddr);

  if (!socketValid(s)) return -1;

  if (getsockname(SOCKET(s), (struct sockaddr *)&saddr, &saddrSize) ||
      AF_INET != saddr.sin_family)
    return 0;
  return ntohs(saddr.sin_port);
}

/* return the peer's port number */
int
sqSocketRemotePort(SocketPtr s)
{
  struct sockaddr_in saddr;
  int saddrSize;

  if (!socketValid(s)) return -1;

  if (TCPSocketType == s->socketType) {
    /* --- TCP --- */
    if (getpeername(SOCKET(s), (struct sockaddr *)&saddr, &saddrSize) ||
	AF_INET != saddr.sin_family)
      return 0;
    return ntohs(saddr.sin_port);
  }
}

/* answer whether the socket has data available for reading */
int
sqSocketReceiveDataAvailable(SocketPtr s)
{
  if (!socketValid(s)) return -1;
  if (SOCKETSTATE(s) == Connected) {
    if (socketReadable(SOCKET(s))) {
      return true;
    }
    aioHandle(PSP(s), dataHandler, READ_NOTIFY);
  }
  return false;
}

/* answer whether the socket has space to receive more data */
int
sqSocketSendDone(SocketPtr s)
{
  if (!socketValid(s)) return -1;
  if (SOCKETSTATE(s) == Connected) {
    if (socketWritable(SOCKET(s))) {
      return true;
    }
    aioHandle(PSP(s), dataHandler, WRITE_NOTIFY);
  }
  return false;
}

/* read data from the socket s into buf for
   at most bufSize bytes.  answer the number actually read.
   */
int
sqSocketReceiveDataBufCount(SocketPtr s, int buf, int bufSize)
{
  int err;
  int sock;
  int result;

  sock = SOCKET(s);
  /* nonblocking(sock); */
  result = recv(sock, (void*)buf, bufSize,
		MSG_NONBLOCKING, &err);

  if (result <= 0) {
    if (err == EWOULDBLOCK) {
      aioHandle(PSP(s), dataHandler, READ_NOTIFY);
    } else {
      /* error: most probably "connection closed by peer" */
      SOCKETSTATE(s) = OtherEndClosed;
      SOCKETERROR(s) = err;
#ifdef DEBUG
      printf("receiveData(%d) = %db\n", sock, 0);
#endif
    }
    return 0;
  }

#ifdef DEBUG
  printf("receiveData(%d) = %d\n", sock, result);
#endif
  aioSuspend(PSP(s));
  return result;
}

/* write data to the socket s from buf for at most bufSize bytes.
   answer the number of bytes actually written. */ 
int sqSocketSendDataBufCount(SocketPtr s, int buf, int bufSize)
{
  int err;
  int result;
  int sock;
  sock = SOCKET(s);

  /* nonblocking(sock); */
  result = send(sock, (void*)buf, bufSize,
		MSG_NONBLOCKING, &err);
  if (result <= 0) {
    if (err == EWOULDBLOCK) {
      aioHandle(PSP(s), dataHandler, WRITE_NOTIFY);
      /* => the semantics of WRITE_NOTIFY seems different
	 from UNICES' */
    } else {
#ifdef DEBUG
      printf("sendData(%d) = %d\n", sock, 0);
#endif
      SOCKETSTATE(s) = OtherEndClosed;
      SOCKETERROR(s) = err;
    }
    return 0;
  }

  /* synchronous send */
#ifdef DEBUG
  printf("sendData(%d) = %d\n", sock, result);
#endif
  aioSuspend(PSP(s));
  return result;
}

void
sqSocketListenOnPortBacklogSize(SocketPtr s, int port, int backlogSize)
{
  success(false);
  return;
}

void
sqSocketAcceptFromRecvBytesSendBytesSemaID(SocketPtr s, SocketPtr serverSocket,
					   int recvBufSize, int sendBufSize, int semaIndex)
{
  success(false);
  return;
}

/*  --- resolver --- */

void
sqResolverAbort()
{
}

int
sqResolverStatus()
{
  if(!thisNetSession) return ResolverUninitialized;
  if(lastError) return ResolverError;
  return ResolverSuccess;
}

void
sqResolverStartAddrLookup(int address)
{
  lastError = 0;
  addrToName(htonl(address));
  if (lastError) {
    lastName[0] = '\0';
  }
}

void
sqResolverStartNameLookup(char *hostName, int nameSize)
{
  int len =
    (nameSize < MAXHOSTNAMELEN) ? nameSize : MAXHOSTNAMELEN;
  memcpy(lastName, hostName, len);
  lastName[len] = 0;
  lastError = 0;
  lastAddr = nameToAddr(lastName);
  /* we're done before we even started */
  signalSemaphoreWithIndex(resolverSema);
}

int
sqResolverNameLookupResult()
{
  return lastAddr;
}
  
int
sqResolverAddrLookupResultSize()
{
  return strlen(lastName);
}

int
sqResolverError()
{
  return lastError;
}

void
sqResolverAddrLookupResult(char *nameForAddress, int nameSize)
{
  memcpy(nameForAddress, lastName, nameSize);
}

int
sqResolverLocalAddress()
{
  return localAddress;
}

static int
nameToAddr(char *hostName)
{
  unsigned long val;

  val = ntohl(resolve_addr_sync(hostName));

  lastError = 0;
  if (val == 0xFFFFFFFF) {
    printf("nameToAddr error\n");
    lastError = 1;
    /* success(false); */
  }

  return val;
}

static void
addrToName(int inetAddress)
{
  char *nAddr;
  char buf[MAXHOSTNAMELEN];
  char *res;
  struct in_addr inaddr;

  inaddr.s_addr = inetAddress;
  nAddr = inet_ntoa(inaddr);
  strcpy(buf, nAddr);
  res = resolve_ptr_sync(buf);

  if (res == NULL) {
    printf("addrToName error\n");
    lastError = 1;
  } else {
    strcpy(lastName, res);
  }
}

