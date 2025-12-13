<img src="Assets/GitHubImages/intn_logo.png?"/>

# Overview
Intuition is a VST/AU/standalone digital synthesizer designed to combine high-performance digital signal processing with an intuitive, modular workflow for sound design and music production. 

The project aims to provide musicians and producers with a flexible environment for creating and manipulating sound in real time, without sacrificing usability or creative flow. Unlike many existing synthesizers that trade performance for complexity or simplicity for control, Intuition seeks to bridge that gap. It offers a robust synthesis engine built in C++ using JUCE, capable of handling multiple oscillators, modulation sources, and signal routing options efficiently. The system architecture emphasizes modularity, allowing features such as wavetable synthesis, unison detuning, and morphing to be expanded and customized over time. Ultimately, Intuition’s goal is to empower users to explore advanced sound design techniques through a responsive, user-friendly interface that feels creative rather than technical.

# Setup Instructions
## Step 1 - Install JUCE
JUCE and the Projucer are required to build Intuition's project files for Windows, Mac, and Linux. While CMake can be used to compile the project, using the Projucer is recommended. The latest version of JUCE can be downloaded from the official JUCE website [here](https://juce.com/download). Once JUCE and the Projucer are installed, we will move to compiling the AI microservice executable.

## Step 2 - Setup Python Virtual Environment for Building AI Microservice Executable
> [!NOTE]
> This step can be skipped if no changes have been made to the `intumi.py` file.

We know, it's a very scary step name, but it is actually very straightforward. Intuition uses Python 3.12.2 to build an executable from the included Groq AI microservice (without using the Groq API). The official Python 3.12.2 release can be downloaded [here](https://www.python.org/downloads/release/python-3122).

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
This should correctly activate the virtaul environment. You will see `(.venv)` at the beginning of each succeeding command-line input.

### Mac
Open the terminal inside the `/Intumi` directory. We need to create a Python virtual environment to build the app bundle. Run the command:
```
python3 -m venv venv
```
Now, to activate the virtal environment, run the command:
```
source venv/bin/activate
```
This should correctly activate the virtual environment. You should see `(venv)` at the beginning of each succeeding terminal input.

### Linux
NYI

## Step 3 - Build the Microservice Executable
> [!NOTE]
> This step can be skipped if no changes have been made to `intumi.py`

> [!WARNING]
> Before installing the required Python packages, it is wise to check that the virtual environment is properly activated. To check the current Python and PIP installation directories, you can run the commands:
> ```
> where python
> ```
> ```
> where pip
> ```
> The first listed directory for both should be the location of the virtual environment.

Now that our virtual environment is set up and activated, we need to install the Python packages necessary to build the executable. This can be done with the command:
```
pip install -r requirements.txt
```
Once all dependencies are installed, we can now build the final exectutable with the command:
```
pyinstaller --onefile --collect-all requests intumi.py
```
Once completed, `pyinstaller` will create several new directories, one of them being called `\dist`, which will include the final executable. Copy the executable file and move it to the `\IntumiExes` directory. This is where the Projucer references all the AI microservice executables for each operating system.

## Step 4 - Build Project Files
With the AI microservice executable(s) created, we can now open the project in the Projucer. Open up the [Intuitition.jucer](Intuition.jucer) file in the Projucer and select "save and open in IDE," with the correct IDE for exporting selected. Once loaded, the project can be compiled and ran.

## Step 5 - Using Intumi
TO use intumi, you will need your own Groq API key to use for querying. You can do that [here](https://console.groq.com/keys). once your API keyt is created, you will insert it into the API key field in the Intumi tab.

## Step 6 (Optional) - Finding Waveforms for Wavetable Morphing
All sorts of unique single-cycle waveforms for wavetable morphing can be found [here](https://github.com/KristofferKarlAxelEkstrand/AKWF-FREE) thanks to KristofferKarlAxelEkstrand. After running the application at least once, you can drop any waves you wish to use into the "Waveforms" folder in your documents (Intuition creates the directory upon first boot).
