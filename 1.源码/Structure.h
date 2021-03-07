#include<stdio.h>
#include<stdlib.h>
typedef int Status;
#define stack_init_size 200
#define stack_increment 10
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW 0
#define OK 1


typedef struct Passenger
{
	int InitialFloor;//[1,12]
	int TargetFloor;//[1,12]
	int Weight;//[40kg,99kg]
	int Patient;//[10s,99s]
	int InitialTime;
}Passenger;

typedef struct QNode
{
	Passenger people;
	struct QNode* next;
}QNode, * QueuePtr;

typedef struct LinkQueue
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

typedef struct Elevator
{
	int NowFloor;//起始楼层为1层
	int MaxFloor;
	int NowWeight;
	int MaxWeight;
	int NowWaitTime;
	int MaxWaitTime;//当前楼层等待时间
	int NowStayTime;
	int MaxStayTime;//无人使用的等待时间
	int NowPassengerNumber;
	int MaxPassengerNumber;
	int Mode; //0等待，1上升，2下降
	int WeightWarn;//1为报警，0为正常状态
	int TimeWarn;//1为报警，0为正常状态
	LinkQueue UpQueue;
	LinkQueue DownQueue;
}Elevator;

typedef struct Floor
{
	int UpNum;
	int DownNum;
	LinkQueue UpQueue;
	LinkQueue DownQueue;
}Floor;

Status InitQueue(LinkQueue* Q)
{
	(*Q).front = (QueuePtr)malloc(sizeof(QNode));
	if (!(*Q).front) exit(OVERFLOW);
	(*Q).front->next = NULL;
	return OK;
}

Status IsQueueEmpty(LinkQueue Q)
{
	if (Q.front == Q.rear)return OK;
	else return FALSE;
}

Status DestroyQueue(LinkQueue* Q)
{
	while ((*Q).front)
	{
		(*Q).rear = (*Q).front->next;
		free((*Q).front);
		(*Q).front = (*Q).rear;
	}
	return OK;
}

Status EnQueue(LinkQueue* Q, Passenger e)//将元素加到队尾
{
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (!p) exit(OVERFLOW);
	p->people = e;
	p->next = NULL;
	QueuePtr q = (*Q).front;
	while(q&&q->next!=NULL)
		q = q->next;
	q->next = p;
	return OK;
}

Status EnFront(LinkQueue* Q, Passenger e)//将元素加到队首
{
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (!p) exit(OVERFLOW);
	p->people = e;
	p->next = (*Q).front->next;
	(*Q).front->next = p;
	return OK;
}

Status DeQueue(LinkQueue* Q, Passenger* e)//将元素从队首删除
{
	QueuePtr p = (*Q).front->next;
	if(!p)
	{
		puts("DeQueue出错");
		return FALSE;
	}
	(*e) = p->people;
	(*Q).front->next = p->next;
	free(p);
	return OK;
}