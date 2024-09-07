#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef long double R;

const R eps = 1e-10;

R f(R x) {
    return 0;
}

R simpson(R l, R r) {
    return R(1) / R(6) * (r - l) * (f(l) + R(4) * f((l + r) / R(2)) + f(r));
}

// R integral(R l, R r, std::function<R(R)> f) {
R integral(R l, R r) {
    R m  = (l + r) / R(2);
    R tot = simpson(l, r);
    R sum = simpson(l, m) + simpson(m, r);
    if (abs(tot - sum) < eps) return sum;
    return integral(l, m) + integral(m, r);
}

R generate_random_number() {
    mt19937 rnd;
    uniform_real_distribution<R> dist(0.0, 1.0);
    return dist(rnd);
}
