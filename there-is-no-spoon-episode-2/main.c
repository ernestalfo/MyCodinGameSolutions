#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#define MAX_W 50
#define MAX_H 50
#define MAX_NODES 100

#define debout stderr
#define MAX_MOVS 500

int movs[500][5];
int i_mov = 0;
int Qnodes = 0;

struct square {
    int L; //initial Links
    int l; //Remaining links
    int rightN, leftN, topN, bottomN; //x or y coordinate of each neighbour
    int qR, qL, qT, qB; //for maths, potential links in each direction
    int strongQr, strongQl, strongQt, strongQb; //real connections
};

typedef struct square square;

struct islandInfo {
    int Qnodes;
    int remaining_links;
};
 typedef struct islandInfo islandInfo;

void initSquare(int row, int col, int L);

void printBoard(square **board);

void initBoard();

void printNodes(square** board);

void printNode(square *cas, int y, int x);

bool executeSureMoves(square **);

bool drawLink(int start_x, int start_y, int end_x, int end_y, int mov, square**);

void updSquare(square *cas, int links_done);

void printMoves();

bool solveGrid(square **board);

//bool IsDone (square **board);

int chckGrid (square **board);

//int travelMap(square **board,int curr_y, int curr_x, bool **visited);

islandInfo travelIsland(square **board,int start_y, int start_x, bool **visited);

square** cloneBoard(square** src);

void copyBoard(square **src, square **dest);

void freeBoard(square** src);

bool chckNode(square** board, int y, int x);

bool makeGuess(square **board, int *sx, int *sy, int *ex, int *ey);

bool makeEducatedGuess(square **board, int *sx, int *sy, int *ex, int *ey);

void drawLink_II(int start_x, int start_y, int end_x, int end_y, int mov, square **board);

void deleteGuess(square **board, int start_x, int start_y, int end_x, int end_y);

void cleanUnusedLinks(square** board, int start_x, int start_y, int end_x, int end_y);

