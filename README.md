```
[..               [......    [..
[.. [... [.. [..  [..  [.. [.[. [.
[..  [..  [.  [.. [. [..     [..
[..  [..  [.  [.. [..  [..   [..
[..  [..  [.  [.. [..    [.. [..
```

![version](https://img.shields.io/badge/version-0.1-red)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/mimic-sussex/eppEditor/blob/master/LICENSE)

# What is ImRt?

A framework for the developement of audio applications using __Dear ImGui__ with the backends __GLFW__ and __OpenGL__ as well as __ImPlot__ and __RtAudio__.

## Project Setup

In your terminal:

```bash
mkdir ____ # Use project name.
cd ____ # Use project name.
git init
git submodule add https://github.com/tonstein/imrt.git lib/imrt
git submodule update --init --recursive
cp -r lib/imrt/examples/utility/* .
code .
```

## Documentation

![https://tonstein.github.io/imrt](https://tonstein.github.io/imrt)

## Examples

### Oscilloscope

<img src="examples/oscilloscope/img/oscilloscope.gif" alt="Oscilloscope" width="2034">

See [examples/oscilloscope](examples/oscilloscope).

### Utility 

<img src="examples/utility/img/utility.png" alt="Oscilloscope" width="100">

See [examples/utility](examples/utility).
