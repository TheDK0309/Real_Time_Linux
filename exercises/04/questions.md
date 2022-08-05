# TIE-50307 - Exercise 04 - Q&A

## 1. What is the goal of this exercise? What did you accomplish?

We have learned more about developing LKM, interrupt process and how to creat interrupt using interrupt generator IP block from the hardware. Furthermore we learned the functions used for memory addresses and bitfield macro usage.

## 2. Summarize the functionality provided by the IRQ Generator IP block, then describe what is the purpose of the various registers exposed by the FPGA device. Finally, ignore the fact that you were instructed to look for the base address of the IRQ Generator register space in the provided `devicetree.dts` and instead imagine you were also the developer of the FPGA core design, where would you have found the base address of the register space?

The IRQ generation logic enables generation of user specified amount of interrupts. The IRQ generator core can be programmed to generate interrupts to one interrupt line at the time.

Registers in the IRQ generator:
IRQ_GEN_CTRL_REG  :Core control register
IRQ_GEN_GENIRQ_REG  :IRQ generation control register
IRQ_GEN_IRQ_COUNT_REG  :Total IRQ count status register
IRQ_GEN_LATENCY_REG  :Last served IRQ latency status register

## 3. Describe the relationship between interrupt lines in the IRQ Generator, HW IRQ and Linux IRQ numbers, and how did you proceed to discover the IRQ number to use in the driver.
16 IRQ lines of the IRQ Generator block are connected to a (non contiguous) sequence of HW IRQ. Linux kernel drivers don't directly use the HW IRQ number,instead the kernel provides its own mapping layer
Based on the document, the 1st IRQ line is 61, using the command from finding_the_Linux_IRQ_number.md, going to /sys/kernel/irq on the board, the line is mapped to number 45.

## 4. How many interrupt lines are available in the documented FPGA core (refer to the spec, even if we only used one in the *beta* bitstream we were given)?

16

## 5. What value is reported in the devicetree for the first IRQ line? How is it determined? (check the spec document, it has all the required information to map the line number to the HW IRQ on the processing system)

Value is ACK1d, specified under wapice,intrack field of irq_gen@43C00000.

## 6. Using the information detailed in the previous answer, what should be written in the `devicetree.dts` line describing the IRQ Generator interrupts if it were to describe all the 16 HW IRQs? (write the exact line as it would appear in the `devicetree.dts`, notice that the HW IRQs are not contiguous!). Each interrupt line is specified by three cells in the device tree file; what information does each of these three cells represent?

The lines defined the IRQ generator characteristics are from 441 to 447. Line 446 defined its address and size. 3 cells are:
1st cell for interrupt type
2nd cell for the interrupt number 
3rd for interrupt type 

## 7. Why do we need to use `ioremap`, `ioread32` and `iowrite32`? Why do we want to use the Linux kernel bitfields macroes rather than using bitwise arithmetic directly?

In order to to accessing a memory region (aftersuccessfully requesting it), the region must be mapped into kernel address space by calling special architecture-dependent functions. ioread32() and iowrite32() are used to accessing these mapped memory.
A bitfield is a range of bits working as a single number. You usually can not access these ranges directly because memory is accessed in (multi-)byte-sized datatypes.


## 8. (BONUS, optional) Did you find any bug in the bitstream implementation while testing the sysfs interface?

## 9. Feedback (what was difficult? what was easy? how would you improve it?)

This exercise is not so easy. It is quite confusing with the overwhelming material to read, after reading them it is easier to figuring out what to do.
In my opinion, if there are more examples of how ioremap, ioread32, iowrite32 and bitfield macro are used can be nice, since they have vital role in this exercise and finding them online or books are not so clear and somewhat difficult.

