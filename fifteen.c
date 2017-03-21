/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//tile positions
int a_i,a_j,b_i,b_j;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
//void set(void);
//void shuffle(void);
void board_swap(int,int);
bool move_check(int);
void get_pos(int,int);

int main(int argc, string argv[]){
    // ensure proper usage
    if (argc != 2){
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX){
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL){
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true){
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++){
            for (int j = 0; j < d; j++){
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1){
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won()){
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0){
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile)){
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void){
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void){
    int i,j;
    for(i=0;i<d;i++){
        for(j=0;j<d;j++){
            board[i][j]=d*d-(d*i+j+1);
        }
    }
    if((d%2)==0) board_swap(1,2);
}

/**
 * Prints the board in its current state.
 */
void draw(void){
    int i,j;
    for(i=0;i<d;i++){
        for(j=0;j<d;j++){
            if(board[i][j]!=0) printf("  %2i",board[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile){
    if(move_check(tile)){
        board_swap(tile,0);
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void){
    int i,j;
    for(i=0;i<d;i++){
        for(j=0;j<d;j++){
            if((i==(d-1))&&(j==(d-1))){
                break;
            }
            else{
                if(board[i][j]!=d*i+j+1){
                    return false;
                }
            }
        }
    }
    return true;
}

/*void shuffle(void){
    for(k=d*d;k>0;k--){
        swap(k,rand()%(k));
    }
}

void set(void){
    for(k=0;k<d*d;k++){
        dummy[k]=k;
    }
}

void swap(int sw_1,int sw_2){
    int temp;
    temp=dummy[sw_1];
    dummy[sw_1]=dummy[sw_2];
    dummy[sw_2]=temp;
}*/

/**Swaps positions of a and b o the board*/
void board_swap(int a,int b){
    int temp;
    get_pos(a,b);
    temp=board[a_i][a_j];
    board[a_i][a_j]=board[b_i][b_j];
    board[b_i][b_j]=temp;
}

bool move_check(int tile){
    get_pos(tile,0);
    int tile_i,tile_j;
    tile_i=a_i;
    tile_j=a_j;
    if(((tile_i==b_i)&&(((tile_j-b_j)==1)||((tile_j-b_j)==(-1)))) || ((tile_j==b_j)&&(((tile_i-b_i)==1)||((tile_i-b_i)==(-1))))){
        return true;
    }
    return false;
}

void get_pos(int a,int b){
    int i,j;
    for(i=0;i<d;i++){
        for(j=0;j<d;j++){
            if(board[i][j]==a){
                a_i=i;
                a_j=j;
            }
            if(board[i][j]==b){
                b_i=i;
                b_j=j;
            }
        }

    }
}