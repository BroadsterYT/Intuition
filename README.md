# Intuition
Intuition is a VST/AU/standalone digital synthesizer designed to combine high-performance digital signal processing with an intuitive, modular workflow for sound design and music production. 

# Overview
The project aims to provide musicians and producers with a flexible environment for creating and manipulating sound in real time, without sacrificing usability or creative flow. Unlike many existing synthesizers that trade performance for complexity or simplicity for control, Intuition seeks to bridge that gap. It offers a robust synthesis engine built in C++ using JUCE, capable of handling multiple oscillators, modulation sources, and signal routing options efficiently. The system architecture emphasizes modularity, allowing features such as wavetable synthesis, unison detuning, and morphing to be expanded and customized over time. Ultimately, Intuition’s goal is to empower users to explore advanced sound design techniques through a responsive, user-friendly interface that feels creative rather than technical.

# Features
## 4 Powerful Oscillator Modules
## Wavetable Morphing
## Advanced Modulation Routing

# Setup Instructions
## Step 1 - Install JUCE
JUCE and the Projucer are required to build Intuition's project files for Windows, Mac, and Linux. While CMake can be used to compile the project, using the Projucer is recommended. The latest version of JUCE can be downloaded from the official JUCE website [here](https://juce.com/download). Once JUCE and the Projucer are installed, we will move to compiling the AI microservice executable.

## Step 2 - Build the AI Microservice Executable
Intuition uses Python 3.12.2 to build an executable from the included Groq AI microservice (without using the Groq API). The official Python 3.12.2 release can be downloaded [here](https://www.python.org/downloads/release/python-3122).

Depending on the operating system the project is being built in, the executable generation process will vary.

### Windows
Open the Command Prompt (<kbd>Win</kbd> + <kbd>R</kbd> and enter `cmd`) and naviagte to the `\Intumi` directory inside the project.
Once inside the directory, we will create a virtal environment to build within. enter the command:
```
python -m venv .venv
```
This command will create a new Python virtal enviroment within the `\Intumi` directory. Once completed, activate the virtual environment with:
```
.venv\Scripts\activate.bat
```
> [!WARNING]
> After running the activation command, it is wise to check that the virtual environment is properly activated. To check the current Python and PIP installation directories, you can run the commands:
> ```
> where python
> ```
> ```
> where pip
> ```
> The first listed directory for both should be the location of the virtual environment.

Now, we need to install the python packages necessary to build the executable. This can be done with the command:
```
pip install -r requirements.txt
```
Once all dependencies are installed, we can now build the final exectutable with the command:
```
pyinstaller --onefile --collect-all requests intumi.py
```
Once completed, pyinstaller will create several new directories, one of them being called `\dist`, which will include the final executable.
### Mac
NYI
### Linux
NYI

## Step - Building Project Files
With the AI microservice executable created, we can now open the project in the Projucer. Open up the [Intuitition.jucer](Intuition.jucer) file and select "save and open in IDE," with the correct IDE for exporting selected.
