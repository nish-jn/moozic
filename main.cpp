#define MINIAUDIO_IMPLEMENTATION   // must come before the first include of miniaudio.h
#include "library.h"
#include <iomanip>

#define RESET "\033[0m"
#define BOLD  "\033[1m"
#define RED   "\033[1;31m"   // bright red: borders and title
#define WHITE "\033[1;97m"   // bright white: keys and text
#define GREY  "\033[37m"     // soft white: normal text

// one row of the menu: [key]  label
void menuLine(const string& key, const string& label)
{
    cout<<RED<<"║"<<RESET<<"  "<<RED<<"["<<WHITE<<key<<RED<<"]"<<RESET<<"  "
        <<GREY<<left<<setw(31)<<label<<RESET<<RED<<"║\n"<<RESET;
}

void printMenu()
{
    cout<<"\n"<<RED<<"╔══════════════════════════════════════╗\n"
        <<"║"<<WHITE<<"              M O O Z I C             "<<RED<<"║\n"
        <<"╠══════════════════════════════════════╣\n"<<RESET;
    menuLine("1","Next song");
    menuLine("2","Previous song");
    menuLine("3","Add new playlist and songs");
    menuLine("4","Switch playlist");
    menuLine("5","Add a song to the queue");
    menuLine("6","Pause the Song");
    menuLine("7","Resume the Song");
    menuLine("0","Exit");
    cout<<RED<<"╚══════════════════════════════════════╝\n"<<RESET
        <<WHITE<<"Choose: "<<RESET;
}

int main()
{
    LinkerOfPlayLists lop;
    lop.check=lop.ftmPlayList();

    cout<<RED<<"Welcome to "<<WHITE<<"Moozic"<<RED<<"!"<<RESET
        <<GREY<<"\nPress 1 to start, anything else to exit: "<<RESET;
    int x; cin>>x; cin.ignore(10000, '\n'); //so the \n doesnt ruin my getline...
    if(x!=1){cout<<"Terminating..."; return 0;}

    string ln;
    cout<<GREY<<"Enter the name of the playlist to be played: "<<RESET;
    getline(cin, ln);
    lop.play(ln);

    while(true){
        printMenu();
        cin>>x;
        cin.ignore(10000, '\n');
        switch(x){
            case 0: cout<<"Terminating..."; return 0;
            case 1: lop.nextSong(); break;
            case 2: lop.previousSong(); break;
            case 3: lop.utmPlayList(); break;
            case 4: cout<<GREY<<"Enter the name of the playlist to be played: "<<RESET;
                    getline(cin, ln);
                    lop.play(ln); break;
            case 5: lop.playNext(); break;
            case 6: lop.music_pause(); break;
                case 7: lop.music_resume(); break;
            default: cout<<RED<<"Invalid input, try again.\n"<<RESET;
        }
    }
    return 0;
}
