# Snake Game in OpenGL

* [Preview](https://youtu.be/-5x51jU4bFw) on YouTube
* [Download](https://github.com/01e9/snake-game-opengl/releases/latest) latest release

### Build

1. Build target `external_all`
2. Reload CMake
3. Build target `snake_game_opengl`

#### IDE in Docker

You can run IDE isolated in a docker container that has all required libs.

[This project](https://github.com/01e9/ide-in-docker) has a script and docker images for different environments.

```shell
env DOCKER_IMAGE='01e9/ide-in-docker:cpp' X11DOCKER_OPTS='--gpu' ~/.../ide-in-docker ~/.../your-ide
# CLion IDE works
```

### Thanks

Huge thanks to [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL)!!!

P.S. I wanted to create 3D games [since I was in high-school](https://github.com/01e9/pascal-games) and this is my first 3D game!
