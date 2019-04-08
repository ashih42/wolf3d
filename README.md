# wolf3d
Our own [Wolfenstein 3D](https://en.wikipedia.org/wiki/Wolfenstein_3D) with [ray-casting](https://en.wikipedia.org/wiki/Ray_casting) engine in C using OpenGL. (42 Silicon Valley)

<p float="left">
  <img src="https://github.com/ashih42/wolf3d/blob/master/Screenshots/screenshot1.png" width="280" />
  <img src="https://github.com/ashih42/wolf3d/blob/master/Screenshots/screenshot4.png" width="280" />
  <img src="https://github.com/ashih42/wolf3d/blob/master/Screenshots/burgers.png" width="280" />
</p>

## Prerequisites

You are on macOS with `gcc`, `make`, and `brew` installed.

## Installing

```
./setup.sh
```

## Running

### Single-Player Mode

```
./wolf3d map_file
```

### Online Multi-Player Mode

Theoretically it supports up to 256 players!

#### Server

```
./wolf3d map_file -s port
```

#### Client

```
./wolf3d map_file -c ip port
```

## Controls

### Top-level Controls

* `Tab` Capture/release mouse cursor.
* `~` Save screenshot in PPM format.
* `Escape` Terminate the game.

# Gameplay Controls

* `W`, `A`, `S`, `D` Move player.
* `Q`, `E` Rotate camera.
* `Move mouse` Rotate camera.
* `Spacebar` Shoot burger!

### Graphics Controls

* `M` Select no/fixed/rotating mini-map.
* `F` Select next floor tile set.
* `C` Toggle [fisheye distortion](https://en.wikipedia.org/wiki/Fisheye_lens) on/off.
* `V` Select no/some/extreme viewport wobbling.
