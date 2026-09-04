# -*- coding: utf-8 -*-
"""
In this file, we implement the Digital Signal Processing (DSP) functions.
Some functions are just wrappers of the scipy.signal library. Other functions are inspired on sensormotion project.
"""

import numpy as np
from scipy.signal import butter, filtfilt, resample, savgol_filter, lfilter
from scipy.integrate import trapz
import matplotlib.pyplot as plt


def build_filter(frequency, sampling_rate, filter_type, filter_order):
    """
    This function build a butterworth filter based on the given parameters.
    It's a wrapper of the `scipy.signal` package and it is based on `sensormotion` project.

    Parameters
    ----------
    frequency : int or tuple of ints
        The cutoff frequency for the filter. If `filter_type` is set as
        'bandpass' then this needs to be a tuple of integers representing
        the lower and upper bound frequencies. For example, for a bandpass
        filter with range of 2Hz and 10Hz, you would pass in the tuple (2, 10).
        For filter types with a single cutoff frequency then a single integer
        should be used.
    sampling_rate : float
        Sampling rate of the signal.
    filter_type : {'lowpass', 'highpass', 'bandpass', 'bandstop'}
        Type of filter to build.
    filter_order: int, optional
        Order of the filter.

    Returns
    -------
    num : numpy.ndarray
        Numerator polynomials of the IIR filter.
    den : numpy.ndarray
        Denominator polynomials of the IIR filter.
    """

    nyq = 0.5 * sampling_rate

    if filter_type == "bandpass":
        nyq_cutoff = (frequency[0] / nyq, frequency[1] / nyq)
    else:
        nyq_cutoff = frequency / nyq

    num, den = butter(filter_order, nyq_cutoff, btype=filter_type, analog=False)

    return num, den


def filter_signal(*args, num, den):
    """
    It is a wrapper of the `scipy.signal.filtfilt` to filter a signal.
    The parameters num and den are the numerator and denominator obtained from `builde_filter`.
    It is also based on sensormotion.

    Parameters
    ----------
    *args : numpy.ndarray
        A set of signals to filter.
    num : numpy.ndarray
        Numerator polynomial coefficients of the filter.
    den : numpy.ndarray
        Denominator polynomial coefficients of the filter.

    Returns
    -------
    signal_filtered : numpy.ndarray
        Filtered output of the original input signal.
    """

    return [filtfilt(num, den, signal) for signal in args]


def filter_signal_like_C(*args, num, den):
    """
    It is a wrapper of the `_filter` function to filter a signal.
    The parameters num and den are the numerator and denominator obtained from `builde_filter`.

    Parameters
    ----------
    *args : numpy.ndarray
        A set of signals to filter.
    num : numpy.ndarray
        Numerator polynomial coefficients of the filter.
    den : numpy.ndarray
        Denominator polynomial coefficients of the filter.

    Returns
    -------
    signal_filtered : numpy.ndarray
        Filtered output of the original input signal.

    """
    return [lfilter(num, den, signal) for signal in args]
    # return [_filter(signal, num, den) for signal in args]


def _filter(s, num, den):
    """
    Filter a data sequence, `s`, using a digital filter. It's designed to match the C-API.

    Parameters
    ----------
    s: numpy.ndarray
        The signal to be filtered
    num : numpy.ndarray
        Numerator polynomial coefficients of the filter.
    den : numpy.ndarray
        Denominator polynomial coefficients of the filter.

    Returns
    -------
    filt_s : numpy.ndarray
        Filtered output of the original input signal.
    """
    d = np.zeros(len(num) - 1)
    filt_s = np.zeros_like(s)
    for k in range(len(s)):
        filt_s[k] = (num[0] * s[k] + d[0]) / den[0]

        d[0] = (num[1] * s[k]) - (den[1] * filt_s[k]) + d[1]
        d[1] = (num[2] * s[k]) - (den[2] * filt_s[k]) + d[2]
        d[2] = (num[3] * s[k]) - (den[3] * filt_s[k]) + d[3]
        d[3] = (num[4] * s[k]) - (den[4] * filt_s[k])

    return filt_s


def frequency_response(signal, sampling_rate, scale="dB", show=True):
    """
    This function is a wrapper of the `plt.magnitude_spectrum` to compute the magnitude spectrum of the signal

    Parameters
    ----------
    signal : numpy.ndarray
        The signal to compute the spectrum
    sampling_rate : int
        The signal sampling frequency in Hz
    scale : {'linear', 'dB'}
        The scaling of the values in the spec. 'linear' is no scaling. 'dB' returns the values in dB scale
    show : boolean
        A boolean to control if you want to show or not the plot of the spectrum

    Returns
    -------
    spectrum : numpy.ndarray
        The values for the magnitude spectrum before scaling (real valued).
    freqs : numpy.ndarray
        The frequencies corresponding to the elements in spectrum.
    """

    spectrum, freqs, _ = plt.magnitude_spectrum(signal, sampling_rate, scale=scale)
    if show:
        plt.show()
    return spectrum, freqs


