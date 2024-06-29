#ifndef GBAMATH_HPP
#define GBAMATH_HPP

#include <cstdint>
#include <cstdlib>

extern "C" {

#include <fpsqrt.h>
#include "../print.h"

}

namespace GBAEngine {


struct fixed32 {
    int value;

    inline fixed32(double val) {
        *this = val;
    }
    
    inline fixed32() {
        this->value = 0;
    }

    inline fixed32(int val) {
        *this = val;
    }

    // Double conversion from https://stackoverflow.com/a/187823
    inline fixed32 operator=(double val) {
        bool isNegative = val < 0;
        int whole, frac;

        if (isNegative) val = -val;

        whole = static_cast<int>(val);
        frac = static_cast<int>(val * (1<<16));
        this->value = ((int)whole)<<16 | (frac & 0x0000ffff);

        if (isNegative) this->value = -this->value;
        return *this;
    }

    inline fixed32 operator=(int val) {
        this->value = ((int)val)<<16;
        return *this;
    }

    inline struct fixed32 sqrt() {
        fixed32 result;
        result.value = sqrt_fx16_16_to_fx16_16((fx16_16_t)this->value);
        return result;
    }

    inline fixed32 pow(int b);

    inline static fixed32 lerp(fixed32 a, fixed32 b, fixed32 t);

    inline static fixed32 abs(fixed32 v) {
        fixed32 result;
        result.value = std::abs(v.value);
        return result;
    }

    inline operator short() {
        return this->value >> 16;
    }

    inline operator int() {
        return (short)*this;
    }

    inline operator bool() {
        return (bool)this->value;
    }

    static struct fixed32 sin(int);
    static struct fixed32 cos(int);
    static struct fixed32 tan(int);

