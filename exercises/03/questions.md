# TIE-50307 - Exercise 03 - Q&A

## 1. What is the goal of this exercise? What did you accomplish?

From this exercise we learned about basic structure of linux driver, writing/reading file from entry dir, kernel specific library, memory allocation and debugging bugs for kernel driver.

## 2. Where is the `evil-tests` script installed inside the image? Where is the accompanying data installed? Describe how this is set up in Yocto.

evil-tests is installed under opt/evil-tests/sbin/evil-tests.sh
accompanied data is installed under opt/evil-tests/share/data.txt
these 2 files are set up under evil-tests.bb file of recipe evil-tests of meta-tie50307 layer. The layer's path is included in build/conf/layer.conf. Inside evil-tests.bb the do_install() code block is used to specify the path to install the 2 above files into the image.

## 3. How are the LKM and `evil-tests` added to `core-image-minimal`? Briefly describe what sets apart the `evil-tests` recipe compared with most recipes included in `meta-groupXX`, `meta-pynq`, `meta-xilinx` and `poky`.

They are recipes included in meta-tie50307, which path is specified at build/conf/layer.conf for building with core-image-minimal.

## 4. How many buffers does the `evil` LKM use? List their names, their size and their purpose. How is their memory allocated?

there are data_storage buffer for storing data from input buffer, input_buf for carrying data for the writing, buf buffer in each read/write function to extract data from 2 mentiond buffer. input_buf is statically allocated with fixed INPUT_BUFSIZE, while data_storage is dynamically allocated with STORAGE_SIZE size, using kmalloc function.

## 5. How are user-input strings stored inside the module? How does the module iterate over this list? What's the loop end condition?

User input strings are stored using sprintf() to a buf in evil_store() function. The function tasklet_schedule is used to schedule a tasklet to store a string.

## 6. What is the path for the `evil` LKM's sysfs entry? How is it determined? How is it mapped to the module functionality?

path of evil's sysfs entry is under /sys/kernel/evil_module/
It is created as a kobject. Also a struct is used to declare its attribute(name, permission, function to read/write from). Finally add the attribute to kobject using sysfs_create_file

## 7. What bugs did you find/fix? Categorize them and briefly describe each matching *category* (what's the *impact* of each *category*? How common do you *think* it is?)

Segmentation fault: mostly came from memory allocation, where memory was initialzied NULL. Other reason for this bug was kfree freeing memory not created by kmalloc. order of kfree and tasklet_kill also caused this problem. Finally array read/writing more than defined size might caused this problem.

Permission denied: lack of permission to write to sysfs.

Most common bug was related to read/write size: read/write more than defined size,...

## 8. What are out-of-bounds accesses and stack overflows? Are they relevant for this exercise? What could be the consequences of such defects in a LKM?

Out-of-bounds happens when array accessed outside of its size or accessed to unallocated memory.

Stack-overflow happens when a program uses more memory space than the stack size, but forgot to free the memory.

The 2 errors are the most common bug in the exercise.

The consequences of these 2 errors are causing segmentation faults, make it harder to find bug, making program freeze or crash.

## 9. Feedback (what was difficult? what was easy? how would you improve it?)

This exercise was really difficult in my opinion. Fixing bugs related to size of the buffers is the hardest part, easy to get lost. The easier part (but not so easy) is fixing the segmentation fault related to memory allocation.
