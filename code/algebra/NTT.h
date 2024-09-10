#include <bits/stdc++.h>
#include "../number_theory/mint.h"
using namespace std;
typedef long long ll;

// https://judge.yosupo.jp/problem/convolution_mod

/*
prime <985661441> { static constexpr int a =         5 * 47, b = 22, g =  3; };
prime <976224257> { static constexpr int a =     7 * 7 * 19, b = 20, g =  3; };
prime <975175681> { static constexpr int a =     3 * 5 * 31, b = 21, g = 17; };
prime <962592769> { static constexpr int a = 3 * 3 * 3 * 17, b = 21, g =  7; };
*/

template <int mod = 998244353, int root = 3>
struct NTT {
    using Mint = mint<mod>;
    using Poly = vector<Mint>;
    // n, m <= 2^19, time = 206 ms
    static Poly mult(Poly a, Poly b) {
        int n = (int) a.size();
        int m = (int) b.size();
        if (!n || !m) return {};
        int k = 1;
        while (k < n + m - 1) k <<= 1;;
        a.resize(k); b.resize(k);
        nft(false, a); nft(false, b);
        for (int i = 0; i < k; i++) a[i] *= b[i];
        nft(true, a);
        a.resize(n + m - 1);
        return a;
    }
    static void nft(bool inverse, Poly& a) {
        int n = (int) a.size();
        int s = 0;
        while ((1 << s) < n) s++;
        assert(1 << s == n);
        static Poly ep, iep;
        while (ep.size() <= s) {
            ep.push_back(binpow(Mint(root), (mod - 1) / (1 << ep.size())));
            iep.push_back(ep.back().inv());
        }
        Poly b(n);
        for (int i = 1; i <= s; i++) {
            int w = 1 << (s - i);
            Mint base = inverse ? iep[i] : ep[i];
            Mint cur = 1;
            for (int y = 0; y < n / 2; y += w) {
                for (int x = 0; x < w; x++) {
                    auto l = a[(y << 1) | x];
                    auto r = cur * a[(y << 1) | x | w];
                    b[y | x] = l + r;
                    b[y | x | (n >> 1)] = l - r;
                }
                cur *= base;
            }
            swap(a, b);
        }
        if (inverse) {
            auto inv_n = Mint(n).inv();
            for (auto& i : a) i *= inv_n;
        }
    }
};
