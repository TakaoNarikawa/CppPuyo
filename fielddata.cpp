#include <fielddata.hpp>

fielddata::fielddata(T defaultValue, int line, int column)
    : defaultValue(defaultValue), line(line), column(column)
{
    fielddata::data = new T[line * column];
}
fielddata::ReleaseData()
{
    if (fielddata::data == NULL)
    {
        return;
    }
    delete[] fielddata::data;
}

bool isInRange(unsigned int y, unsigned int x);
T GetValue(unsigned int y, unsigned int x)
{
    return fielddata::isInRange(y, x) ? fielddata::data[y * fielddata::column + x] : fielddata::defaultValue;
}
void SetValue(unsigned int y, unsigned int x, T value)
{
    if (!fielddata::isInRange(y, x))
        return;
    fielddata::data[y * fielddata::column + x] = value;
}

void fielddata::ResetData()
{
    for (int y = 0; y < fielddata::line; y++)
    {
        for (int x = 0; x < fielddata::column; x++)
        {
            SetValue(y, x, fielddata::defaultValue);
        }
    }
}