// containers.h
#pragma once
#include <iostream>
#include "song_list.h"
using namespace std;
// ... Box, Queue, Stack
struct Box{
    Node* song;
    SongNode* playlist;
    Box* next;
    Box* prev;
    Box(Node* sng, SongNode* plist)
    {
        song=sng;
        playlist=plist;
        next=nullptr;
        prev=nullptr;
    }
};
class Queue{
    Box* first; Box* last;

    public:
    Queue(){first=nullptr; last=first;}
    bool queueUnderflow()
    {
        if(first==nullptr){return true;}
        else{return false;}
    }


    void enqueue(Node* sng, SongNode* plist)
    {
        Box* newBox = new Box(sng, plist);
        if(first==nullptr){first=newBox; last=first;}
        else{
            last->next=newBox;
            last=last->next;
        }
    }
    void dequeue()
    {
        if(queueUnderflow()){cout<<"Queue emptied."; return;}
        Box* temp=first;
        first=first->next;
        delete temp;
    }
    Box* front_peek()
    {
        return first;
    }

~Queue(){ while(!queueUnderflow()) dequeue(); }
};

class Stack{
    Box* top;

    public:
        Stack(){top=nullptr;}
        bool stackUnderflow()
        {
            if(top==nullptr){return true;}
            else{return false;}
        }
        void push(Node* sng, SongNode* plist)
        {
            Box* newBox = new Box(sng, plist);
            if(top==nullptr){top=newBox;}
            else{top->next=newBox;  newBox->prev=top; top=top->next;}
        }

        void pop()
        {
            if(!stackUnderflow())
            {
                Box* temp=top;
                top=top->prev;
                delete temp;
            }
        }

        Box* Top()
        {
            return top;
        }
        ~Stack(){ while(!stackUnderflow()) pop(); }
};