    private:
        static struct fixed32 sinData[360];
        static struct fixed32 tanData[180];
        static bool trigDataInitialized;
        static void initializeTrigData();
        static void initializeSin();
        static void initializeTan();

};


#pragma region fixed32arithmetic

inline fixed32 operator+(const fixed32& a, const fixed32& b) {
    fixed32 result;
    result.value = a.value + b.value;
    return result;
}
inline fixed32 operator+(const fixed32& a, const int& b) {
    fixed32 result;
    result.value = a.value + (b<<16);
    return result;
}
inline fixed32 operator+(const int& a, const fixed32& b) {
    fixed32 result;
    result.value = (a<<16) + b.value;
    return result;
}
inline fixed32 operator+=(fixed32& a, const fixed32& b) {
    a.value = a.value + b.value;
    return a;
}
inline fixed32 operator+=(fixed32& a, const int& b) {
    a.value = a.value + (b<<16);
    return a;
}


inline fixed32 operator-(const fixed32& a, const fixed32& b) {
    fixed32 result;
    result.value = a.value - b.value;
    return result;
}
inline fixed32 operator-(const fixed32& a, const int& b) {
    fixed32 result;
    result.value = a.value - (b<<16);
    return result;
}
inline fixed32 operator-(const int& a, const fixed32& b) {
    fixed32 result;
    result.value = (a<<16) - b.value;
    return result;
}
inline fixed32 operator-=(fixed32& a, const fixed32& b) {
    a.value = a.value - b.value;
    return a;
}
inline fixed32 operator-=(fixed32& a, const int& b) {
    a.value = a.value - (b<<16);
    return a;
}

inline fixed32 operator*(const fixed32& a, const fixed32& b) {
    fixed32 result;
    long long resultVal = ((long long)a.value) * ((long long)b.value);
    result.value = (int)(resultVal>>16);
    return result;
}
inline fixed32 operator*(const fixed32& a, const int& b) {
    fixed32 result;
    result.value = a.value * b;
    return result;
}
inline fixed32 operator*(const int& a, const fixed32& b) {
    fixed32 result;
    result.value = a * b.value;
    return result;
}
inline fixed32 operator*=(fixed32& a, const fixed32& b) {
    long long resultVal = ((long long)a.value) * ((long long)b.value);
    a.value = (int)(resultVal>>16);
    return a;
}
inline fixed32 operator*=(fixed32& a, const int& b) {
    a.value = a.value * b;
    return a;
}

inline fixed32 operator/(const fixed32& a, const fixed32& b) {
    fixed32 result;
    result.value = (a.value / (b.value >> 8)) << 8;
    return result;
}
inline fixed32 operator/(const fixed32& a, const int& b) {
    fixed32 result;
    result.value = a.value / b;
    return result;
}
inline fixed32 operator/(const int& a, const fixed32& b) {
    fixed32 result;
    result.value = (a << 16 / (b.value>>8)) << 8;
    return result;
}
inline fixed32 operator/=(fixed32& a, const fixed32& b) {
    long long dividend = ((long long)a.value) << 16;
    a.value = (a.value / (b.value >> 8)) << 8;
    return a;
}
inline fixed32 operator/=(fixed32& a, const int& b) {
    a.value = a.value / b;
    return a;
}

inline fixed32 operator-(const fixed32& v) {
    fixed32 result;
    result.value = -v.value;
    return result;
}

#pragma endregion

#pragma region fixed32comparison

inline bool operator ==(const fixed32& a, const fixed32& b) {
    return a.value == b.value;
}
inline bool operator ==(const fixed32& a, const int& b) {
    return a.value == b<<16;
}
inline bool operator ==(const int& a, const fixed32& b) {
    return a<<16 == b.value;
}

inline bool operator !=(const fixed32& a, const fixed32& b) {
    return a.value != b.value;
}
inline bool operator !=(const fixed32& a, const int& b) {
    return a.value != b<<16;
}
inline bool operator !=(const int& a, const fixed32& b) {
    return a<<16 != b.value;
}

inline bool operator <(const fixed32& a, const fixed32& b) {
    return a.value < b.value;
}
inline bool operator <(const fixed32& a, const int& b) {
    return a.value < b<<16;
}
inline bool operator <(const int& a, const fixed32& b) {
    return a<<16 < b.value;
}

inline bool operator >(const fixed32& a, const fixed32& b) {
    return a.value > b.value;
}
inline bool operator >(const fixed32& a, const int& b) {
    return a.value > b<<16;
}
inline bool operator >(const int& a, const fixed32& b) {
    return a<<16 > b.value;
}

inline bool operator <=(const fixed32& a, const fixed32& b) {
    return a.value <= b.value;
}
inline bool operator <=(const fixed32& a, const int& b) {
    return a.value <= b<<16;
}
inline bool operator <=(const int& a, const fixed32& b) {
    return a<<16 <= b.value;
}

inline bool operator >=(const fixed32& a, const fixed32& b) {
    return a.value >= b.value;
}
inline bool operator >=(const fixed32& a, const int& b) {
    return a.value >= b<<16;
}
inline bool operator >=(const int& a, const fixed32& b) {
    return a<<16 >= b.value;
}

#pragma endregion

inline fixed32 fixed32::lerp(fixed32 a, fixed32 b, fixed32 t) {
    return a + (b-a) * t;
}

inline fixed32 fixed32::pow(int b) {
    fixed32 result;
    result = 1;
    for (int i = 0; i < b; i++) {
        result = result * *this;
    }
    return result;
}

struct Vector2 {

    fixed32 x;
    fixed32 y;

    inline Vector2(fixed32 x, fixed32 y);

    inline Vector2(fixed32 m);

    inline Vector2();

    inline fixed32 magnitude();

    inline Vector2 normalized();

    inline fixed32 dot(Vector2 b);

    explicit inline operator bool() {
        return (this->x || this->y);
    }

    static inline Vector2 lerp(Vector2 a, Vector2 b, fixed32 t);

