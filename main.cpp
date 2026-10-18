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




//for all playlists being linked:
//each linked list here contains its own linked list, consider one of these as the
// horizontal branch

class LinkerOfPlayLists{
    SongNode* head;
    SongNode* tail;
    SongNode* currentPlayList;
    Node* currentSong;
    Queue q;
    Stack s;

    public:
        int check=0;
        LinkerOfPlayLists()
        {
            head=nullptr;
            tail=head;
            currentPlayList=nullptr;
            currentSong=nullptr;
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


        int ftmPlayList() //file to memory.
        {
            ifstream file("text.txt");
            if(!file.is_open()){cout<<"Error accessing library"; return 1;}

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

            }
            return 0;
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
        ~LinkerOfPlayLists()
        {
            if(check==1){cout<<"The changes made during the session (if any)\n couldnt be saved due to some trouble accessing the library file on storage";}
            else{memorytofile();}

         SongNode* temp = head;
         while(temp!=nullptr)
         {
             SongNode* nxt=temp->next;
             delete temp;
             temp = nxt;
         }
        }

        //ACTUAL PLAYER FUNCTIONS START HERE FINALLY, AFTER ALL THE MEMORY STUFF
        void play(string ln)
        {
            SongNode* iterator=head;
            while(iterator!=nullptr&&iterator->list_name!=ln){iterator=iterator->next;}
            if(iterator==nullptr){cout<<"Could not find the playlist, cant play."; return;}
            currentPlayList=iterator;

            currentSong=currentPlayList->list.getHead_0w0();


            if(currentSong!=nullptr){cout<<"From Playlist "<<currentPlayList->list_name<<"\n";
                cout<<"Playing "<<currentSong->song_name<<"....\n";}
            else{cout<<"Empty playlist\n";}


            }
            void nextSong()
            {if(!q.queueUnderflow())
            {
                Box* queued = q.front_peek();
                cout<<"Playing "<<queued->song->song_name<<"\n";
                s.push(queued->song,queued->playlist);
                q.dequeue();
                return;
            }

                // nothing playing (never started, or already ended)
                if(currentSong==nullptr){ cout<<"Nothing is playing.\n"; return; }
                s.push(currentSong,currentPlayList);
                currentSong=currentSong->next;


                // song list ended: hop forward until a playlist has a song, or we run out
                while(currentSong==nullptr && currentPlayList!=nullptr)
                {
                    cout<<"Playlist end, moving to next one\n";
                    currentPlayList=currentPlayList->next;
                    if(currentPlayList!=nullptr)
                        currentSong=currentPlayList->list.getHead_0w0();
                }

                // ran out of playlists
                if(currentSong==nullptr){ cout<<"End of all playlists.\n"; return; }

                cout<<"Playing "<<currentSong->song_name<<"....\n";



            }


                void previousSong()
                {
                    if(s.stackUnderflow()){ cout<<"No history.\n"; return; }
                    currentSong=s.Top()->song;
                    currentPlayList=s.Top()->playlist;
                    s.pop();
                    cout<<"Playing "<<currentSong->song_name<<"\n";
                }



            void playNext()
            {
                cout<<"Enter playlist name, then song name in this order, for the song to be played next: ";
                string pn, sn;
                getline(cin, pn); getline(cin, sn);

                SongNode* it = head;
                while(it!=nullptr&&it->list_name!=pn){it=it->next;}
                if(it==nullptr){cout<<"Couldnt find the required playlist! "; return;}
                Node* ti = it->list.getHead_0w0();
                while(ti!=nullptr&&ti->song_name!=sn){ti=ti->next;}
                if(ti==nullptr){cout<<"Couldnt find required song"; return;}
                    q.enqueue(ti, it);


            }

        };



int main()
{

    LinkerOfPlayLists lop;
    lop.check=lop.ftmPlayList();

    //lop.allListPrint();

//    lop.utmPlayList();

    //lop.memorytofile();

    cout<<"Welcome! \nPress 1 to start using the player, something else to exit the program: ";
    int x; cin>>x; cin.ignore(10000, '\n'); //so the \n doesnt ruin my getline...
    if(x!=1){cout<<"Terminating..."; return 0;}



    string ln;
    cout<<"Enter the name of the playlist to be played: ";
    getline(cin, ln);
    lop.play(ln);
    while(true){
    cout<<"Enter 1 to go to next song, 2 to go to previous, 3 to add new playlist and songs, 4 to go to another playlist, 5 to add to immediate queue, 0 to exit.\n";
    cin>>x;
    cin.ignore(10000, '\n');
    switch(x){
        case 0: cout<<"Terminating..."; return 0;
        case 1: lop.nextSong(); break;
        case 2: lop.previousSong(); break;
        case 3: lop.utmPlayList(); break;
        case 4: cout<<"Enter the name of the playlist to be played: ";
        getline(cin, ln);
        lop.play(ln);break;
        case 5: lop.playNext(); break;
        default: cout<<"Invalid input, try again. ";
    }
    }
    return 0;
}
