#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef long double R;

#define con const
#define oper operator
#define fri friend

ll sq(ll x) { return x * x; }

struct P {
    ll x = 0; ll y = 0;
    P() = default;
    P(ll x, ll y) : x(x), y(y) {}
    fri ostream& oper<<(ostream& out, P& a) { return out << a.x << " " << a.y; }
    P oper-() con { return {-x, -y}; }
    fri P oper+(P a, con P& b) { a.x += b.x; a.y += b.y; return a; }
    fri P oper-(P a, con P& b) { a.x -= b.x; a.y -= b.y; return a; }
    fri P oper*(ll a, P b) { b.x *= a; b.y *= a; return b; }
    fri P oper/(P a, ll b) { a.x /= b; a.y /= b; return a; }
    bool is_zero() con { return x == 0 && y == 0; }
    bool oper==(con P& a) con { return (*this - a).is_zero(); }
    fri bool oper<(con P& a, con P& b) {
        return std::tie(a.y, a.x) < std::tie(b.y, b.x);
    }
    fri ll dot(con P& a, con P& b) { return a.x * b.x + a.y * b.y; }
    fri ll dot (con P& a, con P& b, con P& c) { return dot(b - a, c - a); }
    fri ll cro(con P& a, con P& b) { return a.x * b.y - a.y * b.x; }
    fri ll cro (con P& a, con P& b, con P& c) { return cro(b - a, c - a); }
    ll norm2() con { return sq(x) + sq(y); }
    ll norm() con { return sqrt(R(sq(x) + sq(y))); }
    fri ll dist2(con P& a, con P& b) { return (a - b).norm2(); }
    fri R dist(con P& a, con P& b) { return (a - b).norm(); }
    R angle() { return atan2(R(y), R(x)); }
};
