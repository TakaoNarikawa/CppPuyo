#include <iostream>
// #include "fielddata.hpp"

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
        // cout << "getvalue" << endl;
        return isInRange(y, x) ? data[y * rows + x] : defaultValue;
    }
    void SetValue(unsigned int y, unsigned int x, T value) {
        // cout << "setvalue" << endl;
        if (!isInRange(y, x)) return;
        data[y * rows + x] = value;
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

using namespace std;

int main() {
    int row = 15;
    int col = 10;
    FieldData<int> int_data;
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            cout << int_data.GetValue(y, x);
        }
        cout << endl;
    }
    return 0;
}