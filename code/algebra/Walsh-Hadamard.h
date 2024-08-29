#include <bits/stdc++.h>
#include "../number_theory/mint.h"
using namespace std;
typedef long long ll;

// https://codeforces.com/problemset/problem/914/G
// https://judge.yosupo.jp/problem/bitwise_xor_convolution
// https://judge.yosupo.jp/problem/bitwise_and_convolution

const int mod = 1e9 + 7;
typedef mint<mod> mi;
typedef vector<mi> poly;

void tr_xor(poly& a, int l, int r) {
    if (r - l == 1) return;
    int m = (l + r) / 2;
    tr_xor(a, l, m);
    tr_xor(a, m, r);
    for (int i = 0; i < m - l; i++) {
        mi x = a[l + i];
        mi y = a[m + i];
        a[l + i] = x + y;
        a[m + i] = x - y;
    }
}

void inv_xor(poly& a) {
    int n = a.size();
    tr_xor(a, 0, n);
    mi inv_n = mi(n).inv();
    for (int i = 0; i < n; i++) a[i] *= inv_n;
}

poly conv_xor(poly a, poly b) {
    // n = power of 2
    int n = a.size();
    tr_xor(a, 0, n);
    tr_xor(b, 0, n);
    for (int i = 0; i < n; i++) a[i] *= b[i];
    inv_xor(a);
    return a;
}

void tr_or(poly& a, int l, int r) {
    if (r - l == 1) return;
    int m = (l + r) / 2;
    tr_or(a, l, m);
    tr_or(a, m, r);
    for (int i = 0; i < m - l; i++) a[m + i] += a[l + i];
}

void inv_or(poly& a, int l, int r) {
    if (r - l == 1) return;
    int m = (l + r) / 2;
    for (int i = 0; i < m - l; i++) a[m + i] -= a[l + i];
    inv_or(a, l, m);
    inv_or(a, m, r);
}

poly conv_or(poly a, poly b) {
    // n = power of 2
    int n = a.size();
    tr_or(a, 0, n);
    tr_or(b, 0, n);
    for (int i = 0; i < n; i++) a[i] *= b[i];
    inv_or(a, 0, n);
    return a;
}

poly conv_and(poly a, poly b) {
    // n = power of 2
    int n = a.size();
    int mask = n - 1;
    for (int i = 0; i < n / 2; i++) {
        swap(a[i], a[i ^ mask]);
        swap(b[i], b[i ^ mask]);
    }
    a = conv_or(a, b);
    for (int i = 0; i < n / 2; i++) {
        swap(a[i], a[i ^ mask]);
    }
    return a;
}