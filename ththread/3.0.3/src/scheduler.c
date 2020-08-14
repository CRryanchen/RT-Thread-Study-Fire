#include "rtdef.h"
#include "rtconfig.h"

// ȫ�ֱ���
struct rt_thread *rt_current_thread = RT_NULL;

/* ��֪һ���ṹ������ĳ�Ա�ĵ�ַ�����Ƴ��ýṹ����׵�ַ */
#define rt_container_of(ptr, type, member)            ((type *)((char *)(ptr) - (unsigned long)(&((type*)0)->member)))
#define rt_list_entry(node, type, member)              rt_container_of(node, type, member)	


/* ��ʼ��ϵͳ������ */
void rt_system_scheduler_init(void)
{
    register rt_base_t offset;
    
    /* �߳̾����б��ʼ�� */
    for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset++)
    {
        rt_list_init(&rt_thread_priority_table[offset]);
    }
    
    /* ��ʼ����ǰ�߳̿��ƿ�ָ�� */
    rt_current_thread = RT_NULL;
}

/* ����ϵͳ������ */
void rt_system_scheduler_start(void)
{
    register struct rt_thread *to_thread;
    
    /* �ֶ�ָ����һ�����е��߳� */
    to_thread = rt_list_entry(rt_thread_priority_table[0].next,
                              struct rt_thread,
                              tlist);
    rt_current_thread = to_thread;
    
    /* �л�����һ���̣߳��ú�����context_rvds.S��ʵ�֡�
       ��rthw.h����������ʵ�ֵ�һ���߳��л���
       ��һ����ຯ����C�ļ��е��õ�ʱ��������βΣ�
       ��ִ�е�ʱ��Ὣ�βδ��뵽COU�Ĵ���R0
    */
    rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
}

/* ϵͳ���� */
void rt_schedule(void)
{
    struct rt_thread *to_thread;
    struct rt_thread *from_thread;
    
    /* �����߳������л� */
    if ( rt_current_thread == rt_list_entry( rt_thread_priority_table[0].next,
                                            struct rt_thread,
                                            tlist ))
    {
        from_thread = rt_current_thread;
        to_thread = rt_list_entry( rt_thread_priority_table[1].next,
                                            struct rt_thread,
                                            tlist ));
        rt_current_thread = to_thread;
    }
    
    else
    {
        from_thread = rt_current_thread;
        to_thread = rt_list_entry( rt_thread_priority_table[0].next,
                                            struct rt_thread,
                                            tlist ));
        rt_current_thread = to_thread;
    }
    /* �����������л� */
    rt_hw_context_switch((rt_uint32_t)&from_thread->sp, (rt_uint32_t)&to_thread->sp);
}