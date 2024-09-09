#include <bits/stdc++.h>
#include "../number_theory/mint.h"
using namespace std;
typedef long long ll;

// https://official.contest.yandex.com/ptz-winter-2022/contest/34820/problems/M1
// https://codeforces.com/blog/entry/61306
// https://judge.yosupo.jp/problem/find_linear_recurrence

const int mod = 998244353;

typedef mint<mod> mi;
typedef vector<mi> poly;

// a.size() = 10000, time = 126 ms
// res = {b_1, b_2, ..., b_n}, shortest recurrence, not unique
// a_i = \sum_{j = 1}^{n} b_j a_{i - j} (i >= n)
// O(n^2)
poly find_linear_recurrence(poly a) {
    poly s, cur; int pos = 0; mi d;
    for (int i = 0; i < a.size(); i++) {
        mi t = 0;
        for (int j = 0; j < cur.size(); j++) t += a[i - j - 1] * cur[j];
        if (t.x == a[i].x) continue;
        if (cur.empty()) { cur.resize(i + 1); pos = i; d = t - a[i]; continue; }
        mi k = -(a[i] - t) / d; poly c(i - pos - 1); c.push_back(k);
        for (int j = 0; j < s.size(); j++) c.push_back(-s[j] * k);
        if (c.size() < cur.size()) c.resize(cur.size());
        for (int j = 0; j < cur.size(); j++) c[j] += cur[j];
        if (i - pos + s.size() >= cur.size()) { s = cur; pos = i; d = t - a[i]; }
        cur = c;
    }
    return cur;
}
