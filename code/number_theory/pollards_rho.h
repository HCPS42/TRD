#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://judge.yosupo.jp/problem/primality_test
// https://judge.yosupo.jp/problem/factorize

typedef __int128_t LL;
typedef uint64_t ull;

ull modmul(ull a, ull b, ull M) {
    return (LL(a) * b) % M;
}

ull modpow(ull b, ull e, ull mod) {
    ull ans = 1;
    for (; e; b = modmul(b, b, mod), e >>= 1) {
        if (e & 1) ans = modmul(ans, b, mod);
    }
    return ans;
}

// 1e5 tests, n <= 1e18, time = 341 ms
bool is_prime(ull n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    ull s = __builtin_ctzll(n - 1), d = n >> s;
    for (ull a : A) {
        ull p = modpow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) p = modmul(p, p, n);
        if (p != n - 1 && i != s) return false;
    }
    return true;
}

/* sometimes incorrect
// 100 tests, n <= 1e18, time = 280 ms (sometimes > 10 s)
ull pollard(ull n) {
    auto f = [n](ull x) { return modmul(x, x, n) + 1; };
    ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
    while (t++ % 40 || __gcd(prd, n) == 1) {
        if (x == y) { x = ++i; y = f(x); }
        if ((q = modmul(prd, max(x, y) - min(x, y), n))) prd = q;
        x = f(x); y = f(f(y));
    }
    return __gcd(prd, n);
}
*/

// 100 tests, n <= 1e18, time = 1432 ms
ull pollard(ull n) { // correct but slower
    if (is_prime(n)) return n;
    if (n % 2 == 0) return 2;
    ull s = 0;
    auto f = [&](ull x) { return (LL(x) * x + s) % n; };
    while (true) {
        s++; ull x = s, y = f(x);
        while (true) {
            ull d = gcd(y - x + n, n);
            if (d == 0 || d == n) break;
            if (d != 1) return d;
            x = f(x); y = f(f(y));
        }
    }
}

// res = {prime factors in random order}
vector<ull> factorize(ull n) {
    if (n == 1) return {};
    if (is_prime(n)) return {n};
    ull x = pollard(n);
    auto l = factorize(x), r = factorize(n / x);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
