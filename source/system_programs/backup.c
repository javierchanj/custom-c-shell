#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int main() {
    // Get BACKUP_DIR environment variable
    char *target = getenv("BACKUP_DIR");
    if (!target) {
        fprintf(stderr, "Error: BACKUP_DIR environment variable not set.\n");
        return 1;
    }

    // Get datetime stamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", t);

    // Determine filename
    char backupFile[256];
    snprintf(backupFile, sizeof(backupFile), "backup_%s.zip", timestamp);

    // Create archive directory if needed
    system("mkdir -p archive");

    // Build zip command
    char cmd[512];
    struct stat path_stat;
    stat(target, &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
        // Target is a directory
        snprintf(cmd, sizeof(cmd), "zip -r archive/%s %s > /dev/null", backupFile, target);
    } else {
        // Target is a file
        snprintf(cmd, sizeof(cmd), "zip archive/%s %s > /dev/null", backupFile, target);
    }

    // Run the command
    int result = system(cmd);
    if (result == 0) {
        printf("✅ Backup successful: archive/%s\n", backupFile);
    } else {
        fprintf(stderr, "❌ Backup failed.\n");
    }

    return 0;
}
