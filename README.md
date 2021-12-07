# Webserver Daemon Created in C for the Linux Kernel

This server allows clients over the internet/network to connect and receieve content from the host (which is the server the daemon is running on). To use this software, you will need to ensure that you put all the images within the images folder and the webpage code in the root folder. 

## Steps to Install
1. Ensure that you download the source code to a folder on a Linux platform.
2. Use gcc server.c -lpthread
3. Run the a.out file
3. Connect using localhost with port 5000

## Features

This software includes multithreading support up to four threads that can fetch and send information to the client via a thread pool. This software is able to execute and handle php requests via the php-cgi. If you wish to use php, you will need to install the php-cgi.
