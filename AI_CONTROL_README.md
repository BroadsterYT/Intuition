# AI Sound Control Feature

## Overview
The Intuition synthesizer now includes an **AI-powered parameter control system** that allows you to modify the synth's sound using natural language commands! Simply type what you want to hear, and the AI will translate your request into parameter changes.

## Features
- 🤖 **Natural Language Control**: Type commands like "make it warm", "add brightness", or "make it punchy"
- 🔄 **Real-time Parameter Adjustment**: Changes are applied immediately to the synth
- 💾 **Persistent API Key Storage**: Your Google AI Studio API key is saved with the plugin state
- 🎨 **Intuitive UI**: Clean, integrated interface with status feedback

## Getting Started

### 1. Get a Google AI Studio API Key
1. Visit [Google AI Studio](https://aistudio.google.com/apikey)
2. Sign in with your Google account
3. Click "Get API Key" or "Create API Key"
4. Copy your API key (it will look like: `AIzaSy...`)

**Note**: The free tier of Google AI Studio is generous and should be sufficient for most usage!

### 2. Configure the Plugin
1. Open the Intuition plugin in your DAW
2. Navigate to the **AI** tab
3. Paste your API key into the "Google AI Studio API Key" field
4. Click "Save Key"

### 3. Start Controlling with AI!
Type natural language commands in the text box and press Enter or click "Apply AI Changes".

## Example Commands

### Tonal Quality
- `make it warm` - Lowers filter cutoff, adds subtle detune
- `make it bright` - Raises filter cutoff for more high frequencies
- `add some grit` - Increases resonance and harmonic content
- `make it smooth` - Softens the sound with longer attack/release

### Dynamics
- `make it punchy` - Fast attack, short decay, moderate sustain
- `make it soft` - Slower attack, longer release
- `add some attack` - Increases envelope attack time
- `shorter release` - Reduces envelope release time

### Space & Effects
- `make it spacey` - Enables reverb with large room size
- `add reverb` - Enables reverb effect with moderate settings
- `more room size` - Increases reverb room size
- `dryer sound` - Reduces reverb wet level

### Oscillator Control
- `add bass` - Enables an oscillator pitched down an octave
- `fat sound` - Enables multiple oscillators with detune
- `thin it out` - Reduces oscillator count and detune
- `more detune` - Increases detuning for a wider sound

### Filter Control
- `open the filter` - Increases filter cutoff frequency
- `close the filter` - Decreases filter cutoff frequency
- `more resonance` - Increases filter resonance for emphasis
- `less resonance` - Reduces filter resonance

## How It Works

### Architecture
```
User Command → AI (Gemini 2.5) → JSON Response → Parameter Mapper → Synth Parameters
```

1. **User Input**: You type a natural language command
2. **AI Processing**: The command is sent to Google's Gemini 2.5 Flash model with context about the synth's parameters
3. **JSON Translation**: The AI returns a JSON object with specific parameter changes
4. **Parameter Mapping**: The system validates and applies the parameter changes
5. **Audio Update**: The synth immediately reflects the new parameter values

**Note**: This plugin uses **Gemini 2.5 Flash**, the latest model available as of 2025. If you have an older API key, make sure it's still valid at [Google AI Studio](https://aistudio.google.com/apikey).

### Technical Components
- **GeminiAPIClient**: Handles HTTP communication with Google AI Studio API
- **AIParameterMapper**: Translates AI responses into valid synth parameter changes
- **AIControlPanel**: User interface for interaction
- **System Prompt**: Teaches the AI about available parameters and their effects

## Available Parameters

The AI can control the following parameters:

### Master
- `MASTER` (0.0-1.0): Master volume

### Oscillators (A, B, C, D)
- `*_TOGGLE` (bool): Enable/disable oscillator
- `*_VOLUME` (0.0-1.0): Oscillator volume
- `*_DETUNE` (0-100): Detune amount
- `*_MORPH` (0.0-1.0): Wavetable morph position
- `*_OCTAVE` (-4 to 4): Octave shift
- `*_COARSE` (-12 to 12): Semitone shift
- `*_FINE` (-100 to 100): Fine pitch adjustment

### Filter
- `FILTER_CUTOFF` (20-20000 Hz): Cutoff frequency
- `FILTER_RESONANCE` (0.01-1.0): Filter resonance

### Reverb
- `REVERB_TOGGLE` (bool): Enable/disable reverb
- `REVERB_DAMPING` (0.0-1.0): High frequency damping
- `REVERB_ROOM_SIZE` (0.0-1.0): Room size
- `REVERB_WIDTH` (0.0-1.0): Stereo width
- `REVERB_DRY_LEVEL` (0.0-1.0): Dry signal level
- `REVERB_WET_LEVEL` (0.0-1.0): Wet signal level

### Envelopes
- `ENV_OSC_ATTACK`, `ENV_OSC_DECAY`, `ENV_OSC_SUSTAIN`, `ENV_OSC_RELEASE` (0.0-1.0)

## Tips for Best Results

1. **Be Specific**: More specific commands generally yield better results
   - Good: "increase filter cutoff to 2000 Hz"
   - Also Good: "make it brighter"

2. **Iterate**: You can send multiple commands in sequence to refine the sound
   - "make it warm" → "add more detune" → "increase reverb"

3. **Combine Concepts**: The AI understands compound requests
   - "make it warm and spacey"
   - "punchy with lots of reverb"

4. **Experiment**: Try different phrasings to discover what works
   - The AI has been trained on common audio production terminology

## Troubleshooting

### "API key is empty" Error
- Make sure you've pasted your API key and clicked "Save Key"

### "Request failed" Error
- Check your internet connection
- Verify your API key is valid at [Google AI Studio](https://aistudio.google.com/apikey)
- Ensure you haven't exceeded your API quota

### "models/gemini-1.5-flash is not found" Error
- This error occurred in earlier versions that used retired models
- **Solution**: Update to the latest version which uses Gemini 2.5 Flash
- If you still see this, your build may be outdated

### Changes Not Applied
- Check the status message for specific parameter errors
- Some parameter combinations might be invalid
- Try more specific commands

### Unexpected Results
- The AI interprets commands based on common audio production conventions
- Try rephrasing your request
- Use more specific parameter values if needed

## Privacy & Security

- **API Key Storage**: Your API key is stored locally in the plugin's state
- **Network Requests**: Only sent when you click "Apply AI Changes"
- **Data Sent**: Only your text command and the synth's parameter definitions
- **No Audio Data**: Your audio is never sent to any external service

## Development Details

### Files Added
- `Source/GeminiAPIClient.h/cpp` - API communication
- `Source/AIParameterMapper.h/cpp` - Parameter translation
- `Source/AIControlPanel.h/cpp` - User interface

### Files Modified
- `Source/MainTabs.h/cpp` - Added AI tab
- `Source/PluginProcessor.h/cpp` - API key storage
- `Source/PluginEditor.cpp` - API key persistence
- `Intuition.jucer` - Project configuration

## Future Enhancements (Ideas)

- Preset generation: "create a warm pad preset"
- Command history for quick recall
- Favorite commands/macros
- Voice input support
- Learning from user adjustments
- Multi-language support

## Credits

Developed as an enhancement to the Intuition wavetable synthesizer, integrating Google's Gemini AI for natural language parameter control.

---

**Enjoy creating sounds with AI! 🎵🤖**
