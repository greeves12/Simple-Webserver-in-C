#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fcntl.h>

#define MAX_ARRAY 15000

void write_error(char *arr);
void handle_connection(int);
int get_page(char[], char[]);
void clear_buffer(char[], int);
int m_strcmp(char[], char[]);
void m_strcat(char [], char[], char[]);
void shift_array(char [], int);
void add_carriage(char[]);


int main(){
    struct sockaddr_in server_addr, client_addr;
    socklen_t length = sizeof(client_addr);
    int pid;
    int server_fd, client_fd;
    int port = 5000;
    int on = 1;
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
	write_error("Could not create the socket");
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    
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
    char page_buffer[MAX_ARRAY];
    int filefd, filesize;
    
    int i = 0;
    int x = 0;
    read(client_fd, buffer, 4999);

    printf("Client connected\n");

    get_page(buffer, page_buffer);

    if(!m_strcmp(page_buffer, " ")){
	m_strcat("root/", page_buffer, page_buffer);

	filefd = open(page_buffer, O_RDONLY);
	close(filefd);
	
	if(filefd < 0){
	    clear_buffer(page_buffer, MAX_ARRAY);
	    m_strcat("root/", "mant.html", page_buffer);

	    filefd = open(page_buffer, O_RDONLY);
	    

	    if(filefd > 0){
		clear_buffer(page_buffer, MAX_ARRAY);
		filesize = read(filefd, page_buffer, MAX_ARRAY -1); 

		if(filesize > 0){
		    page_buffer[filesize - 1] = '\0';
		}
	    }
	    close(filefd);
	}else{
	    

	   
	}
    }else if(m_strcmp(page_buffer, " ")){
        m_strcat("root/", "index.html", page_buffer);

	filefd = open(page_buffer, O_RDONLY);
	
	if(filefd < 0){
	    
	}else{
	    clear_buffer(page_buffer, MAX_ARRAY);
	    filesize = read(filefd, page_buffer, MAX_ARRAY - 1);

	    close(filefd);

	    if(filesize > 0){
		page_buffer[filesize - 1] = '\0';
	    }
	}
    }
    
    m_strcat("HTTP/1.1 200 OK\r\n"
	     "Content-Type: text/html; charset=UTF-8\r\n\r\n",
	     page_buffer, page_buffer);
    
    write(client_fd, page_buffer, sizeof(page_buffer) - 1);
}

/*
Purpose: to locate and send back the page that the client requested from the
         server. 


Example: assume the client requested the page called home.html. The function
         will strip the home.html from the html protocol request and store it
         in the page array.
*/

int get_page(char client[], char page[]){
    int x;
    int i = 0;
    for(x = 5; client[x] != ' '; x++){
	page[i] = client[x];
	i++;
    }
    page[i+1] = '\0';
}

void clear_buffer(char buf[], int size){
    int x;

    for(x = 0; x < size; x++){
	buf[x] = '\0';
    }
}

int m_strcmp(char arr1[], char arr2[]){
    int index = 0;
    
    while(arr1[index] != '\0' && arr2[index] != '\0'){
	if(arr1[index] != arr2[index]){
	    return 0;
	}
    }

    return 1;
}


void m_strcat(char arr1[], char arr2[], char copied[]){
    char temp[MAX_ARRAY];
    int x = 0;
    int i = 0;
    
    while(arr1[x] != '\0'){
	temp[x] = arr1[x];
	x++;
    }

    while(arr2[i] != '\0'){
	temp[x] = arr2[i];
	x++;
	i++;
    }

    temp[x] = '\0';
    
    x = 0;

    while(temp[x] != '\0'){
	copied[x] = temp[x];
	x++;
    }
    copied[x] = '\0';
}

void add_carriage(char arr[]){
    int i = 0;

    while(arr[i] != '\0'){
	if(arr[i] == '\n'){
	    shift_array(arr, i);
	    arr[i] = '\r';
	    i++;
	}
	i++;
    }
}

void shift_array(char arr[], int index){
    int i;
    int x;

    while(arr[x] != '\0'){
	x++;
    }
    x = x+1;
    
    for(i = x; i > index; i--){
	arr[i] = arr[i - 1];
    }
}
