/**
 * ttyEcho - Send a command from a console to another one
 *
 * @author  Andrea Benini (andreabenini@gmail.com)
 * @history v1.0.1  (2016/11)
 *              Trimmed out trailing space on sent command, minor code cleanup
 *          v1.0.0  (2016/10)
 *              First working version
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>


/**
 * PRINT HELP - Print out program usage
 * @param prog_name (char*) Program name
 */
void print_help(char *prog_name) {
    printf("Usage: %s [-n] <DeviceName> <Command>\n", basename(prog_name));
    printf("    -n    Optional argument to send a new line at the end of the Command\n\n");
    printf("Note:     This program needs 'sudo' to run if the executable is not setuid root\n");
    printf("\n");
    exit(1);
}


/**
 * MAIN - Main program
 * @param  argc (int) Number of arguments passed
 * @param  argv (char *[]) Array of arguments
 *
 * @return (int) 0 when OK, !=0 otherwise
 */
int main(int argc, char *argv[]) {
    char *cmd;
    int i, fd;
    int devno, commandno, newline;
    int mem_len;
    devno = 1; commandno = 2; newline = 0;
    if (argc < 3) {
        print_help(argv[0]);
    }
    if (argc > 3 && argv[1][0] == '-' && argv[1][1] == 'n') {
        devno = 2;
        commandno = 3;
        newline=1;
    } else if (argc > 3 && argv[1][0] == '-' && argv[1][1] != 'n') {
        printf("Invalid Option\n");
        print_help(argv[0]);
    }
    fd = open(argv[devno],O_RDWR);
    if(fd == -1) {
        perror("Cannot open specified device");
        exit(1);
    }
    mem_len = 0;
    for (i = commandno; i < argc; i++) {
        mem_len += strlen(argv[i]) + 2;
        if ( i > commandno ) {
            cmd = (char *)realloc((void *)cmd, mem_len);
        } else {
            cmd = (char *)malloc(mem_len);
        }
        strcat(cmd, argv[i]);
    }
    if (newline == 0) {
        usleep(200000);
    }
    for (i = 0; cmd[i]; i++) {
        ioctl(fd, TIOCSTI, cmd+i);
    }
    if (newline == 1) {
        ioctl(fd, TIOCSTI, "\n");
    }
    close(fd);
    free((void *)cmd);
    exit(0);
} /**/

