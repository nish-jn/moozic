# Moozic

A command-line music player in C++. Playlists are nested doubly linked
lists, and the "up next" queue and play history are hand-written (no STL).

## Features

- Playlists loaded from and saved to `music_library.txt`
- Next and previous song (previous uses a history stack)
- "Play next" queue
- Add new playlists and songs while running
- mp3 playback using miniaudio

## Build

```
g++ main.cpp -o moozic -ldl -lpthread -lm
```

## Run

```
./moozic
```

## Library file format (`music_library.txt`)

```
#Rock
Song name|/full/path/to/song.mp3
Another song|/full/path/to/another.mp3
#Jazz
Song with no file
```

- A line starting with `#` starts a playlist.
- `Name|path` is a song with an mp3 file.
- A song with no `|` has no file and stays silent.
- Don't put `|` in a song name.

## Menu

1 next, 2 previous, 3 add playlist, 4 switch playlist, 5 add to queue, 6 to pause, 7 to resume, 0 exit

## How it works

- `song_list.h`: song nodes and playlists (doubly linked list)
- `containers.h`: hand-written stack and queue
- `audio.h`: miniaudio wrapper for audio playback
- `library.h`: the player logic
- `main.cpp`: menu

## Known limitations

- Songs don't auto-advance when they end
- Names are case sensitive

## Credits

Audio by [miniaudio](https://github.com/mackron/miniaudio).
