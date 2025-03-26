![logo](https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc/internal/logo/e34logo.png)

# e34

Vim-like UI engine.

![demo1](https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc/internal/demo/e34demo.gif)

![demo2](https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc/internal/demo/e34demo_android12.gif)

## Prerequisites

The installation of prerequisites is shown for
[ArchLinux](https://archlinux.org/).

**Install the gcc package**

```sh
sudo pacman -S gcc
```

**Install the make package**

```sh
sudo pacman -S make
```

## Build

**Make build**

```sh
git clone https://gitflic.ru/project/0mega3/e34.git
cd /target/e34
make
```

**Clean build (remove all compilation artifacts)**

```sh
cd /target/e34
make clean
```

## Usage

**Start the e34 ui engine**

```sh
./e34 tui
```

**Help**

`:help<Enter>`

## License

`e34` was created by Denis Khorkin. It is licensed under the terms
of the BSD-3-Clause license.

## Credits

- Denis Khorkin
