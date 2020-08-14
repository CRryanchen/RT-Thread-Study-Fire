;**********************************************************
;                   ȫ�ֱ���
;**********************************************************
    IMPORT rt_thread_switch_interrupt_flag
    IMPORT rt_interrupt_from_thread
    IMPORT rt_interrupt_to_thread
;**********************************************************
;                   ����
;**********************************************************
;----------------------------------------------------------
;�й�ϵͳ�ں�����Ĵ�������ɲο��ٷ��ĵ���STM32F10xx Cortex-M3 programing manual
;ϵͳ���ƿ�����SCB��ַ��Χ��0XE000ED00~0XE000ED3F
;----------------------------------------------------------
SCB_VTOR		EQU			0XE000ED08			; ������ƫ�ƼĴ���
NVIC_INT_CTRL	EQU			0XE000ED04			; �жϿ���״̬�Ĵ���
NVIC_SYSPRI2	EQU			0XE000ED20			; ϵͳ���ȼ��Ĵ���(2)
NVIC_PENDSV_PRI	EQU			0X00FF0000			; PendSV ���ȼ�ֵ��lowest��
NVIC_PENDSVSET	EQU			0X10000000			; ����pendSV exception��ֵ

;**********************************************************
;                   �������ָ��
;**********************************************************
	AREA | .text |, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8


;/*
; *----------------------------------------------------------------------------
; * void rt_hw_context_switch(rt_uint32 from, rt_uint32 to)
; * r0 --> from
; * r1 --> to
; *----------------------------------------------------------------------------
; */
rt_hw_context_switch		PROC
	EXPORT	rt_hw_context_switch
	
	; �����жϱ�־λ rt_thread_switch_interrupt_flag Ϊ1
	; ���� rt_thread_switch_interrupt_flag �ĵ�ַ��r2
	LDR r2, =rt_thread_switch_interrupt_flag
	; ���� rt_thread_switch_interrupt_flag ��ֵ��r3
	LDR r3, [r2]
	; r3��1�Ƚϣ������ִ��BEQָ�����ִ��
	CMP r3, #1
	BEQ _reswitch
	; ����r3��ֵ
	MOV r3, #1
	; ��r3��ֵ�洢�� rt_thread_switch_interrupt_flag ,����1
	STR r3, [r2]
	
	; ���� rt_interrupt_from_thread ��ֵ
	; ���� rt_interrupt_from_thread �ĵ�ַ��r2
	LDR r2, =rt_interrupt_from_thread
	; �洢r0��ֵ�� rt_interrupt_from_thread������һ���߳�ջָ��sp��ָ��
	STR r0, [r2]
	
_reswitch
	; ���� rt_interrupt_to_thread ��ֵ
	; ���� rt_interrupt_to_thread �ĵ�ַ��r2
	LDR r2, =rt_interrupt_to_thread
	; �洢r1��ֵ�� rt_interrupt_to_thread������һ���߳�ջָ��sp��ָ��
	STR r1, [r2]
	
	; ���� PendSV �쳣�� ʵ���������л�
	LDR			r0, =NVIC_INT_CTRL
	LDR			r1, =NVIC_PENDSVSET
	STR			r1, [r0]
	; �ӳ��򷵻�
	BX LR
	; �ӳ������
	ENDP



;/*
; *------------------------------------------------------------------------
; * void PendSV_Handler();
; * r0 -- > switch from thread stack
; * r1 --> switch to thread stack
; * psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
; *------------------------------------------------------------------------
; */

PendSV_Handler		PROC
    EXPORT PendSV_Handler
	; ʧ���жϣ�Ϊ�˱����������л������ж�
    MRS		r2, PRIMASK
    CPSID	I
    
    ; ��ȡ�жϱ�־λ�������Ƿ�Ϊ0
    ; ���� rt_thread_switch_interrupt_flag �ĵ�ַ��r0
    LDR r0, =rt_thread_switch_interrupt_flag
    ; ���� rt_thread_switch_interrupt_flag ��ֵ��r1
    LDR r1, [r0]
    ; �ж�r1�Ƿ�Ϊ0��Ϊ0����ת��pendsv_exit
    CBZ r1, pendsv_exit
        
    ; r1��Ϊ0����0
    MOV r1, #0x00		
    ; ��r1��ֵ�洢�� rt_thread_switch_interrupt_flag��������
    STR r1, [r0]
    
    ; �ж� rt_interrupt_from_thread ��ֵ�Ƿ�Ϊ0
    ; ���� rt_interrupt_from_thread �ĵ�ַ��r0
    LDR r0, =rt_interrupt_from_thread
    ; ���� rt_interrupt_from_thread ��ֵ��r1
    LDR r1, [r0]
    ; �ж�r1�Ƿ�Ϊ0��Ϊ0����ת�� switch_to_thread
    ; ��һ���߳��л�ʱ rt_interrupt_from_thread �϶�Ϊ0������ת��switch_to_thread
    CBZ r1, switch_to_thread
        
