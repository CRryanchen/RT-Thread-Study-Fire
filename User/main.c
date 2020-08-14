/**
 * @file main.c
 * @brief �½�RT-Thread ����-�������
 * @author Ryan��Chen (ryan.cr.chen@gmail.com)
 * @version 1.0
 * @date 14-08-2020
 * 
 * @copyright Copyright (c) 2020  Ryan��Chen
 * 
 * @par ������־:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>14-08-2020 <td>1.0     <td>Ryan��Chen     <td>����
 * </table>
 */

/* ������ͷ�ļ� */
#include <rtthread.h>
//#include "ARMCM3.h"
#include "rtdef.h"
#include "rtconfig.h"
#include "rtthread.h"
#include "rtservice.h"



extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* ȫ�ֱ��� */
rt_uint8_t flag1;
rt_uint8_t flag2;

/* �߳̿��ƿ� & STACK & �߳����� */
// �����߳̿��ƿ�
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
ALIGN(RT_ALIGN_SIZE)
// �����߳�ջ
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
// �߳�����
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);


int main(void)
{
    /* Ӳ����ʼ�� */
    /* ��Ӳ����صĳ�ʼ�����������������������û����س�ʼ������ */

    /* ��ʼ����������� */
    /*
    1����ʼ�������б�
    2����ʼ����ǰ�߳̿��ƿ�ָ�� 
    */
    rt_system_scheduler_init();
    
    /* ��ʼ���߳�
    1��tlist��ʼ��
    2��entry��parameter��ʼ��
    3��stack_start��stack_size��ʼ��
    4���߳�ջ��ʼ���������Ĵ�����ռ��һ���ռ䣬����ָ�뷵�ظ�sp
     */
    rt_thread_init( &rt_flag1_thread,					/* �߳̿��ƿ� */
                    flag1_thread_entry,					/* �߳���ڵ�ַ */
                    RT_NULL,							/* �߳��β� */
                    &rt_flag1_thread_stack[0],			/* �߳�ջ��ʼ��ַ */
                    sizeof(rt_flag1_thread_stack)		/* �߳�ջ��С����λΪ�ֽ� */
    				);
    /* ���̲߳��뵽�����б� */
    rt_list_insert_before(&(rt_thread_priority_table[0]), &(rt_flag1_thread.tlist));
    
    /* ��ʼ���߳� */
    rt_thread_init( &rt_flag2_thread,					/* �߳̿��ƿ� */
                    flag2_thread_entry,					/* �߳���ڵ�ַ */
                    RT_NULL,							/* �߳��β� */
                    &rt_flag2_thread_stack[0],			/* �߳�ջ��ʼ��ַ */
                    sizeof(rt_flag2_thread_stack)		/* �߳�ջ��С����λΪ�ֽ� */
    				);
    /* ���̲߳��뵽�����б� */
    rt_list_insert_before(&(rt_thread_priority_table[1]), &(rt_flag2_thread.tlist));

    /* ������������� */
    rt_system_scheduler_start();
}

/*
 *********************************************************** 
 *                          ����ʵ��
 *********************************************************** 
 */
// �����ʱ
void delay(rt_uint32_t count)
{
    for (; count != 0; count--);
}

/* �߳�1 */
void flag1_thread_entry(void *p_arg)
{
    for (;;)
    {
        flag1 = 1;
        delay(10);
        flag1 = 0;
        delay(10);

        /* �߳��л����������ֶ��л� */
        rt_schedule();
    }
}

/* �߳�2 */
void flag2_thread_entry(void *p_arg)
{
    for (;;)
    {
        flag2 = 1;
        delay(10);
        flag2 = 0;
        delay(10);

        /* �߳��л����������ֶ��л� */
        rt_schedule();
    }
}