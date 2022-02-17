![logo](logo.jpg)

---

# Prerequisites

- PC or Laptop
- Xilinx Vivado 2019.1.3 Software
- HDK with Power Supply
- USB micro B Cable

# Partitioning the eMMC 

To partion the eMMC on a new unit, follow these steps:

1. Place the SOM on the HDK ensuring both board-to-board connectors are fully seated.

2. Connect the PC to the micro B USB port of the HDK.

3. Open Vivado.

4. Under "Tasks", click `"Open Hardware Manager"`.

![eMMC_01](eMMC_01.PNG)

5. Power up the HDK.
- Note: You do not need to have an SD card installed.

6. Near the top of the Hardware Manager is a notification stating "No hardware target is open."  Click the `"Open target"` link next to the notification, then click `"Auto Connect"`.

![eMMC_02](eMMC_02.PNG)

7. You should now see a device connected named "xczu3_0".

![eMMC_03](eMMC_03.PNG)

8. In the "Hardware" box, right-click on `"xczu3_0"`, then click `"Add Configuration Memory Device"`.

![eMMC_04](eMMC_04.PNG)

9. In the pop-up window, change the "Type" dropdown menu to `"emmc"`.

10. Near the bottom under "Select Configuration Memory Part", select `"jedec4.51-32gb-emmc"`, then click `"OK"`.

![eMMC_05](eMMC_05.PNG)

11. A confirmation window will appear.  Click `"OK"`.

![eMMC_06](eMMC_06.PNG)

12. Another pop-up window will appear.  Click the `"+"` button under "Files to load".

![eMMC_07](eMMC_07.PNG)

13. In the file selection pop-up, navigate to "C:\BytePipe\eMMC Programming" (the program should remember this location) and select `"README.html"`, then click `"OK"`.

![eMMC_07a](eMMC_07a.PNG)

14. Then click the three dots `"..."` to the right of "Zynq FSBL:".

![eMMC_07b](eMMC_07b.PNG)

15. Select the `"fsbl.elf"` file, then click `"OK"`.

![eMMC_07c](eMMC_07c.PNG)

16. Now click `"OK"` 

![eMMC_07d](eMMC_07d.PNG)

17. The device will now be programmed.  This step will take a few minutes.

![eMMC_08](eMMC_08.PNG)

18. You may get a warning message stating the current boot mode is SD1.  This is ok, click `"OK"`.

19. Other warnings may also appear and are likely not an issue.  Click `"OK"` for those as well.  The device will be tested at the end.

![eMMC_09](eMMC_09.PNG)

20. Once complete, you should see the "jedec4.51-32gb-emmc" device in the hardware manager.  If you do, continue to the next step.  If you do not, the process failed - complete the next step, check all of your connections, and repeat the process.

![eMMC_10](eMMC_10.PNG)

21. Disconnect from the device by right-clicking on "xilinx_tcf..." and selecting `"Close Target"`.

![eMMC_11](eMMC_11.PNG)

22.  The eMMC is now programmed and can be run through the self-test process, then disconnected.  If you are programming another eMMC, you can leave the Hardware Manager open, plug in the new unit, power it on, and skip back to step 6.  Some settings will be retained until the Vivado is closed.

