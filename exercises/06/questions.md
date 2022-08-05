# TIE-50307 - Exercise 06 - Q&A

## 1. What is the goal of this exercise? What did you accomplish?

In this exercise I have learned about platform device, its driver and resources management. Also modifying the original code of irqgen IP block to implement platform device driver as FPGA IP block that can be setup to generate interrupt requests (IRQs), apply the info of device tree to support other IRQ lines.

## 2. What is a platform device? What makes such a device different from, e.g., a USB device or a PCI device?

Platform devices are devices appear as autonomous entities in the system, including port-based device, host bridges to peripheral buses, controllers in SoC platforms. Devices such as USB or PCI device are plug-and-play devices, they are automatically detected when plugged in, while platform device is not hotplug, the kernel need to be told which platform device is going to connect with.

## 3. What is the relation between the device-tree and platform device drivers?

Platform driver and device-tree are bounded together using match OF. This method using compatible field of of_match_table inside struct platform_driver. Each device node on the DT also has a device tree property, which can be a string or a list of strings. Any platform driver that declares its compatible field as a device compatible property on the DT will have a match.

## 4. Why did we bother to rewrite the LKM code to implement a platform device driver?

In order to implement a platform device driver require:

Register a platform driver (with the driver name being unique in the system) to manage the device. 
Register the platform device with the same driver's name so the kernel knows its device is being used. 

Drive is registered using a struct with name, owner, probe function each time a kernel module is added and the device receives the corresponding driver, remove function to remove the driver when not needed ,of_match_table to match with compatible field in the device tree. Moreover, using devres to efficiency manage resources, while the old implementation might causes bug in probe/remove function, which need to free everything mid way, and resources leakage.


## 5. Imagine you are developing a product on the PYNQ-Z1 board involving several IP blocks of your own design to squeeze all the resources and cabalities of the SoC FPGA; most IP blocks are accessible from the PS through the AMBA/AXI bus. Do you need to alter the device-tree we are using? What's the minimum set of information that you would require for each addressable IP block?

Generally we do not have to change anything from the device tree. Most of the devices/IPs inside AMBA/AXI bus are interrupt generate-based devices, more interrupt sources can be added into the device tree of these devices in interrupt field.
At least each IP block must have of_match_table in order to read the data from the device tree. In addition the IP block should have the address and the length of the address.


## 6. What capabilities are exposed through the current sysfs interface of the `irqgen`? Describe the available entry points inside `/sys/kernel/irqgen` and their functions.

line: IRQ line currently using.
delay: amount of delay time of each IRQ
total_handled: total number of IRQs handled since loading the kernel.
latency: latency is derived by calculating the positive clock edges between an interrupt line assertion and IRQ handling by user
count_register: total number of IRQs overall since booting the system (since the very first kernel call).
line_count: total number of IRQ line available.
intr_ids: id corresponding to an IRQ line.
intr_idx: index of the IRQ line from 0-15
intr_acks: nterrupt acknowledgment values needed for interrupt clearing from the device
intr_handled: number of IRQ handled in each IRQ lines.


## 7. Are there code sections that are missing concurrency barriers? Where? Can you think of a way of triggering unintended behaviour?


## 8. Feedback (what was difficult? what was easy? how would you improve it?)

This exercise is rather difficult in both understanding the concepts and implementation.

