#ifndef _P_CUI_H_
#define _P_CUI_H_

#include <curses.h>
#include <iostream>
#include "p_system.hpp"

using namespace sys;

namespace cui {
void InitColor() {
    init_pair(NONE, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
}

void InitCurses() {
    initscr();
    start_color();
    InitColor();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(0);
}
void EndFrame() { endwin(); }

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
    DrawPuyo(2, GetCol() + 1, puyoNext[cPuyoIndex].main);
    DrawPuyo(1, GetCol() + 1, puyoNext[cPuyoIndex].sub);

    DrawPuyo(5, GetCol() + 1, puyoNext[cPuyoIndex + 1].main);
    DrawPuyo(4, GetCol() + 1, puyoNext[cPuyoIndex + 1].sub);
}
void DrawMoveable() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            char c = isMoveable(y, x) ? 'T' : 'F';
            DrawObject(y, x + GetCol() * 1 + 4, c, 5);
        }
    }
}
void DrawLinkedNum() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            char c = '0' + field_linked_num.GetValue(y, x);
            DrawObject(y, x + GetCol() * 2 + 6, c, 5);
        }
    }
}
void DrawRawField() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            char c = '0' + field_color.GetValue(y, x);
            DrawObject(y, x + GetCol() * 3 + 8, c, 5);
        }
    }
}
void DrawFrame() {
    for (int y = 0; y < GetRow() + 2; y++) {
        mvaddch(y, 0, '#');
        int x = FieldAdjustX(GetCol() - 1) + 1;
        mvaddch(y, x, '#');
    }
    for (int x = 1; x < GetCol() * 2 + 1; x++) {
        mvaddch(0, x, '#');
        int y = FieldAdjustY(GetRow());
        mvaddch(y, x, '#');
    }
}

void Display() {
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            puyocolor c = field_color.GetValue(y, x);
            DrawPuyo(y, x, c);
        }
    }
    if (!isChaining) {
        int *s_puyo_axis = GetSubPuyoAxis();
        DrawPuyo(m_puyo_axis[0], m_puyo_axis[1], c_puyo_color[0]);
        DrawPuyo(s_puyo_axis[0], s_puyo_axis[1], c_puyo_color[1]);
    }

    DrawNextPuyo();
    DrawFrame();
    DrawMoveable();
    DrawLinkedNum();
    DrawRawField();

    //情報表示
    int count = 0;
    for (int y = 0; y < GetRow(); y++) {
        for (int x = 0; x < GetCol(); x++) {
            if (field_color.GetValue(y, x) != NONE) {
                count++;
            }
        }
    }

    char msg[256];
    sprintf(msg, "Field: %d x %d, Pnum: %03d, r %d", GetRow(), GetCol(), count,
            (int)rotate_state);

    mvaddstr(2, COLS - 35, msg);
    mvaddstr(0, GetCol() * 2 + 1, "next");
    refresh();
}
}  // namespace cui

#endif