import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

class AmplitudeGraph:
    def __init__(self, min_freq=20, max_freq=20000):
        """
        Initialize a log-scale amplitude-frequency graph
        
        Parameters:
            min_freq (float): Minimum frequency value for X-axis (Hz)
            max_freq (float): Maximum frequency value for X-axis (Hz)
        """
        self.min_freq = min_freq
        self.max_freq = max_freq
        self._setup_figure()

    def _setup_figure(self):
        """Configure base figure parameters"""
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
        """
        Set the graph title
        
        Parameters:
            title (str): Title text for the graph
        """
        self.ax.set_title(title, fontsize=16)
        return self

    def set_xlabel(self, label='Frequency (Hz)'):
        """
        Set X-axis label
        
        Parameters:
            label (str): Label text for X-axis
        """
        self.ax.set_xlabel(label, fontsize=12)
        return self

    def set_ylabel(self, label='Normalized Amplitude'):
        """
        Set Y-axis label
        
        Parameters:
            label (str): Label text for Y-axis
        """
        self.ax.set_ylabel(label, fontsize=12)
        return self

    def add_plot(self, x_data, y_data, color='blue', label='Signal'):
        """
        Add a spectrum plot to the graph
        
        Parameters:
            x_data (array): Frequency data points
            y_data (array): Amplitude data points
            color (str): Line color (matplotlib compatible)
            label (str): Legend entry for this plot
        """
        self.ax.semilogx(x_data, y_data, 
                        color=color, 
                        label=label, 
                        alpha=0.8, 
                        linewidth=1.5)
        return self

    def show(self):
        """Display the graph with legend and proper layout"""
        plt.legend(fontsize=12, framealpha=0.9)
        plt.tight_layout()
        plt.show()

    def save(self, filename, dpi=300):
        """
        Save the graph to a file
        
        Parameters:
            filename (str): Output file path
            dpi (int): Resolution in dots per inch
        """
        plt.legend(fontsize=12, framealpha=0.9)
        plt.tight_layout()
        plt.savefig(filename, dpi=dpi, bbox_inches='tight')
        plt.close()