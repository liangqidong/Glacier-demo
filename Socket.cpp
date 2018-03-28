//---------------------------------------------------------------------------
#include <vcl.h>
#include <mem.h>
#include <winsock.h>
#include <sys\stat.h>
#include <math.h>
#pragma hdrstop

#include "socket.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
static BOOL ontime = false;
VOID CALLBACK OnTimer(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
{
	ontime=true;
}
//---------------------------------------------------------------------------

void Delay(int i)  // 延时函数
{
	ontime=false;
	UINT timer;
	timer = SetTimer(NULL,NULL,i,TIMERPROC(OnTimer));
	do { 
		Application->ProcessMessages();
	} while (!ontime);
	KillTimer(NULL,timer);
}

// 连接远程主机，返回有效SOCKET
int Connect_Server(AnsiString host, u_short port)
{
	int i, s;
	long* p;
	hostent* phe;
	sockaddr_in sin;
	DWORD Address;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	Address = inet_addr(host.c_str());
	if (Address == INADDR_NONE) {
		phe = gethostbyname(host.c_str());
		if (phe) {
			p = (long*)(*phe->h_addr_list);
			sin.sin_addr.s_addr = *p;
		}
	}
	else {
		i = inet_addr(host.c_str());
		if (i!=-1)
			sin.sin_addr.s_addr = i;
	}

	s = socket(PF_INET,SOCK_STREAM,0);
	if (s == INVALID_SOCKET)
		return 0;
	if (connect(s,(struct sockaddr FAR *)&sin,sizeof(sin)) == SOCKET_ERROR) {
	//int i = WSAGetLastError();
		return 0;
	}
	else
		return s;
}

// 向SOCKET写字符串
int Write_Socket(int sockfd, AnsiString s)
{
	return send(sockfd, s.c_str(), s.Length(),0);
}

// 从SOCKET中读入一个以'\0'结束的字符串
AnsiString Socket_Readln(int sockfd)
{
	AnsiString str = "";
	char buf[2] = "\0";
	Cardinal n;
	n = recv(sockfd, buf, 1, 0);
	while (n>0) { 
		Application->ProcessMessages();
		buf[1] = '\0';
		str = str+buf;
		if (buf[0]==0x10)
			break;
		n = recv(sockfd, buf, 1, 0);
	}
	return Trim(str);
}

// 向远程主机的指定端口发送字符串
BOOL SendMsg(AnsiString addr, u_short port, AnsiString msg)
{ 
	int s;
	s = Connect_Server(addr,port);
	if (s){ 
		Write_Socket(s,msg);
		closesocket(s);
		return true;
	}
	else
		return false;
}

// 动态分配端口，并与SOCKET绑定，返回该SOCKET
SOCKET BindSocket(u_short* port)
{
	struct sockaddr_in server;
	int s;
	if ((s=socket(AF_INET, SOCK_STREAM, 0))<0) {
		//Socket()失败
		return 0;
	}
	for (*port = STREAMPORT; *port<=32767; (*port)++) {
		server.sin_family = AF_INET;
		server.sin_port = htons(*port);
		server.sin_addr.s_addr = INADDR_ANY;
		if (bind(s, (struct sockaddr *)&server, sizeof(server))==0)
			break;
	}
	//Bind()失败
	if (!s)
		return 0;

	if (listen(s,1)!=0) {
		//Listen()失败
		return 0;
	}
	return s;
}

// 向远程主机的指定端口发送数据流
BOOL SendStream(AnsiString addr, u_short port, TMemoryStream* Stream)
{
	char buf[PACKAGESIZE];
	int block = 0;
	SOCKET s;
	Stream->Seek(0,soFromBeginning);
	block = floor(Stream->Size/sizeof(buf));

	s = Connect_Server(addr,port);
	if (!s) { 
		//发送数据失败
		return false;
	}
	for (int i = 1; i<=block; i++) {
		Stream->Read(buf,sizeof(buf));
		Stream->Seek(i*sizeof(buf),soFromBeginning);
		if (s) {
			send(s,buf,sizeof(buf),0);
		}
		else
			return false;
		Application->ProcessMessages();
	}
	int others = Stream->Size-(block*sizeof(buf));
	if (others>0) {
		if (s) {
			Stream->Read(buf,others);
			send(s,buf,others,0);
		}
		else
			return false;
	}
	closesocket(s);
	return true;
}

// 从远程主机的指定端口接收数据流
BOOL RecvStream(SOCKET s, TMemoryStream* Stream)
{
	char buf[PACKAGESIZE];
	struct sockaddr_in client;
	int ns;
	int namelen;
	int pktlen;

	namelen = sizeof(client);
	if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) ==-1) {
		//Accept()失败
		return false;
	}
	int i = 1;
	for (;;i++) {
		Application->ProcessMessages();
		if ((pktlen = recv(ns,buf,sizeof(buf),0))<0) {
			//接收数据失败
			return false;
		}
		else if (pktlen == 0)
			break;
		else {
			Stream->Seek(0,soFromEnd);
			Stream->Write(buf,pktlen);
		}
	}
	Stream->Seek(0,soFromBeginning);
	closesocket(ns);
	closesocket(s);
	return true;
}


