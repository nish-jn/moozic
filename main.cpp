#include<iostream>
#include<string>
#include<fstream>

using namespace std;

//For indivisual playlist:
struct Node{
    string song_name;
    Node* next;
    Node* prev;

    Node(string sn)
    {
        next=nullptr;
        prev=nullptr;
        song_name=sn;
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
    void addSong(string sn)
    {
        Node* newNode=new Node(sn);
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
//for all playlists being linked:
//each linked list here contains its own linked list, consider one of these as the
// horizontal branch

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
class LinkerOfPlayLists{
    SongNode* head;
    SongNode* tail;

    public:
        LinkerOfPlayLists()
        {
            head=nullptr;
            tail=head;
        }

        //eats string to give name to a new playlist(names of the vertical branches)
        // and adds it to its tail or wherever it needs be added
        void addPlayList(string ln)
        {

            SongNode* newList=new SongNode(ln);



            if(head==nullptr)
            {
                head=newList;
                tail=head;
            }
            else {
                tail->next=newList;
                newList->prev=tail;
                tail=tail->next;
            }
        }


        void ftmPlayList() //file to memory.
        {
            ifstream file("text.txt");
            if(!file.is_open()){cout<<"Error accessing library"; return;}

            string line;
            while(getline(file, line))
            {
                if (!line.empty() && line.back() == '\r') line.pop_back(); //fix for windows
                if(line.empty()){continue;}

                if(line[0]=='#')//# to denote playlist name, adds playlist (new vertical branch)
                                //to out horizontal branch and gives it name
                {
                    addPlayList(line.substr(1));

                }
                else if(tail!=nullptr){tail->list.addSong(line);}
                else {
                tail->list.addSong(line);//our tail is the last vertical branch in the collection
                                        //of playlists. So it does add song fn for that branch
                }
            }
        }
        void utmPlayList()//user to memory
        {
            string line;
            cout<<"Enter name of the new play list: ";
            getline(cin,line);
                addPlayList(line);
                int j=1;
                while(true){
                    cout<<"Enter the name of song number "<<j++<<"(enter 0 to stop): ";
                    getline(cin, line);
                    if(line=="0"){break;}
                    tail->list.addSong(line);
                }
        }
        void allListPrint()
        {
            SongNode* cur = head;
            int j=1;
            while(cur!=nullptr)
            {
                cout<<"Playlist no. "<<j++<<": "<<cur->list_name<<"\n";
                cur->list.indivisualListPrint();
                cout<<"\n";
                cur=cur->next;
            }
            cout<<endl;
        }

        ~LinkerOfPlayLists()
        {
         SongNode* temp = head;
         while(temp!=nullptr)
         {
             SongNode* nxt=temp->next;
             delete temp;
             temp = nxt;
         }
        }
        //writing any changes to file
        void memorytofile()
        {
            ofstream file("text.txt");
            if(!file.is_open()){cout<<"Error writing changes to file, exiting. "; return;}

            //traversal through the library, adding playlist names with # and songs without #
            SongNode* hor = head;
            while(hor!=nullptr)
            {
                file<<"#"<<hor->list_name<<"\n";
                Node* ver = hor->list.getHead_0w0();
                while(ver!=nullptr)
                {
                    file<<ver->song_name<<"\n";
                    ver=ver->next;
                }
                hor=hor->next;
            }

        }
};




int main()
{

    LinkerOfPlayLists lop;
    lop.ftmPlayList();

    lop.allListPrint();

    lop.utmPlayList();

    lop.memorytofile();

    return 0;
}
