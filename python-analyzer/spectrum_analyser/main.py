from analyzer import AudioAnalyzer
from graph import AmplitudeGraph

audio_files = [
    ("sounds/volkswagen.wav", "blue", "Volkswagen"),
    ("sounds/kombayn.wav",    "red",  "Kombayn"   )
]

if __name__ == "__main__":
    analyzer = AudioAnalyzer(num_bins=100)
    graph = AmplitudeGraph()

    graph.set_title('Comparison of Audio Spectra')
    graph.set_xlabel('Frequences (Hz)')
    graph.set_ylabel('Amplitudes')
    
    for file_path, color, label in audio_files:
        frequences, amplitudes = analyzer.process_audio_file(file_path)
        graph.add_plot(frequences, amplitudes, color, label)
        
    graph.show()