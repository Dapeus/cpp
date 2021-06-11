#include<bits/stdc++.h>
using namespace std;

class Table_u //已分配区表
{
public:
    float address; //已分分区起始地址
    float length; //已分分区长度，单位为字节
    int flag; //已分配表区登记栏标志，用0表示空栏目
};
    
class Table_f //空闲区表
{
public:
    float address; //空闲区起始地址
    float length; //空闲区长度，单位为字节
    int flag; //空闲区表登记栏目用0表示空栏目，1表示未分配
};       

void initTable(Table_u *&used_table, Table_f *&free_table,int n) //初始化空闲表和已分配表
{
    int t = 512;
    used_table[0].flag = 0;
    used_table[0].length = 0;
    used_table[0].address = 0;

    free_table[0].flag = 1;
    free_table[0].length = 1024*1024/t;
    free_table[0].address = 0;
    // cout<<free_table[0].address<<endl;
    t/=2;
    for(int i = 1; i < n-1; i++,t/=2)
    {
        used_table[i].flag = 0;
        used_table[i].length = 0;
        used_table[i].address = 0;

        free_table[i].flag = 1;
        free_table[i].length = 1024*1024/t;
        free_table[i].address = free_table[i-1].length+free_table[i-1].address;
        // cout<<free_table[i].address<<endl;
    }
    used_table[9].flag = 0;
    used_table[9].length = 0;
    used_table[9].address = 0;

    free_table[9].flag = 1;
    free_table[9].length = 2048;
    free_table[9].address = free_table[8].length+free_table[8].address;
}

//作业
class JOB{
public:
    float length;   //作业所需内存
    int id;
    int flag;
};

void initJob(JOB *&job,int n)
{
    for(int i = 0; i < n; i++)
    {
        job[i].id = i+1;
        job[i].length = rand() / double(RAND_MAX)*512*1024;
        job[i].flag = 0;
        cout<<job[i].length<<endl;
    }
}

void printJob(queue<JOB> job)
{
    queue<JOB> j = job;
    while(!j.empty())
    {
        cout<<j.front().id<<",";
        j.pop();
    }
    cout<<endl;
}

void usedPrint(Table_u *used_table, int n)
{
    cout<<"used_table:"<<endl;
    for(int i = 0; i < n; i++)
    {
        cout<<used_table[i].address<<"--"<<used_table[i].length<<"--"<<used_table[i].flag<<endl;
    }
    cout<<"-----------"<<endl;
}

void freePrint(Table_f *free_table, int n)
{
    cout<<"free_table:"<<endl;
    for(int i = 0; i < n; i++)
    {
        cout<<free_table[i].address<<"--"<<free_table[i].length<<"--"<<free_table[i].flag<<endl;
    }
    cout<<"------------"<<endl;
}

void assignJob(Table_u *&used_table,Table_f *&free_table,queue<JOB>&job, int n)
{
    queue<JOB> temp;
    int l = job.size();
    
    for(int i = 0; i < n; i++)
    {
        if(job.empty())
        {
            return ;
        }
        int jud=0;
        for(int m = 0; m < n; m++)
        {
            if(free_table[m].length > job.front().length)
            {
                for(int j = 0; j < n; j++)
                {
                    if(used_table[j].flag == 0)
                    {
                        used_table[j].address = free_table[m].address;
                        used_table[j].length = job.front().length;
                        used_table[j].flag = job.front().id;
                        break;
                    }
                }
                free_table[m].address += job.front().length;
                free_table[m].length -= job.front().length;
                // free_table[m].flag = 1;
                job.pop();
                jud = 1;
                break;
            }
            else if(free_table[m].length == job.front().length)
            {
                for(int j = 0; j < n; j++)
                {
                    if(used_table[j].flag == 0)
                    {
                        used_table[j].address = free_table[m].address;
                        used_table[j].length = job.front().length;
                        used_table[j].flag = job.front().id;
                        break;
                    }
                }
                free_table[m].length -= job.front().length;
                free_table[m].flag = 0;
                job.pop();
                jud = 1;
                break;
            }
        }
        if(!jud)
        {
            job.push(job.front());
            job.pop();
        }
    }
    printJob(job);
    usedPrint(used_table,n);
    freePrint(free_table,n);
}

void recycle(Table_u *&used_table,Table_f *&free_table,queue<JOB> job, int n)
{
    int cnt=0;
    for(int i = 0; i < n; i++)
    {
        if(used_table[i].flag > 0)
        {
            cnt++;
            srand(time(NULL));
            int ran = rand()%2;
            if(ran)
            {
                int j = 0,j_f = 0;
                int k = 0,k_f = 0;
                for(; j < n; j++)
                {
                    if(free_table[j].address+free_table[j].length == used_table[i].address && free_table[j].flag == 1)
                    {
                        // cout<<"aaaa"<<endl;
                        j_f = 1;
                        break;
                    }
                }
                for(; k < n; k++)
                {
                    if(free_table[k].address == used_table[i].address+used_table[i].length && free_table[k].flag == 1)
                    {
                        // cout<<"bbbb"<<endl;
                        k_f = 1;
                        break;
                    }
                }
                
                if(j_f && k_f)
                {
                    free_table[j].length += free_table[k].length+used_table[i].length;
                    // free_table[k].address = 0;
                    free_table[j].flag = 1;
                    free_table[k].length = 0;
                    free_table[k].flag = 0;
                }
                else if(j_f && !k_f)
                {
                    free_table[j].length += used_table[i].length;
                    free_table[j].flag = 1;
                }
                else if(!j_f && k_f)
                {
                    free_table[k].address = used_table[i].address;
                    free_table[k].length += used_table[i].length;
                    free_table[k].flag = 1;
                }
                else
                {
                    for(int l = 0; l < n; l++)
                    {
                        if(free_table[l].flag == 0)
                        {
                            free_table[l].address = used_table[i].address;
                            free_table[l].length = used_table[i].length;
                            free_table[l].flag = 1;
                            break;
                        }
                    }
                }
                used_table[i].address=0;
                used_table[i].length=0;
                used_table[i].flag=0;
            }
        }
        }
    usedPrint(used_table,n);
    freePrint(free_table,n);
    cout<<"cnt:"<<cnt<<endl;
}

int main()
{
    int n = 10;
    srand(time(NULL)); //随机数种子
    JOB *job = new JOB[10];
    Table_u *used_table = new Table_u[10];
    Table_f *free_table = new Table_f[10];
    queue<JOB> j;
    initJob(job,n);
    //作业队列
    for(int i = 0; i < n; i++)
        j.push(job[i]);
    //初始化空闲表和已分配表
    initTable(used_table,free_table,n);
    freePrint(free_table,n);
    while(!j.empty())
    {
        //分配任务
        assignJob(used_table,free_table,j,n);
        //回收
        recycle(used_table,free_table,j,n);
    }
    cout<<"job size:"<<j.size()<<endl;
    freePrint(free_table,n);
    system("pause");
    return 0;
}