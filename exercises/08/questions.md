# TIE-50307 - Exercise 08 - Q&A

## 1. What is `uname -a` output on the PYNQ board before starting Ex 8?

Linux pynq 4.9.0-xilinx-v2017.3 #1 SMP PREEMPT Thu Aug 9 16:33:05 UTC 2018 armv7l GNU/Linux

## 2. What is the average latency and worst case latency you measure after Ex 7 using the application you developed?

average latency of 230.46
worst latency was 4074.

## 3. What is `uname -a` output on the PYNQ board after rebooting with the new kernel image?

Linux pynq 4.9.0-rt1-xilinx-v2017.3 #1 SMP PREEMPT RT Wed Dec 1 16:54:20 UTC 2021 armv7l GNU/Linux

## 4. What is the average latency and worst case latency you measure in the new kernel image (same procedure as question 2)?

average latency of 1035.51
worst latency was 7571

## 5. Compare the results of questions 2 and 4; do they differ significantly? why?

The result of Non-RT kernel had better average latency and the worst case scenario.

This happened because the spinlock in Non-RT kernel is transformed into mutex in RT kernel, that can sleep and interruptable. Also running interrupt handler in a kernel thread causes scheduling overhead. 

## 6. Compare the plots for the 4 different profiles. Describe each of them and compare them?

According to the plot, the result matches the descriptions in question 5. Also in RT-kernel, the latency spike is less than the Non-RT kernel.

## 7. Document the RT performance differences and their reasons

The PDF file contains the plots and statistics is located in this folder, as "latency.pdf"

## 8. Is there any noticeable difference between the two images when saturating the system with IRQ?

Only the 1st IRQ line 0 works due to 0 is the default parameter of the handling. Other lines do not work. 

## 9. Using the information in [scripts/README.txt](scripts/README.txt), try to tune the RT-system to overcome the limits described in the previous question

The plots do not have sudden spikes like non-tuing version.

## 10. What is the goal of this exercise? What did you accomplish?

At this exercise we used the irqgen-mod module to compare the performance of non-RT and RT linux kernel.

## 11. Feedback (what was difficult? what was easy? how would you improve it?)

This exercise is rather easier than the previous one.

