#include <bits/stdc++.h>
#include "../number_theory/mint.h"
#include "NTT.h"
using namespace std;
typedef long long ll;

const int mod = 998244353;

typedef mint<mod> mi;
typedef vector<mi> poly;

// n <= 5e5, time = 444 ms
// res = (1 / a(x)) mod x^{n}
poly inverse(const poly& a, int n) {
    auto mult = [](poly a, poly b, int m) -> poly {
        a.resize(m); b.resize(m);
        auto res = NTT<>::mult(a, b);
        res.resize(m);
        return res;
    };
    int k = 0;
    while ((1 << k) < n) k++;
    poly res = {a[0].inv()};
    for (int i = 1; i <= k; i++) {
        int m = 1 << i;
        auto cur = mult(a, res, m);
        for (auto& j : cur) j = -j;
        cur[0] += 2;
        res = mult(res, cur, m);
    }
    res.resize(n);
    return res;
}

poly subtract(poly a, poly b) {
    int n = (int) max(a.size(), b.size());
    a.resize(n); b.resize(n);
    for (int i = 0; i < n; i++) a[i] -= b[i];
    return a;
}

// n, m <= 5e5 + 5, time = 583 ms
// res = {d, r}, a(x) = b(x) d(x) + r(x), deg(r) < deg(b)
pair<poly, poly> euclidean_division(const poly& a, const poly& b) {
    auto rev = [](poly a) -> poly { reverse(a.begin(), a.end()); return a; };
    int n = (int) a.size(); int m = (int) b.size();
    if (n < m) return {{0}, a};
    auto a_r = rev(a);
    auto b_r = rev(b);
    auto b_r_inv = inverse(b_r, n - m + 1);
    auto d_r = NTT<>::mult(a_r, b_r_inv);
    d_r.resize(n - m + 1);
    auto d = rev(d_r);
    auto r = subtract(a, NTT<>::mult(b, d));
    r.resize(b.size() - 1);
    return {d, r};
}