    inline static Vector2 clampBounds(Vector2 v, Vector2 a, Vector2 b);

};

#pragma region vector2arithmetic

inline Vector2 operator+(Vector2 a, Vector2 b) {
    Vector2 result = Vector2(a.x + b.x, a.y + b.y);
    return result;
}

inline Vector2 operator-(Vector2 a, Vector2 b) {
    Vector2 result = Vector2(a.x - b.x, a.y - b.y);
    return result;
}

inline Vector2 operator*(Vector2 a, Vector2 b) {
    Vector2 result = Vector2(a.x * b.x, a.y * b.y);
    return result;
}
inline Vector2 operator*(Vector2 a, fixed32 b) {
    Vector2 result = Vector2(a.x * b, a.y * b);
    return result;
}
inline Vector2 operator*(fixed32 a, Vector2 b) {
    Vector2 result = Vector2(a * b.x, a * b.y);
    return result;
}
inline Vector2 operator*(Vector2 a, int b) {
    Vector2 result = Vector2(a.x * b, a.y * b);
    return result;
}
inline Vector2 operator*(int a, Vector2 b) {
    Vector2 result = Vector2(a * b.x, a * b.y);
    return result;
}

inline Vector2 operator/(Vector2 a, Vector2 b) {
    Vector2 result = Vector2(a.x / b.x, a.y / b.y);
    return result;
}
inline Vector2 operator/(Vector2 a, fixed32 b) {
    Vector2 result = Vector2(a.x / b, a.y / b);
    return result;
}
inline Vector2 operator/(fixed32 a, Vector2 b) {
    Vector2 result = Vector2(a / b.x, a / b.y);
    return result;
}
inline Vector2 operator/(Vector2 a, int b) {
    Vector2 result = Vector2(a.x / b, a.y / b);
    return result;
}
inline Vector2 operator/(int a, Vector2 b) {
    Vector2 result = Vector2(a / b.x, a / b.y);
    return result;
}

inline Vector2 operator-(Vector2 v) {
    Vector2 result = Vector2(-v.x, -v.y);
    return result;
}

#pragma endregion

#pragma region vector2comparison

inline bool operator ==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}

#pragma endregion

#pragma region vector2functions

inline Vector2::Vector2(fixed32 x, fixed32 y) {
    this->x = x;
    this->y = y;
}

inline Vector2::Vector2(fixed32 m) : Vector2(m,m) {

}

inline Vector2::Vector2() : Vector2(0) {
    
}

inline fixed32 Vector2::magnitude() {
    return (x*x + y*y).sqrt();
}

inline Vector2 Vector2::normalized() {
    fixed32 magnitude = this->magnitude();
    Vector2 v = Vector2(*this);
    Vector2 result = v/magnitude;
    return result;
}

inline fixed32 Vector2::dot(Vector2 b) {
    return this->x * b.x + this->y * b.y;
}

inline Vector2 Vector2::lerp(Vector2 a, Vector2 b, fixed32 t) {
    Vector2 result = Vector2();
    result.x = fixed32::lerp(a.x, b.x, t);
    result.y = fixed32::lerp(a.y, b.y, t);
    return result;
}

inline Vector2 Vector2::clampBounds(Vector2 v, Vector2 a, Vector2 b) {
    Vector2 result;
    result.x = (v.x > a.x) ? ((v.x < b.x) ? v.x : b.x) : a.x;
    result.y = (v.y > a.y) ? ((v.y < b.y) ? v.y : b.y) : a.y;
    return result;
}

#pragma endregion

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

inline int max(int a, int b) {
    return (a > b) ? a : b;
}
inline fixed32 max(int a, fixed32 b) {
    return (a > b) ? fixed32(a) : b;
}
inline fixed32 max(fixed32 a, int b) {
    return (a > b) ? a : fixed32(b);
}
inline fixed32 max(fixed32 a, fixed32 b) {
    return (a > b) ? a : b;
}
inline int min(int a, int b) {
    return (a < b) ? a : b;
}
inline fixed32 min(int a, fixed32 b) {
    return (a < b) ? fixed32(a) : b;
}
inline fixed32 min(fixed32 a, int b) {
    return (a < b) ? a : fixed32(b);
}
inline fixed32 min(fixed32 a, fixed32 b) {
    return (a < b) ? a : b;
}
inline fixed32 clamp(fixed32 a, fixed32 b, fixed32 v) {
    return min(b, max(a, v));
}

typedef struct rect {
    Vector2 pos;
    Vector2 size;
} Rect;

Rect resizeRect(Rect r, int magnitudeDifference);

// Returns int in [min, max)
int randRange(int min, int max);

// Returns -1, 0, or 1 based on sign
int sign(int v);

int mod(int a, int b);

}

#endif