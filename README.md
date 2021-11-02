# bareOS

x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

This is an academic project where we develop a basic OS capable of managing basic inter process comunications (IPC's) methods like pipes and semaphores.

## Environment setup:
1- Install the following packages before building the Toolchain and Kernel:
```bash
nasm qemu gcc make
```
## Build the Toolchain

Execute the following commands on the x64BareBones project directory:
```bash
  user@linux:$ cd Toolchain
  user@linux:$ make all
```
## Build the Kernel

From the x64BareBones project directory run:
```bash
  user@linux:$ make all MM=BUDDY
```
> Note that you can build the kernel with either `MM=BUDDY` or the default, `MM=FREE_MALLOC`

## Run the kernel

From the x64BareBones project directory run:
```bash
  user@linux:$ ./run.sh
```

This project's report is in this repository's wiki.
