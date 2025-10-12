import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

class AmplitudeGraph:
    def __init__(self, min_freq=20, max_freq=20000):
        self.min_freq = min_freq
        self.max_freq = max_freq
        self._setup_figure()

    def _setup_figure(self):
        self.fig, self.ax = plt.subplots(figsize=(14, 8))
        
        self.ax.set_xscale('log')
        self.ax.set_yscale('log')
        self.ax.set_xlim(self.min_freq, self.max_freq)
        self.ax.set_ylim(bottom=1e-5, top=1.0)
        self.ax.grid(True, which="both", ls="--", alpha=0.5)
        self.ax.xaxis.set_major_formatter(ScalarFormatter())
        self.ax.xaxis.set_minor_formatter(ScalarFormatter())
        self.ax.tick_params(axis='x', which='minor', labelsize=8)

    def set_title(self, title='Frequency Spectrum Analysis'):
        self.ax.set_title(title, fontsize=16)
        return self

    def set_xlabel(self, label='Frequency (Hz)'):
        self.ax.set_xlabel(label, fontsize=12)
        return self

    def set_ylabel(self, label='Normalized Amplitude'):
        self.ax.set_ylabel(label, fontsize=12)
        return self

    def add_plot(self, x_data, y_data, color='blue', label='Signal'):
        self.ax.semilogx(x_data, y_data, 
                        color=color, 
                        label=label, 
                        alpha=0.8, 
                        linewidth=1.5)
        return self

    def show(self):
        plt.legend(fontsize=12, framealpha=0.9)
        plt.tight_layout()
        plt.show()

    def save(self, filename, dpi=300):
        plt.legend(fontsize=12, framealpha=0.9)
        plt.tight_layout()
        plt.savefig(filename, dpi=dpi, bbox_inches='tight')
        plt.close()