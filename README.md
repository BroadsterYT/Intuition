<img src="Assets/GitHubImages/intn_logo.png?"/>

# Overview
Intuition is a VST/AU/standalone digital synthesizer designed to combine high-performance digital signal processing with an intuitive, modular workflow for sound design and music production. 

The project aims to provide musicians and producers with a flexible environment for creating and manipulating sound in real time, without sacrificing usability or creative flow. Unlike many existing synthesizers that trade performance for complexity or simplicity for control, Intuition seeks to bridge that gap. It offers a robust synthesis engine built in C++ using JUCE, capable of handling multiple oscillators, modulation sources, and signal routing options efficiently. The system architecture emphasizes modularity, allowing features such as wavetable synthesis, unison detuning, and morphing to be expanded and customized over time. Ultimately, Intuition’s goal is to empower users to explore advanced sound design techniques through a responsive, user-friendly interface that feels creative rather than technical.

# Setup Instructions
## Step 1 - Install JUCE
JUCE and the Projucer are required to build Intuition's project files for Windows, Mac, and Linux. While CMake can be used to compile the project, using the Projucer is recommended. The latest version of JUCE can be downloaded from the official JUCE website [here](https://juce.com/download). Once JUCE and the Projucer are installed, we will move to compiling the AI microservice executable.

## Step 2 - Build Project Files
Open the project in the Projucer. Open up the [Intuitition.jucer](Intuition.jucer) file in the Projucer and select "save and open in IDE," with the correct IDE for exporting selected. Once loaded, the project can be compiled and ran.

## Step 3 - Using Intumi
To use Intumi, you will need your own Groq API key to use for querying. You can get one [here](https://console.groq.com/keys) for free. once your API key is created, you will insert it into the API key field in the Intumi tab.

## Step 4 (Optional) - Finding Waveforms for Wavetable Morphing
All sorts of unique single-cycle waveforms for wavetable morphing can be found [here](https://github.com/KristofferKarlAxelEkstrand/AKWF-FREE) thanks to KristofferKarlAxelEkstrand. After running the application at least once, you can drop any waves you wish to use into the "Waveforms" folder in your documents (Intuition creates the directory upon first boot).
