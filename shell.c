#include "user.h"

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

        if (strcmp(cmdline, "hello") == 0)
            printf("Hello world from shell!\n");
        else if (strcmp(cmdline, "exit") == 0)
            exit();
        if (strncmp(cmdline, "read", 4) == 0)
        {
            char *filename = cmdline + 5;
            char buf[128];
            int len = readfile(filename, buf, sizeof(buf));
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        // write filename content
        else if (strncmp(cmdline, "write", 5) == 0)
        {
            char *filename = cmdline + 6;
            char *pos = strchr(filename, ' ');
            if (pos == NULL)
            {
                printf("invalid command: %s\n", cmdline);
                continue;
            }
            char *content = pos + 1;
            writefile(filename, content, strlen(content));
        }
        else
            printf("unknown command: %s\n", cmdline);
    }
}