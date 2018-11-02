#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
 
int main(int argc, char **argv){
    int sockfd, *sizefile;
    struct sockaddr_in serv_addr;  
 
    // Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero (&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9001);
 
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
 
    // ket noi toi server
    connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
   
    char buff[1024] = {0};
    FILE *f;
    // Cap phat bo nho de doc kich thuoc file
    sizefile = (int *) calloc(1, sizeof(int));
    int check = 1;
    while (check ==1){
        char filename[40] = {0};
        fflush (stdin);
        printf ("Nhap ten file can download, bam QUIT de ket thuc: ");
        fgets (filename, 40, stdin);
        filename[strcspn(filename, "\n")]=0;
 
        // Gui ten file len server
        write (sockfd, filename, sizeof(filename));
        // Kiem tra ki tu dac biet
        if (strcmp (filename, "QUIT") == 0){
            check = 0; 
        }else{
            // Doc kich thuoc file duoc gui tu server
            read(sockfd, sizefile, sizeof(int));
            // Doc va luu file
            f = fopen (filename, "wb");
            while (*sizefile > 0){
                // Download file nho
                if (*sizefile >= 1024){
                    read (sockfd, buff, 1024);
                    fwrite(buff, 1024, 1, f);
                }
                else{
                    // Download file to
                    read (sockfd, buff, *sizefile);
                    fwrite(buff, *sizefile, 1, f);
                }
                *sizefile = *sizefile - 1024;
            }
            printf("Da nhan file.\n");
            fclose(f);
        }
    }
    free(sizefile);
    close (sockfd);
    exit(0);
}