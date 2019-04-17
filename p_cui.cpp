#include <curses.h>
#include <iostream>
#include "p_system.hpp"

void DrawPuyo(int y, int x, puyocolor c);
void Display();
void set_debug_field();

int main(int argc, char **argv) {
    //画面の初期化
    initscr();
    //カラー属性を扱うための初期化
    start_color();
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
    set_debug_field();
    UpdateLinkedNum();
    UpdateMoveableField();

    GeneratePuyo();

    int delay = 0;
    int waitCount = 25000;

    int puyostate = 0;

    //メイン処理ループ
    while (1) {
        if (!isWaiting) {
            //キー入力受付
            int ch;
            ch = getch();
            // Qの入力で終了
            if (ch == 'Q') {
                break;
            }
            //入力キーごとの処理
            switch (ch) {
                case KEY_LEFT:
                    MoveLeft();
                    break;
                case KEY_RIGHT:
                    MoveRight();
                    break;
                case 'z':
                    //ぷよ回転処理
                    break;
                default:
                    break;
            }

            if (delay % waitCount == 0) {
                MoveDown();
                if (isPuyoLanding()) {
                    PutPuyo();
                    UpdateLinkedNum();
                    UpdateMoveableField();
                    if (Chains()) {
                        ChainPuyo();
                    }
                    GeneratePuyo();
                }
            }
        } else {
        }
        delay++;
        Display();
    }

    //画面をリセット
    endwin();
    return 0;
}

void DrawPuyo(int y, int x, puyocolor c) {
    x *= 2;
    switch (c) {
        case NONE:
            mvaddch(y, x, ' ');
            break;
        case RED:
            mvaddch(y, x, 'R');
            break;
        case BLUE:
            mvaddch(y, x, 'B');
            break;
        case GREEN:
            mvaddch(y, x, 'G');
            break;
        case YELLOW:
            mvaddch(y, x, 'Y');
            break;
        default:
            mvaddch(y, x, '?');
            break;
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
    int *s_puyo_axis = get_s_puyo_axis();
    DrawPuyo(m_puyo_axis[0], m_puyo_axis[1], c_puyo_color[0]);
    DrawPuyo(s_puyo_axis[0], s_puyo_axis[1], c_puyo_color[1]);

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