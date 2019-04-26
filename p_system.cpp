#include "p_system.hpp"
#include <curses.h>
#include <iostream>
#include "p_cui.hpp"

using namespace std;

int main(int argc, char **argv) {
    // 初期化
    cui::InitCurses();
    unsigned int rows = 13;
    unsigned int cols = 6;

    ChangeDataSize(rows, cols);

    UpdateLinkedNum();
    UpdateMoveableField();

    CreatePuyoNext();
    GeneratePuyo();

    int delay = 0;
    int chainDelay = 0;

    // true -> erase, false -> drop
    bool whichEraseDrop = true;
    int waitCount = 25000;
    int chainWaitCount = 10000;

    int puyostate = 0;

    while (1) {
        if (!isChaining) {
            //キー入力受付
            int ch;
            ch = getch();
            // Qの入力で終了
            if (ch == 'Q') {
                break;
            }
            switch (ch) {
                case 'a':
                    MoveLeft();
                    break;
                case 'd':
                    MoveRight();
                    break;
                case 's':
                    MoveDown();
                    break;
                case 'j':
                    RotateLeft();
                    break;
                case 'k':
                    RotateRight();
                    break;
                default:
                    break;
            }

            if (delay % waitCount == 0) {
                delay = 0;
                MoveDown();
                if (isPuyoLanding()) {
                    PutPuyo();
                    UpdateLinkedNum();
                    UpdateMoveableField();
                    if (Chains()) {
                        isChaining = true;
                    }
                    GeneratePuyo();
                }
            }
            delay++;
        } else {
            if (chainDelay % chainWaitCount == 0) {
                chainDelay = 0;
                if (whichEraseDrop) {
                    ErasePuyo();
                } else {
                    DropPuyo();
                    UpdateLinkedNum();
                }
                whichEraseDrop = !whichEraseDrop;
                if (whichEraseDrop && !Chains()) {
                    isChaining = false;
                    UpdateMoveableField();
                }
            }
            chainDelay++;
        }

        cui::Display();
    }

    cui::EndFrame();
    return 0;
}

namespace sys {
//盤面サイズ変更
void ChangeDataSize(unsigned int rows, unsigned int cols) {
    field_color.ChangeDataSize(rows, cols);
    field_moveable.ChangeDataSize(rows, cols);
    field_linked_num.ChangeDataSize(rows, cols);
    field_linked_num_scanned.ChangeDataSize(rows, cols);
    field_linked_num_applied.ChangeDataSize(rows, cols);
    data_rows = rows;
    data_cols = cols;
}
void UpdateSubPuyoAxis() {
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
int *GetSubPuyoAxis() {
    UpdateSubPuyoAxis();
    return s_puyo_axis;
}
puyopair GetRandPuyoPair() {
    std::random_device rnd;  // 非決定的な乱数生成器を生成
    puyopair puyo;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> rand100(1, 4);
    puyo.main = (puyocolor)rand100(mt);
    puyo.sub = (puyocolor)rand100(mt);
    return puyo;
}

void CreatePuyoNext() {
    for (int i = 0; i < 1000; i++) {
        puyoNext[i] = GetRandPuyoPair();
    }
}

void GeneratePuyo() {
    memcpy(m_puyo_axis, default_axis, sizeof(default_axis));

    puyopair puyo = puyoNext[cPuyoIndex];
    cPuyoIndex++;

    c_puyo_color[0] = puyo.main;
    c_puyo_color[1] = puyo.sub;

    rotate_state = UP;
}

// -- 連結数 計算 start --
directionBoolSet GetLinkedDir(unsigned int y, unsigned x) {
    directionBoolSet dirs = directionBoolSet();
    puyocolor currentColor = field_color.GetValue(y, x);

    dirs.up = y != 0 && field_color.GetValue(y - 1, x) == currentColor;
    dirs.down =
        y < GetRow() - 1 && field_color.GetValue(y + 1, x) == currentColor;
    dirs.left = x != 0 && field_color.GetValue(y, x - 1) == currentColor;
    dirs.right =
        x < GetCol() - 1 && field_color.GetValue(y, x + 1) == currentColor;

    return dirs;
}
int GetLinkedNum(unsigned int y, unsigned x) {
    field_linked_num_scanned.SetValue(y, x, true);
    int num = 1;
    directionBoolSet dirs = GetLinkedDir(y, x);

    if (dirs.up && !field_linked_num_scanned.GetValue(y - 1, x)) {
        num += GetLinkedNum(y - 1, x);
    }
    if (dirs.down && !field_linked_num_scanned.GetValue(y + 1, x)) {
        num += GetLinkedNum(y + 1, x);
    }
    if (dirs.left && !field_linked_num_scanned.GetValue(y, x - 1)) {
        num += GetLinkedNum(y, x - 1);
    }
    if (dirs.right && !field_linked_num_scanned.GetValue(y, x + 1)) {
        num += GetLinkedNum(y, x + 1);
    }
    return num;
}
void ApplyLinkedNum(unsigned int y, unsigned int x, int num) {
    field_linked_num_applied.SetValue(y, x, true);
    field_linked_num.SetValue(y, x, num);
    directionBoolSet dirs = GetLinkedDir(y, x);

    if (dirs.up && !field_linked_num_applied.GetValue(y - 1, x)) {
        ApplyLinkedNum(y - 1, x, num);
    }
    if (dirs.down && !field_linked_num_applied.GetValue(y + 1, x)) {
        ApplyLinkedNum(y + 1, x, num);
    }
    if (dirs.left && !field_linked_num_applied.GetValue(y, x - 1)) {
        ApplyLinkedNum(y, x - 1, num);
    }
    if (dirs.right && !field_linked_num_applied.GetValue(y, x + 1)) {
        ApplyLinkedNum(y, x + 1, num);
    }
}
void UpdateLinkedNum() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            if (field_color.GetValue(y, x) != NONE) {
                if (!field_linked_num_applied.GetValue(y, x)) {
                    int num = GetLinkedNum(y, x);
                    ApplyLinkedNum(y, x, num);
                }
            } else {
                field_linked_num.SetValue(y, x, 0);
                field_linked_num_applied.SetValue(y, x, true);
                field_linked_num_scanned.SetValue(y, x, true);
            }
        }
    }
    field_linked_num_scanned.ResetData();
    field_linked_num_applied.ResetData();
}
// -- 連結数 計算 end --

