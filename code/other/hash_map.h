#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// https://codeforces.com/contest/2004/problem/E

enum class S { FREE, IN_USE, ERASED };

template <class K, class V>
struct node { K k; V v; S s = S::FREE; };

template <class K, class V, class H = std::hash<K>>
class hash_map {
public:
    hash_map(size_t cap): cap(cap), sz(0) {
        a = new node<K, V>[cap];
        for (int i = 0; i < cap; i++) a[i] = node<K, V>();
    }
    hash_map(): hash_map(3) {}
    size_t count(const K& key) const {
        size_t id = get_id(key, cap);
        for (int d = 0; d < cap; d++) {
            if (a[id].s == S::FREE) return false;
            if (a[id].s == S::IN_USE && a[id].key == key) return true;
            id++; if (id == cap) id = 0;
        }
        return false;
    }
    V& operator[](const K& key) {
        if ((sz << 1) > cap) rehash(); size_t id;
        bool res = put(key, id, a, cap); if (res) sz++;
        return a[id].v;
    }
    ~hash_map() { delete[] a; }

private:
    size_t get_id(const K& key, size_t sz) const { return (h(key) * 22543) % sz; }
    void rehash() {
        size_t n_cap = (cap << 1);
        node<K, V>* b = new node<K, V>[n_cap];
        for (size_t i = 0; i < n_cap; i++) b[i] = node<K, V>();
        for (size_t i = 0; i < cap; i++)
            if (a[i].s == S::IN_USE) {
                size_t id; put(a[i].k, id, b, n_cap); b[id].v = a[i].v;
            }
        delete[] a; a = b; cap = n_cap;
    }
    bool put(const K& k, size_t& id, node<K, V>* a, size_t len) {
        id = get_id(k, len);
        for (size_t i = 0; i < len; i++) {
            if (a[id].s == S::FREE || a[id].s == S::ERASED) { 
                a[id].k = k; a[id].s = S::IN_USE; return true;
            }
            if (a[id].k == k) return false; id++; if (id == len) id = 0;
        }
        throw std::logic_error("Unexpected case.");
    }
    size_t cap, sz; node<K, V>* a; H h;
};
