//
//  Server.h
//  Printer Manager
//
//  Created by Developer on 23/09/15.
//  Copyright (c) 2015 Chip Unit. All rights reserved.
//

#ifndef __PRINTER_MANAGER_SERVER_H
#define __PRINTER_MANAGER_SERVER_H

#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT         9160
#define BUFFER_SIZE         1024 * 1024 * 3

#define PRODUCT_ID          0x2016
#define VENDOR_ID           0x1fc9
#define INTERFACE_NUM       0
#define ENDPOINT_A          0x01
#define ENDPOINT_B          0x81
#define TIME_OUT            600

#define XMP_SIGN            0xFFBA88

#define XPMRES_OK           0x1
#define XPMRES_OUT_BUF      0x2
#define XPMRES_ERR_BUF      0x3
#define XPMRES_ERR_SIGN     0x4

void *start_server(void *arg);

#endif /* __PRINTER_MANAGER_SERVER_H */
