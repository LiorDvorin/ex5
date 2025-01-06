#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WATCH_PLAYLISTS 1
#define ADD_PLAYLIST 2
#define REMOVE_PLAYLIST 3
#define EXIT 4

#define SHOW_PLAYLIST 1
#define ADD_SONG 2
#define DELETE_SONG 3
#define SORT 4
#define PLAY 5
#define EXIT2 6




typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

typedef struct PlaylistItem {
    Playlist* data;
    struct PlaylistItem* next;
} PlaylistItem;

typedef struct {
    PlaylistItem *head, *last;
} PlaylistList;

void printMyPlaylists();
char* createString();


void deleteSong();
void playSong();
void freeSong();
void freePlaylist();
void printPlaylistsMenu();
void printPlaylistsOptions();
void sortPlaylist();

int main() {
    int choice = -1, choice2 = -1, choice3 = -1, choice4 = -1;
    int playlistCount = 0;
    PlaylistList *pl = (PlaylistList*)malloc(sizeof(PlaylistList));
    pl->head = NULL;
    pl->last = NULL;
    printPlaylistsMenu();
    do {
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case WATCH_PLAYLISTS:{
                //if we have no playlists then only print the back to main menu option
                if(pl->head == NULL) {
                    printf("Choose a playlist:\n\t1. Back to main menu\n");
                    scanf("%d", &choice2);
                    getchar();
                    while(choice2 != 1) {
                        printf("Invalid option\n");
                        scanf("%d", &choice2);
                        getchar();
                    }
                    break;
                }
                //if we have at least 1 playlist we continue and ask for the option the user wants
                printMyPlaylists(pl);
                scanf(" %d", &choice2);
                getchar();
                while(choice2 < 1 || choice2 > playlistCount + 1) {
                    printf("Invalid option\n");
                    scanf(" %d", &choice2);
                    getchar();
                }
                if(choice2 == playlistCount + 1)
                    break;
                //save the playlistList to a temporary list so we can play with it
                PlaylistList plTmp = *pl;
                //find the correct playlist to work with
                for(int i = 1; i < choice2; i++)
                    plTmp.head = plTmp.head->next;
                printf("playlist %s:\n", plTmp.head->data->name);
                do {
                    printPlaylistsOptions(plTmp.head);
                    scanf(" %d", &choice4);
                    getchar();
                    switch(choice4) {
                        case SHOW_PLAYLIST: {
                            while(choice4 !=0) {
                                for(int i = 0; i < plTmp.head->data->songsNum; i++) {
                                    printf("Title: %s\n", plTmp.head->data->songs[i]->title);
                                    printf("Artist: %s\n", plTmp.head->data->songs[i]->artist);
                                    printf("Released: %d\n", plTmp.head->data->songs[i]->year);
                                    printf("Streams: 0\n");
                                    //printf("Streams: %s\n", plTmp.head->data->songs[choice4-1]->title);
                                }
                                printf("choose a song to play, or 0 to quit:\n");
                                do {
                                    scanf("%d", &choice4);
                                    getchar();
                                    if(plTmp.head->data->songsNum == 0 || choice4 == 0)
                                        break;
                                    if(choice4 > plTmp.head->data->songsNum)
                                        printf("invalid option\n");
                                } while(choice4 > plTmp.head->data->songsNum || choice4 == 0);
                                printf("Now playing %s:\n", plTmp.head->data->songs[choice4-1]->title);
                                printf("$ %s $\n",plTmp.head->data->songs[choice4-1]->lyrics);
                            }
                            break;
                        }
                        case ADD_SONG: {
                            if(plTmp.head->data->songs == NULL) {
                                plTmp.head->data->songs = (Song**)malloc(sizeof(Song*));
                                plTmp.head->data->songs[0] = malloc(sizeof(Song));
                            }
                            else {
                                plTmp.head->data->songs = (Song**)realloc(plTmp.head->data->songs, sizeof(Song*)*
                                                                                        (plTmp.head->data->songsNum + 1));
                                plTmp.head->data->songs[plTmp.head->data->songsNum] = malloc(sizeof(Song));
                            }
                            printf("Enter song's details\n");
                            printf("Title:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->title = createString();
                            printf("Artist:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->artist = createString();
                            printf("Year of release:\n");
                            scanf("%d", &plTmp.head->data->songs[plTmp.head->data->songsNum]->year);
                            getchar();
                            printf("Lyrics:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->lyrics = createString();
                            plTmp.head->data->songsNum++;
                            break;
                        }
                        case DELETE_SONG: {

                            break;
                        }
                        case SORT: {

                            break;
                        }
                        case PLAY: {

                            break;
                        }
                        case EXIT2: {
                            printf("Invalid option\n");
                            break;
                        }
                        default:
                            printf("Invalid option\n");
                    }
                }while(choice4 != EXIT2);
                break;
            }
            case ADD_PLAYLIST:{
                printf("Enter playlist's name:\n");
                PlaylistItem* item = (PlaylistItem*)malloc(sizeof(PlaylistItem));
                playlistCount++;
                item->data = (Playlist*)malloc(sizeof(Playlist));
                item->data->name = createString();
                item->data->songs = NULL;
                item->data->songsNum = 0;
                item->next = NULL;
                if (pl->head == NULL) {
                    pl->head = item;
                    pl->last = item;
                }
                else {
                    pl->last->next = item;
                    pl->last = item;
                }
                printf("TEST\n");

                break;
            }
            case REMOVE_PLAYLIST:{

                break;
            }
            case EXIT: {
                printf("Goodbye!\n");
                return 0;
            }
            default:{
                printf("Invalid option\n");
            break;
            }
        }
        printPlaylistsMenu();
        printf("TEST 2\n");
    }while(choice != EXIT);
    return 0;
}

//function that creates a string from the user's input without limit to length
char* createString() {
    //set initial size to 10 and length to 0
    int size = 10;
    int length = 0;
    char *buffer = (char*)malloc(size * sizeof(char));
    if(buffer == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    char ch;
    while ((ch = getchar()) != '\n') {
        buffer[length++] = ch;
        //if the buffer is full, we resize it
        if (length >= size) {
            size *= 2;
            buffer = (char*)realloc(buffer, size * sizeof(char));
            if (buffer == NULL) {
                printf("Memory allocation error\n");
                exit(1);
            }
        }
    }
    //mark the end of the string as NULL
    buffer[length] = '\0';
    return buffer;
}
//some recommendations for functions, you may implement however you want
void printMyPlaylists(PlaylistList* pl) {
    printf("Choose a playlist:\n");
    PlaylistItem* current = pl->head;
    int i = 1;
    while(current != NULL) {
        printf("\t%d. %s:\n", i ,current->data->name);
        current = current->next;
        i++;
    }
    printf("\t%d. Back to main menu\n", i);
}

void deleteSong() {

    printf("Song deleted successfully.\n");
}



void playSong() {
    
}



void freeSong() {

}

void freePlaylist() {
    
}

void printPlaylistsOptions() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}



void sortPlaylist() {
    
    printf("sorted\n");
}


