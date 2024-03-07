![](https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc%2Finternal%2Flogo%2Fe34logo.png)

e34
=====

Vim-like UI engine.

<img
src="https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc%2Finternal%2Fdemo%2Fe34demo.gif"
style="height:900px;">

<img
src="https://gitflic.ru/project/0mega3/e34/blob/raw?file=doc%2Finternal%2Fdemo%2Fe34demo_android12.gif"
style="height:300px;">

Prerequisites
-------------

The installation of prerequisites is shown for
[ArchLinux](https://archlinux.org/).

**Install the gcc package**

```console
    $ sudo pacman -S gcc
```

**Install the make package**

```console
    $ sudo pacman -S make
```

Build
-----

**Make build**

```console
    $ git clone https://gitflic.ru/project/0mega3/e34.git
    $ cd /target/e34
    $ make
```

**Clean build (remove all compilation artifacts)**

```console
    $ cd /target/e34
    $ make clean
```

Usage
-----

**Start the e34 ui engine**

```console
    $ ./e34 tui
```

**Help**

`:help<Enter>`

License
-------

`e34` was created by Denis Khorkin. It is licensed under the terms
of the BSD-3-Clause license.

Credits
-------

- Denis Khorkin
