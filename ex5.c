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

void sortByYear();
void sortByStreamsAscending();
void sortByStreamsDescending();
void sortAlphabetically();

void deletePlaylist();

void deleteSong();
void playSong();
void freeSong();
void freePlaylist();
void printPlaylistsMenu();
void printPlaylistsOptions();
void sortPlaylist();

int main() {
    int choice = -1, choice2 = -1, choice3 = -1, choice4 = -1, choiceDeleteSong = -1, choiceDeletePlaylist = -1;
    int playlistCount = 0;
    PlaylistList *pl = (PlaylistList*)malloc(sizeof(PlaylistList));
    pl->head = NULL;
    pl->last = NULL;
    printPlaylistsMenu();
    do {
        if(choice != WATCH_PLAYLISTS) {
            scanf("%d", &choice);
            getchar();
        }
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
                printMyPlaylists(pl, playlistCount);
                scanf(" %d", &choice2);
                getchar();
                while(choice2 < 1 || choice2 > playlistCount + 1) {
                    printf("Invalid option\n");
                    scanf(" %d", &choice2);
                    getchar();
                }
                if(choice2 == playlistCount + 1) {
                    choice = -1;
                    choice3 = -1;
                    break;
                }
                //save the playlistList to a temporary list so we can play with it
                PlaylistItem* current = pl->head;
                //find the correct playlist to work with
                for(int i = 1; i < choice2; i++)
                    current = current->next;
                printf("playlist %s:\n", current->data->name);
                do {
                    if(choice3 != SHOW_PLAYLIST) {
                        printPlaylistsOptions(current);
                        scanf(" %d", &choice3);
                        getchar();
                    }
                    switch(choice3) {
                        case SHOW_PLAYLIST: {
                            while(choice3 !=0) {
                                for(int i = 0; i < current->data->songsNum; i++) {
                                    printf("%d. Title: %s\n", i, current->data->songs[i]->title);
                                    printf("  Artist: %s\n", current->data->songs[i]->artist);
                                    printf("  Released: %d\n", current->data->songs[i]->year);
                                    //printf("Streams: 0\n");
                                    printf("  Streams: %d\n", current->data->songs[i]->streams);
                                }
                                do {
                                    printf("choose a song to play, or 0 to quit:\n");
                                    do {
                                        scanf("%d", &choice3);
                                        getchar();
                                        if(current->data->songsNum == 0 || choice3 == 0)
                                            break;
                                        if(choice3 > current->data->songsNum)
                                            printf("invalid option\n");
                                    } while(choice3 > current->data->songsNum || choice3 == 0);
                                    if(choice3 == 0)
                                        break;
                                    printf("Now playing %s:\n", current->data->songs[choice3-1]->title);
                                    printf("$ %s $\n",current->data->songs[choice3-1]->lyrics);
                                    PlaylistItem* current = pl->head;
                                    for(int i = 1; i < choice2; i++)
                                        current = current->next;
                                    current->data->songs[choice3-1]->streams++;
                                } while(choice3 != 0);
                            }
                            break;
                        }
                        case ADD_SONG: {
                            if(current->data->songs == NULL) {
                                current->data->songs = (Song**)malloc(sizeof(Song*));
                                current->data->songs[0] = malloc(sizeof(Song));
                                current->data->songs[0]->streams = 0;
                            }
                            else {
                                current->data->songs = (Song**)realloc(current->data->songs, sizeof(Song*)*
                                                                                        (current->data->songsNum + 1));
                                current->data->songs[current->data->songsNum] = malloc(sizeof(Song));
                                current->data->songs[current->data->songsNum]->streams = 0;
                            }
                            printf("Enter song's details\n");
                            printf("Title:\n");
                            current->data->songs[current->data->songsNum]->title = createString();
                            printf("Artist:\n");
                            current->data->songs[current->data->songsNum]->artist = createString();
                            printf("Year of release:\n");
                            scanf("%d", &current->data->songs[current->data->songsNum]->year);
                            getchar();
                            printf("Lyrics:\n");
                            current->data->songs[current->data->songsNum]->lyrics = createString();
                            current->data->songsNum++;
                            break;
                        }
                        case DELETE_SONG: {
                            printf("choose a song to delete, or 0 to quit:\n");
                            scanf("%d", &choiceDeleteSong);
                            getchar();
                            PlaylistItem* current = pl->head;
                            for(int i = 1; i < choice2; i++)
                                current = current->next;
                            deleteSong(current->data, choiceDeleteSong-1);
                            break;
                        }
                        case SORT: {
                            printf("choose:\n");
                            printf("1. sort by year\n");
                            printf("2. sort by streams - ascending order\n");
                            printf("3. sort by streams - descending order\n");
                            printf("4. sort alphabetically\n");
                            scanf("%d", &choice4);
                            getchar();
                            switch (choice4) {
                                case 1: {
                                    sortByYear(pl->head->data);
                                    break;
                                }
                                case 2: {
                                    sortByStreamsAscending(pl->head->data);
                                    break;
                                }
                                case 3: {
                                    sortByStreamsDescending(pl->head->data);
                                    break;
                                }
                                case 4: {
                                    sortAlphabetically(pl->head->data);
                                    break;
                                }
                                default:
                                    sortAlphabetically(pl->head->data);
                            }
                            break;
                        }
                        case PLAY: {
                            PlaylistItem* current = pl->head;
                            for(int i = 1; i < choice2; i++)
                                current = current->next;
                            for(int i = 0; i < current->data->songsNum; i++) {
                                printf("Now playing %s:\n", current->data->songs[i]->title);
                                printf("$ %s $\n",current->data->songs[i]->lyrics);
                                printf("\n");
                                current->data->songs[i]->streams++;
                            }
                            break;
                        }
                        case EXIT2: {
                            //printf("Invalid option\n");
                            break;
                        }
                        default:
                            printf("Invalid option\n");
                    }
                }while(choice3 != EXIT2);
                break;
            }
            case ADD_PLAYLIST:{
                printf("Enter playlist's name:\n");
                PlaylistItem* item = (PlaylistItem*)malloc(sizeof(PlaylistItem));


                if (item == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(1);
                }


                playlistCount++;
                item->data = (Playlist*)malloc(sizeof(Playlist));


                if (item->data == NULL) {
                    printf("Memory allocation failed.\n");
                    free(item);  // Clean up already allocated memory
                    exit(1);
                }


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
                break;
            }
            case REMOVE_PLAYLIST:{
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
                printf("choose a playlist:\n");
                printMyPlaylists(pl, playlistCount);
                scanf(" %d", &choiceDeletePlaylist);
                getchar();
                while(choiceDeletePlaylist < 1 || choiceDeletePlaylist > playlistCount + 1) {
                    printf("Invalid option\n");
                    scanf(" %d", &choiceDeletePlaylist);
                    getchar();
                }
                deletePlaylist(pl, choiceDeletePlaylist-1, playlistCount);
                playlistCount--;
                //=-==========================================
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
        if(choice != WATCH_PLAYLISTS)
            printPlaylistsMenu();
    }while(choice != EXIT);
    return 0;
}

