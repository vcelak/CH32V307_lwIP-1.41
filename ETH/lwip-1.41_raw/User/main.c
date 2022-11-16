/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "debug.h"
#include "Jubei_eth_driver.h"
#include "lwip/tcp.h"
#include "string.h"
/* Global typedef */

/* Global define */

/* Global Variable */

static void echo_client_init(void);

//������ʧ�ܺ� ����echo_client_init���½����ͻ��ˣ����¿�ʼ����
static void echo_client_conn_err(void *arg,err_t err)
{
    printf("connect error! closed by core!!\n");
    printf("try to connect to sever again!!\n");
    echo_client_init();
}

//�����յ������������ݺ�����������ں˵��ã�p�а����˷���������
static err_t echo_client_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
    if(p!=NULL)
    {
        tcp_recved(pcb, p->tot_len);     //���½��մ���
        tcp_write(pcb, p->payload, p->len, 1);   //���Է�����������
        pbuf_free(p);
    }else if(err == ERR_OK)   //����������Ͽ�������
    {
        tcp_close(pcb);      //�ȶϿ���������������
        echo_client_init();  //�ٴν����µ�����
        return ERR_OK;
    }
    return ERR_OK;
}

//���ͻ�����������������ݳɹ�������������ں˵���
static err_t echo_client_sent(void *arg,struct tcp_pcb *pcb,uint16_t len)
{
    printf("echo client send data OK! sent len = %d\n",len);
    return ERR_OK;
}

//���ͻ���ͬ���������ӳɹ�������������ں˵���
static err_t echo_client_connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
    char GREENTING[] = "Hi,I am a new Client!\n";

    tcp_recv(pcb,echo_client_recv);        //ע��recv����
    tcp_sent(pcb,echo_client_sent);        //ע��sent����
    tcp_write(pcb,GREENTING,sizeof(GREENTING),1);     //�����������GREETING
    return ERR_OK;
}

//�ͻ��˳�ʼ����������Ҫ���ں˳�ʼ�������һ��
static void echo_client_init(void)
{
    struct tcp_pcb *pcb = NULL;
    struct ip_addr server_ip;
    pcb = tcp_new();
    IP4_ADDR(&server_ip,192,168,1,11);
    tcp_bind(pcb, IP_ADDR_ANY, 20);
    tcp_connect(pcb,&server_ip,5,echo_client_connected);  //���ӷ�����21�˿�
    tcp_err(pcb,echo_client_conn_err);                    //ע�����Ӵ���ʱ�Ļص�����
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART_Printf_Init(115200);

	printf("SystemClk:%d\r\n",SystemCoreClock);

	printf("This is printf example\r\n");
	LwIP_Init();
    echo_client_init();

	while(1)
    {
     sys_check_timeouts();
	}
}

