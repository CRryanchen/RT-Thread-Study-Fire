#ifndef __RTCONFIG_H
#define __RTCONFIG_H

#define RT_ALIGN_SIZE					4				/* �����ֽڶ��� */

#define RT_THREAD_PRIORITY_MAX          32              /* ������ȼ� */
/* �߳̾����б� */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

#endif /* __RTCONFIG_H */
