#ifndef _P_FieldData_H_
#define _P_FieldData_H_

#include <iostream>

// Remember that if you are doing template programming, then you have to keep
// everything in the .h file so that the compiler will instantiate the right
// code at the moment of compilation.

template <typename T>
class FieldData {
   public:
    FieldData() : data(0), defaultValue((T)0), rows(0), cols(0) {}
    FieldData(T defaultValue, int rows, int cols)
        : data(rows * cols, defaultValue),
          defaultValue(defaultValue),
          rows(rows),
          cols(cols) {}

    bool isInRange(unsigned int y, unsigned int x) {
        return y < rows && x < cols;
    }
    T GetValue(unsigned int y, unsigned int x) {
        return isInRange(y, x) ? data[y * rows + x] : defaultValue;
    }
    void SetValue(unsigned int y, unsigned int x, T value) {
        if (!isInRange(y, x)) return;
        data[y * cols + x] = value;
    }
    void ChangeDataSize(unsigned int _rows, unsigned int _cols) {
        data.resize(_cols * _rows);
        rows = _rows;
        cols = _cols;
    }
    void ResetData() {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                SetValue(y, x, defaultValue);
            }
        }
    }

   private:
    std::vector<T> data;
    T defaultValue;
    int rows;
    int cols;
};

// class FieldData {
//    private:
//     T *data;
//     T defaultValue;
//     int line;
//     int column;

//    public:
//     FieldData() {}
//     FieldData(T defaultValue, int line, int column)
//         : defaultValue(defaultValue), line(line), column(column) {
//         ChangeDataSize(line, column);
//     }
//     ~FieldData(){};

// bool isInRange(unsigned int y, unsigned int x) {
//     return y < line && x < column;
// }
// T GetValue(unsigned int y, unsigned int x) {
//     return isInRange(y, x) ? data[y * column + x] : defaultValue;
// }
// void SetValue(unsigned int y, unsigned int x, T value) {
//     if (!isInRange(y, x)) return;
//     data[y * column + x] = value;
// }
//     void ReleaseData() {
//         if (data == NULL) {
//             return;
//         }
//         delete[] data;
//     };
//     void ResetData() {
//         for (int y = 0; y < line; y++) {
//             for (int x = 0; x < column; x++) {
//                 SetValue(y, x, defaultValue);
//             }
//         }
//     }
//     void ChangeDataSize(unsigned int _line, unsigned int _column) {
//         ReleaseData();
//         data = new T[line * column];
//         line = _line;
//         column = _column;
//         ResetData();
//     }
// };

#endif