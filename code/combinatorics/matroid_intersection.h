#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// MW 2020 Fall Day 3 Problem K
// https://official.contest.yandex.com/mw2020fall/contest/23193/problems/K3

// #vertices or colors <= 100
// #edges <= 5000
// O((solution size) * (ground set size) * log(ground set size)) = O(#vertices * #edges * log(#edges))
const int K = 105;

struct Edge { int u, v; };
struct Elem { Edge e; int col; bool in = false; int pos, par; };

vector<Elem> all; vector<int> ind;

struct Graph {
    vector<Edge> es;
    int par[K], dep[K];
    int find(int v) { return par[v] == v ? v : par[v] = find(par[v]); }
    void unite(int u, int v) { u = find(u); v = find(v);
        if (u != v) {
            if (dep[u] < dep[v]) swap(u, v); par[v] = u; if (dep[u] == dep[v]) dep[u]++; } }
    void reset() { es.clear(); }
    void add_e(const Edge& e) { es.push_back(e); }
    void build() { fill(dep, dep + K, 0); iota(par, par + K, 0);
        for (auto& e : es) unite(e.u, e.v); }
    bool indep_with(const Edge& e) { return find(e.u) != find(e.v); }
};

struct GraphMatroid {
    Graph b, bw[K];
    bool oracle(int add) { return b.indep_with(all[add].e); }
    bool oracle(int add, int rem) { return bw[all[rem].pos].indep_with(all[add].e); }
    void init() { b.reset();
        for (int i = 0; i < ind.size(); i++) bw[i].reset();
        for (int i = 0; i < ind.size(); i++) { b.add_e(all[ind[i]].e);
            for (int j = 0; j < ind.size(); j++) if (j != i) bw[j].add_e(all[ind[i]].e); }
        b.build(); for (int i = 0; i < ind.size(); i++) bw[i].build(); }
};

struct ColorfulMatroid {
    bool used[K];
    bool oracle(int add) { return !used[all[add].col]; }
    bool oracle(int add, int rem) { add = all[add].col; rem = all[rem].col;
        if (!used[add]) return true; return add == rem; }
    void init() { fill(used, used + K, false); for (int i : ind) used[all[i].col] = true; }
};

GraphMatroid graph; ColorfulMatroid color;

bool augment() {
    const int src = -1, nvis = -2, nfou = -3; graph.init(); color.init();
    for (auto& e : all) e.par = nvis; int id = nfou; queue<int> q;
    for (int i = 0; i < all.size(); i++) if (color.oracle(i)) { all[i].par = src; q.push(i); }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        if (all[v].in) {
            for (int u = 0; u < all.size(); u++) {
                if (all[u].par == nvis && color.oracle(u, v)) {
                    all[u].par = v; q.push(u); } } continue; }
        if (graph.oracle(v)) { id = v; break; }
        for (int u : ind) {
            if (all[u].par == nvis && graph.oracle(v, u)) {
                all[u].par = v; q.push(u); } } }
    if (id == nfou) return false;
    do { all[id].in ^= true; id = all[id].par; } while (id != src);
    ind.clear();
    for (int i = 0; i < all.size(); i++) if (all[i].in) { all[i].pos = (int) ind.size(); ind.push_back(i); }
    return true;
}

void add_element(Edge e, int col) { all.push_back({e, col}); }
void matroid_intersection() { while (augment()); }

void Solve() {
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        add_element({u, v}, w);
    }
    matroid_intersection();
    cout << ind.size() << "\n";
    for (int i : ind) {
        cout << i + 1 << " ";
    }
    cout << "\n";
}
