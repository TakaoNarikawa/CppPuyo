#ifndef _P_SYSTEM_H_
#define _P_SYSTEM_H_

#include <iostream>

enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };
enum direction { UP, DOWN, LEFT, RIGHT };
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
int default_axis[2] = {1, 3};
int m_puyo_axis[2] = {0, 0};
int s_puyo_axis[2] = {0, 0};
bool isWaiting = false;
void update_s_puyo_axis() {
    memcpy(s_puyo_axis, m_puyo_axis, sizeof(m_puyo_axis));
    switch (rotate_state) {
        case UP:
            s_puyo_axis[0]--;
            break;
        case DOWN:
            s_puyo_axis[0]++;
            break;
        case LEFT:
            s_puyo_axis[1]--;
            break;
        case RIGHT:
            s_puyo_axis[1]++;
            break;
    }
}
int *get_s_puyo_axis() {
    update_s_puyo_axis();
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
    field_moveable = new bool[line * column];
    field_linked_num = new int[line * column];
    field_linked_num_scanned = new bool[line * column];
    field_linked_num_applied = new bool[line * column];
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
int GetFieldInt(int *data, unsigned int y, unsigned int x) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return 0;
    }
    return data[y * GetColumn() + x];
}
void SetFieldInt(int *data, unsigned int y, unsigned int x, int value) {
    if (y >= GetLine() || x >= GetColumn()) {
        //引数の値が正しくない
        return;
    }
    data[y * GetColumn() + x] = value;
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

void GeneratePuyo() { memcpy(m_puyo_axis, default_axis, sizeof(default_axis)); }

// -- 連結数 計算 start --
directionBoolSet GetLinkedDir(unsigned int y, unsigned x) {
    directionBoolSet dirs = directionBoolSet();
    puyocolor currentColor = GetFieldColor(y, x);

    dirs.up = y != 0 && GetFieldColor(y - 1, x) == currentColor;
    dirs.down = y < GetLine() - 1 && GetFieldColor(y + 1, x) == currentColor;
    dirs.left = x != 0 && GetFieldColor(y, x - 1) == currentColor;
    dirs.right = x < GetColumn() - 1 && GetFieldColor(y, x + 1) == currentColor;

    return dirs;
}
int GetLinkedNum(unsigned int y, unsigned x) {
    SetFieldBool(field_linked_num_scanned, y, x, true);
    int num = 1;
    directionBoolSet dirs = GetLinkedDir(y, x);

    if (dirs.up && !GetFieldBool(field_linked_num_scanned, y - 1, x)) {
        num += GetLinkedNum(y - 1, x);
    }
    if (dirs.down && !GetFieldBool(field_linked_num_scanned, y + 1, x)) {
        num += GetLinkedNum(y + 1, x);
    }
    if (dirs.left && !GetFieldBool(field_linked_num_scanned, y, x - 1)) {
        num += GetLinkedNum(y, x - 1);
    }
    if (dirs.right && !GetFieldBool(field_linked_num_scanned, y, x + 1)) {
        num += GetLinkedNum(y, x + 1);
    }
    return num;
}
void ApplyLinkedNum(unsigned int y, unsigned int x, int num) {
    SetFieldBool(field_linked_num_applied, y, x, true);
    SetFieldInt(field_linked_num, y, x, num);
    directionBoolSet dirs = GetLinkedDir(y, x);

    if (dirs.up && !GetFieldBool(field_linked_num_applied, y - 1, x)) {
        ApplyLinkedNum(y - 1, x, num);
    }
    if (dirs.down && !GetFieldBool(field_linked_num_applied, y + 1, x)) {
        ApplyLinkedNum(y + 1, x, num);
    }
    if (dirs.left && !GetFieldBool(field_linked_num_applied, y, x - 1)) {
        ApplyLinkedNum(y, x - 1, num);
    }
    if (dirs.right && !GetFieldBool(field_linked_num_applied, y, x + 1)) {
        ApplyLinkedNum(y, x + 1, num);
    }
}
void UpdateLinkedNum() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (GetFieldColor(y, x) != NONE) {
                if (!GetFieldBool(field_linked_num_applied, y, x)) {
                    int num = GetLinkedNum(y, x);
                    ApplyLinkedNum(y, x, num);
                }
            } else {
                SetFieldInt(field_linked_num, y, x, 0);
                SetFieldBool(field_linked_num_scanned, y, x, true);
                SetFieldBool(field_linked_num_applied, y, x, true);
            }
        }
    }
    field_linked_num_scanned = new bool[GetLine() * GetColumn()];
    field_linked_num_applied = new bool[GetLine() * GetColumn()];
}