def aggregate_signal(*args, mode="norm", offset=0, clip=None):
    """
    This function aggregates n phases of the same signal. For example, it may be used to aggregate the three axis of
    an accelerometer signal or four phases of PPG signal.

    Parameters
    ----------
    *args : numpy.ndarray
        A set of numpy arrays representing a phase of the same signal. Example: aggregate_signal(x, y, z)
    mode : {'norm'}
        A string representing the aggregation mode
    offset : number
        A offset number if you want to offset the signal phase values before aggregate them

    Returns
    -------
    agg : numpy.ndarray
        The aggregate signal
    """

    len_ = len(args[0])

    if not all(len(s) == len_ for s in args):
        raise Exception("All phases of this signal must have the same length!")

    if mode == "norm":
        agg = np.abs(np.sqrt(np.sum(s**2 for s in args)) + offset)
    else:
        raise Exception(f"There is no mode '{mode}' available for this function!")

    if clip is not None:
        agg = np.clip(agg, clip[0], clip[1])

    return agg


def downsample_signal(*args, down_rate=25):
    """
    This function downsamples the signal according to the Fourier method and the down_rate.

    Parameters
    ----------
    *args : numpy.ndarray
        A set of numpy arrays representing a phase of the same signal to downsample. Example: aggregate_signal(x, y, z)
    down_rate : int
        The sampling rate to determine the new frequency. For example, if the signal is sampled in 25Hz and down_rate is
        also 25 Hz, the resampled signal will be in 1Hz.

    Returns
    -------
    resampled_signals : list
        A list of numpy.ndarrays containined the resampled signals
    """

    resampled_signals = [resample(s, int(len(s) / down_rate)) for s in args]
    return resampled_signals


def signal_integral(s, sampling_rate=25, tau=10, norm_val=None, slide_window=False, gamma=None):
    """
    This function computes the signal integral using the trapezoidal rule

    Parameters
    ----------
    s : numpy.ndarray
        The signal to be integrated
    sampling_rate : int
        The signal sampling rate (Hz)
    tau : int
        The integration time in seconds
    norm_val : number or None
        If you want to normalize the integral value, you need to inform the max value of the window. For the momentum,
        it going to be the max(clip). If it's None, no normalization operation is performed. Default is None.
    slide_window : boolean
        Set it as True to use a slinding window instead of a fixed one. Default is False.

    Returns
    -------
    integrated_singal : numpy.ndarray
        The integrated signal according to the time window
    """

    win_len = sampling_rate * tau
    int_s = list()

    if gamma is None:
        if slide_window:
            _aux = s[0:win_len]
            for step in range(0, len(s), sampling_rate):
                if step + sampling_rate > len(s):
                    break
                int_s.append(trapz(_aux))
                _aux = s[step + sampling_rate:step + sampling_rate + win_len]
        else:
            for step in range(0, len(s), win_len):
                if step + win_len > len(s):
                    break
                _aux = s[step:step + win_len]
                int_s.append(trapz(_aux))
    else:
        integ = 0
        for k in range(len(s)):
            integ = gamma * integ + (1 - gamma) * s[k]
            if k % sampling_rate == 0:
                int_s.append(integ)

    if norm_val is not None:
        return np.array(int_s) / (tau * sampling_rate * norm_val)
    else:
        return np.array(int_s)


def savitzky_golay_filter(signal, win_len, polyorder):
    """
    This function is a wrapper of the `signal.savgol_filter` to apply a smooth on the signal using interpolation mode
    and derivative = 0.

    Parameters
    ----------
    signal : numpy.ndarray
        The signal to be smoothed
    win_len : int
        The length of the filter window (i.e., the number of coefficients).
    polyorder : int
        The order of the polynomial used to fit the samples.
        `polyorder` must be less than `window_length`.

    Returns
    -------
    filtered_signal : numpy.ndarray
        The signal smoothed by the filter
    """
    return savgol_filter(signal, win_len, polyorder)


def step_counter(agg_acc, threshold, gamma=0.98, win_t=10, sampling_rate=25):
    """
    This function computes the number of steps according to a frame window
    Parameters
    ----------
    agg_acc: array-like
        The acc magnitude
    threshold: float
        The threashold to consider a peak
    gamma: float
        The parameters to compute the onlime avg
    win_t: int
        The window frame length
    sampling_rate: int
        The acc sampling rate

    Returns
    -------
    steps_win_sec: np.array
        The steps within the window frame
    n_peaks: int
        The number os steps
    peaks: np.array
        All peaks founded in the signal
    """

    if gamma is not None:
        agg_acc_diff = list()
        acc_avg = 0
        for k in range(len(agg_acc)):
            acc_avg = gamma * acc_avg + (1 - gamma) * agg_acc[k]
            agg_acc_diff.append(agg_acc[k] - acc_avg)
    else:
        agg_acc_diff = agg_acc

    peaks = dict()
    peak_val, peak_t = 0, 0
    crossess = 0
    for t, acc in enumerate(agg_acc_diff):
        if acc > threshold and crossess == 0:
            crossess += 1
            peak_val, peak_t = acc, t
        elif acc < threshold and crossess == 1:
            crossess += 1
        else:
            if peak_val < acc:
                peak_val, peak_t = acc, t

        if crossess == 2:
            crossess = 0
            peaks[t] = peak_val

    steps_sec = np.zeros_like(agg_acc)
    for t in range(len(agg_acc)):
        if t in peaks:
            steps_sec[t] += 1

    win_len = sampling_rate * win_t
    _aux = steps_sec[0:win_len]
    steps_win_sec = list()
    for step in range(0, len(steps_sec), sampling_rate):
        if step + sampling_rate > len(steps_sec):
            break
        steps_win_sec.append(_aux.sum())
        _aux = steps_sec[step + sampling_rate:step + sampling_rate + win_len]

    return np.array(steps_win_sec), len(peaks), peaks
