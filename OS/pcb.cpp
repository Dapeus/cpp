#include<bits/stdc++.h>
using namespace std;

class PCB
{
public:
    // int id;
    string name;
    int status;
    int priority;
    double time; //单位ms
    // static queue<PCB> rq;
    // static queue<PCB> wq;

    //输出
    void print_pcb()
    {
        // cout<<this->name<<' '<<this->status<<endl;
        cout<<this->name<<" priority:"<<this->priority<<" status:"<<this->status<<" time:"<<this->time<<"ms"<<endl;   
    }
};

void excute(queue<PCB>& rq,queue<PCB> &wq)
{
    //每次执行时，生成随机数，若随机数为1时，将等待队列中的第一个PCB加入就绪队列的对尾
    if(rand()%2 == 1 && !wq.empty()) //防止wq已经为空的情况
    {
        cout<<"add a pcb to ready queue"<<endl;
        rq.push(wq.front());
        wq.pop();
    }
    //随机设定时间
    int time = rand()%50;
    if(time > rq.front().time){ //如果能够执行完,则打印ok，并将其移除就绪队列
        cout<<rq.front().name<<" is excuted"<<endl;
        rq.pop();
    }
    else{ //如果不能够执行完,则打印time out，并将其执行时间所需时间减去执行的时间,移除就绪队列，加入等待队列的末尾
        rq.front().time-=time;
        wq.push(rq.front());
        cout<<"time out,"<<rq.front().name<<" is not excuted"<<endl;
        rq.pop();
    }
}

void FIFO(queue<PCB> &rq,queue<PCB> &wq)
{
    excute(rq,wq);
}

void createPCB(PCB pcb[],queue<PCB> &rq,queue<PCB> &wq)
{
    string name = "pcb";
    
    for(int i = 0; i < 10; i++)
    {
        // pcb[i].id=i;
        pcb[i].name=name+char(i+'0');
        pcb[i].status = rand()%2;
        pcb[i].priority = rand()%10+1;
        pcb[i].time = rand()%50+1;

        if(pcb[i].status == 0) //如果处于就绪态，则加入就绪队列
            rq.push(pcb[i]);
        else
            wq.push(pcb[i]); //否则加入等待队列
    }
}   

void printPCB(PCB pcb[])
{
    for(int i = 0; i < 10; i++)
    {
        pcb[i].print_pcb();
    }
}

void print_queue(queue<PCB> rq,queue<PCB> wq)
{
    queue<PCB> rq_temp = rq;
    queue<PCB> wq_temp = wq;
    cout<<"ready queue:"<<endl;
    if(!rq_temp.empty())
        while(!rq_temp.empty())
        {
            rq_temp.front().print_pcb();
            rq_temp.pop();
        }
    else
        cout<<"null"<<endl;
    cout<<"wait queue:"<<endl;
    if(!wq_temp.empty())
        while(!wq_temp.empty())
        {
            wq_temp.front().print_pcb();
            wq_temp.pop();
        }
    else
        cout<<"null"<<endl;
}

int main()
{
    PCB *pcb = new PCB[10];
    queue<PCB> rq;
    queue<PCB> wq;

    createPCB(pcb,rq,wq);
    cout<<"pcb is created:"<<endl;
    printPCB(pcb);
    // cout<<rq.front().id<<endl;
    // cout<<wq.front().id<<endl;
    while(!rq.empty())
    {
        FIFO(rq,wq);
        print_queue(rq,wq);
    }
    system("pause");
    return 0;
}