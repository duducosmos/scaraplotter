from math import sqrt

def intersection(x0, y0, r0, x1, y1, r1):
    dx = x1 - x0
    dy = y1 - y0
    dxy = dx / dy
    d = sqrt(dx ** 2.0 + dy ** 2.0)

    n = (d + r0 + r1)*(d + r0 - r1)* (d - r0 + r1) * (-d + r0 + r1)
    n = sqrt(n) / 4
    x = 0.5 * (x0 + x1) + dx * (r0 ** 2 - r1 ** 2) / (2.0 * d ** 2)
    x1 = x + 2.0 * n * dy / d ** 2
    x2 = x - 2.0 * n * dy / d ** 2
    print(x1, x2)

if(__name__ == "__main__"):
    intersection(-21.5, 0, 45, 0, 85, 45)
