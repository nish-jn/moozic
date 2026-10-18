// library.h
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "audio.h"
#include "song_list.h"
#include "containers.h"
using namespace std;
// ... LinkerOfPlayLists
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
            ifstream file("music_library.txt");
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
