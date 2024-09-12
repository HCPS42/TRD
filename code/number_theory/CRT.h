#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://codeforces.com/gym/102155/problem/F

template <class T>
T mod_inv(T r, T a) {
    T g = a, z = 0, y = 1;
    while (r != 0) {
        T q = g / r; g %= r;
        swap(g, r); z -= q * y; swap(z, y);
    }
    return z < 0 ? z + a : z;
}

template <class T>
pair<T, T> crt(T ra, T a, T rb, T b) {
    T d = __gcd(a, b); if (ra % d != rb % d) return {0, 0};
    a /= d; b /= d; T mod = a * b; T r = 0;
    r = ((ra / d) * b % mod) * mod_inv(b, a) % mod;
    r += ((rb / d) * a % mod) * mod_inv(a, b) % mod;
    if (r >= mod) r -= mod;
    mod *= d;
    T res = (r * d % mod + ra % d) % mod;
    return {res, mod};
}

template <class T>
pair<T, T> CRT(vector<T> r, vector<T> a) { // 0 <= r[i] < a[i]
    pair<T, T> res = {0, 1};
    for (int i = 0; i < a.size(); i++) {
        res = crt(res.first, res.second, r[i], a[i]);
        if (res.second == 0) return {0, 0};
    }
    return res;
}
