#include <stdio.h>
#include <stdlib.h>

int main() {
    // Shell command to count non-terminal 'dspawn' processes
    const char *cmd = "ps -efj | grep dspawn | grep -Ev 'tty|pts' | grep -v grep | wc -l";

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("Failed to run command");
        return 1;
    }

    int count = 0;
    fscanf(fp, "%d", &count);
    pclose(fp);

    printf("Live dspawn daemon processes: %d\n", count);
    return 0;
}

