#ifndef _FIELDDATA_H_
#define _FIELDDATA_H_

#include <vector>

using namespace std;

template <typename T>
class FieldData {
   public:
    FieldData() : data(0), defaultValue((T)0), rows(0), cols(0) {}
    FieldData(T defaultValue)
        : data(0), defaultValue(defaultValue), rows(0), cols(0) {}
    FieldData(T defaultValue, int rows, int cols)
        : data(rows * cols, defaultValue),
          defaultValue(defaultValue),
          rows(rows),
          cols(cols) {}
    bool isInRange(unsigned int y, unsigned int x) {
        return y < rows && x < cols;
    }
    T GetValue(unsigned int y, unsigned int x) {
        return isInRange(y, x) ? data[y * cols + x] : defaultValue;
    }
    void SetValue(unsigned int y, unsigned int x, T value) {
        if (!isInRange(y, x)) return;
        data[y * cols + x] = value;
    }
    void ChangeDataSize(unsigned int _rows, unsigned int _cols) {
        data.resize(_cols * _rows);
        ResetData();
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
#endif
