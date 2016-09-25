// sudo g++ -o check check.cpp -lpthread
// ./check

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <errno.h>
#include <iostream>
//#include <map>

#include <fcntl.h>
#include <termios.h>

//using namespace std;



#include <sys/time.h>
#include  <pthread.h>
typedef unsigned char byte;

pthread_t m_thReceive;
static void* Thread_Receive(void* arg);

 int m_nClientFd;
// serial 
int m_nSerial = -1;
 
// int m_nSockFd;
// int m_nPort = 5000;
  
#define _SIZE_BUFFER 1024

byte m_abEn[256];
int m_anData[256];

bool m_bProgEnd = false;
  
void* Thread_Receive(void* arg)
{
	int nSize = 0;
	int nIndex = 0;
	char buf[256];
	byte byCheckSum = 0;
	byte byCheckSum1 = 0;
	byte byCheckSum2 = 0;
	int nPacketSize = 0;
	int nDataLength = 0;
	int nIndexData = 0;
	int nCmd = 0;
	int nId = 0;
	int nData_Address = 0;
	int nData_Length = 0;
	printf("[Thread] Running Thread\r\n");
	char acMem[256];

	int nPos = 0;

	// Socket	
	int nSockFd;
	int nPort = 5000;
	struct sockaddr_in SServerAddr, SClientAddr;
	printf("Server Started\n");
	//nSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	nSockFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSockFd < 0)
	{
		printf("Socket Error\n");
		exit(-1);
	}

	bzero(&SServerAddr, sizeof(SServerAddr));
	SServerAddr.sin_family = AF_INET;
	SServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SServerAddr.sin_port = htons(5000);

	int nLength_Client;
	bind(nSockFd, (struct sockaddr *)&SServerAddr, sizeof(SServerAddr));
	listen(nSockFd, 5); // 연결 대기



	while(m_bProgEnd == false)
	{
	    	printf("Wait Client Connected\n");	
		m_nClientFd = accept(nSockFd, (struct sockaddr *)&SClientAddr, (socklen_t*)&nLength_Client);

		if (m_nClientFd < 0)
		{
			printf("Client Connection Error\n");
			m_bProgEnd = true;
			break;
			//return;// -1;
		}
		printf("Client Connected\r\n");// %s\n", inet_ntoa(SClientAddr.sin_addr));

		while ((m_nClientFd >= 0) && (m_bProgEnd == false))
		{
	
			if ((nSize = read(m_nClientFd, buf, _SIZE_BUFFER)) < 0)
			{		
				printf("Receive Error\n");
				close(m_nClientFd);
				m_nClientFd = -1;
				break;
			}
			
			if (nSize > 0)
			{
				// Packet Size = Header (5) + 3 * Datas
				printf("[Receive]");
				for (int i = 0; i < nSize; i++)
				{
//					printf("[Index=%d]", nIndex);
//					printf("0x%02X,", buf[i]);
					
					switch(nIndex)
					{
						case 0 : 
							if (buf[i] == 0x02) 
							{
								byCheckSum = 0;
								nIndexData = 0;
								
								nData_Address = 0;
								nData_Length = 0;
								//
								nPos = 0;
								acMem[nPos++] = buf[i];
								//
								nIndex++;
							}
							break;
						case 1 : // Command
							nCmd = (int)buf[i];
							byCheckSum = buf[i];
							//
							acMem[nPos++] = buf[i];
							//
							nIndex++;
							break;
						case 2 : // Packet Size ( size +1 ~  checksum - 1) : only datas...
							nPacketSize = (int)buf[i];
							byCheckSum ^= buf[i];
							//
							acMem[nPos++] = buf[i];
							//
							nIndex++;
							nIndexData = 0;
							
#if 1
							break;	
						case 3 : // Datas...						
							//printf("[DataLength=%d/%d]", nIndexData, nDataLength);

							//acMem[nIndex + nIndexData++] = buf[i];												

							acMem[nPos++] = buf[i];
							nIndexData++;
							
							byCheckSum ^= buf[i];	
							if (nIndexData >= nPacketSize) nIndex++;
							break;	
						case 4 : // CheckSum1
							byCheckSum1 = buf[i];

							acMem[nPos++] = buf[i];
							
							if (byCheckSum == byCheckSum1)
							{
								
								//printf("checksum Ok([calc]0x%02x != [receive]0x%02x)\r\n", byCheckSum, byCheckSum1);
							}
							else 
							{
								printf("checksum error([calc]0x%02x != [receive]0x%02x)\r\n", byCheckSum, byCheckSum1);
								nIndex = 0;
							}

							nIndex++;
#endif
							break;
						case 5 : // ETX
							if (buf[i] == 0x03)
							{							
								acMem[nPos++] = buf[i];
							
								//printf("======== Data =========\r\n");
								for (int i = 0; i < nPacketSize / 3; i++)
								{
									m_abEn[i] = acMem[3 + i * 3];
									m_anData[i] = ((acMem[3 + i * 3 + 2] << 8) | acMem[3 + i * 3 + 1]);
									//printf("[%d]%d, ", m_abEn[i], m_anData[i]);
								}
								//printf("\r\n====> End\r\n");
								
								///////////////////
								// Done

								write(m_nSerial, acMem, nPos);
					    			tcflush(m_nSerial, TCIFLUSH);
								//printf("Send -> Serial datas\r\n");
							}
							nIndex = 0;
							break;
					}
				}		
				//printf("\r\n");
			}
		}
		usleep(1);
	}
	
	printf("[Thread] Closed Thread\r\n");
}

