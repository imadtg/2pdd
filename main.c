#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "game.h"
#include "minimax.h"

void start(){
    Game *g = malloc(sizeof(Game));
    init_game(g);
    get_hand_sizes(&g->hands);
    get_hand(&g->hands, 1);
    printf("Game starts with turn : ");
    scanf("%d", &g->turn);
    int n, cant_pass, ai_play, depth;
    enum Mode ai_mode;
    Move moves[DCOUNT], move;
    do {
        print_game(g);
        get_moves(g, moves, &n, &cant_pass);
        print_moves(moves, n);
        printf("%d moves\n", n);
        if(cant_pass) printf("cant pass\n");
        if(n == 0){
            if(g->hands.hand_sizes[NP]){
                if(is_passing(g, NP)){
                    Move pick_all_of_boneyard;
                    pick_all_of_boneyard.type = IMPERFECT_PICK;
                    pick_all_of_boneyard.imperfect_pick.count = g->hands.hand_sizes[NP];
                    imperfect_pick(g, pick_all_of_boneyard);
                    continue;
                }
            } else {
                pass(g);
                continue;
            }
        } else if (cant_pass && n == 1){
            play_move(g, moves[0]);
            continue;
        }
        if(!cant_pass && n == 0)
            printf("give move type (perf pick = %d, imp pick = %d, pass = %d): ", PERFECT_PICK, IMPERFECT_PICK, PASS);
        else if(!cant_pass)
            printf("give move type (left = %d, right = %d, perf pick = %d, imp pick = %d, pass = %d)(AI def): ", LEFT, RIGHT, PERFECT_PICK, IMPERFECT_PICK, PASS);
        else
            printf("give move type (left = %d, right = %d)(AI def): ", LEFT, RIGHT);
        scanf("%d", &move.type);
        switch(move.type){
        case LEFT:
        case RIGHT:
            scanf("%d %d", &move.play.left, &move.play.right);
            if(playable_move(&g->snake, move.type, move.play.left, move.play.right) && possible_possession(g->turn, &g->hands, move.play.left, move.play.right)){
                play_move(g, move);
            } else {
                printf("invalid move\n");
            }
            break;
        case PERFECT_PICK:
            if(cant_pass){
                printf("invalid move\n");
                break;
            }
            scanf("%d %d", &move.perfect_pick.left, &move.perfect_pick.right);
            if(possible_possession(NP, &g->hands, move.perfect_pick.left, move.perfect_pick.right))
                perfect_pick(g, move);
            else
                printf("invalid move\n");
            break;
        case IMPERFECT_PICK:
            if(cant_pass){
                printf("invalid move\n");
                break;
            }
            scanf("%d", &move.imperfect_pick.count);
            if(move.imperfect_pick.count > 0 && move.imperfect_pick.count <= g->hands.hand_sizes[NP])
                imperfect_pick(g, move);
            else
                printf("invalid move\n");
            break;
        case PASS:
            if(cant_pass){
                printf("invalid move\n");
                break;
            }
            pass(g);
            break;
        default: // play AI move
            if(n == 0){
                printf("reading three ints (consistency):");
                scanf("%*d %*d %*d");
                printf("no moves to choose AI move from\n");
                break;
            }
            printf("give ai mode (pessimist = %d, expect = %d): ", PESSIMIST, EXPECT);
            scanf("%d", &ai_mode);
            printf("give depth of search (-1 = inf): ");
            scanf("%d", &depth);
            switch(ai_mode){
            case PESSIMIST:
                move = best_move(g, depth, minimax);
                break;
            case EXPECT:
                move = best_move(g, depth, expectiminimax);
                break;
            }
            printf("play the move? : ");
            scanf("%d", &ai_play);
            printf("\n");
            if(ai_play){
                play_move(g, move);
            }
        }
    } while(!over(g));
    print_game(g);
    printf("game over, score: %f\n", endgame_evaluation(g));
}

int main(){
    init_fact();
    start();
    getch();
    return 0;
}