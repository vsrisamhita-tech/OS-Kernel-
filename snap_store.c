/*
 * SNAP STORE - Canonical Snap Package Manager for MicroOS
 * Clean ASCII terminal UI (No Unicode / emoji glitching)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    char name[32];
    char summary[64];
    char version[16];
    char publisher[32];
    char size[16];
    bool installed;
} SnapApp;

SnapApp catalog[] = {
    {"vscode",       "Visual Studio Code - Code editing. Redefined.", "1.95.2",  "Microsoft*", "84.5 MB", false},
    {"spotify",      "Spotify - Music for everyone.",                 "1.2.48",  "Spotify*",   "62.1 MB", false},
    {"chromium",     "Chromium Web Browser - Fast and secure.",       "130.0",   "Canonical*", "98.4 MB", false},
    {"deadlock-lab", "Safe-State Deadlock Avoidance Research Lab",    "2.4.0",   "Samhita",    "1.2 MB",  true},
    {"snake-game",   "Retro Arcade Snake Game (Interactive Raw TUI)", "1.0.0",   "MicroOS",    "0.8 MB",  true},
    {"wifi-manager", "802.11 Wireless Control Center & NAT Bridge",   "2.2.0",   "MicroOS",    "0.9 MB",  true},
    {"htop",         "Interactive process viewer and system monitor", "3.3.0",   "Hisham",     "2.4 MB",  false},
    {"python3",      "Python 3.12 Dynamic Programming Environment",   "3.12.3",  "Python Org", "34.0 MB", false},
    {"git",          "Fast, scalable distributed version control",    "2.46.0",  "Git Team",   "18.2 MB", false},
    {"rustos-kernel","32-bit Bare-Metal Rust Kernel & Shell",         "0.1.0",   "Samhita",    "0.4 MB",  true}
};

int catalog_count = sizeof(catalog) / sizeof(catalog[0]);

void print_banner() {
    printf("\033[1;36m+------------------------------------------------------------------------------+\033[0m\n");
    printf("\033[1;37m|  [SNAP STORE] :: Canonical Software & Package Center v2.68.4                 |\033[0m\n");
    printf("|  Channel: stable/linux-x86 | Repo: Snapcraft Cloud & MicroOS Core Archive    |\n");
    printf("\033[1;36m+------------------------------------------------------------------------------+\033[0m\n");
}

void print_catalog() {
    print_banner();
    printf("\n\033[1;33m  [FEATURED & POPULAR APPLICATIONS IN SNAP STORE]:\033[0m\n\n");
    printf("    %-4s | %-15s | %-8s | %-10s | %-12s | %s\n", "No.", "Snap Name", "Version", "Size", "Publisher", "Status");
    printf("    -----+-----------------+----------+------------+--------------+-------------\n");
    for (int i = 0; i < catalog_count; i++) {
        char status_str[32];
        if (catalog[i].installed) {
            snprintf(status_str, sizeof(status_str), "\033[1;32m[INSTALLED]\033[0m");
        } else {
            snprintf(status_str, sizeof(status_str), "\033[1;36m[Available]\033[0m");
        }
        printf("    %2d.  | %-15s | %-8s | %-10s | %-12s | %s\n",
               i + 1, catalog[i].name, catalog[i].version, catalog[i].size,
               catalog[i].publisher, status_str);
    }
    printf("\n\033[1;36m================================================================================\033[0m\n");
    printf("  \033[1;37mCommands:\033[0m\n");
    printf("    Type \033[1;32m1 - 10\033[0m to install or launch an application\n");
    printf("    \033[1;33msnap install <name>\033[0m  - Install package from Snapcraft\n");
    printf("    \033[1;33msnap list\033[0m            - Show installed snaps\n");
    printf("    \033[1;33msnap find <query>\033[0m    - Search store for packages\n");
    printf("    \033[1;33mexit\033[0m                 - Close Snap Store\n");
    printf("\033[1;36m================================================================================\033[0m\n");
}

void install_snap(const char *name) {
    int idx = -1;
    for (int i = 0; i < catalog_count; i++) {
        if (strcmp(catalog[i].name, name) == 0) {
            idx = i;
            break;
        }
    }

    printf("\n\033[1;33m[*] Connecting to Canonical Snapcraft API for '%s'...\033[0m\n", name);
    usleep(250000);
    printf("[+] Fetching assertion & signature from keys.snapcraft.io... Verified.\n");
    usleep(200000);
    printf("[+] Downloading snap '%s' from channel 'latest/stable'...\n", name);
    
    // Smooth progress bar
    for (int p = 0; p <= 100; p += 10) {
        printf("\r    [");
        for (int b = 0; b < 20; b++) {
            if (b < p / 5) printf("=");
            else if (b == p / 5) printf(">");
            else printf(" ");
        }
        printf("] %3d%% (%s) 8.4 MB/s", p, idx >= 0 ? catalog[idx].size : "Ready");
        fflush(stdout);
        usleep(60000);
    }
    printf("\n");
    usleep(150000);
    printf("[+] Mounting squashfs snap image to /snap/%s/current...\n", name);
    usleep(150000);
    printf("[+] Generating desktop wrappers & AppArmor profiles...\n");
    usleep(150000);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "echo '#!/bin/sh\nexec aterm -title \"%s\"' > /usr/local/bin/%s && chmod 755 /usr/local/bin/%s",
             name, name, name);
    system(cmd);

    printf("\033[1;32m[SUCCESS] Snap '%s' successfully installed and ready to launch!\033[0m\n", name);
    printf("  Run command: \033[1;37m%s\033[0m\n\n", name);

    if (idx >= 0) catalog[idx].installed = true;
}

void list_snaps() {
    print_banner();
    printf("\n\033[1;33m  [INSTALLED SNAPS (Tracking stable)]:\033[0m\n\n");
    printf("    %-16s %-10s %-8s %-12s %s\n", "Name", "Version", "Rev", "Tracking", "Publisher");
    printf("    --------------------------------------------------------------\n");
    printf("    deadlock-lab     2.4.0      14      latest/stable Samhita\n");
    printf("    snake-game       1.0.0      8       latest/stable MicroOS\n");
    printf("    wifi-manager     2.2.0      21      latest/stable MicroOS\n");
    printf("    rustos-kernel    0.1.0      3       latest/stable Samhita\n");
    printf("    core22           20240402   1612    latest/stable canonical*\n");
    printf("    snapd            2.68.4     23541   latest/stable canonical*\n\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "list") == 0) {
            list_snaps();
            return 0;
        } else if (strcmp(argv[1], "install") == 0) {
            if (argc >= 3) {
                install_snap(argv[2]);
            } else {
                printf("Usage: snap install <snap-name>\n");
            }
            return 0;
        } else if (strcmp(argv[1], "find") == 0 || strcmp(argv[1], "search") == 0) {
            print_catalog();
            return 0;
        }
    }

    // Interactive Mode
    while (1) {
        print_catalog();
        printf("\033[1;36msnap-store>\033[0m ");
        char input[128];
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\r\n")] = 0;

        if (strlen(input) == 0) continue;
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
            printf("Exiting Snap Store.\n");
            break;
        }

        int num = atoi(input);
        if (num >= 1 && num <= catalog_count) {
            SnapApp *app = &catalog[num - 1];
            if (app->installed) {
                printf("\n\033[1;32m[*] Launching %s...\033[0m\n", app->name);
                char launch_cmd[128];
                snprintf(launch_cmd, sizeof(launch_cmd), "%s &", app->name);
                system(launch_cmd);
                sleep(1);
            } else {
                install_snap(app->name);
                printf("Press Enter to continue...");
                getchar();
            }
        } else if (strncmp(input, "install ", 8) == 0) {
            install_snap(input + 8);
            printf("Press Enter to continue...");
            getchar();
        } else if (strcmp(input, "list") == 0) {
            list_snaps();
            printf("Press Enter to continue...");
            getchar();
        } else {
            printf("Unknown option. Select 1-%d or type 'exit'.\n", catalog_count);
            sleep(1);
        }
    }
    return 0;
}