void sortByYear(Playlist* p){
    for(int i = 0; i < p->songsNum; i++) {
        for(int j = 0; j < p->songsNum; j++) {
            if(p->songs[j]->year > p->songs[i]->year) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[i];
                p->songs[i] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortByStreamsAscending(Playlist* p) {
    for(int i = 0; i < p->songsNum; i++) {
        for(int j = i; j < p->songsNum; j++) {
            if(p->songs[i]->streams > p->songs[j]->streams) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[i];
                p->songs[i] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortByStreamsDescending(Playlist* p){
    for(int i = 0; i < p->songsNum; i++) {
        for(int j = i; j < p->songsNum; j++) {
            if(p->songs[j]->streams > p->songs[i]->streams) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[i];
                p->songs[i] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortAlphabetically(Playlist* p){
    for(int i = 0; i < p->songsNum; i++) {
        for(int j = 0; j < p->songsNum; j++) {
            if(strcmp(p->songs[j]->title, p->songs[i]->title) > 0) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[i];
                p->songs[i] = temp;
            }
        }
    }
    printf("sorted\n");
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
    while ((ch = getchar()) != '\n' && ch != '\r') {
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
    if (ch == '\r')
        getchar();
    //mark the end of the string as NULL
    buffer[length] = '\0';
    return buffer;
}
//some recommendations for functions, you may implement however you want
void printMyPlaylists(PlaylistList* pl) {
    if (pl->head == NULL) {
        printf("\t1. Back to main menu\n");
        return;
    }

    printf("Choose a playlist:\n");
    PlaylistItem* current = pl->head;
    int i = 1;
    while (current != NULL) {
        printf("\t%d. %s\n", i, current->data->name);
        current = current->next;
        i++;
    }
    printf("\t%d. Back to main menu\n", i);
}

void deleteSong(Playlist* p, int choiceDelete) {
    freeSong(p->songs[choiceDelete]);
    for(int j = choiceDelete; j < p->songsNum-1; j++) {
        p->songs[j] = p->songs[j+1];
    }
    p->songsNum--;
    printf("Song deleted successfully.\n");
}

void deletePlaylist(PlaylistList* pl, int choiceDelete) {
    PlaylistItem* current = pl->head;
    PlaylistItem* previous = NULL;

    for(int i = 0; i < choiceDelete; i++) {
        previous = current;
        current = current->next;
    }
    freePlaylist(current->data);
    if(previous == NULL)
        pl->head = current->next;
    else
        previous->next = current->next;
    if(current == pl->last)
        pl->last = previous;
    free(current);
    printf("Playlist deleted successfully.\n");
}

void freeSong(Song* s) {
    if (s) {
        free(s->title);
        free(s->artist);
        free(s->lyrics);
        free(s);
        printf("Song freed\n");
    }
}

void freePlaylist(Playlist* p) {
    for (int i = 0; i < p->songsNum; i++) {
        freeSong(p->songs[i]);
    }
    free(p->songs);
    free(p->name);
    free(p);
    printf("Playlist freed\n");
}

void printPlaylistsOptions() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}



