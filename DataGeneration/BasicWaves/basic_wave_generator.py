# %%
# Basic Wave Generator

from scipy.signal import chirp, sawtooth, square, sweep_poly, gausspulse
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from numpy.random import randint
from random import shuffle

seconds = 4
sampling_rate = 8000
amps = [1000, 2000, 3000, 5000, 8000, 10000,
        12000, 15000, 17000, 20000]  # amplitude list
periods = [12, 10, 8, 6]
signal = ['sin', 'sawtooth''square', 'zero']
signalPos = [30, 60, 90, 100]


def sin_s(sample, amp):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * np.sin(2 * np.pi * 1 * t)


def sin_s(sample, amp, phase):
    t = np.linspace(0, 1, sample, endpoint=False)
    t = np.concatenate((t[phase:], t[:phase]))
    return amp * np.sin(2 * np.pi * 1 * t)


def damp_sin_s(sample, amp, decay):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * np.exp(-1 * decay * t) * np.sin(2 * np.pi * t)


def tri_s(sample, amp):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * sawtooth(2 * np.pi * 1 * t+np.pi/2, 0.5)


def saw_s(sample, amp):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * sawtooth(2 * np.pi * 1 * t)


def square_s(sample, amp):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * square(2 * np.pi * 1 * t)


def chirp_s(sample, amp):
    t = np.linspace(0, 1, sample, endpoint=False)
    return amp * chirp(t, 0, sampling_rate/8, sampling_rate/2, phi=90)


total_sample = seconds * sampling_rate
SType = [33, 66, 99, 100]
signals = []

i = 0

while i < total_sample:
    s = randint(100)
    amp = randint(len(amps))
    period = randint(len(periods))
    if(s < SType[0]):  # sinus

        signals.append(sin_s(periods[period], amps[amp], 0))
        i += periods[period]
    elif (s < SType[1]):  # saw tooth
        signals.append(saw_s(periods[period], amps[amp]))
        i += periods[period]
    elif (s < SType[2]):  # square
        signals.append(square_s(periods[period], amps[amp]))
        i += periods[period]
    else:  # zero
        zeroLen = randint(periods[0]*2)
        signals.append(np.zeros(zeroLen))
        i += zeroLen

signals = np.concatenate(signals)
signals = np.asarray(signals).astype(np.int16)
