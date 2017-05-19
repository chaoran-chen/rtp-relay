#ifndef UTILS_H
#define UTILS_H

inline unsigned char extractBits(unsigned char data, int pos, int length)
{
    const int shiftPos = 8 - pos - length;
    return (data & ((1 << length) - 1) << shiftPos) >> shiftPos;
}

#endif // UTILS_H
