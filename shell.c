#include "user.h"

void help(void)
{
    printf("These shell commands are defined internally.  Type `help' to see this list.\n");
    printf("\n");
    printf("hello\n");
    printf("help\n");
    printf("exit\n");
    printf("shutdown\n");
    printf("read <filename>\n");
    printf("write <filename> <content>\n");
    printf("\n");
}

void main(void)
{
    while (1)
    {
    prompt:
        printf("> ");
        char cmdline[128];
        for (int i = 0;; i++)
        {
            char ch = getchar();
            putchar(ch);
            if (i == sizeof(cmdline) - 1)
            {
                printf("command line too long\n");
                goto prompt;
            }
            else if (ch == '\r')
            {
                printf("\n");
                cmdline[i] = '\0';
                break;
            }
            else
            {
                cmdline[i] = ch;
            }
        }

        if (strcmp(cmdline, "help") == 0)
            help();
        else if (strcmp(cmdline, "hello") == 0)
            printf("Hello world from shell!\n");
        else if (strcmp(cmdline, "exit") == 0)
            exit();
        else if (strcmp(cmdline, "shutdown") == 0)
            shutdown();
        else if (strncmp(cmdline, "read", 4) == 0)
        { // read filename
            char *filename = cmdline + 5;
            char buf[128];
            int len = readfile(filename, buf, sizeof(buf));
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        else if (strncmp(cmdline, "write", 5) == 0)
        { // write filename content
            char *filename = cmdline + 6;
            char *pos = strchr(filename, ' ');
            if (pos == NULL)
            {
                printf("invalid command: %s\n", cmdline);
                help();
                continue;
            }
            char *content = pos + 1;
            writefile(filename, content, strlen(content));
        }
        else
            printf("unknown command: %s\n", cmdline);
    }
}
