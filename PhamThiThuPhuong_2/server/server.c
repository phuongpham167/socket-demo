#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>
#define PORT 8080

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[5242880] = {0}; 
    bool is_conn = false;
       
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
         
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    while(1)
    {
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        }    
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        if(!fork()){
            int nBytes = 1;
            while(nBytes!=0){
                is_conn =true;
                struct sockaddr_in* sock_client = (struct sockaddr_in*)&address;
                while(is_conn){
                    valread = read( new_socket , buffer, 5242880);
                    buffer[valread]='\0'; 
                    fflush(stdout);
                    if(strcmp(buffer,"QUIT") == 0){
                        is_conn = false;
                        printf("\nNgat ket noi\n");
                        exit(1);
                    }
                    char* fs_name =buffer;
                    FILE *fs = fopen(fs_name, "r");
                    if(fs == NULL)
                    {
                        fprintf(stderr, "Khong tim thay file: %s.\n", fs_name);
                    }
                    bzero(buffer, 5242880); 
                    int fs_block_sz;
                    while((fs_block_sz = fread(buffer, sizeof(char), 5242880, fs))>0)
                        {
                            if(send(new_socket, buffer, fs_block_sz, 0) < 0)
                            {
                                fprintf(stderr, "Gui file loi: %s.\n", fs_name);
                            }
                            bzero(buffer, 5242880);
                        }
                        printf("Gui file thanh cong!\n");
                    }
                    close(new_socket);
                    exit(0);
                }
        }
        else{
            close(new_socket);
        }
    }
    return 0; 
} 
