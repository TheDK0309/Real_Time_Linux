# TIE-50307 - Exercise 02 - Q&A

## 1. What is the goal of this exercise? What did you accomplish?

The goal of this exercise was learning about yocto-layer and basic kernel development, using out-of-tree module method. We have created our own layer, added its recipe to the build, created LKM using out-of-tree module, showed info of the module, showed greeting and farewell message when load/unload the module.

## 2. What is a Yocto layer? What are the essential requirements to correctly define a new one?
Yocto layer is a repository in which contains set of instructions(recipes) to tell the OpenEmbedded what to do. Layers can be modified, recycled and shared.

Requirements to create new one is having a source file to contain source code, a recipe (.bb), a configuration file(.conf) and assign a priority for the layer. Finally , in order to include the layer to the build, including the layer's path into build/conf/bblayer.conf file.

## 3. What priority did you assign to your layer? How and why?
Priority determines which layer takes precedence if there are recipe files with the same name in multiple layers. As such, the recipe file from the layer with a higher priority number takes precedence. Also affect order in multiple .bbappend files from same recipe are applied. 

Default priority 6. By either change the layer.conf of the layer or when creating the layer, the menu asked to input layer, or defaisult is 6. The reason was, in this case no layers contain the same recipe's name as the custom layer so priority should not matter here.

## 4. What does LKM stand for (in the context of this exercise)? What does it mean and what are the characteristics of a LKM? What's the alternative?

Loadable Kernel Module, is a mechanism to add/remove code from the module at runtime. It allows the kernel to communicate with the hardware even does not know how the hardware works.

Alternative to LKMs would be to build the code for each and every driver into the Linux kernel(in tree).

## 5. What does Out-of-Tree Module mean? What's the alternative and what are the differences?

Drivers in the Linux kernel can either be built into the kernel, or built as modules that stay on disk and get loaded as needed. Out-of-tree has source code located elsewhere.

Alternative is in-tree build, which the modules can be generated while building the kernel package

## 6. How did you define the module metadata? How does it work?
MODULE_LICENSE("GPL");              /*lincense*/
MODULE_AUTHOR("Le Do");      /* The author(s)*/
MODULE_AUTHOR("Robert Cantaragiu");
MODULE_DESCRIPTION("Linux Driver for Exercise 2");  /* The description */
MODULE_VERSION("0.1");              /* The version of the module*/

The metadata of the module is shown using modinfo <module-name>.ko

## 7. How is the module built? How does it get installed inside `core-image-minimal`?

The module is built by a recipe under a layer, by including the path of the layer in bbappend file. It is installed to core-image-minimal by configuring local.conf file using IMAGE_INSTALL_append "<module-name>" and MACHINE_EXTRA_RRECOMMENDS += "kernel-module-<module-name>"

## 8. What is the path for the `sysfs` entry for the module? List and explain its contents.

Under /sys/module/<module-name>/
Some files in the folder :
parameters: changeable corresponding parameter
srcversion: the version of the module
sections: module sections, used for debugging.
coresize: size of sys
taint: tells if kernel is clean or anything unusual happened. In this case "O" means an externally-built ("out-of-tree") module has been loaded.
## 9. Feedback (what was difficult? what was easy? how would you improve it?)
The exercise was difficult at the beginning, since there were overwhelmingly large ammount of material to read. After we have carefully read and try out, the exercise was pretty straight-forward.
