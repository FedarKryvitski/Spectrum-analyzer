import numpy as np
from scipy.fft import fft, fftfreq
from scipy.io import wavfile
import matplotlib.pyplot as plt

FREQ_MIN = 20
FREQ_MAX = 20000

def get_axis(path):
    sample_rate, audio_data = wavfile.read(path)
    if len(audio_data.shape) > 1:
        audio_data = audio_data[:, 0]

    audio_data = audio_data.astype(np.float32) / np.max(np.abs(audio_data))

    n = len(audio_data)
    yf = fft(audio_data, workers=-1)
    xf = fftfreq(n, d=1/sample_rate) 

    mask = (xf >= FREQ_MIN) & (xf <= FREQ_MAX)
    filtered_xf = xf[mask]
    filtered_yf = yf[mask]

    amplitudes = np.abs(filtered_yf) / n

    return filtered_xf, amplitudes


plt.figure(figsize=(10, 5))

path_array = [
    "./sounds/volkswagen.wav",
    "./sounds/kombayn.wav"
]

for path in path_array:
    x_axis, y_axis = get_axis(path)
    plt.plot(x_axis, y_axis)

plt.xlim(20.0, 20000.0)
plt.xlabel("Частота (Гц)")
plt.xscale('log')
plt.ylabel("Амплитуда")
plt.title("Частотный спектр (20 - 20 000 Гц)")
plt.grid()
plt.show()