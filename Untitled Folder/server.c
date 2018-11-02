#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
int main(int argc, char **argv){
    int lfd, newfd, *sizefile;
    socklen_t clilen;
    struct sockaddr_in cliaddr, serv_addr;
 
    // Tao socket
    lfd = socket(AF_INET, SOCK_STREAM, 0);
       
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9001);
   
    bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
   
    // Lang nghe ket noi tu client
    listen(lfd, 5);
   
    while (1){
        // Dong y ket noi voi client
        clilen = sizeof(cliaddr);
        newfd = accept(lfd, (struct sockaddr *) &cliaddr, &clilen);
        char *addr;
        addr = inet_ntoa(cliaddr.sin_addr);
        printf("Dia chi IP cua Client:: %s, Port: %d \n", addr, cliaddr.sin_port);
 
        char buff[1024] = {0};
        FILE *f;
        // Cap phat bo nho de luu kich thuoc file
        sizefile = (int *) calloc(1, sizeof(int));
        int check = 1;
        while (check == 1){
            char filename[40];
            *sizefile = -1;
            // Doc ten file tu client
            read(newfd, filename, 40);
            // kiem tra ki tu dac biet
            if (strcmp(filename, "QUIT") == 0){
                check = 0;
            }else{
                // Mo file
                f = fopen(filename, "rb");
                if (f == NULL){;
                    fprintf (stderr, "Khong tim thay file \"%s\".\n", filename);
                    write (newfd, sizefile, sizeof(int));
                }
           
 
                // Tinh kich thuoc file va gui cho client
                fseek(f, 0L, SEEK_END);
                *sizefile = ftell(f);
                fseek(f, SEEK_SET, 0);
                write (newfd, sizefile, sizeof(int));
           
                while (*sizefile > 0){
                    // Truyen file nho
                    if (*sizefile >= 1024){
                        fread (buff, 1024, 1, f);
                        write (newfd, buff, 1024);
                    }
                    else{
                        // Truyen file to
                        fread (buff, *sizefile, 1, f);
                        write (newfd, buff, *sizefile);
                    }
                    *sizefile = *sizefile - 1024;  
                }
                printf ("Da gui file!\n");
                fclose(f);
            }
        }  
        free(sizefile);
        // Ngat ket noi voi client
        close(newfd);
        printf ("Ket thuc!\n");
    }
}