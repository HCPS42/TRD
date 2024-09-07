#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

template <int m>
struct mint {
    int x;
    mint(int x = 0) : x(x) {}
    friend mint binpow(mint a, ll b) {
        mint res = 1;
        while (b) { if (b & 1) res *= a; b >>= 1; a *= a; }
        return res;
    }
    mint inv() const { return binpow(*this, m - 2); }
    mint operator-() const { return x ? m - x : 0; }
    mint& operator+=(const mint& a) { x += a.x; if (x >= m) x -= m; return *this; }
    mint& operator-=(const mint& a) { x -= a.x; if (x < 0) x += m; return *this; }
    mint& operator*=(const mint& a) { x = x * 1ll * a.x % m; return *this; }
    mint& operator/=(const mint& a) { return *this *= a.inv(); }
    friend mint operator+(mint a, const mint& b) { return a += b; }
    friend mint operator-(mint a, const mint& b) { return a -= b; }
    friend mint operator*(mint a, const mint& b) { return a *= b; }
    friend mint operator/(mint a, const mint& b) { return a /= b; }
};
