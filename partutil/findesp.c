//
//  findesp.c
//  partutil
//
//  Created by Micky1979 on 04/02/18 (https://github.com/Micky1979/findesp).
//  Copyright © 2018 Micky1979. All rights reserved.
//

#include "partutil.h"

/*
 find the ESP associated with the given disk object:
 the "EFI System Partition" that belong with the given disk (slice or not) must have
 the same path in the IO Registry returned by kDADiskDescriptionBusPathKey.
 The scenario is simple now, you can now found the ESP associated with the given disk no matter the filesystem and
 even on a non GUID partition scheme (yes, both GPT and MBR partitioned disks can contain an EFI system partition,
 as UEFI firmware is required to support both partitioning schemes).
 */

void getESPFor(char const* pathName) {
  CFDictionaryRef bsdNameDesc = NULL;
  DADiskRef disk;
  assert(pathName);
  
  char device[ MAXPATHLEN ];
  strlcpy(device, toBSDName(pathName), sizeof(device));
  
  if (getDASessionAndDisk(device, NULL, &disk) == 0) {
    bsdNameDesc = DADiskCopyDescription(disk);
    CFRelease(disk);
  }
  
  if (bsdNameDesc) {
    CFStringRef busPath = CFDictionaryGetValue(bsdNameDesc, kDADiskDescriptionBusPathKey);
    if (busPath) {
      CFMutableDictionaryRef matching = IOServiceMatching(kIOMediaClass);
      io_iterator_t existing;
      if (IOServiceGetMatchingServices(kIOMasterPortDefault,
                                       matching,
                                       &existing) == kIOReturnSuccess) {
        io_registry_entry_t serviceObject;
        while ((serviceObject = IOIteratorNext(existing))) {
          CFMutableDictionaryRef properties = NULL;
          kern_return_t kres = IORegistryEntryCreateCFProperties(serviceObject,
                                                                 &properties,
                                                                 kCFAllocatorDefault,
                                                                 0);
          if (kres != KERN_SUCCESS) {
            IOObjectRelease(serviceObject);
            if (properties) {
              CFRelease(properties);
            }
            break;
          }
          
          if (properties != NULL) {
            CFStringRef v = CFDictionaryGetValue(properties, CFSTR(kIOBSDNameKey));
            // I don't expect "v" to be not present.. let it crash otherwise, and file a (big) bug at Apple
            
            CFIndex length = CFStringGetLength(v);
            CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
            char *esp = (char *)malloc(maxSize);
            if (CFStringGetCString(v, esp, maxSize,
                                   kCFStringEncodingUTF8)) {
              CFDictionaryRef desc = NULL;
              DADiskRef iodisk;
              if (getDASessionAndDisk(esp, NULL, &iodisk) == 0) {
                desc = DADiskCopyDescription(iodisk);
              }
              
              if (desc != NULL) {
                CFStringRef mediaName = CFDictionaryGetValue(desc, kDADiskDescriptionMediaNameKey);
                CFStringRef ioBusPath = CFDictionaryGetValue(desc, kDADiskDescriptionBusPathKey);
                if (mediaName &&
                    ioBusPath &&
                    CFEqual(mediaName, CFSTR("EFI System Partition")) &&
                    CFEqual(busPath, ioBusPath)) {
                  printf("%s\n", esp);
                  CFRelease(iodisk);
                  free(esp);
                  CFRelease(desc);
                  IOObjectRelease(serviceObject);
                  break;
                }
                CFRelease(desc);
              }
            }
            free(esp);
          }
          IOObjectRelease(serviceObject);
        }
      }
    }
    CFRelease(bsdNameDesc);
  }
}
