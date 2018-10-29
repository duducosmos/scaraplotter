#!/usr/bin/env python
# -*- Coding: UTF 8 -*-

'''
SVG converterter
Based on project: https://github.com/cavenel/ev3-print3rbot
author: E. S. Pereira
date: 28/10/2018
'''


from svg.parser import parse_path
from svg.path import Line

from xml.dom import minidom
from math import floor

class ReadSVG:
    def __init__(self, filename):
        self.filename = filename
        self.xmldoc = minidom.parse(filename)
        self.itemlist = self.xmldoc.getElementsByTagName("path")
        self.list_points = []

        self._start()

    def __call__(self):
        return self.list_points

    def _start(self):
        try:
            self.itemlist = filter(lambda x: x.attributes['id'].value != "borders", self.itemlist)
        except:
            pass
        path = [s.attributes['d'].value for s in self.itemlist]

        actual = (0+0j)
        for p_ in path:
            p__ = parse_path(p_)
            for p in p__:
                start = p.point(0.)
                if not self.feq(actual,start):
                    self.list_points.append(0)
                    self.list_points.append(self.svg_point_to_coord(start))
                    self.list_points.append(1)
                if ( isinstance(p, Line)):
                    interv = 15
                else:
                    interv = 3
                length = p.length(error=1e-2)
                for i in range(interv,int(floor(length)),interv):
                    self.list_points.append(self.svg_point_to_coord(p.point(i/length)))
                end = p.point(1.)
                self.list_points.append(self.svg_point_to_coord(end))
                actual = end
        self.list_points.append(0)



    def svg_point_to_coord(self, svg_points):
        scale = 10.0
        ciblex = svg_points.real / scale
        cibley = (272.74 - svg_points.imag) / scale
        return (ciblex, cibley)

    def feq(self, a, b):
        if abs(a-b) < 0.0001:
            return 1
        return 0

if __name__ == "__main__":
    svg = ReadSVG(filename="./images/test.svg")

    import serial
    import time
    from numpy import array
    import matplotlib.pyplot as plt
    path = svg()
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
