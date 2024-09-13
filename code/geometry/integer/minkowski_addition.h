#include "point.h"

typedef vector<P> poly;

poly inv(poly a) {
    for (P& p : a) {
        p = -p;
    }
    int id = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] < a[id]) {
            id = i;
        }
    }
    rotate(a.begin(), a.begin() + id, a.end());
    return a;
}

poly sum(vector<poly> a) {
    int n = (int) a.size();
    for (int i = 0; i < n; i++) {
        a[i].push_back(a[i][0]);
        a[i].push_back(a[i][1]);
    }
    vector<int> ptr(n, 0);
    poly res;
    P A;
    for (int i = 0; i < n; i++) {
        A = A + a[i][0];
    }
    while (true) {
        P delta;
        vector<int> ids;
        for (int i = 0; i < n; i++) {
            if (a[i].size() == 3) continue;
            if (ptr[i] == a[i].size() - 2) continue;
            P cur = a[i][ptr[i] + 1] - a[i][ptr[i]];
            ll area = cro(cur, delta);
            if (ids.empty()) {
                delta = cur;
                ids.push_back(i);
            }
            else if (area > 0) {
                delta = cur;
                ids.clear();
                ids.push_back(i);
            }
            else if (area == 0) {
                delta = delta + cur;
                ids.push_back(i);
            }
        }
        if (ids.empty()) break;
        res.push_back(A);
        A = A + delta;
        for (int id : ids) {
            ptr[id]++;
        }
    }
    if (res.empty()) {
        res.push_back(A);
    }
    return res;
}
