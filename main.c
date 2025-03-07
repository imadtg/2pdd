#include <stdio.h>
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
    fflush(stdout);
    scanf("%d", &g->turn);
    int n, nplypck, npck, cant_pass, ai_play, skip, depth;
    float (*ai_function)(Game *, int, int, int *);
    float unplayable_prob, pass_prob;
    enum Mode ai_mode;
    Move moves[DCOUNT], playable_picking_moves[DCOUNT], picking_moves[DCOUNT], move;
    do {
        print_game(g);
        get_playing_moves(g, moves, &n, &cant_pass);
        print_playing_moves(moves, n);
        printf("%d moves\n", n);
        get_playable_perfect_picking_moves(g, playable_picking_moves, &nplypck);
        get_perfect_picking_moves(g, picking_moves, &npck);
        pass_prob = pass_probability_from_num_moves(g, n);
        pass_probability_from_num_moves(g, n);
        print_picking_moves(picking_moves, npck);
        print_picking_moves(playable_picking_moves, nplypck);
        printf("pass prob = %f\n", pass_prob);
        unplayable_prob = pick_unplayable_domino_probability_from_moves(g, playable_picking_moves, nplypck);
        printf("unplayable pick prob = %f\n", unplayable_prob);
        if(cant_pass) printf("cant pass\n");
        if(n == 0){
            if(boneyard_is_pickable(&g->hands)){
                if(hand_is_solid(NP, &g->hands) || hand_is_liquid(g->turn, &g->hands)){
                    if(is_passing(g, NP)){
                        Move pick_all_of_boneyard;
                        pick_all_of_boneyard.type = IMPERFECT_PICK;
                        pick_all_of_boneyard.imperfect_pick.count = g->hands.hand_sizes[NP];
                        imperfect_pick(g, pick_all_of_boneyard);
                        continue;
                    }
                    else if(g->hands.hand_sizes[NP] == 1){
                        perfect_pick(g, picking_moves[0]);
                        continue;
                    }
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
            printf("give move type (left = %d, right = %d, perf pick = %d, imp pick = %d, pass = %d)(AI = -1): ", LEFT, RIGHT, PERFECT_PICK, IMPERFECT_PICK, PASS);
        else
            printf("give move type (left = %d, right = %d)(AI = -1): ", LEFT, RIGHT);
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
            if(cant_pass || !boneyard_is_pickable(&g->hands)){
                fflush(stdin);
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
            if(cant_pass || !boneyard_is_pickable(&g->hands)){
                fflush(stdin);
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
        case -1: // AI move, don't know if i should enumerate in enum Move.
            if(n == 0){
                fflush(stdin);
                printf("no moves to choose AI move from\n");
                break;
            }
            printf("give ai mode (pessimist = %d, expect = %d): ", PESSIMIST, EXPECT);
            scanf("%d", &ai_mode);
            printf("skip exploring picking positions?: ");
            scanf("%d", &skip);
            printf("give depth of search (0 is iterative deepening (windows only), negative is infinite): ");
            scanf("%d", &depth);
            switch(ai_mode){
            case PESSIMIST:
                ai_function = minimax;
                break;
            case EXPECT:
                ai_function = expectiminimax;
                break;
            }
            #ifdef _WIN32
            if(depth != 0) move = best_move(g, moves, NULL, n, depth, skip, NULL, ai_function);
            else move = iterative_deepening(g, moves, n, skip, ai_function);
            #else
            if(depth == 0){
                printf("iterative deepening is only available in windows\n");
                break;
            }
            move = best_move(g, moves, NULL, n, depth, skip, NULL, ai_function);
            #endif

            printf("play the move?: ");
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
    while(1){
        start();
        getchar();
    }
    return 0;
}