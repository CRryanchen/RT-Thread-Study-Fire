/**
 * @file main.c
 * @brief 新建RT-Thread 工程-软件仿真
 * @author Ryan·Chen (ryan.cr.chen@gmail.com)
 * @version 1.0
 * @date 14-08-2020
 * 
 * @copyright Copyright (c) 2020  Ryan·Chen
 * 
 * @par 更改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>14-08-2020 <td>1.0     <td>Ryan·Chen     <td>内容
 * </table>
 */

/* 包含的头文件 */
#include <rtthread.h>
//#include "ARMCM3.h"
#include "rtdef.h"
#include "rtconfig.h"
#include "rtthread.h"
#include "rtservice.h"



extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* 全局变量 */
rt_uint8_t flag1;
rt_uint8_t flag2;

/* 线程控制块 & STACK & 线程声明 */
// 定义线程控制块
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
ALIGN(RT_ALIGN_SIZE)
// 定义线程栈
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
// 线程声明
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);


int main(void)
{
    /* 硬件初始化 */
    /* 将硬件相关的初始化放在这里，如果是软件仿真则没有相关初始化代码 */

    /* 初始化任务调度器 */
    /*
    1、初始化就绪列表
    2、初始化当前线程控制块指针 
    */
    rt_system_scheduler_init();
    
    /* 初始化线程
    1、tlist初始化
    2、entry，parameter初始化
    3、stack_start、stack_size初始化
    4、线程栈初始化，保留寄存器会占据一定空间，并将指针返回给sp
     */
    rt_thread_init( &rt_flag1_thread,					/* 线程控制块 */
                    flag1_thread_entry,					/* 线程入口地址 */
                    RT_NULL,							/* 线程形参 */
                    &rt_flag1_thread_stack[0],			/* 线程栈起始地址 */
                    sizeof(rt_flag1_thread_stack)		/* 线程栈大小，单位为字节 */
    				);
    /* 将线程插入到就绪列表 */
    rt_list_insert_before(&(rt_thread_priority_table[0]), &(rt_flag1_thread.tlist));
    
    /* 初始化线程 */
    rt_thread_init( &rt_flag2_thread,					/* 线程控制块 */
                    flag2_thread_entry,					/* 线程入口地址 */
                    RT_NULL,							/* 线程形参 */
                    &rt_flag2_thread_stack[0],			/* 线程栈起始地址 */
                    sizeof(rt_flag2_thread_stack)		/* 线程栈大小，单位为字节 */
    				);
    /* 将线程插入到就绪列表 */
    rt_list_insert_before(&(rt_thread_priority_table[1]), &(rt_flag2_thread.tlist));

    /* 启动任务调度器 */
    rt_system_scheduler_start();
}

/*
 *********************************************************** 
 *                          函数实现
 *********************************************************** 
 */
// 软件延时
void delay(rt_uint32_t count)
{
    for (; count != 0; count--);
}

/* 线程1 */
void flag1_thread_entry(void *p_arg)
{
    for (;;)
    {
        flag1 = 1;
        delay(10);
        flag1 = 0;
        delay(10);

        /* 线程切换，这里是手动切换 */
        rt_schedule();
    }
}

/* 线程2 */
void flag2_thread_entry(void *p_arg)
{
    for (;;)
    {
        flag2 = 1;
        delay(10);
        flag2 = 0;
        delay(10);

        /* 线程切换，这里是手动切换 */
        rt_schedule();
    }
}