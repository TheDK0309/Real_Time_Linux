# Exercise 01 - Q&A

## 1. What did you accomplish in this exercise?

In this exercise we created our own linux distro using yocto project, quick start, by adding dependencies to the distro with meta data. We got familiar with the yocto project, process and structure.

## 2. How does Yocto recognize that the target machine "pynq" is supported?

in .../build/conf/local.conf, target machine in MACHINE ? = "pynq"

## 3. Where do you find the device-tree source code for the PYNQ board?

In .../meta-pynq/recipes-bsp/device-tree/files/devicetree.dts

## 4. What is the priority of the PYNQ support layer?

In the following order:
.../poky/meta
.../poky/meta-poky
.../poky/meta-yocto-bsp
.../poky/meta-xilinx/meta-xilinx-bsp
.../poky/meta-xilinx/meta-xilinx-contrib
.../poky/meta-pynq

## 5. What additional applications are installed to the `core-image-minimal` target in the PYNQ support layer?

rt-test and dropbear
