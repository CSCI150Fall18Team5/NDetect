## Welcome to NDetect!

### After loading the project change the NDetect.vcxproj to run in your machine
These are the quick steps:
Go to project > NDetect properties and add the following configurations
<ol>
  <li>C++ > General >Additional Include Directories: D:\WinPcap\WpdPack\Include (your local file)</li>
  <li>Linker-> General> Additional Library Directories D:\WinPcap\WpdPack\Lib (your local file)</li>
  <li>Linker > Input > Additional Dependencies: wpcap.lib Packet.lib ws2_32.lib </li>
  <li>C++ > Preprocessor: HAVE_REMOTE WIN32 WPCAP</li>
</ol>

To find more information go to [Our Wiki](https://github.com/CSCI150Fall18Team5/NDetect/wiki)
