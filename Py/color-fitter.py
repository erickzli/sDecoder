#!/usr/bin/env python

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

SIGMA = 0.008856

def frange(start,stop, step=1.0):
    while start < stop:
        yield start
        start +=step

def color_dist(clist1, clist2):
    p = 3
    dr = abs(clist1[0] - clist2[0])
    dg = abs(clist1[1] - clist2[1])
    db = abs(clist1[2] - clist2[2])

    return (dr ** p + dg ** p + db ** p) ** (1 / p)

def private_f(t):
    if t > SIGMA:
        return t ** 3.0
    else:
        return 3 * (SIGMA ** 2.0) * (t - 4.0 / 29.0)


def format_rgb(code):
    if code > 255.0:
        return 255.0
    elif code < 0.0:
        return 0.0
    else:
        return code


def CIELAB_to_RGB_HSV(L, a, b, X_n, Y_n, Z_n):
    x = X_n / 100.0 * private_f((L + 16.0) / 116.0 + a / 500.0)
    y = Y_n / 100.0 * private_f((L + 16.0) / 116.0)
    z = Z_n / 100.0 * private_f((L + 16.0) / 116.0 - b / 200.0)

    r = 3.240479 * x - 1.537150 * y - 0.498535 * z
    g = -0.969256 * x + 1.875992 * y + 0.041556 * z
    bl = 0.055648 * x - 0.204043 * y + 1.057311 * z

    r = 1.055 * (r ** (1 / 2.4)) - 0.055 if r > 0.0031308 else 12.92 * r
    g = 1.055 * (g ** (1 / 2.4)) - 0.055 if g > 0.0031308 else 12.92 * g
    bl = 1.055 * (bl ** (1 / 2.4)) - 0.055 if bl > 0.0031308 else 12.92 * bl

    r = format_rgb(r * 255)
    g = format_rgb(g * 255)
    bl = format_rgb(bl * 255)

    myrgb = []
    myrgb.append(r)
    myrgb.append(g)
    myrgb.append(bl)
    return myrgb

def get_distdf(xstart, xend, zstart, zend, rstep, output_df=False):
    print('Start: %.2f, %.2f, %.2f, %.2f' % (xstart, xend, zstart, zend))
    distdf = pd.DataFrame(columns=['X_n', 'Z_n', 'dist'])
    smallest_dist = [0, 0, 100000.0]

    for xn in frange(xstart, xend, rstep):
        yn = 100.0
        for zn in frange(zstart, zend, rstep):
            dist = 0.0
            num_rows = 0

            for index, row in colors.iterrows():
                num_rows += 1
                L = float(row['L'])
                a = float(row['a'])
                b = float(row['b'])
                R = float(row['R'])
                G = float(row['G'])
                BL = float(row['B'])
                converted_rgb = CIELAB_to_RGB_HSV(L, a, b, xn, yn, zn)
                original_rgb = [R, G, BL]
                dist += color_dist(original_rgb, converted_rgb)

            dist = dist / num_rows
            row = pd.Series({'X_n': xn, 'Z_n': zn, 'dist': dist})
            if output_df:
                distdf = distdf.append(row, ignore_index=True)

            if dist < smallest_dist[2]:
                smallest_dist = [xn, zn, dist]

    return distdf, smallest_dist


colors = pd.read_csv('color.csv', dtype='a')
plt.subplot(111)
# print(colors)

distdf, smallest_dist = get_distdf(90, 100, 85, 95, 1, output_df=True)
rowIDs = distdf['X_n'].astype(int)
colIDs = distdf['Z_n'].astype(int)

rmax = rowIDs.max()
cmax = colIDs.max()
rmin = rowIDs.min()
cmin = colIDs.min()

newdf = np.zeros((int(rmax - rmin + 1), int(cmax - cmin + 1)))

ridx = rowIDs - rmin
cidx = colIDs - cmin

newdf[ridx, cidx] = distdf['dist']
rowIDs = rowIDs.unique()
colIDs = colIDs.unique()

plt.contourf(colIDs, rowIDs, newdf, 10, cmap='gnuplot')
currentAxis = plt.gca()
currentAxis.add_patch(Rectangle((smallest_dist[1] - 1.5, smallest_dist[0] - 1.5), 3, 3, fill=None, alpha=1, ec='white', ls=':'))
print('Round: %d, X_n: %.4f, Y_n: 100.0, Z_n: %.4f, dist: %.4f.' % (0, smallest_dist[0], smallest_dist[1], smallest_dist[2]))

for num_of_decimal in range(1, 4):
    prec = 10.0 ** (-num_of_decimal)
    distdf, smallest_dist = get_distdf(smallest_dist[0] - prec * 10.0, smallest_dist[0] + prec * 10.0, smallest_dist[1] - prec * 10.0, smallest_dist[1] + prec * 10.0, prec)
    print('Round: %d, X_n: %.4f, Y_n: 100.0, Z_n: %.4f, dist: %.4f.' % (num_of_decimal, smallest_dist[0], smallest_dist[1], smallest_dist[2]))
    plt.scatter(smallest_dist[1], smallest_dist[0], c='white', marker='o')

plt.scatter(smallest_dist[1], smallest_dist[0], c='white', marker='x')
plt.text(smallest_dist[1] + 0.3, smallest_dist[0], '(%.4f, %.4f)' % (smallest_dist[1], smallest_dist[0]), fontsize=10, c='white')
plt.xlabel('Z_n')
plt.ylabel('X_n')

plt.show()
