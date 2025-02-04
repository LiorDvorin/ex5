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
    //create an empty list with pointers head and last to null
    PlaylistList *pl = (PlaylistList*)malloc(sizeof(PlaylistList));
    pl->head = NULL;
    pl->last = NULL;
    printPlaylistsMenu();
    do {
        //ask the user to select an option on if the last accessed function wasn't watch_playlists
        if(choice != WATCH_PLAYLISTS || pl->head == NULL) {
            //print the menu only if needed
            if(pl->head == NULL && choice != -1)
                printPlaylistsMenu();
            scanf("%d", &choice);
        }
        switch (choice) {
            case WATCH_PLAYLISTS:{
                //if we have no playlists then only print the back to main menu option
                if(pl->head == NULL) {
                    printf("Choose a playlist:\n\t1. Back to main menu\n");
                    scanf("%d", &choice2);
                    while(choice2 != 1) {
                        printf("Invalid option\n");
                        printf("Choose a playlist:\n\t1. Back to main menu\n");
                        scanf("%d", &choice2);
                    }
                    break;
                }
                //if we have at least 1 playlist we continue and ask for the option the user wants
                printMyPlaylists(pl);
                scanf("%d", &choice2);
                while(choice2 < 1 || choice2 > playlistCount + 1) {
                    printf("Invalid option\n");
                    scanf("%d", &choice2);
                }
                //reset the choices as to not affect the next loop
                if(choice2 == playlistCount + 1) {
                    choice = -1;
                    choice3 = -1;
                    break;
                }
                //save the playlistList to a temporary list so we can play with it
                PlaylistList plTmp = *pl;
                //find the correct playlist to work with
                for(int i = 1; i < choice2; i++)
                    plTmp.head = plTmp.head->next;
                printf("playlist %s:\n", plTmp.head->data->name);
                do {
                    //ask the user to select an option on if the last accessed function wasn't show_playlists
                    if(choice3 != SHOW_PLAYLIST) {
                        printPlaylistsOptions(plTmp.head);
                        scanf("%d", &choice3);
                    }
                    switch(choice3) {
                        case SHOW_PLAYLIST: {
                            while(choice3 !=0) {
                                //print all songs and their info in the current playlist
                                for(int i = 0; i < plTmp.head->data->songsNum; i++) {
                                    printf("%d. Title: %s\n", i+1, plTmp.head->data->songs[i]->title);
                                    printf("  Artist: %s\n", plTmp.head->data->songs[i]->artist);
                                    printf("  Released: %d\n", plTmp.head->data->songs[i]->year);
                                    //printf("Streams: 0\n");
                                    printf("  Streams: %d\n", plTmp.head->data->songs[i]->streams);
                                }
                                do {
                                    printf("choose a song to play, or 0 to quit:\n");
                                    do {
                                        scanf("%d", &choice3);
                                        //if the playlist is empty just return after scanning the output
                                        if(plTmp.head->data->songsNum == 0 || choice3 == 0)
                                            break;
                                        if(choice3 > plTmp.head->data->songsNum)
                                            printf("invalid option\n");
                                    } while(choice3 > plTmp.head->data->songsNum || choice3 == 0);
                                    if(choice3 == 0)
                                        break;
                                    //print the chosen song and it's lyrics
                                    printf("Now playing %s:\n", plTmp.head->data->songs[choice3-1]->title);
                                    printf("$ %s $\n",plTmp.head->data->songs[choice3-1]->lyrics);
                                    PlaylistItem* current = pl->head;
                                    //traverse the playlist and update the correct song's streams
                                    for(int i = 1; i < choice2; i++)
                                        current = current->next;
                                    current->data->songs[choice3-1]->streams++;
                                } while(choice3 != 0);
                            }
                            break;
                        }
                        case ADD_SONG: {
                            //if the playlist is empty allocate memory for the first song
                            if(plTmp.head->data->songs == NULL) {
                                plTmp.head->data->songs = (Song**)malloc(sizeof(Song*));
                                plTmp.head->data->songs[0] = malloc(sizeof(Song));
                                plTmp.head->data->songs[0]->streams = 0;
                            }
                            //if the playlist is not empty reallocate memory for the next song
                            else {
                                plTmp.head->data->songs = (Song**)realloc(plTmp.head->data->songs, sizeof(Song*)*
                                                                                        (plTmp.head->data->songsNum + 1));
                                plTmp.head->data->songs[plTmp.head->data->songsNum] = malloc(sizeof(Song));
                                plTmp.head->data->songs[plTmp.head->data->songsNum]->streams = 0;
                            }
                            //scan the song's info and save it
                            printf("Enter song's details\n");
                            printf("Title:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->title = createString();
                            printf("Artist:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->artist = createString();
                            printf("Year of release:\n");
                            scanf("%d", &plTmp.head->data->songs[plTmp.head->data->songsNum]->year);
                            printf("Lyrics:\n");
                            plTmp.head->data->songs[plTmp.head->data->songsNum]->lyrics = createString();
                            plTmp.head->data->songsNum++;
                            break;
                        }
                        case DELETE_SONG: {
                            //print all songs in the playlist
                            if(plTmp.head->data->songsNum == 0) {
                                printf("choose a song to delete, or 0 to quit:\n");
                                int garbage;
                                scanf("%d", &garbage);
                                break;
                            }
                            for(int i = 0; i < plTmp.head->data->songsNum; i++) {
                                printf("%d. Title: %s\n", i+1, plTmp.head->data->songs[i]->title);
                                printf("  Artist: %s\n", plTmp.head->data->songs[i]->artist);
                                printf("  Released: %d\n", plTmp.head->data->songs[i]->year);
                                //printf("Streams: 0\n");
                                printf("  Streams: %d\n", plTmp.head->data->songs[i]->streams);
                            }
                            printf("\nchoose a song to delete, or 0 to quit:\n");
                            scanf("%d", &choiceDeleteSong);
                            PlaylistItem* current = pl->head;
                            //traverse the playlist up until the current song and call the deleteSong function
                            for(int i = 1; i < choice2; i++)
                                current = current->next;
                            deleteSong(current->data, choiceDeleteSong-1);
                            break;
                        }
                        case SORT: {
                            //print the menu and wait for the user's choice and call the desired sort function
                            printf("choose:\n");
                            printf("1. sort by year\n");
                            printf("2. sort by streams - ascending order\n");
                            printf("3. sort by streams - descending order\n");
                            printf("4. sort alphabetically\n");
                            scanf("%d", &choice4);
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
                            //traverse the list to the current playlist and play all it's songs
                            PlaylistItem* current = pl->head;
                            for(int i = 1; i < choice2; i++)
                                current = current->next;
                            for(int i = 0; i < plTmp.head->data->songsNum; i++) {
                                printf("Now playing %s:\n", plTmp.head->data->songs[i]->title);
                                printf("$ %s $\n",plTmp.head->data->songs[i]->lyrics);
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
                //allocate memory for a new playlist
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
                break;
            }
            case REMOVE_PLAYLIST:{
                //if the list is empty then just wait for an input from the user and go back
                if(pl->head == NULL) {
                    printf("Choose a playlist:\n\t1. Back to main menu\n");
                    scanf("%d", &choice2);
                    while(choice2 != 1) {
                        printf("Invalid option\n");
                        scanf("%d", &choice2);
                    }
                    choice = -1;
                    break;
                }
                //if the list is not empty then print all playlists and call the deletePlaylist function on the correct playlist
                printMyPlaylists(pl);
                scanf("%d", &choiceDeletePlaylist);
                while(choiceDeletePlaylist < 1 || choiceDeletePlaylist > playlistCount + 1) {
                    printf("Invalid option\n");
                    scanf("%d", &choiceDeletePlaylist);
                }
                deletePlaylist(pl, choiceDeletePlaylist-1, playlistCount);
                playlistCount--;
                //=-==========================================
                choice = -1;
                break;
            }
            case EXIT: {
                //traverse through the list and free all playlists data
                PlaylistItem* current = pl->head;
                while (current != NULL) {
                    PlaylistItem* next = current->next;
                    freePlaylist(current->data);
                    free(current);
                    current = next;
                }
                free(pl);
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
    for(int i = 0; i < p->songsNum - 1; i++) {
        for(int j = 0; j < p->songsNum - i - 1; j++) {
            if(p->songs[j]->streams > p->songs[j+1]->streams) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[j+1];
                p->songs[j+1] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortByStreamsDescending(Playlist* p){
    for(int i = 0; i < p->songsNum - 1; i++) {
        for(int j = 0; j < p->songsNum - i - 1; j++) {
            if(p->songs[j+1]->streams > p->songs[j]->streams) {
                Song* temp = p->songs[j];
                p->songs[j] = p->songs[j+1];
                p->songs[j+1] = temp;
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
    //set an initial size and length for the string
    int size = 10;
    int length = 0;
    char *buffer = (char*)malloc(size * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    char ch;
    //skip any char that may interfere with the string creation
    while ((ch = getchar()) == '\n' || ch == '\r' || ch == ' ');

    //start reading actual input while allocating more memory when needed
    do {
        buffer[length++] = ch;
        if (length >= size) {
            size *= 2;
            buffer = (char*)realloc(buffer, size * sizeof(char));
            if (buffer == NULL) {
                printf("Memory allocation error\n");
                exit(1);
            }
        }
    } while ((ch = getchar()) != '\n' && ch != '\r');
    //set the last char to null and return
    buffer[length] = '\0';
    return buffer;
}

void printMyPlaylists(PlaylistList* pl) {
    //if the list is empty print the back to main menu
    if(pl->head == NULL) {
        printf("Choose a playlist:\n");
        printf("\t1. Back to main menu\n");
        return;
    }
    //if the list is not empty then we print all the current playlists with their index + 1 as display
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
    //call the freeSong function to free every allocated memory of a song and then move the indexes of the songs
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
    //traverse the list until we reach the desired playlist
    for(int i = 0; i < choiceDelete; i++) {
        previous = current;
        current = current->next;
    }
    //call the freePlaylist function to free every allocated memory of a playlist and then move the positions of the playlists
    freePlaylist(current->data);
    if(previous == NULL)
        pl->head = current->next;
    else
        previous->next = current->next;
    if(current == pl->last)
        pl->last = previous;
    free(current);
    printf("Playlist deleted.\n");
}

void freeSong(Song* s) {
    //free all allocated memory of a song
    if (s) {
        free(s->title);
        free(s->artist);
        free(s->lyrics);
        free(s);
    }
}

void freePlaylist(Playlist* p) {
    //free all allocated memory of a playlist
    for(int i = 0; i < p->songsNum; i++)
        freeSong(p->songs[i]);
    free(p->songs);
    free(p->name);
    free(p);
}

void printPlaylistsOptions() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}



