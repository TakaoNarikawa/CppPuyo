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
    DrawPuyo(2, GetColumn() + 1, puyoNext[cPuyoIndex].main);
    DrawPuyo(1, GetColumn() + 1, puyoNext[cPuyoIndex].sub);

    DrawPuyo(5, GetColumn() + 1, puyoNext[cPuyoIndex + 1].main);
    DrawPuyo(4, GetColumn() + 1, puyoNext[cPuyoIndex + 1].sub);
}
void DrawMoveable() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            char c = isMoveable(y, x) ? 'T' : 'F';
            DrawObject(y, x + GetColumn() * 1 + 4, c, 5);
        }
    }
}
void DrawLinkedNum() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            char c = '0' + field_linked_num.GetValue(y, x);
            DrawObject(y, x + GetColumn() * 2 + 6, c, 5);
        }
    }
}
void DrawRawField() {
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            char c = '0' + field_linked_num.GetValue(y, x);
            DrawObject(y, x + GetColumn() * 3 + 8, c, 5);
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
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
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
    for (int y = 0; y < GetLine(); y++) {
        for (int x = 0; x < GetColumn(); x++) {
            if (field_color.GetValue(y, x) != NONE) {
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
}  // namespace cui

#endif