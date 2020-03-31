#include "rl_net.h"
#include "string.h"
#include "malloc.h"
 
int32_t tcp_sock;
 
uint8_t *sendbuf;
uint32_t maxlen; 
 
void sendMessage(uint8_t * message){
 
	if (netTCP_SendReady (tcp_sock)) {
		// The socket is ready to send the data
		maxlen  = netTCP_GetMaxSegmentSize (tcp_sock);
		sendbuf = netTCP_GetBuffer (maxlen);
		memcpy (sendbuf, message, maxlen);
		netTCP_Send (tcp_sock, sendbuf, maxlen);
	}
}	
 
void connect(NET_ADDR addr) {
  if (tcp_sock > 0) {
    switch (netTCP_GetState (tcp_sock)) {
      case netTCP_StateUNUSED:
      case netTCP_StateCLOSED:
        //reconnect
				netTCP_Connect (tcp_sock, &addr, 0);
        break;
 
      case netTCP_StateESTABLISHED:
        sendMessage((uint8_t*)"Hello from client");
        break;
      default:
        break;
    }
  }
}
 
 
uint32_t tcp_cb_client (int32_t socket, netTCP_Event event,
                        const NET_ADDR *addr, const uint8_t *buf, uint32_t len) {
 
  switch (event) {
    case netTCP_EventConnect:
      // Connect request received
      break;
 
    case netTCP_EventEstablished:
      // Connection established
      break;
 
    case netTCP_EventClosed:
      // Connection was properly closed
      break;
 
    case netTCP_EventAborted:
      // Connection is for some reason aborted
      break;
 
    case netTCP_EventACK:
      // Previously sent data acknowledged
      break;
 
    case netTCP_EventData:
      if(len==2){
				// server send symbol and times of repead
				uint8_t* t = (uint8_t*)malloc(sizeof(uint8_t)*buf[1]);
				for(uint8_t i=0;i<buf[1];i++)t[i]=buf[0];
				sendMessage(t);
			}
			
      break;
  }
  return (0);
}

int main (void) {
  netInitialize ();
  tcp_sock = netTCP_GetSocket(tcp_cb_client);
	NET_ADDR addr = { NET_ADDR_IP4, 2000,
                    192, 168, 0, 1 };
	connect(addr);
	while(1){};
}
