# Snake Game [Download](https://github.com/01e9/snake-game-opengl/releases)

https://youtu.be/-5x51jU4bFw

### Build

1. Build target `external_all`
2. Reload CMake
3. Build target `snake_game_opengl`

#### Run IDE in docker

[This project](https://github.com/01e9/ide-in-docker) has a docker image with required C++ and GUI libraries installed,
so you can run IDE in a docker container and keep your system clean of dev libs.

```shell
env DOCKER_IMAGE='01e9/ide-in-docker:cpp-gpu' X11DOCKER_OPTS='--gpu' ~/.../ide-in-docker ~/.../your-ide
```

_Tested with CLion IDE._

### Thanks

Huge thanks to [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL)!!!

I was dreaming of being able to write 3D apps [since I was in high-school](https://github.com/01e9/pascal-games) and this is my first 3D game!
