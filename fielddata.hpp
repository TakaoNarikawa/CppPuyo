#ifndef _P_FIELDDATA_H_
#define _P_FIELDDATA_H_

#include <iostream>

// Remember that if you are doing template programming, then you have to keep
// everything in the .h file so that the compiler will instantiate the right
// code at the moment of compilation.

template <typename T>
class fielddata {
   public:
    void ChangeDataSize(unsigned int line, unsigned int column) {
        data = new T[line * column];
    }
    bool isInRange(unsigned int y, unsigned int x) {
        return y < line && x < column;
    }
    T GetValue(unsigned int y, unsigned int x) {
        return isInRange(y, x) ? data[y * column + x] : defaultValue;
    }
    void SetValue(unsigned int y, unsigned int x, T value) {
        if (!isInRange(y, x)) return;
        data[y * column + x] = value;
    }
    void ReleaseData() {
        if (data == NULL) {
            return;
        }
        delete[] data;
    };
    void ResetData() {
        for (int y = 0; y < line; y++) {
            for (int x = 0; x < column; x++) {
                SetValue(y, x, defaultValue);
            }
        }
    }
    fielddata(T defaultValue)
        : defaultValue(defaultValue), line(0), column(0) {}
    ~fielddata() { ReleaseData(); };

   private:
    T *data;
    T defaultValue;
    int line;
    int column;
};

#endif