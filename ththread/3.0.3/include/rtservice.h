#ifndef __RTSERVICE_H
#define __RTSERVICE_H

// 头文件
#include "rtdef.h"

// 初始化rt_list_t类型的节点
rt_inline void rt_list_init(rt_list_t *l)
{
    l->next = l->prev = l;
}

/* 在双向链表头部插入一个节点 */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t * n)
{
    l->next->prev = n;
    n->next = l->next;
    
    l->next = n;
    n->prev = l;
}

/* 在双向链表表头前面插入一个节点 */
rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t * n)
{
    l->prev->next = n;
    n->prev = l->prev;
    
    l->prev = n;
    n->next = l;
}

/* 从双向链表删除一个节点 */
rt_inline void rt_list_insert_remove(rt_list_t * n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    
    n->next = n->prev = n;
}

#endif /* __RTSERVICE_H */
