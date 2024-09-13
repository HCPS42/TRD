#include "point.h"

bool cmp_angle(con P& a, con P& b) {
    ll area = cro(a, b);
    if (area > 0) return true;
    else if (area < 0) return false;
    else return a < b;
}

typedef vector<P> poly;

poly convex_hull(poly a) {
    sort(a.begin(), a.end());
    P shift = a[0];
    for (P& p: a) {
        p = p - shift;
    }
    sort(a.begin(), a.end(), cmp_angle);
    poly h;
    for (P& A: a) {
        if (!h.empty() && h.back() == A) continue;
        while (h.size() >= 2) {
            P B = h.back();
            P C = h[h.size() - 2];
            if (cro(C, B, A) <= 0) {
                h.pop_back();
            }
            else break;
        }
        h.push_back(A);
    }
    for (P& p: h) {
        p = p + shift;
    }
    return h;
}
