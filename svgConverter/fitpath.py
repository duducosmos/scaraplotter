from readsvg import ReadSVG
import math


class FitPath:
    def __init__(self, filename):
        self.svgpoints = ReadSVG(filename)()

    def get_bounding_box(self):
        points = self.svgpoints
        min_x, max_x = min([pix[0] for pix in points if type(pix) is not int]),\
            max([pix[0] for pix in points if type(pix) is not int])

        min_y, max_y = min([pix[1] for pix in points if type(pix) is not int]),\
            max([pix[1] for pix in points if type(pix) is not int])
        return (min_x, min_y, max_x - min_x, max_y - min_y)

    def quad_solve(self, a, b, c):
        d = b**2 - 4 * a * c
        if d < 0:
            return None
        elif d == 0:
            return (-b + math.sqrt(d)) / (2 * a)
        else:
            return max((-b + math.sqrt(d)) / (2 * a), (-b - math.sqrt(d)) / (2 * a))

    def get_y_circle(self, circle, x):
        xC, yC, rC = circle
        a = 1
        b = -2 * yC
        c = -2 * xC * x + yC**2 - rC**2 + x**2 + xC**2
        return self.quad_solve(a, b, c)

    def point_pos(self, x0, y0, d, theta):
        theta_rad = math.radians(theta)
        return x0 + d * math.cos(theta_rad), y0 + d * math.sin(theta_rad)

    def get_circles(self, r1, r2, xA, yA, xB, yB):
        angle_min = 13
        left_top = (xB, yB, r1 + r2)
        x, y = self.point_pos(xA, yA, r2, 180 - angle_min)
        left_bottom = (x, y, r1)
        return (left_top, left_bottom)

    def drange(self, start, stop, step):
        r = start
        while r < stop:
            yield r
            r += step

    def __call__(self, xa, ya, xb, yb, ac, ce):
        (bbox_x, bbox_y, bbox_w, bbox_h) = self.get_bounding_box()
        (left_top, left_bottom) = self.get_circles(ce - 1, ac, xa, ya, xb, yb)
        min_x = max(left_top[0] - left_top[2], left_bottom[0] - left_bottom[2])
        best_fit, best_fit_x, best_fit_y, best_scale = 10000, 0, 0, 0
        mx = (xb + xa) / 2.
        for x in self.drange(min_x, mx, 0.5):
            y1 = self.get_y_circle(left_top, x)
            y2 = self.get_y_circle(left_bottom, x)
            if (y1 != None and y2 != None):
                y1 -= 1
                y2 += 1
                if (y1 > y2):
                    if abs(((mx - x) * 2) / (y1 - y2) - (bbox_w / bbox_h)) < best_fit:
                        best_fit = abs((mx - x) * 2) / \
                            (y1 - y2) - (bbox_w / bbox_h)
                        best_fit_x = x
                        best_fit_y = y2
                        best_scale = (mx - x) * 2 / bbox_w

        new_points = []
        for point in self.svgpoints:
            if type(point) is int:
                new_points.append(point)
            else:
                new_points.append(((point[0] - bbox_x) * best_scale + best_fit_x,
                                   (point[1] - bbox_y) * best_scale + best_fit_y)
                                  )
        return new_points


if __name__ == "__main__":
    import serial
    import time
    from numpy import array
    import matplotlib.pyplot as plt




    fitpath = FitPath(filename="./images/test.svg")
    path = fitpath(-45.0 / 2.0, 0, 45.0 / 2.0, 0, 45, 45)

    x = []
    y = []

    for points in path:
        if type(points) is int:
            print("Oi")
        else:
            x.append(points[0])
            y.append(points[1])
    plt.plot(x, y)
    plt.show()

    ser = serial.Serial('/dev/ttyUSB1', 9600)
    ser.flushInput()
    i = 0
    for points in path:
        if type(points) is int:
            print("Oi")
        else:
            print(points)

            a = str(points[0]) + "\n"
            ser.write(a.encode())
            time.sleep(1)
            print(ser.readline(ser.inWaiting()))
            a = str(points[1]) + "\n"
            ser.write(a.encode())
            time.sleep(1)
            print(ser.readline(ser.inWaiting()))
