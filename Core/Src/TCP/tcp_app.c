#include "tcp_app.h"
#include "lwip/tcp.h"
#include "string.h"


typedef enum {
    TCP_SERVER_OK   = 0,
    TCP_SERVER_ERR  = 1,
} tcpStatus_t;

/* Global Threads Ids */
osThreadId_t TcpTaskHandle;

/* Global Threads Attributes */
const osThreadAttr_t TcpTask_attributes = {
  .name = "TCP Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Task Prototypes */
tcpStatus_t tcpServer_Init();
static err_t tcpServer_AcceptCallback(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

void StartTcpTask(void *argument) {
    
    tcpServer_Init();
    while (1) {
        osDelay(200);
    }
}

#define TCP_SERVER_PORT 5000
#define RECV_BUFFER_SIZE 128


tcpStatus_t tcpServer_Init() {
    // tcpStatus_t ret = TCP_SERVER_ERR;
    struct tcp_pcb *server_pcb;


    server_pcb = tcp_new();
    if (server_pcb == NULL) {
        return TCP_SERVER_ERR;
    }
    else {
        err_t err = tcp_bind(server_pcb, IP_ADDR_ANY, TCP_SERVER_PORT);
        if (err == ERR_OK) {
            server_pcb = tcp_listen(server_pcb);
            tcp_accept(server_pcb, tcpServer_AcceptCallback);
        } 
        else {
            tcp_close(server_pcb);
        }
    }
    return TCP_SERVER_OK;
}

static err_t tcpServer_AcceptCallback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_setprio(newpcb, TCP_PRIO_MIN);
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
     if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Przetwarzanie danych
    char recv_buf[RECV_BUFFER_SIZE] = {0};
    pbuf_copy_partial(p, recv_buf, p->len, 0);
    recv_buf[p->len] = '\0';

    // Można odesłać dane z powrotem
    tcp_write(tpcb, recv_buf, strlen(recv_buf), TCP_WRITE_FLAG_COPY);

    tcp_recved(tpcb, p->len);
    pbuf_free(p);
    return ERR_OK;
}