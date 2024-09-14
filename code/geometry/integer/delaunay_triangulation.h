#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// PTZ Winter 2024, Day 1, problem I
// https://official.contest.yandex.com/ptz-winter-2024/contest/59080/problems/I1/

bool is_eq(ll x, ll y) { return x == y; }
bool is_less(ll x, ll y) { return x < y; }
int sgn(ll a) { return a >= 0 ? a ? 1 : 0 : -1; }

struct pt {
    ll x, y;
    pt() = default;
    pt(ll x, ll y) : x(x), y(y) { }
    pt operator-(const pt& p) const { return {x - p.x, y - p.y}; }
    ll cro(const pt& p) const { return x * p.y - y * p.x; }
    ll cro(const pt& a, const pt& b) const { return (a - *this).cro(b - *this); }
    ll dot(const pt& p) const { return x * p.x + y * p.y; }
    ll dot(const pt& a, const pt& b) const { return (a - *this).dot(b - *this); }
    ll norm2() const { return this->dot(*this); }
    bool operator==(const pt& p) const { return is_eq(x, p.x) && is_eq(y, p.y); }
};

const pt inf_pt = pt(1e18, 1e18);

struct Edge {
    pt cen; Edge* rot = nullptr, *nxt = nullptr; bool used = false;
    Edge* rev() const { return rot->rot; }
    Edge* lnext() const { return rot->rev()->nxt->rot; }
    Edge* oprev() const { return rot->nxt->rot; }
    pt dest() const { return rev()->cen; }
};

Edge* make_edge(pt from, pt to) {
    Edge* e1 = new Edge, *e2 = new Edge, *e3 = new Edge, *e4 = new Edge;
    e1->cen = from; e2->cen = to; e3->cen = e4->cen = inf_pt;
    e1->rot = e3; e2->rot = e4; e3->rot = e2; e4->rot = e1;
    e1->nxt = e1; e2->nxt = e2; e3->nxt = e4; e4->nxt = e3;
    return e1;
}

void splice(Edge* a, Edge* b) {
    swap(a->nxt->rot->nxt, b->nxt->rot->nxt); swap(a->nxt, b->nxt);
}

void delete_edge(Edge* e) {
    splice(e, e->oprev()); splice(e->rev(), e->rev()->oprev());
    delete e->rev()->rot; delete e->rev(); delete e->rot; delete e;
}

Edge* connect(Edge* a, Edge* b) {
    Edge* e = make_edge(a->dest(), b->cen);
    splice(e, a->lnext()); splice(e->rev(), b); return e;
}

bool left_of(pt p, Edge* e) { return is_less(0, p.cro(e->cen, e->dest())); }
bool right_of(pt p, Edge* e) { return is_less(p.cro(e->cen, e->dest()), 0); }

template <class T>
T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
    return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

bool in_circle(pt a, pt b, pt c, pt d) {
// If there is __int128, calculate directly.
// Otherwise, calculate angles.
#if defined(__LP64__) || defined(_WIN64)
    __int128 det = -det3<__int128>(b.x, b.y, b.norm2(), c.x, c.y, c.norm2(), d.x, d.y, d.norm2());
    det += det3<__int128>(a.x, a.y, a.norm2(), c.x, c.y, c.norm2(), d.x, d.y, d.norm2());
    det -= det3<__int128>(a.x, a.y, a.norm2(), b.x, b.y, b.norm2(), d.x, d.y, d.norm2());
    det += det3<__int128>(a.x, a.y, a.norm2(), b.x, b.y, b.norm2(), c.x, c.y, c.norm2());
    return det > 0;
#else
    auto ang = [](pt l, pt mid, pt r) {
        ll x = mid.dot(l, r); ll y = mid.cro(l, r);
        long double res = atan2((long double)x, (long double)y); return res; };
    long double kek = ang(a, b, c) + ang(c, d, a) - ang(b, c, d) - ang(d, a, b);
    if (kek > 1e-8) return true; else return false;
#endif
}

pair<Edge*, Edge*> build_tr(int l, int r, vector<pt>& p) {
    if (r - l + 1 == 2) {
        Edge* res = make_edge(p[l], p[r]); return make_pair(res, res->rev());
    }
    if (r - l + 1 == 3) {
        Edge *a = make_edge(p[l], p[l + 1]), *b = make_edge(p[l + 1], p[r]);
        splice(a->rev(), b);
        int sg = sgn(p[l].cro(p[l + 1], p[r]));
        if (sg == 0) return make_pair(a, b->rev());
        Edge* c = connect(b, a);
        if (sg == 1) return make_pair(a, b->rev());
        else return make_pair(c->rev(), c);
    }
    int mid = (l + r) / 2; Edge *ldo, *ldi, *rdo, *rdi;
    tie(ldo, ldi) = build_tr(l, mid, p); tie(rdi, rdo) = build_tr(mid + 1, r, p);
    while (true) {
        if (left_of(rdi->cen, ldi)) { ldi = ldi->lnext(); continue; }
        if (right_of(ldi->cen, rdi)) { rdi = rdi->rev()->nxt; continue; }
        break;
    }
    Edge* basel = connect(rdi->rev(), ldi);
    auto valid = [&basel](Edge* e) { return right_of(e->dest(), basel); };
    if (ldi->cen == ldo->cen) ldo = basel->rev();
    if (rdi->cen == rdo->cen) rdo = basel;
    while (true) {
        Edge* lcand = basel->rev()->nxt;
        if (valid(lcand)) {
            while (in_circle(basel->dest(), basel->cen, lcand->dest(), lcand->nxt->dest())) {
                Edge* t = lcand->nxt; delete_edge(lcand); lcand = t;
            }
        }
        Edge* rcand = basel->oprev();
        if (valid(rcand)) {
            while (in_circle(basel->dest(), basel->cen, rcand->dest(), rcand->oprev()->dest())) {
                Edge* t = rcand->oprev(); delete_edge(rcand); rcand = t;
            }
        }
        if (!valid(lcand) && !valid(rcand)) break;
        if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->cen,
                                                        rcand->cen, rcand->dest())))
            basel = connect(rcand, basel->rev());
        else basel = connect(basel->rev(), lcand->rev());
    }
    return make_pair(ldo, rdo);
}

vector<tuple<pt, pt, pt>> delaunay(vector<pt> p) {
    sort(p.begin(), p.end(), [](const pt& a, const pt& b) {
        return is_less(a.x, b.x) || (is_eq(a.x, b.x) && is_less(a.y, b.y));
    });
    auto res = build_tr(0, (int)p.size() - 1, p);
    Edge* e = res.first; vector<Edge*> edges = {e};
    while (is_less(e->nxt->dest().cro(e->dest(), e->cen), 0)) e = e->nxt;
    auto add = [&p, &e, &edges]() {
        Edge* curr = e;
        do { curr->used = true; p.push_back(curr->cen);
            edges.push_back(curr->rev()); curr = curr->lnext();
        } while (curr != e);
    };
    add(); p.clear(); int kek = 0;
    while (kek < (int)edges.size()) { if (!(e = edges[kek++])->used) add(); }
    vector<tuple<pt, pt, pt>> ans;
    for (int i = 0; i < (int)p.size(); i += 3) {
        ans.push_back(make_tuple(p[i], p[i + 1], p[i + 2]));
    }
    return ans;
}
