#ifndef __RTCONFIG_H
#define __RTCONFIG_H

#define RT_ALIGN_SIZE					4				/* 多少字节对齐 */

#define RT_THREAD_PRIORITY_MAX          32              /* 最大优先级 */
/* 线程就绪列表 */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

#endif /* __RTCONFIG_H */
