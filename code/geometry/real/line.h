#include "point.h"

struct Line {
    P p[2];
    Line(P a, P b) { p[0] = a; p[1] = b; }
    P& oper[](int i) { return p[i]; }
    con P& oper[](int i) con { return p[i]; }
    P normal() const {
        P a = p[1] - p[0];
        return {-a.y, a.x};
    }
    vector<R> line_eq() { // ax + by + c = 0; (a, b) > (0, 0)
        R a = p[1].y - p[0].y;
        R b = p[0].x - p[1].x;
        R c = -(a * p[0].x + b * p[0].y);
        vector<R> res{a, b, c};
        if (is_less(a, 0) || (zero(a) && is_less(b, 0))) {
            for (R& i : res) i *= -1;
        }
        return res;
    }
    vector<R> line_eq_norm() { // ax + by + c = 0; (a, b) > (0, 0); a^2 + b^2 = 1
        vector<R> res = line_eq();
        R a = res[0]; R b = res[1];
        R d = sqrt(sq(a) + sq(b));
        for (R& i : res) i /= d;
        return res;
    }
};

P proj(con P& p, con Line& l) {
    P v = l[1] - l[0];
    return l[0] + (dot(v, p - l[0]) / dot(v, v)) * v;
}

bool is_on_line(con P& p, con Line& l) {
    return zero(cro(l[0], l[1], p));
}

vector<P> inter_lines(con Line& a, con Line& b) {
    P va = a[1] - a[0];
    P vb0 = b[0] - a[0];
    P vb1 = b[1] - a[0];
    R quad = cro(vb1, va) + cro(va, vb0);
    if (zero(quad)) {
        if (is_on_line(a[0], b)) {
            return {a[0], a[1]};
        }
        return {};
    }
    R tri = cro(vb1, vb0);
    return {a[0] + (tri / quad) * va};
}

P reflect(P p, Line l) {
    P A = proj(p, l);
    return 2 * A - p;
}

bool is_on_seg(P p, Line l) {
    return zero(dist(p, l[0]) + dist(p, l[1]) - dist(l[0], l[1]));
}

bool are_parallel(Line a, Line b) {
    return is_eq(cro(a[0], b[0], a[1]), cro(a[0], b[1], a[1]));
}

vector<P> inter_segs(Line a, Line b) {
    if (!(a[0] < a[1])) swap(a[0], a[1]);
    if (!(b[0] < b[1])) swap(b[0], b[1]);
    if (are_parallel(a, b)) {
        if (!is_on_line(b[0], a)) return {};
        vector<P> res(2);
        for (int i = 0; i < 2; i++) {
            if ((a[i] < b[i]) ^ i) res[i] = b[i];
            else res[i] = a[i];
        }
        if (res[0] == res[1]) res.pop_back();
        if (res.size() == 2 && (res[1] < res[0])) return {};
        return res;
    }
    vector<P> p = inter_lines(a, b);
    if (is_on_seg(p[0], a) && is_on_seg(p[0], b)) return p;
    return {};
}
