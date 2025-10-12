import sounddevice as sd
from scipy.io import wavfile
from analyzer import AudioAnalyzer
from graph import AmplitudeGraph


audio_files = [
    ("/home/fedar/Documents/BSUIR/DSP/Lab2/input.wav", "green", "input"),
    ("/home/fedar/Documents/BSUIR/DSP/Lab2/output.wav", "red", "output"),
]

def play_original(audio_data, sample_rate=48000):
    sd.play(audio_data, sample_rate)
    sd.wait()

def play_reconstructed(channel, sample_rate=48000):
    #reconstructed = compute_reverse(channel)
    #sd.play(reconstructed, sample_rate)
    #sd.wait()
    return

if __name__ == "__main__":
    analyzer = AudioAnalyzer(num_bins=100)
    graph = AmplitudeGraph()

    graph.set_title('Comparison of Audio Spectra')
    graph.set_xlabel('Frequences (Hz)')
    graph.set_ylabel('Amplitudes')
    
    for file_path, color, label in audio_files:
        sample_rate, audio_data = wavfile.read(file_path)
        frequences, amplitudes = analyzer.process_audio(audio_data, sample_rate)

        graph.add_plot(frequences, amplitudes, color, label)
        
    graph.show()