#include <bits/stdc++.h>
#include "../other/hash_map.h"
using namespace std;
typedef long long ll;

// https://acm.timus.ru/problem.aspx?space=1&num=2124

vector<int> find_divs(int n) {
    vector<int> res;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) { res.push_back(i); res.push_back(n / i); }
    }
    return res;
}
 
int binpow(int a, ll b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * 1ll * a % p;
        a = a * 1ll * a % p;
        b >>= 1;
    }
    return res;
}
 
int find_root(int p) {
    if (p == 2) return 1;
    vector<int> d = find_divs(p - 1);
    for (int g = 2; ; g++) {
        bool ok = true;
        for (int i : d) {
            if (binpow(g, (p - 1) / i, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
 
hash_map<int, int> Map;
hash_map<int, int> Dlog;

void precalc(int g, int p, int n) {
    n = 2 * (sqrt(double(p) / double(n)) + 1);
    int gn = binpow(g, n, p); int cur = 1;
    for (int x = 0; (x - 1) * n < p - 1; x++) {
        Map[cur] = x * n;
        cur = cur * 1ll * gn % p;
    }
}

int dlog(int x, int g, int p) {
    for (int i = 0; i < N; i++) {
        if (Map.count(x)) return Map[x] - i;
        x = x * 1ll * g % p;
    }
    assert(0);
}

// 0 <= rs[i] < p, g --- primitive root mod p
void find_dlogs(vector<int> rs, int g, int p) {
    precalc(g, p, (int) rs.size());
    for (int x : rs) {
        int l = dlog(x, g, p);
        Dlog[x] = ((l % (p - 1)) + p - 1) % (p - 1);
    }
}