int m_nQue = 0;

//byte m_abySend

int InitSerial()
{
    int nFd = -1;
    nFd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (nFd < 0)
      printf("InitSerial-fail\r\n");
    else printf("InitSerial-opended\r\n");

    struct termios tio;
    tio.c_cflag = B115200;
    tio.c_cflag |= CS8;
    tio.c_cflag |= CLOCAL;
    tio.c_cflag |= CREAD;
    tio.c_iflag |= IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;

    tio.c_cc[VINTR] = 0; /*ctrl-c */
    tio.c_cc[VQUIT] = 0; /* Ctrl-\ */
    tio.c_cc[VERASE] = 0; /* del */
    tio.c_cc[VKILL] = 0; // @
    tio.c_cc[VEOF] = 4; // ctrl-d
    tio.c_cc[VTIME] = 0; // inter-character timer unused
    tio.c_cc[VMIN] = 1;// blocking read until 1 character arrives
    tio.c_cc[VSWTC] = 0; // '\0'
    tio.c_cc[VSTART] = 0; // Ctrl-q
    tio.c_cc[VSTOP] = 0; // Ctrl-s
    tio.c_cc[VSUSP] = 0; // Ctrl-z
    tio.c_cc[VEOL] = 0; //'\0'
    tio.c_cc[VREPRINT] = 0; // ctrl-r
    tio.c_cc[VDISCARD] = 0; // Ctrl-u
    tio.c_cc[VWERASE] = 0; // Ctrl-w
    tio.c_cc[VLNEXT] = 0; // Ctrl-v
    tio.c_cc[VEOL2] = 0; // '\0'

    tcflush(nFd, TCIFLUSH);
    tcsetattr(nFd, TCSANOW, &tio);

    return nFd;
}

int main(int argc, char *argv[])
{

  // Serial
  m_nSerial = InitSerial();
  if (m_nSerial < 0)
  {
    printf("Serial Error\n");
    exit(-1);
  }


#if 1 // for socket
  // Thread
  int nRet = pthread_create(&m_thReceive, NULL, Thread_Receive, NULL);
  if (nRet != 0)
  {
	printf("Thread Init Error\r\n");
	return -1;
  }
  printf("Init Thread\r\n");
#endif


  char buff[_SIZE_BUFFER];
  while(1)
  {
    	memset(buff, 0, 1024);
	
//printf("test_0\n");
	
	
	while(m_bProgEnd == false)
	{
		int n;

		/*if ((n = read(m_nClientFd, buff, _SIZE_BUFFER)) < 0)
		{		
			printf("Receive Error\n");
			break;
		}

		if (n > 0)
		{
			printf("%s\n", buff);
			write(m_nSerial, buff, n);
    			tcflush(m_nSerial, TCIFLUSH);
		}*/
		usleep(0);
	}
	m_bProgEnd = true;
	usleep(1000);
	
    	printf("Socket Close\r\n");
    	close(m_nClientFd);
  }
  close(m_nSerial);
  return 0;
}
