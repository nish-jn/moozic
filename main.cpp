// main.cpp
#define MINIAUDIO_IMPLEMENTATION   // must come before the first include of miniaudio.h
#include "library.h"
// ... main()

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
