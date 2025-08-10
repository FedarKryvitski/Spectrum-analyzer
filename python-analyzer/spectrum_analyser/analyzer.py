import numpy as np
from scipy.fft import fft, fftfreq
from scipy.io import wavfile

class AudioAnalyzer:
    def __init__(self, min_freq=20, max_freq=20000, num_bins=1000):
        """
        Initialize audio frequency analyzer
        
        Parameters:
            min_freq (float): Minimum frequency to analyze (Hz)
            max_freq (float): Maximum frequency to analyze (Hz)
            num_bins (int): Number of frequency bins for analysis
        """
        self.min_freq = min_freq
        self.max_freq = max_freq
        self.num_bins = num_bins

    def load_audio(self, file_path):
        """Load and normalize audio file"""
        sample_rate, audio_data = wavfile.read(file_path)
        
        if len(audio_data.shape) > 1:
            audio_data = audio_data.mean(axis=1)
            
        audio_data = audio_data.astype(np.float32) / np.max(np.abs(audio_data))
        return sample_rate, audio_data

    def compute_spectrum(self, audio_data, sample_rate):
        """Compute FFT spectrum of audio signal"""
        spectrum = fft(audio_data)
        freqs = fftfreq(len(audio_data), d=1/sample_rate)
        return freqs, spectrum

    def filter_frequencies(self, freqs, spectrum):
        """Filter frequencies within specified range"""
        valid_mask = (freqs >= self.min_freq) & (freqs <= self.max_freq)
        filtered_freqs = freqs[valid_mask]
        filtered_amps = spectrum[valid_mask]
        filtered_amps = np.abs(spectrum[valid_mask]) / len(filtered_amps)
        return filtered_freqs, filtered_amps

    def create_frequency_bins(self):
        """Create logarithmic frequency bins"""
        return np.logspace(np.log10(self.min_freq), 
                         np.log10(self.max_freq), 
                         num=self.num_bins + 1)

    def bin_amplitudes(self, freqs, amps, bins):
        """Bin amplitudes into frequency ranges"""
        bin_indices = np.digitize(freqs, bins) - 1
        binned_amps = np.zeros(self.num_bins)
        counts = np.zeros(self.num_bins)
        
        for idx, amp in zip(bin_indices, amps):
            if 0 <= idx < self.num_bins:
                binned_amps[idx] += amp
                counts[idx] += 1
        
        with np.errstate(divide='ignore', invalid='ignore'):
            binned_amps = np.where(counts > 0, binned_amps / counts, 0.0005)
            
        return binned_amps

    def process_audio_file(self, file_path):
        """
        Process audio file and return frequency analysis
        
        Returns:
            tuple: (bin_centers, binned_amplitudes)
        """
        sample_rate, audio_data = self.load_audio(file_path)
        
        freqs, spectrum = self.compute_spectrum(audio_data, sample_rate)
        freqs, amps = self.filter_frequencies(freqs, spectrum)
        
        bins = self.create_frequency_bins()
        binned_amps = self.bin_amplitudes(freqs, amps, bins)
        bin_centers = (bins[:-1] + bins[1:]) / 2
        
        return bin_centers, binned_amps