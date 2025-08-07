import numpy as np
from scipy.fft import fft, fftfreq
from scipy.io import wavfile
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

MIN_FREQUENCY = 20
MAX_FREQUENCY = 20000
NUM_BINS = 1000

AUDIO_FILES = [
    "./sounds/volkswagen.wav",
    "./sounds/kombayn.wav"
]
FILE_COLORS = ['blue', 'red']
FILE_LABELS = ['Volkswagen', 'Kombayn']

def process_audio_file(file_path):
    sample_rate, audio_data = wavfile.read(file_path)
    
    if len(audio_data.shape) > 1:
        audio_data = audio_data.mean(axis=1)
    
    audio_data = audio_data.astype(np.float32) / np.max(np.abs(audio_data))
    spectrum = fft(audio_data)
    freqs = fftfreq(len(audio_data), d=1/sample_rate)
    
    valid_mask = (freqs >= MIN_FREQUENCY) & (freqs <= MAX_FREQUENCY)
    freqs = freqs[valid_mask]
    amps = np.abs(spectrum[valid_mask]) / len(audio_data)
    
    bins = np.logspace(np.log10(MIN_FREQUENCY), np.log10(MAX_FREQUENCY), num=NUM_BINS + 1)
    bin_indices = np.digitize(freqs, bins) - 1
    
    binned_amps = np.zeros(NUM_BINS)
    counts = np.zeros(NUM_BINS)
    
    for idx, amp in zip(bin_indices, amps):
        if 0 <= idx < NUM_BINS:
            binned_amps[idx] += amp
            counts[idx] += 1
    
    with np.errstate(divide='ignore', invalid='ignore'):
        binned_amps = np.where(counts > 0, binned_amps / counts, 0.0005)
    
    bin_centers = (bins[:-1] + bins[1:]) / 2
    return bin_centers, binned_amps

def plot_spectra(results):
    plt.figure(figsize=(14, 8))
    
    ax = plt.gca()
    
    for (file_path, (frequencies, amplitudes)), color, label in zip(results.items(), FILE_COLORS, FILE_LABELS):
        plt.semilogx(frequencies, amplitudes, color=color, label=label, alpha=0.8, linewidth=1.5)
        
    plt.xlim(MIN_FREQUENCY, MAX_FREQUENCY)
    plt.xlabel('Frequency (Hz)', fontsize=12)
    plt.ylabel('Amplitude', fontsize=12)
    plt.title('Comparison of Audio Spectra', fontsize=16)
    
    ax.xaxis.set_major_formatter(ScalarFormatter())
    ax.xaxis.set_minor_formatter(ScalarFormatter())
    ax.tick_params(axis='x', which='minor', labelsize=8)
    
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend(fontsize=12)
    
    plt.yscale('log')
    plt.ylim(bottom=1e-5)
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    results = {}
    for file_path in AUDIO_FILES:
        bin_centers, amplitudes = process_audio_file(file_path)
        results[file_path] = (bin_centers, amplitudes)
    
    plot_spectra(results)