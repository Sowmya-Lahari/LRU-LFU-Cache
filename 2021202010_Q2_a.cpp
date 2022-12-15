#include<iostream>
#include<unordered_map>
using namespace std;

struct Node 
{
    int key;
    int value;
    Node* next;
    Node* prev;
    Node(int k,int v)
    {
        key=k;
        value=v;
        next=NULL;
        prev=NULL;
    }
};
class LList{
    public:
    Node* head;
    Node* tail;
    LList()//Constructor of Linked List
    {
        head=NULL;
        tail=NULL;
    }
    void delete_inter(Node* x)//Deletes a node
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
    }
    Node* insert(int k,int v)//Inserts a node at the end
    {
        Node* nd=new Node(k,v);
        if(head==NULL)
        {
            head=nd;
            tail=nd;
        }
        else
        {
            tail->next=nd;
            nd->prev=tail;
            tail=nd;
        }
        return nd;
    }
    int delete_head()//Deletes the node from the start
    {
        int num=0;
        num=head->key;
        if(head==tail)
        {
            head=NULL;
            tail=NULL;
            return num;
        }
        head=head->next;
        head->prev=NULL;
        return num;
    }
};

class LRUCache{
    int capacity;
    int cur_sz;
    unordered_map <int,Node*> cache;
    LList* queue;
    public:
    LRUCache(int cap)//Constructor to create cache
    {
        capacity=cap;
        cur_sz=0;
        LList* temp=new LList();//Linked list implementation of Queue to store the order of access
        queue=temp;
    }
    void print()
    {
        if(capacity==0)
        {
            cout<<"The capacity of cache is 0"<<endl;
            return;
        }
        if(cur_sz==0)
        {
            cout<<"Cache is empty"<<endl;
            return;
        }
        Node *temp=queue->head;
        while(temp!=NULL)
        {
            cout<<temp->key<<"\t";
            temp=temp->next;
        }
    }
    int get(int key)//Returns the value of the key provided and updates the queue
    {
        if(capacity==0)
            return -1;
        if(cache.find(key)==cache.end())
            return -1;
        int v=cache[key]->value;
        queue->delete_inter(cache[key]);
        cache[key]=queue->insert(key,v);
        return v;
    }
    void set(int key, int val)//Updates an existing pair or adds a new key value pair
    {
        if(capacity==0)
        {
            //cout<<"The capacity of Cache is 0"<<endl;
            return;
        }
        if(cache.find(key)!=cache.end())
        {
            queue->delete_inter(cache[key]);
            cache[key]=queue->insert(key,val);
        }
        else if (cur_sz==capacity)
        {
            int num=queue->delete_head();
            cache.erase(num);
            cache[key]=queue->insert(key,val);
        }
        else 
        {
            cache[key]=queue->insert(key,val);;
            cur_sz++;
        }   
    }
};
int main()
{
    int ch=1;
    LRUCache* Cache1;
    //cout<<"The options are"<<endl<<"1.Create Cache"<<endl<<"2.Get data"<<endl<<"3.Set data"<<endl;
    //cout<<"4.Print Content"<<endl;
    while(ch>0 && ch<5)
    {
        //cout<<"Enter your choice"<<endl;
        cin>>ch;
        if(ch==1)
        {
            int cap;
            cin>>cap;
            LRUCache* temp=new LRUCache(cap);
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