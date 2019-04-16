#ifndef _P_SYSTEM_H_
#define _P_SYSTEM_H_

#include <iostream>

enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };
enum direction { UP, DOWN, LEFT, RIGHT };

namespace sys {
//盤面状態
puyocolor *field_color = NULL;
bool *field_moveable = NULL;
bool *field_linked_num = NULL;
//操作ぷよ
direction rotate_state = UP;
int m_puyo_axis[2] = {0, 3};
int s_puyo_axis[2] = {0, 0};
int *get_s_puyo_axis() {
    memcpy(s_puyo_axis, m_puyo_axis, sizeof(m_puyo_axis));
    switch (rotate_state) {
        case UP:
            s_puyo_axis[0]++;
            break;
        case DOWN:
            s_puyo_axis[0]--;
            break;
        case LEFT:
            s_puyo_axis[1]--;
            break;
        case RIGHT:
            s_puyo_axis[1]++;
            break;
    }
    return s_puyo_axis;
}
puyocolor c_puyo_color[2] = {RED, BLUE};
//盤面の行数，列数
unsigned int data_line = 0;
unsigned int data_column = 0;

unsigned int GetLine() { return data_line; }
unsigned int GetColumn() { return data_column; }

//メモリ開放
void ReleaseData() {
    if (field_color == NULL) {
        return;
    }
    delete[] field_color;
    field_color = NULL;
}
//盤面サイズ変更
void ChangeDataSize(unsigned int line, unsigned int column) {
    field_color = new puyocolor[line * column];

    data_line = line;
    data_column = column;
}
puyocolor GetFieldColor(unsigned int y, unsigned int x) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return NONE;
    }
    return field_color[y * GetColumn() + x];
}
void SetFieldColor(unsigned int y, unsigned int x, puyocolor value) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return;
    }
    field_color[y * GetColumn() + x] = value;
}
void GeneratePuyo() {}
bool isPuyoLanding() {  //　　書き換えろ
    bool landed = false;

    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (GetFieldColor(y, x) != NONE && y == GetLine() - 1) {
                landed = true;
            }
        }
    }

    return landed;
}
bool GetFieldBool(bool *data, unsigned int y, unsigned int x) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return false;
    }
    return data[y * GetColumn() + x];
}

void SetFieldBool(bool *data, unsigned int y, unsigned int x, bool value) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return;
    }
    data[y * GetColumn() + x] = value;
}
void UpdateMoveableField() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            puyocolor c = GetFieldColor(y, x);
            SetFieldBool(field_moveable, y, x, c == NONE);
        }
    }
}
bool isMoveable(int y, int x) {
    UpdateMoveableField();
    return GetFieldBool(field_moveable, y, x);
}
void Move(direction dir) {
    switch (dir) {
        case UP:
            break;
        case DOWN:
            break;
        case LEFT:
            break;
        case RIGHT:
            break;
    }
}
void MoveLeft() {}
void MoveRight() {}
void MoveDown() { Move(DOWN); }
}  // namespace sys

using namespace sys;

#endif  //_P_SYSTEM_H_