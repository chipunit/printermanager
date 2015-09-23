//
//  Server.c
//  Printer Manager
//
//  Created by Developer on 23/09/15.
//  Copyright (c) 2015 Chip Unit. All rights reserved.
//

#include "Server.h"

int print(const char *buffer)
{
    int ret;
    libusb_device_handle *xprinter;
    
    if (libusb_init(NULL))
        return 0;
    
    if ((xprinter = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID)) == NULL)
    {
        libusb_exit(NULL);
        return 0;
    }
    
    if ((ret = libusb_kernel_driver_active(xprinter, 0)))
    {
        if ((ret = libusb_detach_kernel_driver(xprinter, 0)))
        {
            return 0;
        }
    }
    
    if ((ret = libusb_claim_interface(xprinter, INTERFACE_NUM)))
    {
        libusb_exit(NULL);
        return 0;
    }
    
    int received;
    ret = libusb_bulk_transfer(xprinter, ENDPOINT_A, (unsigned char *) buffer, (int) strlen(buffer), &received, 800);
    ret = libusb_release_interface(xprinter, INTERFACE_NUM);
    
    libusb_close(xprinter);
    xprinter = NULL;
    libusb_exit(NULL);
    
    return 1;
}

void processing(int sock)
{
    unsigned char res = XPMRES_OK;
    int packet[3];
    int size = (int) recv(sock, &packet[0], sizeof(int) * 3, 0);
    
    if (size != sizeof(int) * 3)
    {
        res = XPMRES_ERR_BUF;
        send(sock, &res, 1, 0);
        return;
    }
    
    int dsize = packet[1] * packet[2];
    
    if (dsize < 0 || dsize > BUFFER_SIZE)
        return;
    
    char *buffer = (char *) malloc (dsize);
    
    if (!buffer)
        return;
    
    size = (int) recv(sock, buffer, dsize, 0);
    
    while (size < dsize)
    {
        size += (int) recv(sock, &buffer[size], dsize - size, 0);
    }
    
    float fW = packet[1] / 8.0f;
    float fH = packet[2] / 8.0f;
    
    char *outbuf = (char *) malloc (BUFFER_SIZE);
    
    if (!outbuf)
    {
        free(buffer);
        return;
    }
    
    sprintf(outbuf, "SIZE %f mm, %f mm\r\nDIRECTION 0,0\r\nREFERENCE 0,0\r\nOFFSET 0 mm\r\nSET PEEL OFF\r\nSET TEAR ON\r\nCLS\r\n", fW, fH);
    
    for (int y = 0; y < packet[2]; y++)
    {
        int start = -1;
        
        for (int x = 0; x < packet[1]; x++)
        {
            int pos = x + y * packet[1];
            
            if (start < 0 && buffer[pos] == 1)
                start = x;
            
            if (start > 0 && buffer[pos] == 0)
            {
                sprintf(&outbuf[strlen(outbuf)], "BAR %d, %d, %d, 1\r\n", start, y, x - start - 1);
                start = -1;
            }
        }
        
        if (start > 0)
            sprintf(&outbuf[strlen(outbuf)], "BAR %d, %d, %d, 1\r\n", start, y, packet[1] - start - 1);
    }
    
    strcat(outbuf, "PRINT 1,1\r\n");
    print(outbuf);
    
    free(outbuf);
    free(buffer);
}

void *start_server(void *arg)
{
    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
        return 0;
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERVER_PORT);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        return 0;
        
    listen(sockfd,5);
    
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if (newsockfd < 0)
            break;
        
        processing(newsockfd);
        close(newsockfd);
    }

    close(sockfd);
    
    return 0;
}