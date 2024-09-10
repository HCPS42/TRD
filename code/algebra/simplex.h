#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef long double R;
typedef vector<R> vec;
typedef vector<vec> mat;

// maximize c^T x
// subject to: Ax <= b, x >= 0

const R eps = 1e-9;

struct LPSolver {
    int m, n;
    vector<int> B, N;
    mat D;
    LPSolver(const mat &A, const vec &b, const vec &c) :
        m((int) b.size()), n((int) c.size()), N(n + 1), B(m), D(m + 2, vec(n + 2)) {
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) D[i][j] = A[i][j];
        for (int i = 0; i < m; i++) { B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i]; }
        for (int j = 0; j < n; j++) { N[j] = j; D[m][j] = -c[j]; }
        N[n] = -1; D[m + 1][n] = 1;
    }
    void Pivot(int r, int s) {
        R inv = 1.0 / D[r][s];
        for (int i = 0; i < m + 2; i++) if (i != r)
            for (int j = 0; j < n + 2; j++) if (j != s)
                D[i][j] -= D[r][j] * D[i][s] * inv;
        for (int j = 0; j < n + 2; j++) if (j != s) D[r][j] *= inv;
        for (int i = 0; i < m + 2; i++) if (i != r) D[i][s] *= -inv;
        D[r][s] = inv;
        swap(B[r], N[s]);
    }
    bool Simplex(int phase) {
        int x = phase == 1 ? m + 1 : m;
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; j++) {
                if (phase == 2 && N[j] == -1) continue;
                if (s == -1 || (D[x][j] < D[x][s]) || ((D[x][j] == D[x][s]) && (N[j] < N[s]))) {
                    s = j;
                }
            }
            if (D[x][s] > -eps) return true;
            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] < eps) continue;
                if (r == -1 || (D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s]) ||
                    ((D[i][n + 1] / D[i][s]) == (D[r][n + 1] / D[r][s]) && (B[i] < B[r]))) {
                    r = i;
                }
            }
            if (r == -1) return false;
            Pivot(r, s);
        }
    }
    // res = {max c^T x, x}
    pair<R, vec> Solve() {
        vec x;
        int r = 0;
        for (int i = 1; i < m; i++) if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (D[r][n + 1] < -eps) {
            Pivot(r, n);
            if (!Simplex(1) || D[m + 1][n + 1] < -eps) return {-numeric_limits<R>::infinity(), x};
            for (int i = 0; i < m; i++) if (B[i] == -1) {
                int s = -1;
                for (int j = 0; j <= n; j++) {
                    if (s == -1 || (D[i][j] < D[i][s]) || ((D[i][j] == D[i][s]) && (N[j] < N[s]))) s = j;
                }
                Pivot(i, s);
            }
        }
        if (!Simplex(2)) return {numeric_limits<R>::infinity(), x};
        x = vec(n);
        for (int i = 0; i < m; i++) if (B[i] < n) x[B[i]] = D[i][n + 1];
        return {D[m][n + 1], x};
    }
};

// res = {max c^T x, x}
pair<R, vec> simplex(vector<vec> A, vec b, vec c) {
    int n = (int) A.size();
    int m = (int) A[0].size() + 1;
    int r = n;
    int s = m - 1;
    vector<vec> D(n + 2, vec(m + 1));
    vector<int> ix(n + m);
    for (int i = 0; i < n + m; i++) ix[i] = i;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m - 1; j++) D[i][j] = -A[i][j];
        D[i][m - 1] = 1;
        D[i][m] = b[i];
        if (D[r][m] > D[i][m]) r = i;
    }
    for (int j = 0; j < m - 1; j++) D[n][j] = c[j];
    D[n + 1][m - 1] = -1;
    double d;
    while (true) {
        if (r < n) {
            swap(ix[s], ix[r+m]);
            D[r][s] = 1.0 / D[r][s];
            for (int j = 0; j <= m; j++) {
                if (j != s) {
                    D[r][j] *= -D[r][s];
                }
            }
            for (int i = 0; i <= n + 1; i++) {
                if (i == r) continue;
                for (int j = 0; j <= m; j++) {
                    if (j != s) {
                        D[i][j] += D[r][j] * D[i][s];
                    }
                }
                D[i][s] *= D[r][s];
            }
        }
        r = s = -1;
        for (int j = 0; j < m; j++) {
            if (s < 0 || ix[s] > ix[j]) {
                if ((D[n + 1][j] > eps) || ((D[n + 1][j] > -eps) && (D[n][j] > eps))) {
                    s = j;
                }
            }
        }
        if (s < 0) break;
        for (int i = 0; i < n; i++) {
            if (D[i][s] < -eps) {
                if ((r < 0) || (((d = D[r][m] / D[r][s] - D[i][m] / D[i][s]) < -eps)
                    || ((d < eps) && (ix[r + m] > ix[i + m])))) {
                    r = i;
                }
            }
        }
        if (r < 0) {
            return {numeric_limits<R>::infinity(), {}}; // unbounded
        }
    }
    if (D[n + 1][m] < -eps) {
        return {-numeric_limits<R>::infinity(), {}}; // infeasible
    }
    vec x(m - 1);
    for (int i = m; i < n + m; i++) {
        if (ix[i] < m - 1) {
            x[ix[i]] = D[i - m][m];
        }
    }
    return {D[n][m], x}; // ans: D[n][m]
}

void stress() {
    for (int it = 1; it <= 300; it++) {
        int m = 10;
        int n = 1000;
        mat A(m, vec(n));
        vec b(m);
        vec c(n);
        mt19937 rnd((int) time(0));
        const int MX = 1000;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = rnd() % MX;
                b[i] = rnd() % MX;
                c[j] = rnd() % MX;
            }
        }
        LPSolver solver(A, b, c);
        auto [val_1, x_1] = solver.Solve();
        auto [val_2, x_2] = simplex(A, b, c);
        assert(abs(val_1 - val_2) < eps);
    }
}
