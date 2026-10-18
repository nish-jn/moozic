#define MINIAUDIO_IMPLEMENTATION
#include<iostream>
#include<string>
#include<fstream>
#include"miniaudio.h"
using namespace std;

//audio wrapper
    class Audio{
        ma_engine engine;
        ma_sound sound;
        bool engineOk=false, soundLoaded=false;
    public:
        Audio()
        {
            engineOk=(ma_engine_init(NULL,&engine)==MA_SUCCESS);
            if(!engineOk) cout<<"Audio engine failed\n";
        }
        void stop()
        {
            if(soundLoaded){ ma_sound_uninit(&sound); soundLoaded=false; }
        }
        void playFile(const string& path)
        {
            if(!engineOk) return;
            stop();                                   // stop the old song first
            if(path.empty()){ cout<<"(no file for this song)\n"; return; }
            if(ma_sound_init_from_file(&engine,path.c_str(),0,NULL,NULL,&sound)!=MA_SUCCESS)
            { cout<<"Could not open "<<path<<"\n"; return; }
            soundLoaded=true;
            ma_sound_start(&sound);
        }
        ~Audio(){ stop(); if(engineOk) ma_engine_uninit(&engine); }
    };


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
    Audio audio;

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
                else if(tail!=nullptr)
                {
                    size_t bar = line.find('|');
                    if(bar==string::npos){tail->list.addSong(line);} //no path
                    else{tail->list.addSong(line.substr(0,bar),line.substr(bar+1));}
                }

            }
            return 0;
        }
        void utmPlayList()//user to memory
        {
            string line, path;
            cout<<"Enter name of the new play list: ";
            getline(cin,line);
            addPlayList(line);
            int j=1;
            while(true){
                cout<<"Enter the name of song number "<<j<<" (enter 0 to stop): ";
                getline(cin, line);
                if(line=="0"){break;}
                cout<<"Enter the full path of the mp3 (just press Enter for none): ";
                getline(cin, path);
                tail->list.addSong(line, path);
                j++;
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
                    file<<ver->song_name;
                    if(!ver->path.empty()) file<<"|"<<ver->path;
                    file<<"\n";
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
                startSong(currentSong);}
            else{cout<<"Empty playlist\n";}


            }
            void nextSong()
            {if(!q.queueUnderflow())
            {
                Box* queued = q.front_peek();
                startSong(queued->song);
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

                startSong(currentSong);



            }


                void previousSong()
                {
                    if(s.stackUnderflow()){ cout<<"No history.\n"; return; }
                    currentSong=s.Top()->song;
                    currentPlayList=s.Top()->playlist;
                    s.pop();
                    startSong(currentSong);
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
            void startSong(Node* n)
            {
                cout<<"Playing "<<n->song_name<<"\n";
                audio.playFile(n->path);
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
