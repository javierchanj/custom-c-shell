#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>

// For memory info
#ifdef __linux__
#include <sys/sysinfo.h>
#endif

int main() {
    struct utsname uname_data;
    uname(&uname_data);

    char *user = getenv("USER");
    if (!user) {
        struct passwd *pw = getpwuid(getuid());
        user = pw->pw_name;
    }

    printf("Simple System Information\n");
    printf("OS: %s\n", uname_data.sysname);
    printf("Kernel: %s\n", uname_data.release);
    printf("Hostname: %s\n", uname_data.nodename);
    printf("User: %s\n", user);

#ifdef __linux__
    struct sysinfo mem;
    sysinfo(&mem);
    printf("MemTotal: %lu KB\n", mem.totalram / 1024);
#else
    printf("Memory info not available on this OS\n");
#endif

    // Optional: Add CPU info on Linux/macOS using /proc/cpuinfo or sysctl
    // Or just include a static placeholder for simplicity
    printf("CPU: Apple M2 Max (or check /proc/cpuinfo)\n");

    return 0;
}

