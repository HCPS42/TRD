#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://judge.yosupo.jp/problem/eulerian_trail_undirected

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

set<pair<int, int>> g[N]; // g[v] = {{u, id}, ...}
vector<int> vs;
vector<int> es;

void dfs(int v, int par_id = -1) {
    while (!g[v].empty()) {
        auto it = g[v].begin();
        auto [u, id] = *it;
        g[v].erase(it);
        if (v != u) g[u].erase({v, id});
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
            dsu.unite(i, j);
            ls.push_back(i);
        }
    }
    for (int i = 1; i < ls.size(); i++) {
        if (dsu.find(ls[0]) != dsu.find(ls[i])) return;
    }
    vector<int> cs;
    for (int v = 1; v <= n; v++) {
        int cnt = 0;
        for (auto [u, id] : g[v]) {
            if (u != v) cnt++;
        }
        if (cnt & 1) cs.push_back(v);
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
        src = cs[0];
    }
    if (src == -1) {
        vs.push_back(1);
        return;
    }
    dfs(src);
    reverse(vs.begin(), vs.end());
    reverse(es.begin(), es.end());
}
