
``` bash
partutil 0.17
Copyright 2014-2018 JrCs.

            Usage: partutil [QUERY OPTION] [DEVICE|UUID]
            partutil [SEARCH OPTION]
            
            Query options:
            	--show-fstype           display the filesytem type of the partition
            	--show-bsdname          display the device name of the partition
            	--show-mountpoint       display the mount point of the partition
            	--show-volumename       display the volume name of the partition
            	--show-uuid             display the UUID of the partition
            	--show-blocksize        display the prefer blocksize of the partition
            	--show-partitionscheme  display the partition scheme of a disk
            	--show-contenttype      display the content type of the device
            	--show-pbrtype          display the filesystem type from the PBR of the device
            	--show-wholedisk        display the whole disk of the device
            	--find-esp              display the associated EFI System Partition (if any)
            	--dump                  dump properties of the partition
            
            Search options:
            	--search-uuid UUID      search a partition with the specific UUID
            
            Other options:
            	-h, --help              display this message and exit
            	-V, --version           print version information and exit
            	-v, --verbose           print verbose messages
            
            example: partutil --show-fstype disk0s4
            partutil --show-bsdname 6A9017D9-2B9E-4786-B0A5-A75BD2264239
            partutil --show-blocksize disk0s4
            partutil --find-esp disk0s4
            partutil --search-uuid 2C97F84A-F488-4917-A312-5D64BAE5BCFC
```
## How to use in Clover:

copy  partutil-master/**partutil**  and replace in ~/src/edk2/Clover/CloverPackage/utils/
copy  partutil-master/**MountESP**   and replace in ~/src/edk2/Clover/CloverPackage/package/Scripts.templates/EFIFolder/


