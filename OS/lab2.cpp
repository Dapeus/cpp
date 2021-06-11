#include<bits/stdc++.h>
using namespace std;

class PCB
{
public:
    int type; //producer-0,consumer-1
    int sysid; 
    int status;
    string product;

    PCB(int type, int sysid):type(type),sysid(sysid)
    {}

    int execute(int *&buffer, int bs) //return 0->over,1->wait
    {
        if(this->type == 0) //producer
        {
            int i = 0,jud = 0;
            for(; i < bs; i++)
            {
                if(buffer[i] == -1)
                {
                    jud = 1;
                    break;
                }
            }
            if(jud == 1)
            {
                cout<<"product ok"<<endl;
                buffer[i] = this->sysid;
                return 0;
            }
            else
            {
                cout<<"product fail"<<endl;
                return 1;
            }
        }
        else { //consumer
            int i = bs-1,jud = 0;
            for(; i >= 0; i--)
            {
                if(buffer[i] != -1)
                {
                    jud = 1;
                    break;
                }
            }
            if(jud == 1)
            {
                cout<<"consume ok"<<endl;
                buffer[i] = -1;
                return 0;
            }
            else
            {
                cout<<"consume fail"<<endl;
                return 1;
            }
        }
    }
};

void schedule(queue<PCB> &producer,queue<PCB> &consumer,queue<PCB> &ready,list<PCB> &over,int* &buffer,int bs)
{
    PCB p = ready.front();
    cout<<"begin schedule:"<<p.sysid<<" ";
    int t = p.execute(buffer, bs);
    if(t == 0)
    {
        ready.pop();
        if(p.type == 0)
        {
            over.push_back(p);
            if(!consumer.empty())
            {
                ready.push(consumer.front());
                consumer.pop();
            }
        }
        else
        {
            over.push_back(p);
            if(!producer.empty())
            {
                ready.push(producer.front());
                producer.pop();
            }
        }
    }
    else
    {
        ready.pop();
        if(p.type == 0)
        {
            producer.push(p);
            
        }
        else
        {
            consumer.push(p);
        }
    }
    
}

void createPCB(queue<PCB> &ready)
{
    int num;
    cout<<"please input the PCB num:";
    cin>>num;
    int type;
    for(int i = 0; i < num; i++)
    {
        type = rand()%2;
        // cin>>type;
        PCB pcb = PCB(type,i);
        ready.push(pcb);
    }
    cout<<"all PCB are created"<<endl;
}

int initBuffer(int *&buffer)
{
    cout<<"input buffer size:";
    int bs;
    cin>>bs;
    buffer = (int*)malloc(32*bs);
    for(int i = 0; i < bs; i++) //初始化buffer为-1
    {
        buffer[i] = -1;
        // cout<<buffer[i]<<endl;
    }
    return bs;
}

int main()
{
    srand(time(NULL));//随机数种子
    list<PCB> over;
    queue<PCB> producer,consumer,ready;
    
    char q = 'y';
    while(q == 'y')
    {
        int *buffer;
        int bs = initBuffer(buffer);
        createPCB(ready);
        while(!ready.empty())
        {
            schedule(producer,consumer,ready,over,buffer,bs);
        }
        cout<<"over list:"<<endl;
        while(!over.empty())
        {
            cout<<"over id:"<<over.front().sysid<<endl;
            over.pop_front();
        }
        cout<<"producer wait queue:"<<endl;
        while(!producer.empty())
        {
            cout<<"producer id:"<<producer.front().sysid<<endl;
            producer.pop();
        }
        cout<<"consumer wait queue:"<<endl;
        while(!consumer.empty())
        {
            cout<<"consumer id:"<<consumer.front().sysid<<endl;
            consumer.pop();
        }
        cout<<"continue(y/n):";
        cin>>q;
    }
    return 0;
}