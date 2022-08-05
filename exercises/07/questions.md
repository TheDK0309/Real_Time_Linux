# TIE-50307 - Exercise 07 - Q&A

## 1. What is the goal of this exercise? What did you accomplish?
At this exercise I have learn about character device driver, its purpose, how it works and applied it to develop in the exist irqgen-mod, in order to receive stream of data. Moreover, I made the app that read data formatted like the char device and output the statistics of the data.

## 2. What is a Character Device? How is it different from a Block Device?

Character devices are devices that can be accessed as streams of bytes, character by character, by using char driver. These drivers have open, close, read, write system call. 
Block devices are similar to regular files, allowing a buffered array (block) of cached data to be viewed or manipulated with operations such as reads, writes

## 3. What design limits of the Ex6 version of `irqgen-mod` do we try to address adding a character device?

Char device helps in reading stream of info to the module.

## 4. What system calls are we implementing for the filesystem representation of our character device?

Open, close and read system call.

## 5. How does a device node differ from a regular file? What things are similar/identical?

A device file is an interface for a device driver that appears in a file system as if it were an ordinary file. These are usually found under the /dev directory and are also called device nodes. A device file can represent character devices, which emit a stream data one character at a time, or block devices which allow random access to blocks of data. While regular file is used to store information , setting , or commands for computer programs. The similar things between device file and regular file is they can be read, written, opened, closed and device file can be used as a regular file.

## 6. Which parts of our LKM code are potentially running in parallel? Which parts could interact in a conflicting way? Make a few concrete examples of conditions in which conflicts in concurrency may arise.

irqgen_irqhandler() in irqgen_main.c and irqgen_cdev_read() in irqgen_cdev() can be run in parallel and get conflict since they access data from data(struct* irqgen_data). If the irq handler is called, while the character device is reading in other thread, both processes will be terminated and generate error.

## 7. In the proposed structure, we opted for a single lock for the whole `irqgen_data` structure. Could you describe advantages and disadvantages of a single lock vs several locks? What about the effects on the IRQ handling latency?

Advantage:	
Single Lock:can manage the whole code that access to the data at same time with spinlock 	
Multiple Lock:Deadlock can not occur when multiple locks are called
 
Disadvantage:
Single Lock:if 1 function ,which acquiring the lock, call the other attempting to claim the lock -> dead lock -> affecting latency because when the lock is on, every other processes have to wait until unlocked and move on 
Multiple Lock:require priority handling

## 8. To avoid concurrency issues among the different parts of `irqgen-mod` you had to choose a synchronization mechanisms. What kind did you choose? Why? Where? What alternatives did you consider? Why did you reject them?

Spinlock was chosen. Spinlock allows a thread to acquire the lock, execute its critical code section and release the lock when it finished its critical code section, while other threads have to what in a loop until the lock is released, for rather short time -> lower latency. Spinlock is suitable for process should not be put to sleep like ISR.Spinlock was used in the code where read/write to shared data irqgen_data. For example in irqgen_irqhandler() function in irqgen_main.c, where irqgen_data members are modified when handling an ISR.

## 9. To optimize even more the IRQ handling latency we should abandon the monolithic handler function for a more efficient pattern: what's its name? Describe how the code currently handling the requests would be organized and describe a few reasons to choose the alternative over the monolithic style.

The latency of the IRQ line now instead of saving into an array, which had limited amount, is saved into ring buffer, which has unlimited capacity, as long as read and write to the buffer happen constantly. Moreover, this code section will be execute inside a lock. In our case we use spinlock, which arguably have short execution time thus reduce the latency time.

## 10. What is an SDK?

SDK stands for software development kit. It is a tool/program that developer used to create application for different platforms.
SDK tools will include a range of things, including libraries, documentation, code samples, processes, and guides that developers can use and integrate into their own apps. SDKs are designed to be used for specific platforms or programming languages.

## 11. In the context of the Yocto Project, what are the differences between the Standard and the Extensible SDK?

You would use the Standard SDK if you want a more traditional toolchain experience as compared to the extensible SDK, which provides an internal build system and the devtool functionality.
The installed Standard SDK contains an SDK environment setup script, some configuration files, and host and target root filesystems to support usage, while the installed extensible environment contains setup script, some configuration files, an internal build system, and the devtool functionality


## 12. Describe your workflow in developing and testing the `irqgen_statistics` app

The app workflow was: first reading the input from a file through fscanf() function and print the content to terminal. However to read from stdin, getline() function was used. But first, using this function to read from a file again, line by line. The app read a line until it reach the first “,” that value is saved as irq line, the second time it reach “,”, the value is saved as latency, also increase counter for each irq line received. Afterward printing was ok, statistics calculation will be developed. This happens when count of irq > 0. The latency is saved to a 2D array with indexes are irq line and number of times it happened. The latencies received are compared  to find the max one. The latencies with same irq line are summed up to find the average. The same idea applied to find the max latency of all irq line and find the average of all irq line. Finally print the result. This result first applied to the csv file generated from the char device, then change the source receive parameter from getline() function to stdin.

## 13. Feedback (what was difficult? what was easy? how would you improve it?)
The exercise is long and difficult.

