/*
 * C/C++ implementation of a simple Othello game.
 * We make use of Mini-Max with Alpha Beta Pruning to evaluate our heuristic.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
using namespace std;

#define GAME_BOARD_SIZE 8

vector< pair<int, int> > get_directions(int game[][GAME_BOARD_SIZE], int x, int y, int player) {
    vector< pair<int, int> > directions;
    if(game[x][y]) {
        return directions;
    }

    if((y < 6) && (game[x][y+1] == -player)) {
        for(int i=y+2; i < GAME_BOARD_SIZE; i++) {
            if(!game[x][i]) {
                break;
            } else if(game[x][i] == player) {
                directions.push_back(make_pair<int, int>(0, x*GAME_BOARD_SIZE+i));
                break;
            }
        }
    }
    
    if((y < 6) && (x < 6) && (game[x+1][y+1] == -player)) {
        for(int i=0; (x+i+2 < GAME_BOARD_SIZE) && (y+i+2 < GAME_BOARD_SIZE); i++) {
            if(!game[x+i+2][y+i+2]) {
                break;
            } else if(game[x+i+2][y+i+2] == player) {
                directions.push_back(make_pair<int, int>(1, (x+i+2)*GAME_BOARD_SIZE+(y+i+2)));
                break;
            }
        }
    }
  
    if((x < 6) && (game[x+1][y] == -player)) {
        for(int i=x+2; i < GAME_BOARD_SIZE; i++) {
            if(!game[i][y]) {
                break;
            } else if(game[i][y] == player) {
                directions.push_back(make_pair<int, int>(2, i*GAME_BOARD_SIZE+y));
                break;
            }
        }
    }
  
    if((y > 1) && (x < 6) && (game[x+1][y-1] == -player)) {
        for(int i=0; (x+i+2 < GAME_BOARD_SIZE) && (y-i-2 >= 0); i++) {
            if(!game[x+i+2][y-i-2]) {
                break;
            } else if(game[x+i+2][y-i-2] == player) {
                directions.push_back(make_pair<int, int>(3, (x+i+2)*GAME_BOARD_SIZE+(y-i-2)));
                break;
            }
        }
    }

    if((y > 1) && (game[x][y-1] == -player)) {
        for(int i=y-2; i >= 0; i--) {
            if(!game[x][i]) {
                break;
            } else if(game[x][i] == player) {
                directions.push_back(make_pair<int, int>(4, x*GAME_BOARD_SIZE+i));
                break;
            }
        }
    }

    if((y > 1) && (x > 1) && (game[x-1][y-1] == -player)) {
        for(int i=0; (x-i-2 >= 0) && (y-i-2 >= 0); i++) {
            if(!game[x-i-2][y-i-2]) {
                break;
            } else if(game[x-i-2][y-i-2] == player) {
                directions.push_back(make_pair<int, int>(5, (x-i-2)*GAME_BOARD_SIZE+(y-i-2)));
                break;
            }
        }
    }

    if((x > 1) && (game[x-1][y] == -player)) {
        for(int i=x-2; i >= 0; i--) {
            if(!game[i][y]) {
                break;
            } else if(game[i][y] == player) {
                directions.push_back(make_pair<int, int>(6, i*GAME_BOARD_SIZE+y));
                break;
            }
        }
    }

    if((y < 6) && (x > 1) && (game[x-1][y+1] == -player)) {
        for(int i=0; (x-i-2 >= 0) && (y+i+2 < GAME_BOARD_SIZE); i++) {
            if(!game[x-i-2][y+i+2]) {
                break;
            } else if(game[x-i-2][y+i+2] == player) {
                directions.push_back(make_pair<int, int>(7, (x-i-2)*GAME_BOARD_SIZE+(y+i+2)));
                break;
            }
        }
    }
    return directions;
}


vector< pair<int, vector< pair<int, int> > > > get_turns(int game[][GAME_BOARD_SIZE], int player) {
    vector< pair<int, vector< pair<int, int> > > > turns;
    for(int i=0; i < GAME_BOARD_SIZE; i++) {
        for(int j=0; j < GAME_BOARD_SIZE; j++) {
            turns.push_back(make_pair<int, vector< pair<int, int> > >(i*GAME_BOARD_SIZE+j, get_directions(game, i, j, player)));
            if(!turns.back().second.size()) {
                turns.pop_back();
            }
        }
    }
    return turns;
}


void make_move(int game[][GAME_BOARD_SIZE], int x, int y, int player, vector< pair<int, int> > directions) {
    for(auto it=directions.begin(); it != directions.end(); it++) {
        int i = x;
        int j = y;
        while((i != ((*it).second/GAME_BOARD_SIZE)) || (j != ((*it).second&7))) {
            game[i][j] = player;
            if(i < ((*it).second/GAME_BOARD_SIZE)) {
                i++;
            } else if((i > (*it).second/GAME_BOARD_SIZE)) {
                i--;
            }
            if(j < ((*it).second&7)) {
                j++;
            } else if(j > ((*it).second&7)) {
                j--;
            }
        }
    }
}


void undo_move(int game[][GAME_BOARD_SIZE], int x, int y, int player, vector< pair<int, int> > directions) {
    for(auto it=directions.begin(); it != directions.end(); it++) {
        int i = x;
        int j = y;
        while((i != ((*it).second/GAME_BOARD_SIZE)) || (j != ((*it).second&7))) {
            game[i][j] = -player;
            if(i < ((*it).second/GAME_BOARD_SIZE)) {
                i++;
            } else if((i > (*it).second/GAME_BOARD_SIZE)) {
                i--;
            }
            if(j < ((*it).second&7)) {
                j++;
            } else if(j > ((*it).second&7)) {
                j--;
            }
        }
        game[i][j] = player;
    }
    game[x][y] = 0;
}

void print(int game[][GAME_BOARD_SIZE]) {
    for(int i=7; i >= 0; i--) {
        printf("%d ", i);
        for(int j=0; j < GAME_BOARD_SIZE; j++) {
            if(!game[j][i]) {
                printf("-");
            } else if(game[j][i] == 1) {
                printf("O");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
    printf("  ");
    for(int i=0; i < GAME_BOARD_SIZE; i++) {
        printf("%d", i);
    }
    printf("\n\n");
}

int score(int game[][GAME_BOARD_SIZE], int player) {
    int sum = 0;
    for(int i=0; i < GAME_BOARD_SIZE; i++) {
        for(int j=0; j < GAME_BOARD_SIZE; j++) {
            sum += game[i][j];
        }
    }
    return sum * player;
}


int heuristic_util(int game[][GAME_BOARD_SIZE], int player, int depth, int alpha, int beta) {
    if(depth == 0) {
        return score(game, player);
    }
    vector< pair<int, vector< pair<int, int> > > > turns = get_turns(game, player);
    if(turns.size() == 0) {
        if(get_turns(game, -player).size() == 0) {
            return score(game, player);
        }
        int ret= -heuristic_util(game, -player, depth-1, -beta, -alpha);
        if(ret>= beta) {
            return ret;
        }
        if(ret> alpha) {
            alpha = ret;
        }
    } else {
        for(auto it=turns.begin(); it != turns.end(); it++) {
            make_move(game, (*it).first/GAME_BOARD_SIZE, (*it).first&7, player, (*it).second);
            int ret= -heuristic_util(game, -player, depth-1, -beta, -alpha);
            undo_move(game, (*it).first/GAME_BOARD_SIZE, (*it).first&7, player, (*it).second);
            if(ret>= beta) {
                return ret;
            }
            if(ret> alpha) {
                alpha = ret;
            }
        }
    }
    return alpha;
}

int heuristic(int game[][GAME_BOARD_SIZE], int player, int depth) {
    int alpha = -65;
    int beta = 65;
    vector< pair<int, vector< pair<int, int> > > > turns = get_turns(game, player);
    int move = turns[0].first;
    for(auto it=turns.begin(); it != turns.end(); it++) {
        make_move(game, (*it).first/GAME_BOARD_SIZE, (*it).first&7, player, (*it).second);
        int ret= -heuristic_util(game, -player, depth-1, -beta, -alpha);
        undo_move(game, (*it).first/GAME_BOARD_SIZE, (*it).first&7, player, (*it).second);
        if(ret>= beta) {
            return (*it).first;
        }
        if(ret> alpha) {
            alpha = ret;
            move = (*it).first;
        }
    }
    return move;
}

int main(int argc, char **argv)
{
    int depth = 3;
    if(argc > 1) {
        depth = atol(argv[1]);
    }
    depth *= 2;

    int game[GAME_BOARD_SIZE][GAME_BOARD_SIZE];
    memset(game, 0, sizeof(game));
    game[3][3] = game[4][4] = -1;
    game[3][4] = game[4][3] = 1;
    int p = -1;
    while(true) {
        print(game);
        vector< pair<int, vector< pair<int, int> > > > turns= get_turns(game, p);
        printf("available turns: ");
        for(auto it=turns.begin(); it != turns.end(); it++) {
            printf("(%d, %d)  ", (*it).first/GAME_BOARD_SIZE, (*it).first%GAME_BOARD_SIZE);
        }
        printf("\n");
        if(turns.size() == 0) {
            p = -p;
            turns = get_turns(game, p);
            if(turns.size() == 0) {
                printf("final score: %d\n", score(game, -1));
                return 0;
            }
        } else {
            int x, y;
            if(p == 1) {
                scanf("%d %d", &x, &y);
                for(auto it=turns.begin(); it != turns.end(); it++) {
                    if(x*GAME_BOARD_SIZE+y == ((*it).first)) {
                        printf("move chosen: %d %d\n", x, y);
                        make_move(game, x, y, p, (*it).second);
                        p = -p;
                        break;
                    }
                }
            } else {
                x = heuristic(game, p, depth);
                for(auto it=turns.begin(); it != turns.end(); it++) {
                    if(x == ((*it).first)) {
                        printf("move chosen: %d %d\n", x/GAME_BOARD_SIZE, x%GAME_BOARD_SIZE);
                        make_move(game, x/GAME_BOARD_SIZE, x%GAME_BOARD_SIZE, p, (*it).second);
                        p = -p;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}