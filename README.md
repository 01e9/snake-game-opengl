# Snake Game [Download](https://github.com/01e9/snake-game-opengl/releases)

https://youtu.be/-5x51jU4bFw

### Build

1. Open IDE in docker using [this project](https://github.com/01e9/docker-ide)

    ```bash
    .../docker-ide/ide cpp-gpu ~/.../path/to/ide.sh -x11docker "--gpu"
    ```

    Check `x11docker` output about video driver install.

2. Build target `external_all`
3. Reload CMake
4. Build target `snake_game_opengl`

##### P.S.

Huge thanks to [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL)!!!

I was dreaming of being able to write 3D apps [since I was in high-school](https://github.com/01e9/pascal-games) and this is my first 3D game!
