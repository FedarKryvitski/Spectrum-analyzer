import numpy as np
from scipy.fft import fft, ifft, fftfreq

DEFAULT_SAMPLE_RATE = 48000
DEFAULT_MIN_FREQUENCY = 20
DEFAULT_MAX_FREQUENCY = 20000
DEFAULT_NUM_BINS = 1000

class AudioAnalyzer:
    def __init__(self, min_freq=DEFAULT_MIN_FREQUENCY, max_freq=DEFAULT_MAX_FREQUENCY, num_bins=DEFAULT_NUM_BINS):
        self._min_freq = min_freq
        self._max_freq = max_freq
        self._num_bins = num_bins
        self._reset_state()

    def _reset_state(self):
        self._sample_rate = None
        self._audio_data = None
        self._channels = None
        self._freqs = None
        self._spectrum_complex = None
        self._spectrum_magnitude = None
        self._spectrum_phase = None
        self._filtered_freqs = None
        self._filtered_amps = None
        self._bin_centers = None
        self._binned_amps = None

    def _load_audio(self, audio_data, sample_rate):
        self._sample_rate = sample_rate
        self._audio_data = audio_data

        if len(self._audio_data.shape) > 1:
            self._audio_data = self._audio_data.mean(axis=1)

        self._audio_data = self._audio_data.astype(np.float32)
        max_val = np.max(np.abs(self._audio_data))
        if max_val > 0:
            self._audio_data /= max_val

    def _compute_spectrum(self):
        self._spectrum_complex = fft(self._audio_data)
        n = len(self._audio_data)
        self._spectrum_magnitude = np.abs(self._spectrum_complex) / n
        self._spectrum_phase = np.angle(self._spectrum_complex)
        self._freqs = fftfreq(n, d=1/self._sample_rate)

    def _compute_reverse(self):
        signal = ifft(self._spectrum_complex).real
        max_val = np.max(np.abs(signal))
        return signal / max_val if max_val > 0 else signal

    def _filter_frequencies(self):
        valid_mask = (self._freqs >= self._min_freq) & (self._freqs <= self._max_freq)
        self._filtered_freqs = self._freqs[valid_mask]
        self._filtered_amps = self._spectrum_magnitude[valid_mask]

    # TODO сделать, чтобы было дискретным
    def _create_frequency_bins(self):
        return np.logspace(np.log10(self._min_freq), np.log10(self._max_freq), num=self._num_bins + 1)

    def _bin_amplitudes(self):
        bins = self._create_frequency_bins()
        self._binned_amps = self._bin_channel_amplitudes(self._filtered_freqs, self._filtered_amps, bins)
        self._bin_centers = (bins[:-1] + bins[1:]) / 2

    def _bin_channel_amplitudes(self, freqs, amps, bins):
        indexes = np.digitize(freqs, bins) - 1
        binned = np.zeros(self._num_bins)
        counts = np.zeros(self._num_bins)
        for idx, amp in zip(indexes, amps):
            if 0 <= idx < self._num_bins:
                binned[idx] += amp
                counts[idx] += 1
        with np.errstate(divide='ignore', invalid='ignore'):
            return np.where(counts > 0, binned / counts, 0)

    def process_audio(self, audio_data, sample_rate=DEFAULT_SAMPLE_RATE):
        self._reset_state()
        self._load_audio(audio_data, sample_rate)
        self._compute_spectrum()
        self._filter_frequencies()
        self._bin_amplitudes()
        return self._filtered_freqs, self._filtered_amps

    @property
    def sample_rate(self):
        return self._sample_rate
        
    @property
    def frequency_bins(self):
        return self._bin_centers
        
    @property
    def binned_amplitudes(self):
        return self._binned_amps
        
    @property
    def spectrum_magnitude(self):
        return self._spectrum_magnitude
        
    @property
    def spectrum_phase(self):
        return self._spectrum_phase
        
    @property
    def reconstructed_audio(self):
        return self._compute_reverse()