# Copyright (c) 2020, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np
import matplotlib ; matplotlib.use('Agg')
from matplotlib import pyplot as plt

import libst


class LinearProxy(libst.SolverProxy):

    # These are the call-back functions for physical calculations.
    def _xn_calc(self, se, iv):
        displacement = 0.5 * (se.x + se.xneg) - se.xctr
        return se.dxneg * (se.get_so0(iv) + displacement * se.get_so1(iv))

    def _xp_calc(self, se, iv):
        displacement = 0.5 * (se.x + se.xpos) - se.xctr
        return se.dxpos * (se.get_so0(iv) + displacement * se.get_so1(iv))

    def _tn_calc(self, se, iv):
        displacement = se.x - se.xctr
        ret = se.get_so0(iv)  # f(u)
        ret += displacement * se.get_so1(iv)  # displacement in x; f_u == 1
        ret += se.qdt * se.get_so1(iv)  # displacement in t
        return se.hdt * ret

    def _tp_calc(self, se, iv):
        displacement = se.x - se.xctr
        ret = se.get_so0(iv)  # f(u)
        ret += displacement * se.get_so1(iv)  # displacement in x; f_u == 1
        ret -= se.qdt * se.get_so1(iv)  # displacement in t
        return se.hdt * ret

    def _so0p_calc(self, se, iv):
        ret = se.get_so0(iv)
        ret += (se.x-se.xctr) * se.get_so1(iv)  # displacement in x
        ret -= se.hdt * se.get_so1(iv)  # displacement in t
        return ret

    def _cfl_updater(self, se):
        hdx = min(se.dxneg, se.dxpos)
        se.set_cfl(se.hdt / hdx)

def initialize(svr):
    svr.so0.ndarray.fill(0)
    svr.so1.ndarray.fill(0)
    for e in svr.selms(odd_plane=False):
        if e.xctr < 2*np.pi or e.xctr > 2*2*np.pi:
            v = 0
            dv = 0
        else:
            v = np.sin(e.xctr)
            dv = np.cos(e.xctr)
        e.set_so0(0, v)
        e.set_so1(0, dv)

def main():
    # Create the grid.
    grid = libst.Grid(0, 4*2*np.pi, 4*64)
    # Create the solver.
    cfl = 1
    dx = (grid.xmax - grid.xmin) / grid.ncelm
    dt = dx * cfl
    svr = LinearProxy(grid=grid, time_increment=dt, nvar=1)

    # Initialize the field.
    initialize(svr)
    # Plot initial field.
    fig = plt.figure(figsize=(12,8))
    plt.xlim((0, 8))
    plt.xlabel('$x$ ($\pi$)')
    plt.ylabel('value')
    plt.title('Initial field')
    plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-')
    fig.savefig('tmp/initial.png')

    # Time march
    steps = 50
    fig = plt.figure(figsize=(12,8))
    plt.xlim((0, 8))
    plt.xlabel('$x$ ($\pi$)')
    plt.ylabel('value')
    plt.title('Field between initial and step {}'.format(steps))
    plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-', label='initial')
    svr.setup_march()
    svr.march_alpha2(steps)
    plt.plot(svr.xctr() / np.pi, svr.get_so0(0).ndarray, '-',
             label='step {}'.format(steps))
    plt.legend()
    fig.savefig('tmp/march{}.png'.format(steps))

if __name__ == '__main__':
    main()