#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <errno.h>
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[5242880] = {0};
    char ip_add[20]; 
    
    printf("Nhap dia chi ip server:\n");
    gets(ip_add);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\nFailed\n"); 
        exit(1); 
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    if(inet_pton(AF_INET, ip_add, &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        exit(1); 
    } 

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Fails \n"); 
        exit(1); 
    } 
    else{
        printf("\nConnection sucees\n"); 
    }

    while(1){
        char file_name[40];
        printf("\nNhap ten file: ");
        scanf("%s",&file_name); 
        send(sock , file_name , strlen(file_name) , 0 );
        if(strcmp(file_name,"QUIT") == 0){
            return 0; 
        }
        char* fr_name =file_name;
        FILE *fr = fopen(fr_name, "a");
        if(fr == NULL)
            printf("Khong tim thay file:\n", fr_name);
        else
        {
            bzero(buffer, 5242880); 
            int fr_block_sz = 0;
            while((fr_block_sz = recv(sock, buffer, 5242880, 0)) > 0)
            {
                int write_sz = fwrite(buffer, sizeof(char), fr_block_sz, fr);
                if(write_sz < fr_block_sz)
                {
                    error("File write failed.\n");
                }
                bzero(buffer, 5242880);
                if (fr_block_sz == 0 || fr_block_sz != 512) 
                {
                    break;
                }
            }
            if(fr_block_sz < 0)
            {
                printf("Xay ra loi\n");
            }
            printf("Nhan thanh cong!\n");
           
            fclose(fr);
        }
        } 

    close(sock);
    return 0; 
} 

