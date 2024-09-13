#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://judge.yosupo.jp/problem/eulerian_trail_directed

const int N = 2e5 + 5;

struct DSU {
    int par[N];
    int dep[N];
    void init(int n) {
        iota(par, par + n + 1, 0);
        fill(dep, dep + n + 1, 0);
    }
    int find(int v) {
        return par[v] == v ? v : par[v] = find(par[v]);
    }
    void unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return;
        if (dep[u] < dep[v]) swap(u, v);
        par[v] = u;
        if (dep[u] == dep[v]) dep[u]++;
    }
};

DSU dsu;

vector<pair<int, int>> g[N]; // g[v] = {{u, id}, ...}
int cnt[N];
vector<int> vs;
vector<int> es;

void dfs(int v, int par_id = -1) {
    while (!g[v].empty()) {
        auto [u, id] = g[v].back();
        g[v].pop_back();
        dfs(u, id);
    }
    vs.push_back(v);
    if (par_id != -1) es.push_back(par_id);
}

// res = {vs (vertices), es (edges)}
void find_eulerian_path(int n) {
    dsu.init(n);
    vector<int> ls;
    for (int i = 1; i <= n; i++) {
        for (auto [j, id] : g[i]) {
            cnt[i]--;
            cnt[j]++;
            dsu.unite(i, j);
            ls.push_back(i);
        }
    }
    for (int i = 1; i < ls.size(); i++) {
        if (dsu.find(ls[0]) != dsu.find(ls[i])) return;
    }
    vector<pair<int, int>> cs;
    for (int i = 1; i <= n; i++) {
        if (cnt[i] < -1 || 1 < cnt[i]) return;
        if (cnt[i] != 0) cs.push_back({cnt[i], i});
    }
    if (cs.size() > 2) return;
    int src = -1;
    if (cs.empty()) {
        for (int i = 1; i <= n; i++) {
            if (!g[i].empty()) {
                src = i;
                break;
            }
        }
    }
    else {
        sort(cs.begin(), cs.end());
        src = cs[0].second;
    }
    if (src == -1) {
        vs.push_back(1);
        return;
    }
    dfs(src);
    reverse(vs.begin(), vs.end());
    reverse(es.begin(), es.end());
}
