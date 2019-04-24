#include <curses.h>
#include <iostream>
#include "p_system.hpp"

void DrawObject(int y, int x, char c, int colorID);
void DrawPuyo(int y, int x, puyocolor p);
void DrawMoveable();
void DrawFrame();
void Display();
void set_debug_field();
void InitColor();

using namespace sys;
int main(int argc, char **argv) {
    //画面の初期化
    initscr();
    //カラー属性を扱うための初期化
    start_color();
    InitColor();
    //キーを押しても画面に表示しない
    noecho();
    //キー入力を即座に受け付ける
    cbreak();

    curs_set(0);
    //キー入力受付方法指定
    keypad(stdscr, TRUE);
    //キー入力非ブロッキングモード
    timeout(0);

    //初期化処理
    int lines = 6;
    int cols = 13;
    //フィールドは画面サイズの縦横1/2にする
    ChangeDataSize(13, 6);
    //最初のぷよ生成
    // set_debug_field();
    UpdateLinkedNum();
    UpdateMoveableField();

    CreatePuyoSet();
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
            //入力キーごとの処理
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

        Display();
    }

    //画面をリセット
    endwin();
    return 0;
}
int FieldAdjustX(int x) { return x * 2 + 1; }
int FieldAdjustY(int y) { return y + 1; }
void DrawObject(int y, int x, char c, int colorID) {
    attron(COLOR_PAIR(colorID));
    mvaddch(FieldAdjustY(y), FieldAdjustX(x), c);
    attroff(COLOR_PAIR(colorID));
}
void DrawPuyo(int y, int x, puyocolor p) {
    if (p == NONE) {
        DrawObject(y, x, ' ', 0);
    } else {
        DrawObject(y, x, '@', (int)p);
    }
}

void DrawNextPuyo() {
    DrawPuyo(2, GetColumn() + 1, nextPuyoSet[cPuyoIndex].main);
    DrawPuyo(1, GetColumn() + 1, nextPuyoSet[cPuyoIndex].sub);

    DrawPuyo(5, GetColumn() + 1, nextPuyoSet[cPuyoIndex + 1].main);
    DrawPuyo(4, GetColumn() + 1, nextPuyoSet[cPuyoIndex + 1].sub);
}
void DrawMoveable() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            char c = isMoveable(y, x) ? 'T' : 'F';
            DrawObject(y, x + GetColumn() * 1 + 2, c, 5);
        }
    }
}
void DrawLinkedNum() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            char c = '0' + GetFieldInt(field_linked_num, y, x);
            DrawObject(y, x + GetColumn() * 2 + 4, c, 5);
        }
    }
}
void DrawFrame() {
    for (int y = 0; y < GetLine() + 2; y++) {
        mvaddch(y, 0, '#');
        int x = FieldAdjustX(GetColumn() - 1) + 1;
        mvaddch(y, x, '#');
    }
    for (int x = 1; x < GetColumn() * 2 + 1; x++) {
        mvaddch(0, x, '#');
        int y = FieldAdjustY(GetLine());
        mvaddch(y, x, '#');
    }
}

void Display() {
    // clear();
    //フィールドぷよ表示
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            puyocolor c = GetFieldColor(y, x);
            DrawPuyo(y, x, c);
        }
    }
    //落下中ぷよ表示
    if (!isChaining) {
        int *s_puyo_axis = GetSubPuyoAxis();
        DrawPuyo(m_puyo_axis[0], m_puyo_axis[1], c_puyo_color[0]);
        DrawPuyo(s_puyo_axis[0], s_puyo_axis[1], c_puyo_color[1]);
    }

    DrawNextPuyo();
    DrawFrame();
    // DrawMoveable();
    // DrawLinkedNum();

    //情報表示
    int count = 0;
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (GetFieldColor(y, x) != NONE) {
                count++;
            }
        }
    }

    char msg[256];
    sprintf(msg, "Field: %d x %d, Pnum: %03d, r %d", GetLine(), GetColumn(),
            count, (int)rotate_state);

    mvaddstr(2, COLS - 35, msg);
    mvaddstr(0, GetColumn() * 2 + 1, "next");
    refresh();
}

void set_debug_field() {
    int debug_field[13][6] = {
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {4, 4, 0, 0, 0, 0},
        {4, 3, 4, 0, 0, 0}, {3, 1, 3, 2, 3, 1}, {3, 2, 2, 3, 1, 1},
        {1, 1, 1, 2, 3, 3},
    };
    for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 13; y++) {
            puyocolor c = (puyocolor)debug_field[y][x];
            SetFieldColor(y, x, c);
        }
    }
}

void InitColor() {
    init_pair(NONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
}