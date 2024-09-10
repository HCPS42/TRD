#include <bits/stdc++.h>
#include "../number_theory/mint.h"
#include "NTT.h"
#include "polynomial.h"
using namespace std;
typedef long long ll;

// https://codeforces.com/blog/entry/61306
// https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

const int mod = 998244353;

typedef mint<mod> mi;
typedef vector<mi> poly;

// res = {d, r}, a(x) = b(x) d(x) + r(x), deg(r) < deg(b)
pair<poly, poly> euclidean_division_given_inverse(const poly& a, const poly& b, const poly& b_r_inv) {
    auto rev = [](poly a) -> poly { reverse(a.begin(), a.end()); return a; };
    int n = (int) a.size(); int m = (int) b.size();
    if (n < m) return {{0}, a};
    auto a_r = rev(a);
    auto d_r = NTT<>::mult(a_r, b_r_inv);
    d_r.resize(n - m + 1);
    auto d = rev(d_r);
    auto r = subtract(a, NTT<>::mult(b, d));
    r.resize(b.size() - 1);
    return {d, r};
}

// res = (p(x) * q(x)) mod b(x)
// O(n log(n))
void fast_mult(poly& p, poly& q, const poly& b, const poly& b_r_inv) {
    int n = (int) p.size();
    auto r = NTT<>::mult(p, q);
    r.resize(2 * n);
    auto [_, res] = euclidean_division_given_inverse(r, b, b_r_inv);
    p = res; p.resize(n);
}

// n <= 1e5, k <= 1e18, time = 7878 ms
// 0-based, a = {a_0, a_1, ..., a_{n - 1}}
// b = find_linear_recurrence(a)
// a_i = \sum_{j = 1}^{n} b_j a_{i - j} (i >= n)
// O(n log(n) log(k))
mi find_kth(poly a, poly b, ll k) {
    if (k < a.size()) return a[k];
    int n = (int) b.size(); if (n == 0) return 0;
    poly s(n), t(n); s[0] = 1; if (n > 1) t[1] = 1; else t[0] = b[0];
    reverse(b.begin(), b.end());
    for (mi& i : b) i = -i;
    b.push_back(1);
    auto b_r = b;
    reverse(b_r.begin(), b_r.end());
    auto b_r_inv = inverse(b_r, n);
    while (k) {
        if (k & 1) fast_mult(s, t, b, b_r_inv);
        fast_mult(t, t, b, b_r_inv);
        k >>= 1;
    }
    mi res = 0;
    for (int i = 0; i < n; i++) res += s[i] * a[i];
    return res;
}
