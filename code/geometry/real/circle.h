#include "point.h"
#include "line.h"

struct Circle {
    P cen;
    R r = 0;
    Circle() = default;
    Circle(P cen, R r) : cen(cen), r(r) {}
    P rotate_rightmost(R angle) {
        return P(cen.x, cen.y) + P(r * cos(angle), r * sin(angle));
    }
    fri bool oper==(con Circle& a, con Circle& b) {
        return a.cen == b.cen && is_eq(a.r, b.r);
    }
};

vector<P> inter_circle_line(Circle c, Line l) {
    P p = proj(c.cen, l);
    R d = dist(c.cen, p);
    if (is_less(c.r, d)) return {};
    R a = sqrt(max(R(), sq(c.r) - sq(d)));
    P b = l[1] - l[0];
    a /= b.norm();
    P A = p + a * b;
    P B = p - a * b;
    if (A == B) return {A};
    return {A, B};
}

vector<P> inter_circle_circle(Circle a, Circle b) {
    if (a == b) return vector<P>(3);
    if (is_less(a.r, b.r)) swap(a, b);
    P dif = b.cen - a.cen;
    R dis = dif.norm();
    R ang = dif.angle();
    R mx = max(R(a.r), dis);
    R tot = a.r + b.r + dis;
    if (is_less(tot, 2 * mx)) return {};
    if (zero(2 * mx - tot)) return {a.rotate_rightmost(ang)};
    R beta = acos((sq(a.r) + sq(dis) - sq(b.r)) / (2 * a.r * dis));
    return {a.rotate_rightmost(ang - beta), a.rotate_rightmost(ang + beta)};
}

vector<P> tangents_point_circle(P p, Circle c) {
    R d = dist(p, c.cen);
    if (is_less(d, c.r)) return {};
    if (is_eq(d, c.r)) return {p};
    R from_cen = (p - c.cen).angle();
    R ang_dev = acos(c.r / d);
    return {c.rotate_rightmost(from_cen - ang_dev), c.rotate_rightmost(from_cen + ang_dev)};
}

vector<Line> outer_tangents(Circle c1, Circle c2) {
    if (c1 == c2) return {};
    if (c1.r < c2.r) swap(c1, c2);
    R d = dist(c1.cen, c2.cen);
    if (is_less(c2.r + d, c1.r)) return {};
    if (is_eq(c1.r, c2.r)) {
        P diff = c2.cen - c1.cen;
        P rad = (c1.r / diff.norm()) * diff.rotate(pi / 2);
        return {{c1.cen + rad, c2.cen + rad}, {c1.cen - rad, c2.cen - rad}};
    }
    P I = c1.cen + (c1.r / (c1.r - c2.r)) * (c2.cen - c1.cen);
    if (is_eq(c2.r + d, c1.r)) {
        return {{I, I + (c2.cen - c1.cen).rotate(pi / 2)}};
    }
    auto to1 = tangents_point_circle(I, c1);
    auto to2 = tangents_point_circle(I, c2);
    vector<Line> res{{to1[0], to2[0]}, {to1[1], to2[1]}};
    assert(is_on_line(I, res[0]));
    assert(is_on_line(I, res[1]));
    return res;
}

vector<Line> inner_tangents(Circle c1, Circle c2) {
    if (c1 == c2) return {};
    if (c1.r < c2.r) swap(c1, c2);
    R d = dist(c1.cen, c2.cen);
    if (is_less(d, c1.r + c2.r)) return {};
    P I = c1.cen + (c1.r / (c1.r + c2.r)) * (c2.cen - c1.cen);
    if (is_eq(d, c1.r + c2.r)) {
        return {{I, I + (c2.cen - c1.cen).rotate(pi / 2)}};
    }
    vector<P> to1 = tangents_point_circle(I, c1);
    vector<P> to2 = tangents_point_circle(I, c2);
    vector<Line> res{{to1[0], to2[0]}, {to1[1], to2[1]}};
    assert(is_on_line(I, res[0]));
    assert(is_on_line(I, res[1]));
    return res;
}

R disk_inter_area(Circle c1, Circle c2) {
    if (c1.r < c2.r) swap(c1, c2);
    R d = dist(c1.cen, c2.cen);
    if (c1.r + c2.r < d + eps) return 0;
    if (c1.r - c2.r > d - eps) return pi * sq(c2.r);
    R alpha = acos((sq(d) + sq(c1.r) - sq(c2.r)) / (2 * d * c1.r));
    R beta = acos((sq(d) + sq(c2.r) - sq(c1.r)) / (2 * d * c2.r));
    return alpha * sq(c1.r) + beta * sq(c2.r) - sin(2 * alpha) * sq(c1.r) / 2 - sin(2 * beta) * sq(c2.r) / 2;
}

Line rad_axis(Circle c1, Circle c2) {
    R d = dist(c1.cen, c2.cen);
    R a = (sq(c1.r) - sq(c2.r) + sq(d)) / (2 * d);
    P B = c1.cen + (a / d) * (c2.cen - c1.cen);
    P C = B + (c2.cen - c1.cen).rotate(pi / 2);
    return {B, C};
}