// -- 連結数 計算 end --
void UpdateMoveableField() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            puyocolor c = GetFieldColor(y, x);
            SetFieldBool(field_moveable, y, x, c == NONE);
        }
    }
}
bool isMoveable(int y, int x) {
    if (x < 0 || x >= GetColumn() || y >= GetLine()) {
        return false;
    }
    if (y < 0) {
        return true;
    }
    return GetFieldBool(field_moveable, y, x);
    // return true;
}
bool isPuyoLanding() {  //　　書き換えろ
    bool moveable = isMoveable(m_puyo_axis[0] + 1, m_puyo_axis[1]) &&
                    isMoveable(s_puyo_axis[0] + 1, s_puyo_axis[1]);
    return !(moveable);
}

bool Chains() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (GetFieldInt(field_linked_num, y, x) >= 4) {
                return true;
            }
        }
    }
    return false;
}
void ErasePuyo() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (GetFieldInt(field_linked_num, y, x) >= 4) {
                SetFieldColor(y, x, NONE);
            }
        }
    }
}
void DropPuyo() {
    puyocolor *colCache = new puyocolor[GetColumn()];
    int targetIndex = 0;
    for (int x = 0; x < GetColumn(); x++) {
        // y = 0 のときは上にぷよないのでスルー
        for (int y = GetLine() - 1; y > 0; y--) {
            if (GetFieldColor(y, x) != NONE) {
                colCache[targetIndex] = GetFieldColor(y, x);
                targetIndex++;
            }
        }
        for (int y = GetLine() - 1; y > 0; y--) {
            SetFieldColor(y, x, colCache[y]);
        }
        puyocolor *colCache = new puyocolor[GetColumn()];
        targetIndex = 0;
    }
}
void ChainPuyo() {
    while (Chains()) {
        ErasePuyo();
        DropPuyo();
        UpdateLinkedNum();
    }
    UpdateMoveableField();
}

void Move(direction dir) {
    update_s_puyo_axis();
    bool moveable;

    switch (dir) {
        case DOWN:
            moveable = isMoveable(m_puyo_axis[0] + 1, m_puyo_axis[1]) &&
                       isMoveable(s_puyo_axis[0] + 1, s_puyo_axis[1]);
            if (moveable) {
                m_puyo_axis[0]++;
            }
            break;
        case LEFT:
            moveable = isMoveable(m_puyo_axis[0], m_puyo_axis[1] - 1) &&
                       isMoveable(s_puyo_axis[0], s_puyo_axis[1] - 1);
            if (moveable) {
                m_puyo_axis[1]--;
            }
            break;
        case RIGHT:
            moveable = isMoveable(m_puyo_axis[0], m_puyo_axis[1] + 1) &&
                       isMoveable(s_puyo_axis[0], s_puyo_axis[1] + 1);
            if (moveable) {
                m_puyo_axis[1]++;
            }
            break;
        default:
            return;
    }
}
void PutPuyo() {
    update_s_puyo_axis();

    SetFieldColor(m_puyo_axis[0], m_puyo_axis[1], c_puyo_color[0]);
    SetFieldColor(s_puyo_axis[0], s_puyo_axis[1], c_puyo_color[1]);
}
void MoveLeft() { Move(LEFT); }
void MoveRight() { Move(RIGHT); }
void MoveDown() { Move(DOWN); }
}  // namespace sys

using namespace sys;

#endif  //_P_SYSTEM_H_