#include <bits/stdc++.h>
#include "point.h"
using namespace std;
typedef long long ll;

// ICPC WF 2020, Problem C
// https://codeforces.com/gym/104633

struct HP { // half-plane, P --- point with real coordinates
    // every half-plane allows the region to the LEFT of its line
    P p, pq;
    R ang;
    HP() = default;
    HP(con P& a, con P& b) : p(a), pq(b - a) { ang = atan2(pq.y, pq.x); }
    bool cross(con P& a) { return cro(pq, a - p) < -eps; }
    bool operator<(const HP& a) const {
        if (fabs(ang - a.ang) < eps) return cro(pq, a.p - p) < 0;
        return ang < a.ang;
    }
    bool operator==(const HP& ot) const { return fabsl(ang - ot.ang) < eps; }
    friend P inter(const HP& a, const HP& b) { // a and b should not be parallel
        R alpha = R(cro((b.p - a.p), b.pq)) / R(cro(a.pq, b.pq));
        return a.p + (alpha * a.pq);
    }
};

vector<P> inter_hps(vector<HP> a) {
    static con R inf = 1e9;
    P box[4] = { P(inf, inf), P(-inf, inf), P(-inf, -inf), P(inf, -inf) };
    for (int i = 0; i < 4; i++) { a.push_back(HP{box[i], box[(i + 1) % 4]}); }
    sort(a.begin(), a.end()); a.erase(unique(a.begin(), a.end()), a.end());
    deque<HP> q; int n = 0;
    for (int i = 0; i < int(a.size()); i++) {
        while (n > 1 && a[i].cross(inter(q[n - 1], q[n - 2]))) { q.pop_back(); n--; }
        while (n > 1 && a[i].cross(inter(q[0], q[1]))) { q.pop_front(); n--; }
        q.push_back(a[i]); n++;
    }
    while (n > 2 && q[0].cross(inter(q[n - 1], q[n - 2]))) { q.pop_back(); n--; }
    while (n > 2 && q[n - 1].cross(inter(q[0], q[1]))) { q.pop_front(); n--; }
    if (n < 3) return {}; vector<P> res(n);
    for (int i = 0; i + 1 < n; i++) res[i] = inter(q[i], q[i + 1]);
    res.back() = inter(q[n - 1], q[0]);
    return res;
}
