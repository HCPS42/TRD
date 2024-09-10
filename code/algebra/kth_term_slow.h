#include <bits/stdc++.h>
#include "../number_theory/mint.h"
using namespace std;
typedef long long ll;

// https://codeforces.com/blog/entry/61306
// https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

const int mod = 998244353;

typedef mint<mod> mi;
typedef vector<mi> poly;

// res = (p(x) * q(x)) mod (x^n - b_{0} x^{n - 1} - ... - b_{n - 1})
// O(n^2)
void slow_mult(poly& p, poly& q, poly& b) {
    int n = (int) p.size();
    poly r(2 * n);
    for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) { r[i + j] += p[i] * q[j]; } }
    for (int i = 2 * n - 1; i >= n; i--) { for (int j = n - 1; j >= 0; j--) { r[i - j - 1] += r[i] * b[j]; } }
    for (int i = 0; i < n; i++) p[i] = r[i];
}

// 0-based, a = {a_0, a_1, ..., a_{n - 1}}
// b = find_linear_recurrence(a)
// a_i = \sum_{j = 1}^{n} b_j a_{i - j} (i >= n)
// O(n^2 log(k))
mi find_kth(poly a, poly b, ll k) {
    if (k < a.size()) return a[k];
    int n = (int) b.size(); if (n == 0) return 0;
    poly s(n), t(n); s[0] = 1; if (n > 1) t[1] = 1; else t[0] = b[0];
    while (k) {
        if (k & 1) slow_mult(s, t, b);
        slow_mult(t, t, b);
        k >>= 1;
    }
    mi res = 0;
    for (int i = 0; i < n; i++) res += s[i] * a[i];
    return res;
}
