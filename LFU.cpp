#include<iostream>
#include<unordered_map>
using namespace std;

struct Node 
{
    int key;
    int value;
    int freq;
    Node* next;
    Node* prev;
    Node(int k,int v)
    {
        //cout<<k<<endl<<v<<endl;
        key=k;
        value=v;
        freq=1;
        next=NULL;
        prev=NULL;
    }
};

class List{
    public:
    Node* head;
    Node* tail;
    int sz;
    List()
    {
        head=NULL;
        tail=NULL;
        sz=0;
    }
    void insert_end(Node* cur)
    {
        //cout<<"end append"<<endl;
        if(head==NULL && tail==NULL)
        {
            //cout<<"Here"<<endl;
            head=cur;
            tail=cur;
            cur->prev=NULL;
            cur->next=NULL;
        }
        else
        {
            tail->next=cur;
            cur->prev=tail;
            cur->next=NULL;
            tail=cur;
        }
        this->sz++;
    }
    int delete_head()
    {
        int num=head->key;
        if(head==tail)
        {
            head=NULL;
            tail=NULL;
            this->sz=0;
            return num;   
        }
        head=head->next;
        head->prev=NULL;
        this->sz--;
        return num;
    }
    void delete_inter(Node* x)
    {
        if(x==head)
        {
            int num=delete_head();
            return;
        }
        x->prev->next=x->next;
        if(x->next==NULL)
            tail=x->prev;
        else
            x->next->prev=x->prev;
        this->sz--;
    }
};
class LFUCache{
    int capacity;
    int cur_sz;
    unordered_map <int,Node*> cache;
    unordered_map <int,List*> freq_lst;
    int min_freq;
    public:
        LFUCache(int cap)
        {
            capacity=cap;
            cur_sz=0;
            min_freq=0;
        }
        int get(int key)
        {
            if(capacity==0)
                return -1;
            if(cache.find(key)==cache.end())
                return -1;
            //cout<<"get"<<endl;
            int v=cache[key]->value;
            cache[key]->freq+=1;
            //cout<<"Updated freq="<<cache[key]->freq<<endl;
            update_freq(cache[key],cache[key]->freq);
            return v;
        }
        void set(int key,int value)
        {
            if(capacity==0)
            {
                cout<<"The capacity of the Cache is 0"<<endl;
                return;
            }
            if(cache.find(key)!=cache.end())//Update the value of existing key in the Cache
            {
                //cout<<"Update elem"<<endl;
                cache[key]->value=value;
                cache[key]->freq+=1;
                update_freq(cache[key],cache[key]->freq);
            }
            else if(cur_sz==capacity)//Delete the LFU value and insert a new one when the capacity is full
            {
                //cout<<"New elem and full cap"<<endl;
                int num=0;
                num=delete_lfu();
                cache.erase(num);
                Node* newnode=new Node(key,value);
                cache[key]=newnode;
                update_freq(cache[key],1);
            }
            else//insert a new key value when the capacity is not full
            {
                //cout<<"New elem"<<endl;
                Node* newnode=new Node(key,value);
                cache[key]=newnode;
                update_freq(cache[key],1);
                cur_sz++;
            }
        }
        void update_freq(Node* cur,int cur_freq)
        {
            //cout<<"update_freq"<<endl;
            if(cur_freq<min_freq || cur_freq==1)
                min_freq=cur_freq;
            else if(cur_freq-1==min_freq && freq_lst[cur_freq-1]->sz==1)
            {
                min_freq++;
            }
            //cout<<"min_freq="<<min_freq<<endl;
            if(cur_freq>1)
            {
                    freq_lst[cur_freq-1]->delete_inter(cur);
                    cur->next=NULL;
                    if(freq_lst[cur_freq-1]->sz==0)
                        freq_lst.erase(cur_freq-1);
            }
            if(freq_lst.find(cur_freq)==freq_lst.end())
            {
                //cout<<"New lst"<<endl;
                List* newlst=new List();
                newlst->insert_end(cur);
                freq_lst[cur_freq]=newlst;
            }
            else
            {
                //cout<<"Old lst"<<endl;
                freq_lst[cur_freq]->insert_end(cur);
            }
        }
        int delete_lfu()
        {
            int k=freq_lst[min_freq]->delete_head();
            if(freq_lst[min_freq]->sz==0)
                freq_lst.erase(min_freq);
            return k;
        }
        void print()
        {   
            if(capacity==0)
                return;
            for(auto iter=freq_lst.begin();iter!=freq_lst.end();iter++)
            {
                cout<<"Freq="<<iter->first<<endl;
                Node* temp=iter->second->head;
                while(temp!=NULL)
                {
                    cout<<temp->key<<" ";
                    temp=temp->next;
                }                
                cout<<endl;
            }
            Node* temp=freq_lst[min_freq]->head;
            cout<<"Min frequency="<<min_freq<<endl;
            while(temp!=NULL)
            {
                cout<<temp->key<<" ";
                temp=temp->next;
            }
            /*for(auto iter=cache.begin();iter!=cache.end();iter++)
            {
                cout<<iter->second->key<<" ";
                cout<<iter->first<<" ";
            }*/
        }
};
int main()
{
    int ch=1;
    LFUCache* Cache1;
    //cout<<"The options are"<<endl<<"1.Create Cache"<<endl<<"2.Get data"<<endl<<"3.Set data"<<endl;
    //cout<<"4.Print Content"<<endl;
    while(ch>0&&ch<5)
    {
        //cout<<"Enter your choice"<<endl;
        cin>>ch;
        if(ch==1)
        {
            int cap;
            cin>>cap;
            LFUCache* temp=new LFUCache(cap);
            Cache1=temp;
        }
        else if(ch==2)
        {
            int key;
            cin>>key;
            cout<<Cache1->get(key)<<endl;
        }
        else if(ch==3)
        {
            int key,val;
            cin>>key>>val;
            Cache1->set(key,val);
        }
        else if(ch==4)
        {
            Cache1->print();
        }
    }
    return 0;
}