// this is a file only used to expose some utility functions to be able to use with wasm output of emcc
// this file should only appear if compiled with emcc
#ifdef __EMSCRIPTEN__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "game.h"
#include "minimax.h"

int get_number_of_players(){
    return NP;
}

int get_pips(){
    return PIPS;
}

int get_current_player(){
    return 1;
}

Game *alloc_game(){ // TODO: handle failures on this side of the interface
    return malloc(sizeof(Game));
}

Hands *get_hands(Game *game){
    return &game->hands;
}

void set_hand_size(Game *game, int player, int size){
    game->hands.hand_sizes[player] = size;
}

void set_turn(Game *game, int player){
    game->turn = player;
}

int get_turn(Game *game){
    return game->turn;
}

int *alloc_int(){ // yes, i don't know of a better way...
    return malloc(sizeof(int));
}

int deref_int(int *ptr){
    return *ptr;
}

Move *alloc_max_move_arr(){
    return malloc(sizeof(Move) * DCOUNT);
}

Move *alloc_move(){
    return malloc(sizeof(Move));
}

void pick_all_boneyard(Game *game){
    Move pick_all_of_boneyard;
    pick_all_of_boneyard.type = IMPERFECT_PICK;
    pick_all_of_boneyard.imperfect_pick.count = game->hands.hand_sizes[NP];
    imperfect_pick(game, pick_all_of_boneyard);
}

int get_hand_size(Game *game, int player){
    return game->hands.hand_sizes[player];
}

Move *get_move_by_index(Move move_array[], int index){
    return &move_array[index];
}

void play_move_by_pointer(Game *game, Move *move){
    play_move(game, *move);
}

void perfect_pick_by_pointer(Game *game, Move *move){
    perfect_pick(game, *move);
}

void imperfect_pick_by_pointer(Game *game, Move *move){
    imperfect_pick(game, *move);
}

Snake *get_snake(Game *game){
    return &game->snake;
}

void populate_move_from_components(Move *move, enum Type type, int left, int right){
    move->type = type;
    move->play.left = left;
    move->play.right = right;
}

void populate_imperfect_picking_move(Move *move, int amount){
    move->type = IMPERFECT_PICK;
    move->imperfect_pick.count = amount;
}

void populate_move_by_ai(Game *game, Move *move, Move moves[], int n, int depth){
    printf("DEBUGGING PRINTS:\n");
    print_game(game);
    print_playing_moves(moves, n);
    printf("depth: %d, movecount: %d\n", depth, n);
    printf("move pointer: %p\n", move);
    Move best = best_move(game, moves, NULL, n, depth, 1, NULL, expectiminimax);
    printf("[%d|%d] %d\n", best.play.left, best.play.right, best.type);
    *move = best;
    printf("END OF DEBUGGING PRINTS.\n");
}

int get_left_of_move(Move *move){
    return move->play.left;
}

int get_right_of_move(Move *move){
    return move->play.right;
}

int get_type_of_move(Move *move){
    return move->type;
}

enum Type get_LEFT(){
    return LEFT;
}

enum Type get_RIGHT(){
    return RIGHT;
}

enum Type get_PERFECT_PICK(){
    return PERFECT_PICK;
}

enum Type get_IMPERFECT_PICK(){
    return IMPERFECT_PICK;
}

enum Type get_PASS(){
    return PASS;
}

#endif