void UpdateMoveableField() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            puyocolor c = field_color.GetValue(y, x);
            field_moveable.SetValue(y, x, c == NONE);
        }
    }
}
bool isMoveable(int y, int x) {
    if (x < 0 || x >= GetCol() || y >= GetRow()) {
        return false;
    }
    if (y < 0) {
        return true;
    }
    return field_moveable.GetValue(y, x);
    // return true;
}

bool isPuyoLanding() {  //　　書き換えろ
    bool moveable = isMoveable(m_puyo_axis[0] + 1, m_puyo_axis[1]) &&
                    isMoveable(s_puyo_axis[0] + 1, s_puyo_axis[1]);
    return !(moveable);
}

bool Chains() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            if (field_linked_num.GetValue(y, x) >= 4) {
                return true;
            }
        }
    }
    return false;
}
void ErasePuyo() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            if (field_linked_num.GetValue(y, x) >= 4) {
                field_color.SetValue(y, x, NONE);
            }
        }
    }
}
void DropPuyo() {
    puyocolor *colCache = new puyocolor[GetRow()];
    int targetIndex = 0;
    for (int x = 0; x < GetCol(); x++) {
        // y = 0 のときは上にぷよないのでスルー
        for (int i = 0; i < GetRow(); i++) {
            colCache[i] = NONE;
        }
        for (int y = GetRow() - 1; y > 0; y--) {
            if (field_color.GetValue(y, x) != NONE) {
                colCache[targetIndex] = field_color.GetValue(y, x);
                targetIndex++;
            }
        }
        for (int y = 0; y < GetRow(); y++) {
            field_color.SetValue(GetRow() - y - 1, x, colCache[y]);
        }
        targetIndex = 0;
    }
}

void Move(direction dir) {
    UpdateSubPuyoAxis();
    bool moveable;

    switch (dir) {
        case UP:
            moveable = isMoveable(m_puyo_axis[0] - 1, m_puyo_axis[1]) &&
                       isMoveable(s_puyo_axis[0] - 1, s_puyo_axis[1]);
            if (moveable) {
                m_puyo_axis[0]--;
            }
            break;
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
void CheckRotate(direction dir) {
    UpdateSubPuyoAxis();
    //サブぷよの移動先がmoveableならなにもしない
    if (isMoveable(s_puyo_axis[0], s_puyo_axis[1])) {
        return;
    }
    //右も左も!moveable
    if (!isMoveable(s_puyo_axis[0], s_puyo_axis[1] + 1) &&
        !isMoveable(s_puyo_axis[0], s_puyo_axis[1] - 1)) {
        Rotate(dir);
    }
    //壁蹴りで左移動
    if (m_puyo_axis[1] < s_puyo_axis[1]) {
        MoveLeft();
        return;
    }
    //壁蹴りで右移動
    if (m_puyo_axis[1] > s_puyo_axis[1]) {
        MoveRight();
        return;
    }
    // 床けりで上移動
    if (m_puyo_axis[0] < s_puyo_axis[0]) {
        MoveUp();
        return;
    }
}
void Rotate(direction dir) {
    switch (dir) {
        case LEFT:
            rotate_state = (direction)(((int)rotate_state + 1) % 4);
            CheckRotate(dir);
            break;
        case RIGHT:
            rotate_state = (direction)(((int)rotate_state + 3) % 4);
            CheckRotate(dir);
            break;
        default:  // never happen
            break;
    }
}
void PutPuyo() {
    UpdateSubPuyoAxis();
    field_color.SetValue(m_puyo_axis[0], m_puyo_axis[1], c_puyo_color[0]);
    field_color.SetValue(s_puyo_axis[0], s_puyo_axis[1], c_puyo_color[1]);
    DropPuyo();
}
}  // namespace sys