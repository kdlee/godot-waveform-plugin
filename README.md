# Godot Waveform Plugin

A GDExtension plugin for Godot 4.1+ that generates waveform data from audio streams. The plugin provides a singleton utility to extract min/max amplitude pairs from audio streams, which can be used to visualize waveforms.

## Features

- Generate waveform data from any `AudioStream` that supports playback
- Configurable sampling frequency for waveform resolution
- Returns `PackedVector2Array` with min/max pairs for each time window
- Cross-platform support (macOS, Windows, Linux, Android, iOS, Web)

## Building

### Prerequisites

- Python 3.x
- SCons build system
- C++ compiler (Clang, GCC, or MSVC depending on platform)
- Godot 4.1 or later

### Build Steps

1. Clone the repository and initialize submodules:
```shell
git clone <repository-url>
cd godot-waveform-plugin
git submodule update --init --recursive
```

2. Build the plugin:
```shell
scons
```

The compiled library will be placed in `bin/<platform>/` and automatically copied to `demo/bin/<platform>/`.

### Build Options

- Generate compile commands for IDE support:
```shell
scons compiledb=yes
```

- Build for specific platform (see SConstruct for available platforms)

## Installation

1. Copy the `.gdextension` file and library files to your Godot project:
   - Copy `demo/bin/example.gdextension` to your project (rename as needed)
   - Copy the appropriate library file from `demo/bin/<platform>/` to match the `.gdextension` file location

2. Update the library paths in the `.gdextension` file if you place it in a different location

3. The plugin will be automatically loaded when you open your project in Godot

## Usage

The `Waveform` class is available as a singleton. Use it to generate waveform data from audio streams:

```gdscript
# Get the Waveform singleton
var waveform = Waveform.get_singleton()

# Generate waveform data from an AudioStream
# sampling_frequency: how many samples per second (e.g., 60 Hz = 60 samples per second)
var audio_stream = preload("res://path/to/your/audio.ogg")
var result = waveform.minmax(audio_stream, 60.0)

# result is a PackedVector2Array where each element is Vector2(min, max)
# representing the minimum and maximum amplitude for that time window
```

### Example: Visualizing a Waveform

```gdscript
extends TextureRect

@export var audio_stream: AudioStream

func _ready() -> void:
    var waveform = Waveform.get_singleton()
    var result = waveform.minmax(audio_stream, 60.0)

    if result.is_empty():
        return

    # Create image from waveform data
    var width = result.size()
    var height = 256
    var image = Image.create(width, height, false, Image.FORMAT_RGB8)
    image.fill(Color.BLACK)

    # Draw waveform
    for i in range(result.size()):
        var min_max = result[i]
        var min_val = min_max.x
        var max_val = min_max.y

        # Convert from [-1, 1] range to [0, height-1] range
        var min_y = clamp(int((1.0 + min_val) * 0.5 * height), 0, height - 1)
        var max_y = clamp(int((1.0 + max_val) * 0.5 * height), 0, height - 1)

        # Ensure min_y <= max_y
        if min_y > max_y:
            var temp = min_y
            min_y = max_y
            max_y = temp

        # Draw line from min to max
        for y in range(min_y, max_y + 1):
            image.set_pixel(i, y, Color.WHITE)

    # Create texture from image
    self.texture = ImageTexture.create_from_image(image)
```

### API Reference

#### `minmax(stream: AudioStream, sampling_frequency: float) -> PackedVector2Array`

Generates min/max waveform pairs for the given `AudioStream` at the specified sampling frequency.

- **stream**: The audio stream to analyze (must support playback, e.g., `AudioStreamOggVorbis`)
- **sampling_frequency**: Samples per second (e.g., `60.0` for 60 samples per second)
- **Returns**: `PackedVector2Array` where each element is `Vector2(min, max)` representing the amplitude range for that time window

#### `magnitude(stream: AudioStream, sampling_frequency: float) -> PackedFloat32Array`

Generates magnitude values for the given `AudioStream` at the specified sampling frequency.

- **stream**: The audio stream to analyze (must support playback, e.g., `AudioStreamOggVorbis`)
- **sampling_frequency**: Samples per second (e.g., `60.0` for 60 samples per second)
- **Returns**: `PackedFloat32Array` where each element is `max(abs(min), abs(max))` for each time window

## License

MIT License - see [LICENSE.md](LICENSE.md) for details.
