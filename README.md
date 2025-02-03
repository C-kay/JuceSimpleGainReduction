# Juce Simple Gain Reduction Compressor Plugin

A professional-grade compressor VST3 plugin developed using the JUCE framework. This plugin features an industry-standard gain reduction algorithm along with a modern, polished user interface that includes custom rotary knobs and a dynamic vertical gain reduction meter.

## Features

- **Real-Time Compression:**  
  Implements a gain reduction compressor with configurable threshold, ratio, attack, release, makeup gain, and (optional) key filter frequency for sidechain filtering.

- **Modern UI Controls:**  
  Custom rotary knobs with a sleek, modern design, featuring:
  - A radial gradient outer ring.
  - A dynamic value arc with a white indicator.
  - Detailed tick marks with numeric labels around the circumference.
  - A centered display of the current knob value.

- **Dynamic Gain Reduction Meter:**  
  A vertical meter with a multi-stop gradient (green → yellow → red) that displays real-time gain reduction in dB with rounded corners and a subtle background gradient for a refined look.

- **Cross-Platform Compatibility:**  
  Built using the JUCE framework, this plugin is designed to work as a VST3 (and can be configured for AU or AAX) on Windows, macOS, and Linux.

## Requirements

- **JUCE Framework:** Version 6 or later (download from [juce.com](https://juce.com)).
- **C++ Compiler:** Supporting C++11 or later.
- **Supported Platforms:** Windows, macOS, and Linux (configuration-dependent).

## Installation and Build Instructions

1. **Clone or Download the Repository:**

   If using GitHub, once your repository is set up, you can download a ZIP via the "Code" button.

   Alternatively, create a folder named `JuceSimpleGainReduction` and place all the files listed below inside.

2. **Open in Projucer:**

   - Launch the JUCE Projucer.
   - Open the `.jucer` project file (create one if needed).
   - Configure your plugin settings (e.g., VST3, AU, AAX, etc.) as needed.

3. **Export to Your IDE:**

   - Save and export the project to your preferred IDE (Visual Studio, Xcode, etc.).

4. **Build the Plugin:**

   - Open the project in your IDE.
   - Build the solution/project.

## Project Structure

- **PluginProcessor.h / PluginProcessor.cpp:**  
  Contains the main DSP and compression logic.

- **PluginEditor.h / PluginEditor.cpp:**  
  Implements the graphical user interface, including parameter controls and layout.

- **KnobLookAndFeel.h / KnobLookAndFeel.cpp:**  
  Contains custom look-and-feel code for the rotary knobs, featuring modern design elements and detailed numeric tick marks.

- **VerticalMeter.h / VerticalMeter.cpp:**  
  Implements the modern vertical gain reduction meter with gradient fills and rounded corners.

## Usage

- **Load the Plugin:**  
  Load the plugin in your DAW (Digital Audio Workstation) as you would with any other compressor.

- **Adjust Parameters:**  
  Use the rotary knobs to adjust:
  - **Threshold:** The dB level above which compression occurs.
  - **Ratio:** The compression ratio.
  - **Attack:** The time it takes for the compressor to start reducing gain.
  - **Release:** The time for the compressor to recover after the signal falls below the threshold.
  - **Makeup Gain:** The gain applied after compression.
  - **Key Filter Frequency:** (Optional) Adjusts the frequency range of the sidechain signal.

- **Monitor Gain Reduction:**  
  The vertical meter displays the current gain reduction in dB in real time.

## Contributing

Contributions, feature requests, and bug reports are welcome! Please fork the repository, create a new branch for your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- Built on the [JUCE framework](https://juce.com).
- Inspired by industry-standard compressor plugins and modern UI design trends.
- Special thanks to the JUCE community for their ongoing support and resources.