//square *board;
square **initial_board;
int width, height;
int drawI = 0;
//--------------------------------------------------------------------------------------------
int main(void) {
    // Read dimensions
    scanf("%d", &width);
    scanf("%d", &height);

    initial_board = (square **) malloc(height * sizeof(square*));

    //xFinder and yFinder are helpful arrays to find the neighbours
    int *yFinder = malloc(width * sizeof(int));
    int *xFinder = malloc(height * sizeof(int));
    memset(yFinder, -1, sizeof(int)*width);
    memset(xFinder, -1, sizeof(int)*height);

    // Reading the grid and saving neighbour locations
    for (int y = 0; y < height; y++) {
        char line[MAX_W + 1];
        scanf("%s", line);
        initial_board[y] =  (square*) malloc(width * sizeof(square));

        for (int x = 0; x < width; x++) {
            if (line[x] != '.') {
                Qnodes++;
                initSquare(y, x, line[x] - '0');
                if(yFinder[x] != -1) {
                    initial_board[yFinder[x]][x].bottomN = y;
                    initial_board[y][x].topN = yFinder[x];
                }
                yFinder[x] = y;

                if(xFinder[y] != -1) {
                    initial_board[y][xFinder[y]].rightN = x;
                    initial_board[y][x].leftN = xFinder[y];
                }
                xFinder[y] = x;
            }
            else
                initSquare(y, x, 0);
        } //for
    }//for
    fprintf(debout, "Total nodes: %d\n", Qnodes);
    printBoard(initial_board);

    if (solveGrid(initial_board))
        fprintf(debout, "The grid was solved!\n");
    else
        fprintf(debout, "NO solution found.\n");

    freeBoard(initial_board);
    printMoves();

    return 0;
}
//--------------------------------------------------------------------------------------------
void initSquare(int row, int col, int L) {
    initial_board[row][col].L = L;
    initial_board[row][col].l = L;
    initial_board[row][col].rightN = -1;
    initial_board[row][col].leftN = -1;
    initial_board[row][col].bottomN = -1;
    initial_board[row][col].topN = -1;
    initial_board[row][col].qR = initial_board[row][col].qL = initial_board[row][col].qT = initial_board[row][col].qB = initial_board[row][col].l > 1 ? 2 : 1;
    initial_board[row][col].strongQr = initial_board[row][col].strongQl = 0;
    initial_board[row][col].strongQb = initial_board[row][col].strongQt = 0;
}
//--------------------------------------------------------------------------------------------
void updSquare(square *cas, int links_done)
{
    if (cas -> l == 1) {
        if (cas -> qR > 1)
            cas -> qR = 1;
        if (cas -> qL > 1)
            cas -> qL = 1;
        if (cas -> qT > 1)
            cas -> qT = 1;
        if (cas -> qB > 1)
            cas -> qB = 1;
    }
    else if (cas -> l == 0) {
        cas -> qR = cas -> qL = cas -> qT = cas -> qB = 0;
    }
}
//--------------------------------------------------------------------------------------------
void printBoard(square **board) {
    fprintf(debout, "Board, H: %d, W: %d\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x].L == 0)
                fprintf(debout, "  .");
            else
                //fprintf(stderr, "  %d", board[i].L);
                fprintf(debout, "  %d", board[y][x].l);
        }
        fprintf(debout, "\n");
    }
    fprintf(debout, "\n");
}
//--------------------------------------------------------------------------------------------
void printNode(square *cas, int y, int x)
{
    if (cas -> L > 0) {
        //if (cas -> l > 0) { //extra condition for debugging
            fflush(debout);
            fprintf(debout, "row: %d, col: %d\n", y, x);
            fflush(debout);
            fprintf(debout, "N links: %d, l: %d\n", cas -> L, cas -> l);
            fflush(debout);
            fprintf(debout, "right: %d, left: %d\n", cas -> rightN, cas -> leftN);
            fflush(debout);
            fprintf(debout, "top: %d, bottom: %d\n\n", cas -> topN, cas -> bottomN);
        //}
    }
}
// //--------------------------------------------------------------------------------------------
void printNodes(square **board)
{
    for (int i = 0; i < width * height; i++) {
        int curr_x = i % width;
        int curr_y = i / width;
        printNode(&board[curr_y][curr_x], curr_y, curr_x) ;
    }
}
// //--------------------------------------------------------------------------------------------
bool executeSureMoves(square **board)
{
    bool chg = false;
    for (int i = 0; i < height * width; i++) {
        int curr_x = i % width;
        int curr_y = i / width;
        bool changed = false;
        if (board[curr_y][curr_x].l > 0) {
            changed = drawLink(curr_x, curr_y, board[curr_y][curr_x].rightN, curr_y, 0, board);
            changed |= drawLink(curr_x, curr_y, board[curr_y][curr_x].leftN, curr_y, 1, board);
            changed |= drawLink(curr_x, curr_y, curr_x, board[curr_y][curr_x].bottomN, 2, board);
            changed |= drawLink(curr_x, curr_y, curr_x, board[curr_y][curr_x].topN, 3, board);
            if (changed) {
                //printBoard();
                chg = true;
            }
            //fprintf(debout, "Press 'c' to continue\n");
            //while (getchar() != 'c'){}
        }
    }//for
    return chg;
}
//--------------------------------------------------------------------------------------------
//Only for sure movements
bool drawLink(int start_x, int start_y, int end_x, int end_y, int mov, square **board)
{
    if (start_x == -1 || start_y == -1 || end_x == -1 || end_y == -1)
        return false;

    //Calculating S
    int S = 0;
    //int i = start_x * width + start_y;
    if (board[start_y][start_x].rightN != -1) S += board[start_y][board[start_y][start_x].rightN].qL;
    if (board[start_y][start_x].leftN != -1) S += board[start_y][board[start_y][start_x].leftN].qR;
    if (board[start_y][start_x].topN != -1) S += board[board[start_y][start_x].topN][start_x].qB;
    if (board[start_y][start_x].bottomN != -1) S += board[board[start_y][start_x].bottomN][start_x].qT;

    int *p_start, *p_end;
    int *strong_start, *strong_end;
    switch (mov) {
        case 0: //to right
            p_start = &board[start_y][start_x].qR;
            strong_start = &board[start_y][start_x].strongQr;
            p_end = &board[end_y][end_x].qL;
            strong_end = &board[end_y][end_x].strongQl;
            break;
        case 1: //to left
            p_start = &board[start_y][start_x].qL;
            strong_start = &board[start_y][start_x].strongQl;
            p_end = &board[end_y][end_x].qR;
            strong_end = &board[end_y][end_x].strongQr;
            break;
        case 2: //to bottom / down
            p_start = &board[start_y][start_x].qB;
            strong_start = &board[start_y][start_x].strongQb;
            p_end = &board[end_y][end_x].qT;
            strong_end = &board[end_y][end_x].strongQt;
            break;
        case 3: //to top / up
            p_start = &board[start_y][start_x].qT;
            strong_start = &board[start_y][start_x].strongQt;
            p_end = &board[end_y][end_x].qB;
            strong_end = &board[end_y][end_x].strongQb;
            break;
    }

    //Analyzing movement:
    int mV = board[start_y][start_x].l - S + *p_end; //magic Value
    if (mV < 1)
        return false;

    //decrementing links:
    board[start_y][start_x].l -= mV;
    board[end_y][end_x].l -= mV;
    if (*p_start >= mV)
        *p_start-= mV;
    else
        *p_start = 0;
    if (*p_end >= mV)
        *p_end-= mV;
    else
        *p_end = 0;

    *strong_start += mV;
    *strong_end += mV;

    updSquare(&board[start_y][start_x], mV);
    updSquare(&board[end_y][end_x], mV);

    movs[i_mov][0] = start_x;
    movs[i_mov][1] = start_y;
    movs[i_mov][2] = end_x;
    movs[i_mov][3] = end_y;
    movs[i_mov++][4] = mV;

    cleanUnusedLinks(board, start_x, start_y, end_x, end_y);

    return true;
}
//--------------------------------------------------------------------------------------------
void printMoves()
{
    for (int i = 0; i < i_mov; i++) {
        printf("%d %d %d %d %d\n", movs[i][0], movs[i][1], movs[i][2], movs[i][3], movs[i][4]);
    }
}
//--------------------------------------------------------------------------------------------
bool solveGrid(square **board)
{
    static int counter, depth_level;
    counter++;

    while (executeSureMoves(board)) {} //comment or uncomment to disable or enable this heuristic part

    int r = chckGrid(board);
    if (!r) //r == 0 //problem solved!!!
        return true;

    if (r == 1) { //r == 1 incomplete
        square** nxtBoard = cloneBoard(board);
        int bk_iMov = i_mov;

        //Make a guess:
        int sx, sy, ex, ey; //  - Save kind of move, to upd as incorrect or not. (sx, sy, ex, ey)
        bool guess;
        do {
            guess = makeGuess(nxtBoard, &sx, &sy, &ex, &ey);
            //guess = makeEducatedGuess(nxtBoard, &sx, &sy, &ex, &ey);
            if (guess) {
                fprintf(debout, "i: %d, DEPTH: %d\n", counter, depth_level);
                depth_level++;
                if (solveGrid(nxtBoard)) {
                    freeBoard(nxtBoard);
                    return true;
                }
                depth_level--;
                //Eliminate the last wrong guess
                deleteGuess(board, sx, sy, ex, ey);
                //Retrieve previous state:
                copyBoard(board, nxtBoard); //now without the last wrong guess
                //Retrieve the moves counter
                i_mov = bk_iMov;
            }
        }while (guess);
        freeBoard(nxtBoard);
        return false; //no more possible guesses
    }//r == 1

    return false; //r == 2, faulty
}
//--------------------------------------------------------------------------------------------
int chckGrid (square **board)
{
    int result = 1; //incomplete

    bool** visited = (bool**) malloc(height * sizeof(bool*));

    if (visited == NULL) {
        // Handle malloc failure, e.g., print error and exit
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        visited[i] = (bool*) malloc(width * sizeof(bool));
        if (visited[i] == NULL) {
            // Handle malloc failure, e.g., print error and exit
            printf("Memory allocation failed\n");
            exit(1);
        }
        for (int j = 0; j < width; j++)
            visited[i][j] = false;
    }
    islandInfo island_data;
    for (int i = 0; i < width * height; i++) {
        int curr_x = i % width;
        int curr_y = i / width;
        if (board[curr_y][curr_x].L != 0 && !visited[curr_y][curr_x]) {
            //check island:
            island_data = travelIsland(board, curr_y, curr_x, visited);
            if (island_data.Qnodes == Qnodes && island_data.remaining_links == 0) {
                result = 0; //completed
                break;
            }
            if (island_data.Qnodes < Qnodes && island_data.remaining_links == 0){
                result = 2; //faulty
                break;
            }
            //check links and potential neighbours links:
            if (!chckNode(board, curr_y, curr_x)) {
                result = 2; //faulty
                break;
            }
        }
    } //for

    //free visited array:
    for (int i = 0; i < height; i++)
        free(visited[i]);
    free(visited);

    return result;
}
//--------------------------------------------------------------------------------------------
islandInfo travelIsland(square **board,int start_y, int start_x, bool **visited)
{
    int nodes = 0;
    visited[start_y][start_x] = true;

    islandInfo Result;
    Result.Qnodes = 0;
    Result.remaining_links = 0;

    Result.remaining_links += board[start_y][start_x].l;
    Result.Qnodes += 1;

    //if (board[start_y][start_x].strongQr > 0 && !visited[start_y][board[start_y][start_x].rightN]) {
    if (board[start_y][start_x].strongQr > 0 &&  board[start_y][start_x].rightN != -1) {
        if (!visited[start_y][board[start_y][start_x].rightN]) {
            islandInfo tmp;
            tmp = travelIsland(board, start_y, board[start_y][start_x].rightN, visited);
            Result.remaining_links += tmp.remaining_links;
            Result.Qnodes += tmp.Qnodes;
        }
    }

    //if (board[start_y][start_x].strongQl > 0 && !visited[start_y][board[start_y][start_x].leftN]) {
    if (board[start_y][start_x].strongQl > 0 && board[start_y][start_x].leftN != -1) {
        if (!visited[start_y][board[start_y][start_x].leftN]) {
            islandInfo tmp;
            tmp = travelIsland(board, start_y, board[start_y][start_x].leftN, visited);
            Result.remaining_links += tmp.remaining_links;
            Result.Qnodes += tmp.Qnodes;
        }
    }

    //if (board[start_y][start_x].strongQb > 0 && !visited[board[start_y][start_x].bottomN][start_x]) {
    if (board[start_y][start_x].strongQb > 0 && board[start_y][start_x].bottomN != -1) {
        if (!visited[board[start_y][start_x].bottomN][start_x]) {
            islandInfo tmp;
            tmp = travelIsland(board, board[start_y][start_x].bottomN, start_x, visited);
            Result.remaining_links += tmp.remaining_links;
            Result.Qnodes += tmp.Qnodes;
        }
    }

    //if (board[start_y][start_x].strongQt > 0 && !visited[board[start_y][start_x].topN][start_x]) {
    if (board[start_y][start_x].strongQt > 0 && board[start_y][start_x].topN != -1) {
        if ( !visited[board[start_y][start_x].topN][start_x]) {
            islandInfo tmp;
            tmp = travelIsland(board, board[start_y][start_x].topN, start_x, visited);
            Result.remaining_links += tmp.remaining_links;
            Result.Qnodes += tmp.Qnodes;
        }
    }

    return Result;
}
//--------------------------------------------------------------------------------------------
bool chckNode(square** board, int y, int x)
{
    int pLinks = 0; //potential links
    if (board[y][x].rightN != -1)
        pLinks += board[y][board[y][x].rightN].qL;
    if (board[y][x].leftN != -1)
        pLinks += board[y][board[y][x].leftN].qR;
    if (board[y][x].bottomN != -1)
        pLinks += board[board[y][x].bottomN][x].qT;
    if (board[y][x].topN != -1)
        pLinks += board[board[y][x].topN][x].qB;

    return board[y][x].l <= pLinks;
}
//--------------------------------------------------------------------------------------------
square** cloneBoard(square** src)
{
    square** dest = (square**) malloc(sizeof(square*) * height);
    for (int i = 0; i < height; i++) {
        dest[i] = (square*) malloc(sizeof(square) * width);
        memcpy(dest[i], src[i], width * sizeof(square));
    }
    return dest;
}
//--------------------------------------------------------------------------------------------
void copyBoard(square **src, square **dest)
{
    for (int i = 0; i < height; i++)
        memcpy(dest[i], src[i], width * sizeof(square));
}
//--------------------------------------------------------------------------------------------
void freeBoard(square** src)
{
    for (int i = 0; i < height; i++)
        free(src[i]);
    free(src);
}
//--------------------------------------------------------------------------------------------
void deleteGuess(square **board, int start_x, int start_y, int end_x, int end_y)
{
    if (start_x == end_x) { //vertical movement
        if (end_y > start_y) { //down move, to bottom
            board[start_y][start_x].qB = 0;
            board[end_y][end_x].qT = 0;
        }
        else { //up move, to top
            board[start_y][start_x].qT = 0;
            board[end_y][end_x].qB = 0;
        }
    }
    else if (start_y == end_y) { //horizontal movement
        if (end_x > start_x) { //right move
            board[start_y][start_x].qR = 0;
            board[end_y][end_x].qL = 0;
        }
        else { //left move
            board[start_y][start_x].qL = 0;
            board[end_y][end_x].qR = 0;
        }
    }
}
//--------------------------------------------------------------------------------------------
bool makeGuess(square **board, int *sx, int *sy, int *ex, int *ey)
{
    for (int i = 0; i < width * height; i++) {
        int curr_x = i % width;
        int curr_y = i / width;
        if (board[curr_y][curr_x].l > 0) {
            //left some links...
            if (board[curr_y][curr_x].rightN != -1 && board[curr_y][curr_x].qR) { //to right
                if (board[curr_y][board[curr_y][curr_x].rightN].qL && board[curr_y][board[curr_y][curr_x].rightN].l) {
                    *sy = *ey = curr_y;
                    *sx = curr_x;
                    *ex = board[curr_y][curr_x].rightN;
                    drawLink_II(*sx, *sy, *ex, *ey, 0, board);
                    return true;
                }
            }

            if (board[curr_y][curr_x].leftN != -1 && board[curr_y][curr_x].qL) { //to left
                if (board[curr_y][board[curr_y][curr_x].leftN].qR && board[curr_y][board[curr_y][curr_x].leftN].l) {
                    *sy = *ey = curr_y;
                    *sx = curr_x;
                    *ex = board[curr_y][curr_x].leftN;
                    drawLink_II(*sx, *sy, *ex, *ey, 1, board);
                    return true;
                }
            }

            if (board[curr_y][curr_x].bottomN != -1 && board[curr_y][curr_x].qB) { //to bottom , down
                if (board[board[curr_y][curr_x].bottomN][curr_x].qT && board[board[curr_y][curr_x].bottomN][curr_x].l) {
                    *sx = *ex = curr_x;
                    *sy = curr_y;
                    *ey = board[curr_y][curr_x].bottomN;
                    drawLink_II(*sx, *sy, *ex, *ey, 2, board);
                    return true;
                }
            }

            if (board[curr_y][curr_x].topN != -1 && board[curr_y][curr_x].qT) { //to top, up
                if (board[board[curr_y][curr_x].topN][curr_x].qB && board[board[curr_y][curr_x].topN][curr_x].l) {
                    *sx = *ex = curr_x;
                    *sy = curr_y;
                    *ey = board[curr_y][curr_x].topN;
                    drawLink_II(*sx, *sy, *ex, *ey, 3, board);
                    return true;
                }
            }
        }// if (board[curr_y][curr_x].l > 0)
    }//for
    return false;
}
//--------------------------------------------------------------------------------------------
//mov:
//0: to right
//1: to left
//2: to bottom / down
//3: to top / up
void drawLink_II(int start_x, int start_y, int end_x, int end_y, int mov, square **board)
{
    if (start_x == -1 || start_y == -1 || end_x == -1 || end_y == -1)
        return;

    int *p_start, *p_end;
    int *strong_start, *strong_end;
    switch (mov) {
        case 0: //to right
            p_start = &board[start_y][start_x].qR;
            strong_start = &board[start_y][start_x].strongQr;
            p_end = &board[end_y][end_x].qL;
            strong_end = &board[end_y][end_x].strongQl;
            break;
        case 1: //to left
            p_start = &board[start_y][start_x].qL;
            strong_start = &board[start_y][start_x].strongQl;
            p_end = &board[end_y][end_x].qR;
            strong_end = &board[end_y][end_x].strongQr;
            break;
        case 2: //to bottom / down
            p_start = &board[start_y][start_x].qB;
            strong_start = &board[start_y][start_x].strongQb;
            p_end = &board[end_y][end_x].qT;
            strong_end = &board[end_y][end_x].strongQt;
            break;
        case 3: //to top / up
            p_start = &board[start_y][start_x].qT;
            strong_start = &board[start_y][start_x].strongQt;
            p_end = &board[end_y][end_x].qB;
            strong_end = &board[end_y][end_x].strongQb;
            break;
    }

    //decrementing links:
    board[start_y][start_x].l -= 1;
    board[end_y][end_x].l -= 1;
    if (*p_start >= 1)
        *p_start-= 1;
    else
        *p_start = 0;
    if (*p_end >= 1)
        *p_end-= 1;
    else
        *p_end = 0;

    *strong_start += 1;
    *strong_end += 1;

    updSquare(&board[start_y][start_x], 1);
    updSquare(&board[end_y][end_x], 1);

    movs[i_mov][0] = start_x;
    movs[i_mov][1] = start_y;
    movs[i_mov][2] = end_x;
    movs[i_mov][3] = end_y;
    movs[i_mov++][4] = 1;

    cleanUnusedLinks(board, start_x, start_y, end_x, end_y);
}
//--------------------------------------------------------------------------------------------
void cleanUnusedLinks(square **board, int start_x, int start_y, int end_x, int end_y)
{
    //Looking for potential broken links:
    if (start_x > end_x) {
        int temp = start_x;
        start_x = end_x;
        end_x = temp;
    }
    if (start_y > end_y) {
        int temp = start_y;
        start_y = end_y;
        end_y = temp;
    }
    if (start_y == end_y) { //horizontal movement
        for (int col = start_x + 1; col < end_x; col++) {
            for (int row = start_y - 1; row >= 0; row--) {
                if (board[row][col].L > 0) {
                    board[row][col].bottomN = -1;
                    break;
                }
            }//rows above start_y
            for (int row = start_y + 1; row < height; row++) {
                if (board[row][col].L > 0) {
                    board[row][col].topN = -1;
                    break;
                }
            } //rows below start_y
        }//cols
    }else if (start_x == end_x) { //vertical movement
        for (int row = start_y + 1; row < end_y; row++) {
            for (int col = start_x - 1; col >= 0; col--) {
                if (board[row][col].L > 0) {
                    board[row][col].rightN = -1;
                    break;
                }
            }//cols left to start_x
            for (int col = start_x + 1; col < width; col++) {
                if (board[row][col].L > 0) {
                    board[row][col].leftN = -1;
                    break;
                }
            } //rows right to start_x
        }//rows
    }
}
//--------------------------------------------------------------------------------------------
bool makeEducatedGuess(square **board, int *sx, int *sy, int *ex, int *ey)
{
    int start_x = -1, start_y = -1;
    int end_x = -1, end_y = -1;
    int startL = 8, endL = 0;
    int Emov = -1;

    for (int i = 0; i < width * height; i++) {
        int curr_x = i % width;
        int curr_y = i / width;
        if (board[curr_y][curr_x].l > 0) {
            //left some links...
            if (board[curr_y][curr_x].rightN != -1 && board[curr_y][curr_x].qR) { //to right
                if (board[curr_y][board[curr_y][curr_x].rightN].qL && board[curr_y][board[curr_y][curr_x].rightN].l) {
                    if (board[curr_y][board[curr_y][curr_x].rightN].l >= endL && board[curr_y][curr_x].l <= startL) {
                        *sy = *ey = curr_y;
                        *sx = curr_x;
                        *ex = board[curr_y][curr_x].rightN;
                        Emov = 0; //to right
                        endL = board[*ey][*ex].l;
                        startL = board[*sy][*sx].l;
                    }
                }
            }

            if (board[curr_y][curr_x].leftN != -1 && board[curr_y][curr_x].qL) { //to left
                if (board[curr_y][board[curr_y][curr_x].leftN].qR && board[curr_y][board[curr_y][curr_x].leftN].l) {
                    if (board[curr_y][board[curr_y][curr_x].leftN].l >= endL && board[curr_y][curr_x].l <= startL) {
                        *sy = *ey = curr_y;
                        *sx = curr_x;
                        *ex = board[curr_y][curr_x].leftN;
                        Emov = 1; //to left
                        endL = board[*ey][*ex].l;
                        startL = board[*sy][*sx].l;
                    }
                }
            }

            if (board[curr_y][curr_x].bottomN != -1 && board[curr_y][curr_x].qB) { //to bottom , down
                if (board[board[curr_y][curr_x].bottomN][curr_x].qT && board[board[curr_y][curr_x].bottomN][curr_x].l) {
                    if (board[board[curr_y][curr_x].bottomN][curr_x].l >= endL && board[curr_y][curr_x].l <= startL) {
                        *sx = *ex = curr_x;
                        *sy = curr_y;
                        *ey = board[curr_y][curr_x].bottomN;
                        Emov = 2; //to bottom
                        endL = board[*ey][*ex].l;
                        startL = board[*sy][*sx].l;
                    }
                }
            }

            if (board[curr_y][curr_x].topN != -1 && board[curr_y][curr_x].qT) { //to top, up
                if (board[board[curr_y][curr_x].topN][curr_x].qB && board[board[curr_y][curr_x].topN][curr_x].l) {
                    if (board[board[curr_y][curr_x].topN][curr_x].l >= endL && board[curr_y][curr_x].l <= startL) {
                        *sx = *ex = curr_x;
                        *sy = curr_y;
                        *ey = board[curr_y][curr_x].topN;
                        Emov = 3; //to up
                        endL = board[*ey][*ex].l;
                        startL = board[*sy][*sx].l;
                    }
                }
            }
        }// if (board[curr_y][curr_x].l > 0)
    }//for
    if (Emov != -1) {
        drawLink_II(*sx, *sy, *ex, *ey, Emov, board);
        return true;
    }
    else
        return false;
}
