FROM 01e9/ide:cpp

RUN apt-get update \
    && apt-get install -y \
        libgl-dev libvulkan-dev libxrandr-dev libxxf86vm-dev libxinerama-dev libxcursor-dev \
        mesa-utils mesa-utils-extra kmod \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
