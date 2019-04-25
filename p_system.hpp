#ifndef _P_SYSTEM_H_
#define _P_SYSTEM_H_

#include <iostream>
#include <random>
#include "fielddata.hpp"

enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };
enum direction { UP, LEFT, DOWN, RIGHT };

typedef struct {
    puyocolor main;
    puyocolor sub;
} puyopair;
typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
} directionBoolSet;

namespace sys {
//盤面状態
fielddata<puyocolor> field_color;
fielddata<bool> field_moveable;
fielddata<int> field_linked_num;
fielddata<bool> field_linked_num_scanned;
fielddata<bool> field_linked_num_applied;
//操作ぷよ
direction rotate_state = UP;
bool isChaining = false;
int default_axis[2] = {0, 2};
int m_puyo_axis[2] = {0, 0};
int s_puyo_axis[2] = {0, 0};
puyopair puyoNext[1000];
int cPuyoIndex = 0;
puyocolor c_puyo_color[2] = {RED, BLUE};

unsigned int data_line = 0;
unsigned int data_column = 0;

unsigned int GetLine() { return data_line; }
unsigned int GetColumn() { return data_column; }

void ChangeDataSize(unsigned int line, unsigned int column);
void UpdateSubPuyoAxis();
int *GetSubPuyoAxis();
puyopair GetRandPuyoPair();
void CreatePuyoNext();
void GeneratePuyo();
// -- 連結数 計算 start --
directionBoolSet GetLinkedDir(unsigned int y, unsigned x);
int GetLinkedNum(unsigned int y, unsigned x);
void ApplyLinkedNum(unsigned int y, unsigned int x, int num);
void UpdateLinkedNum();
// ---------------------

void UpdateMoveableField();
bool isMoveable(int y, int x);
bool isPuyoLanding();
bool Chains();
void ErasePuyo();
void DropPuyo();
void Move(direction dir);
void MoveUp() { Move(UP); }
void MoveDown() { Move(DOWN); }
void MoveLeft() { Move(LEFT); }
void MoveRight() { Move(RIGHT); }
void CheckRotate(direction dir);
void Rotate(direction dir);
void RotateLeft() { Rotate(LEFT); }
void RotateRight() { Rotate(RIGHT); }
void PutPuyo();
}  // namespace sys

#endif  //_P_SYSTEM_H_