#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>

void write_error(char *arr);
void handle_connection(int);

char page[] =

    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html>\r\n"
    "<html><head><title>Test</title>\r\n"
    "<style>body { background-color: #FFFF00 }</style></head>\r\n"
    "<body><center><h1>Hello World!</h1><br></center></body></html>\r\n";

int main(){
    struct sockaddr_in server_addr, client_addr;
    socklen_t length = sizeof(client_addr);
    int pid;
    int server_fd, client_fd;
    int port = 5000;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
	write_error("Could not create the socket");
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*) &server_addr,
	    sizeof(server_addr)) == -1){
	close(server_fd);
	write_error("Cannot bind");
    }

    
    if(listen(server_fd, 10) == -1){
	close(server_fd);
	write_error("cannot listen");
    }

    while(1){
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &length);

	if(client_fd == -1){
	    perror("Connection refused");
	}
	
	pid = fork();

	if(pid < 0){
	    write_error("Cannot fork");
	}

	if(pid == 0){
	    close(server_fd);
	    handle_connection(client_fd);
	    exit(0);
	}

	close(client_fd);
    }

    return 0;
}


void write_error(char *arr){
    perror(arr);
    exit(1);
}

void handle_connection(int client_fd){
    char buffer[5000];

    read(client_fd, buffer, 4999);

    printf("%s\n", buffer);
//    printf("Client connected\n");

    write(client_fd, page, sizeof(page) -1);
}