; ============================= ���ı��� ==============================
	; ������PendSVC_Handlerʱ����һ���߳����еĻ�������
    ; xPSR,PC���߳���ڵ�ַ��,R14,R12,R3,R2,R1,R0���̵߳��βΣ�
    ; ��ЩCPU�Ĵ�����ֵ���Զ����浽�̵߳�ջ�У�ʣ�µ�r4~r11��Ҫ�ֶ�����
    ; ��ȡ�߳�ջָ�뵽r1
    MRS r1, psp
    ; ��CPU�Ĵ���r4~r11��ֵ�洢��r11ָ��ĵ�ַ��ÿ����һ�ε�ַ�ݼ�һ�Σ�
    STMFD r1!, {r4-r11}
    ; ���� r0 ֵָ��r0����r0=rt_interrupt_from_thread
    LDR r0, [r0]
    ; ��r1��ֵ�洢��r0���������߳�ջsp
    STR r1, [r0]
        
; ============================= �����л� ==============================
switch_to_thread
    ; ���� rt_interrupt_to_thread �ĵ�ַ��r1
    ; rt_interrupt_to_thread ��һ��ȫ�ֱ��������������߳�ջָ��sp��ָ��
    LDR r1, =rt_interrupt_to_thread
    ; ����rt_interrupt_to_thread��ֵ��r1����spָ���ָ��
    LDR r1, [r1]
    ; ���� rt_interrupt_to_thread ��ֵ��r1����sp
    LDR r1, [r1]
    
    ; ���߳�ջָ��r1������֮ǰ�ȵݼ���ָ������ݼ��ص�CPU�Ĵ���r4~r11
    LDMFD r1!, {r4 - r11}
	; ���߳�ջָ����µ�psp
    MSR psp, r1

pendsv_exit								
    ; �ָ��ж�
    MSR PRIMASK, r2					
    
    ; ȷ���쳣����ʹ�õ�ջָ����sp����LR�Ĵ�����λ2ҪΪ1
    ORR lr, lr, #0x04
    ; �쳣���أ����ʱ��ջ�е�ʣ�����ݻ��Զ����ص�CPU�Ĵ�����
    ; xPSR,PC���߳���ڵ�ַ��,R14,R12,R3,R2,R1,R0���̵߳��βΣ�
    ; ͬʱ PSP��ֵҲ����£���ִ���߳�ջ��ջ��
    BX lr
    
    ; PendSV_Handler �ӳ������
    ENDP



;/*
; *--------------------------------------------------------
; * ����ԭ�ͣ�void rt_hw_context_switch_to(rt_uint32 to)
; * r0 --> to
; * �ú������ڿ�����һ���߳��л�
; *--------------------------------------------------------
;/*
rt_hw_context_switch_to		PROC

	; ���� rt_hw_context_switch_to ,�������ȫ�����ԣ�������C�ļ�����
	EXPORT rt_hw_context_switch_to
	
	; ���� rt_interrupt_to_thread ��ֵ
	; �� rt_interrupt_to_thread �ĵ�ַ���ص�r1
	LDR			r1, =rt_interrupt_to_thread
	; �� r0 ��ֵ�洢�� rt_interrupt_to_thread			
	STR			r0, [r1]
	
	; ���� rt_interrupt_from_thread ��ֵΪ0����ʾ������һ���߳��л�
	; �� rt_interrupt_from_thread �ĵ�ַ���ص�r1
	LDR			r1, =rt_interrupt_from_thread
	; ����r0����0
	MOV			r0, #0x0
	; ��r0��ֵ�洢�� rt_interrupt_from_thread
	STR 		r0,	[r1]
	
	; �����жϱ�־λ rt_thread_switch_interrupt_flag ��ֵΪ1
	; �� rt_thread_switch_interrupt_flag �ĵ�ַ���ص�r1
	LDR			r1, =rt_thread_switch_interrupt_flag
	; ����r0����1
	MOV			r0, #1
	; ��r0��ֵ�洢�� rt_thread_switch_interrupt_flag
	STR			r0, [r1]
	
	; ���� PendSV �쳣�����ȼ�
	LDR			r0, =NVIC_SYSPRI2
	LDR			r1, =NVIC_PENDSV_PRI
	LDR.W		r2, [r0, #0x00]			; ��
	ORR			r1,r1,r2				; ��
	STR			r1, [r0]				; д
	
	; ���� PendSV �쳣�������������л���
	LDR			r0, =NVIC_INT_CTRL
	LDR			r1, =NVIC_PENDSVSET
	STR			r1, [r0]
	
	; ���ж�
	CPSIE 	F
	CPSIE	I
	
	; ��Զ���ᵽ������
	ENDP
	
	ALIGN	4
	
	END