// song_list.h
#pragma once
#include <iostream>
#include <string>
using namespace std;
// ... Node, LinkedPlayList, SongNode
//For indivisual playlist:
struct Node{
    string song_name;
    string path;
    Node* next;
    Node* prev;

    Node(string sn, string p="")
    {
        next=nullptr;
        prev=nullptr;
        song_name=sn;
        path=p;
    }
};


class LinkedPlayList{
    Node* head;
    Node* tail;

    public:
    LinkedPlayList()
    {
        head=nullptr;
        tail=nullptr;
    }
    //eats string to give name to new song and adds it to the playlist (vertical branch)
    // that it belongs to
    void addSong(string sn, string p="")
    {
        Node* newNode=new Node(sn,p);
        if(head==nullptr)
        {
            head=newNode;
            tail=head;
        }
        else {
            tail->next=newNode;
            newNode->prev=tail;
            tail=tail->next;
            }
    }




    void indivisualListPrint()
    {
        Node* iterator=head;
        int i=1;
        while(iterator!=nullptr)
        {
            cout<<i++<<": "<<iterator->song_name<<"\n";
            iterator=iterator->next;

        }
        cout<<endl;
    }

    Node* getHead_0w0()
    {
        return head;
    }

    Node* getTail()
    {
        return tail;
    }

    ~LinkedPlayList()
    {
        Node* temp=head;
        while(temp!=nullptr)
        {
            Node* nxt=temp->next;
            delete temp;
            temp=nxt;
        }
    }

};
struct SongNode{
    string list_name;
    LinkedPlayList list;
    SongNode* next;
    SongNode* prev;

    SongNode(string ln)
    {
        list_name=ln;
        next=nullptr;
        prev=nullptr;
    }
};
