#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <portaudio.h>
#include <sndfile.h>

#define FRAMES_PER_BUFFER 1024
#define NUM_CHANNELS 2

typedef struct Song {
    char name[100];
    char filename[256];
    int length;  // in seconds
    struct Song* next;
} Song;

typedef struct Playlist {
    Song* head;
    Song* current;
} Playlist;

typedef struct {
    SNDFILE *file;
    SF_INFO sfinfo;
} paData;

Playlist* createPlaylist() {
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    playlist->head = NULL;
    playlist->current = NULL;
    return playlist;
}

void addSong(Playlist* playlist, const char* name, const char* filename, int length) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->name, name);
    strcpy(newSong->filename, filename);
    newSong->length = length;
    newSong->next = NULL;

    if (playlist->head == NULL) {
        playlist->head = newSong;
    } else {
        Song* current = playlist->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newSong;
    }
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData) {
    paData *data = (paData*)userData;
    float *out = (float*)outputBuffer;
    sf_count_t num_read;

    num_read = sf_read_float(data->file, out, framesPerBuffer * NUM_CHANNELS);

  
    if (num_read < framesPerBuffer * NUM_CHANNELS) {
        memset(out + num_read, 0, (framesPerBuffer * NUM_CHANNELS - num_read) * sizeof(float));
        return paComplete;
    }

    return paContinue;
}

void playSong(Song* song) {
    printf("Now playing: %s\n", song->name);

    PaStream *stream;
    PaError err;
    paData data;

    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) goto error;

    // Open the audio file
    data.file = sf_open(song->filename, SFM_READ, &data.sfinfo);
    if (!data.file) {
        printf("Error opening file: %s\n", song->filename);
        goto error;
    }

    // Open an audio I/O stream
    err = Pa_OpenDefaultStream(&stream,
                               0,          // no input channels
                               NUM_CHANNELS,
                               paFloat32,  // 32 bit floating point output
                               data.sfinfo.samplerate,
                               FRAMES_PER_BUFFER,
                               paCallback,
                               &data);
    if (err != paNoError) goto error;

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

 
    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100);
    }

   
    err = Pa_StopStream(stream);
    if (err != paNoError) goto error;

    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;

    sf_close(data.file);
    Pa_Terminate();
    return;

error:
    Pa_Terminate();
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void playPlaylist(Playlist* playlist) {
    playlist->current = playlist->head;
    while (playlist->current != NULL) {
        playSong(playlist->current);
        playlist->current = playlist->current->next;
    }
    printf("Playlist finished.\n");
}

int main() {
    Playlist* myPlaylist = createPlaylist();

    addSong(myPlaylist, "Song 1", "song1.wav", 180);
    addSong(myPlaylist, "Song 2", "song2.wav", 240);
    addSong(myPlaylist, "Song 3", "song3.wav", 200);

    playPlaylist(myPlaylist);

    return 0;
}
