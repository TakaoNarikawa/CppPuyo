#ifndef _P_FIELDDATA_H_
#define _P_FIELDDATA_H_

#include <iostream>

// Remember that if you are doing template programming, then you have to keep
// everything in the .h file so that the compiler will instantiate the right
// code at the moment of compilation.

template <typename T>
class fielddata {
   public:
    fielddata() {
        data = NULL;
        defaultValue = (T)0;
        line = 0;
        column = 0;
    }
    fielddata(T defaultValue, int line, int column)
        : defaultValue(defaultValue), line(line), column(column) {
        ChangeDataSize(line, column);
    }
    ~fielddata(){};

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
    void ChangeDataSize(unsigned int _line, unsigned int _column) {
        std::cout << "change data size" << std::endl;
        std::cout << typeid(T).name() << std::endl;
        std::cout << "1" << std::endl;
        ReleaseData();
        std::cout << "2" << std::endl;
        data = new T[line * column];
        std::cout << "3" << std::endl;
        line = _line;
        column = _column;
        ResetData();
        std::cout << "4" << std::endl;
    }

   private:
    T *data;
    T defaultValue;
    int line;
    int column;
};

#endif