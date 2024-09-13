#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
// CHT<MIN> https://atcoder.jp/contests/dp/tasks/dp_z
// CHT<MAX> https://atcoder.jp/contests/dp/tasks/dp_z

struct Line {
    mutable ll k, m, p; // k x + m, p is the x-coordinate where the line is no longer optimal
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

enum Pol { MIN, MAX }; // Get policy

template <Pol pol>
struct CHT : multiset<Line, less<>> {
    // (for doubles, use inf = 1 / .0, div(a, b) = a / b)
    static const ll inf = numeric_limits<ll>::max();
    ll div(ll a, ll b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = inf; return false; }
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) { // k x + m
        if constexpr (pol == MIN) { k = -k; m = -m; }
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    ll get(ll x) {
        if (empty()) {
            if constexpr (pol == MIN) return numeric_limits<ll>::max();
            else return numeric_limits<ll>::min();
        }
        auto l = *lower_bound(x);
        auto res = l.k * x + l.m;
        if constexpr (pol == MIN) res = -res;
        return res;
    }
};

// STRESS

template <class T>
struct Line2 {
    T a;
    T b;
    T eval(T x) const {
        return a * x + b;
    }
    friend bool operator<(const Line2& l, const Line2& r) {
        return std::tie(r.a, l.b) < std::tie(l.a, r.b);
    }
};

struct Data {
    vector<Line2<ll>> lines;
    vector<ll> xs;
};

mt19937 rnd;

Data gen() {
    const ll MAX = 1e9;
    uniform_int_distribution<ll> distr(-MAX, MAX);
    Data res;
    int n = rnd() % 100;
    for (int i = 0; i < n; i++) {
        res.lines.push_back({distr(rnd), distr(rnd)});
    }
    int m = rnd() % 100;
    for (int i = 0; i < m; i++) {
        res.xs.push_back(distr(rnd));
    }
    return res;
}

template <Pol pol>
vector<ll> slow(Data d) {
    vector<ll> res;
    for (ll x : d.xs) {
        if constexpr (pol == MIN) {
            ll mn = numeric_limits<ll>::max();
            for (auto& l : d.lines) {
                mn = min(mn, l.eval(x));
            }
            res.push_back(mn);
        }
        else {
            ll mx = numeric_limits<ll>::min();
            for (auto& l : d.lines) {
                mx = max(mx, l.eval(x));
            }
            res.push_back(mx);
        }
    }
    return res;
}

template <Pol pol>
vector<ll> fast(Data d, CHT<pol> cht) {
    vector<ll> res;
    for (auto& l : d.lines) {
        cht.add(l.a, l.b);
    }
    for (ll x : d.xs) {
        res.push_back(cht.get(x));
    }
    return res;
}

void stress() {
    for (int i = 1; i <= 10'000; i++) {
        auto d = gen();
        CHT<MIN> cht;
        assert(slow<MIN>(d) == fast(d, cht));
    }
    for (int i = 1; i <= 10'000; i++) {
        auto d = gen();
        CHT<MAX> cht;
        assert(slow<MAX>(d) == fast(d, cht));
    }
}
