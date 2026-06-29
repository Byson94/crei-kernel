# Crei Kernel

A minimal kernel written in C. Nothing big, not the next linux, just a simple educational project.
Mainly built for and tested on `qemu` virtual machine.

## Compiling

Just run `make` and you will have an iso at `build/crei.iso`.

## Running

Just run the `crei.iso` file in `build/` with this command:

```bash
$ qemu-system-x86_64 -cdrom build/crei.iso -m 256M
```
