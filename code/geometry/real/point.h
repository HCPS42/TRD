#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

#define con const
#define oper operator
#define fri friend

typedef long double R;

const R eps = 1e-10;
const R pi = acos(-1);

R sq(R x) { return x * x; }

bool zero(R x) { return abs(x) < eps; }
bool is_eq(R x, R y) { return zero(x - y); }
bool is_less(R x, R y) { return x - y <= -eps; }

struct P {
    R x = 0; R y = 0;
    P() = default;
    P(R x, R y) : x(x), y(y) {}
    fri ostream& oper<<(ostream& os, P& a) { return os << a.x << " " << a.y; }
    P oper-() con { return {-x, -y}; }
    fri P oper+(P a, con P& b) { a.x += b.x; a.y += b.y; return a; }
    fri P oper-(P a, con P& b) { a.x -= b.x; a.y -= b.y; return a; }
    fri P oper*(R a, P b) { b.x *= a; b.y *= a; return b; }
    fri P oper/(P a, R b) { a.x /= b; a.y /= b; return a; }
    bool is_zero() con { return zero(x) && zero(y); }
    bool oper==(con P& a) con { return (*this - a).is_zero(); }
    fri bool oper<(con P& a, con P& b) {
        return std::tie(a.y, a.x) < std::tie(b.y, b.x);
    }
    fri R dot(con P& a, con P& b) { return a.x * b.x + a.y * b.y; }
    fri R dot(con P& a, con P& b, con P& c) { return dot(b - a, c - a); }
    fri R cro(con P& a, con P& b) { return a.x * b.y - a.y * b.x; }
    fri R cro (con P& a, con P& b, con P& c) { return cro(b - a, c - a); }
    R norm2() con { return sq(x) + sq(y); }
    R norm() con { return sqrt(R(sq(x) + sq(y))); }
    fri R dist2(con P& a, con P& b) { return (a - b).norm2(); }
    fri R dist(con P& a, con P& b) { return (a - b).norm(); }
    R angle() { return atan2(R(y), R(x)); }
    void rotate_self(R ang) {
        R c = cos(ang); R s = sin(ang);
        R nx = c * x - s * y; R ny = s * x + c * y;
        x = nx; y = ny;
    }
    P rotate(R ang) con { P a(*this); a.rotate_self(ang); return a; }
};

R calc_angle(P A, P B, P C) { // angle ABC
    R ang2 = (A - B).angle();
    R ang1 = (C - B).angle();
    R res = ang1 - ang2;
    if (res < eps) {
        res += 2 * pi;
    }
    return res;
}