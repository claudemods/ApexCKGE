<div align="center">
  
## Installation Methods

</div>
simply dd to ssd/hdd 
<div align="center">

</div>

</div>
Example dd command: sudo dd if=/home/apex/Downloads/backup_20250518.img of=/dev/sdb bs=4M status=progress && sync
<div align="center">

</div>

</div>

<div align="center">
Login as root since my home files won't exist until you execute the .sh in /opt........ and reboot
</div>

<div align="center">
  
### If you plan to use from SSD/HDD:
  
</div>

<div align="center">
Navigate to /opt/btrfs-installer/drive1and2 and execute btrfsgenfstab.sh 
</div>

<div align="center">
Reboot and login as usual - my theme and stuff will be available
</div>

<div align="center"> 
You can then resize the partition of the .img you installed from a live environment
</div>

<div align="center">
  
## Default Advised Installation Method (Currently Broken)
  
</div>

<div align="center">
Will install .img on partition 1 and system on partition 3 and 4
</div>

<div align="center">
  
## Secondary Advised Method (ISO)
  
</div>

<div align="center">
I've been doing a lot of building, tinkering and testing lately - nothing has been stable
</div>

<div align="center">
The aim is to build an ISO for downloading and installing the .imgs
</div>
