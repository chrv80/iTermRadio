# iTermRadio
Terminal based internet radio player written in C, using libmpv and ncurses


## Features

- Terminal UI using ncurses
- Media playback via libmpv
- Lightweight and fast
- Working on linux distros, in termux on android and on mac os (tested on OS X Leopard PPC, should work on any other version and architecture with dependencies installed)

## Dependencies

- libmpv (default build – GPL-2.0-or-later) (on some distros or mac OS install mpv via macports)
- ncurses

## Installation

1) Install dependencies
2) Clone this repository
3) Run "make"
4) To install system-wide run "sudo make instal", it installs the iTermRadio app under /usr/sbin

<span style="text-decoration:underline;">   OR
Download the app from releases and run install script </span>

## Uninstall

Run "sudo make uninstall" or remove the iTermRadio app from /usr/sbin or <span style="text-decoration:underline;"> run install script </span>

## Usage

- Run "iTermRadio" from terminal, it will check if there is a radio list file (radiolist.lst) in the same directory, or in home directory (.radiolist.lst)
- Run "iTermRadio --stream URL" to play a specific radio stream
- Run "iTermRadio --list list_name" to load specific radio list

Radio lists are text files, every line must be a internet radio station URL.

You can use mouse or/and keyboard for usage.

## Notes

- If you want to change default radio list file, at the moment you need to edit the iTermRadio.c file and recompile.
- On OS X Leopard, mouse input didn't work.
- In order to add a internet radio station URL to list file, at the moment you need to update the list file manually.
- I included a radiolist.lst with my favourite radio stations

<img width="1680" height="1050" alt="Στιγμιότυπο οθόνης από 2026-03-02 15-37-50" src="https://github.com/user-attachments/assets/c6c557e5-c0cf-48ee-960d-070ea693a388" />
<img width="1440" height="900" alt="Picture 1" src="https://github.com/user-attachments/assets/54d1457e-18c4-47ea-b3dc-c8d88ac84d77" />
