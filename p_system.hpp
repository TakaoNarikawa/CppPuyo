#ifndef _P_SYSTEM_H_
#define _P_SYSTEM_H_

#include <iostream>
#include <random>

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
puyocolor *field_color = NULL;
bool *field_moveable = NULL;
int *field_linked_num = NULL;
bool *field_linked_num_scanned = NULL;
bool *field_linked_num_applied = NULL;
//操作ぷよ
direction rotate_state = UP;
bool isChaining = false;
int default_axis[2] = {0, 2};
int m_puyo_axis[2] = {0, 0};
int s_puyo_axis[2] = {0, 0};
puyopair puyoNext[1000];
int cPuyoIndex = 0;
puyocolor c_puyo_color[2] = {RED, BLUE};

unsigned int data_rows = 0;
unsigned int data_cols = 0;

unsigned int GetRow() { return data_rows; }
unsigned int GetCol() { return data_cols; }

void ReleaseData();
void ChangeDataSize(unsigned int line, unsigned int column);
bool ValueCheck(unsigned int y, unsigned int x);
puyocolor GetFieldColor(unsigned int y, unsigned int x);
void SetFieldColor(unsigned int y, unsigned int x, puyocolor value);
int GetFieldInt(int *data, unsigned int y, unsigned int x);
void SetFieldInt(int *data, unsigned int y, unsigned int x, int value);
bool GetFieldBool(bool *data, unsigned int y, unsigned int x);
void SetFieldBool(bool *data, unsigned int y, unsigned int x, bool value);
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