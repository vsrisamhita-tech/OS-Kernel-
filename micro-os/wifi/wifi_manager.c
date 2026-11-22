/*
 * CYBERLAB OS - MODULAR WI-FI SYSTEM
 * wifi_manager.c - Core Network Management Orchestrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char ssid[64];
    int signal;
    bool secured;
    bool in_use;
} WifiNetwork;

bool detect_virtualbox_environment() {
    FILE *fp = popen("systemd-detect-virt 2>/dev/null || cat /sys/class/dmi/id/product_name 2>/dev/null", "r");
    if (!fp) return false;
    char buf[128] = {0};
    if (fgets(buf, sizeof(buf), fp)) {
        pclose(fp);
        if (strstr(buf, "oracle") || strstr(buf, "VirtualBox") || strstr(buf, "vbox")) {
            return true;
        }
    } else {
        pclose(fp);
    }
    // Also check if eth0 is default route
    FILE *fp2 = popen("ip route | grep default | grep eth0 2>/dev/null", "r");
    if (fp2) {
        if (fgets(buf, sizeof(buf), fp2)) {
            pclose(fp2);
            return true;
        }
        pclose(fp2);
    }
    return false;
}

bool is_wifi_hardware_present() {
    FILE *fp = popen("nmcli -t -f TYPE device 2>/dev/null | grep -i wifi", "r");
    if (!fp) return false;
    char buf[32];
    bool found = (fgets(buf, sizeof(buf), fp) != NULL);
    pclose(fp);
    return found;
}

int scan_wifi_networks(WifiNetwork networks[], int max_nets) {
    if (!is_wifi_hardware_present()) return 0;
    // Trigger rescan
    system("nmcli device wifi rescan 2>/dev/null");
    FILE *fp = popen("nmcli -t -f SSID,SIGNAL,SECURITY,IN-USE device wifi list 2>/dev/null", "r");
    if (!fp) return 0;
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), fp) && count < max_nets) {
        // Strip newline
        line[strcspn(line, "\r\n")] = 0;
        char *token = strtok(line, ":");
        if (!token || strlen(token) == 0) continue;
        strncpy(networks[count].ssid, token, 63);

        token = strtok(NULL, ":");
        networks[count].signal = token ? atoi(token) : 50;

        token = strtok(NULL, ":");
        networks[count].secured = token && (strlen(token) > 0) && (strcmp(token, "--") != 0);

        token = strtok(NULL, ":");
        networks[count].in_use = token && (strchr(token, '*') != NULL);

        count++;
    }
    pclose(fp);
    return count;
}

bool connect_wifi(const char *ssid, const char *password) {
    char cmd[512];
    if (password && strlen(password) > 0) {
        snprintf(cmd, sizeof(cmd), "nmcli device wifi connect \"%s\" password \"%s\" 2>/dev/null", ssid, password);
    } else {
        snprintf(cmd, sizeof(cmd), "nmcli device wifi connect \"%s\" 2>/dev/null", ssid);
    }
    return (system(cmd) == 0);
}

void print_wifi_dashboard() {
    printf("\n========================================================\n");
    printf("         CYBERLAB OS - WI-FI QUICK SETTINGS            \n");
    printf("========================================================\n");

    bool in_vbox = detect_virtualbox_environment();
    bool has_wifi = is_wifi_hardware_present();

    if (in_vbox && !has_wifi) {
        printf("\e[1;32m[CONNECTED]\e[0m VirtualBox NAT Ethernet Adapter (eth0)\n");
        printf("Status: Internet Online via Virtual Machine Host Adapter\n\n");
        printf("\e[1;33m[HARDWARE DIAGNOSTIC NOTICE]\e[0m\n");
        printf("Physical Wi-Fi hardware not detected.\n");
        printf("VirtualBox provides a virtualized Ethernet adapter to guest OSes.\n");
        printf("To scan physical wireless SSIDs:\n");
        printf("  1. Boot CYBERLAB MicroOS directly on a physical PC/Laptop, OR\n");
        printf("  2. Attach a USB Wi-Fi adapter via VirtualBox Devices -> USB Menu.\n");
        printf("========================================================\n\n");
        return;
    }

    if (!has_wifi) {
        printf("\e[1;31m[STATUS]\e[0m No wireless network device detected.\n");
        printf("========================================================\n\n");
        return;
    }

    printf("\e[1;36mScanning nearby Wi-Fi networks...\e[0m\n\n");
    WifiNetwork nets[32];
    int count = scan_wifi_networks(nets, 32);

    if (count == 0) {
        printf("No active SSIDs discovered within range.\n");
    } else {
        printf("%-4s | %-25s | %-8s | %-10s | %s\n", "No.", "SSID Name", "Signal", "Security", "Status");
        printf("--------------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            printf("%-4d | %-25s | %3d%%    | %-10s | %s\n",
                   i + 1, nets[i].ssid, nets[i].signal,
                   nets[i].secured ? "WPA2/WPA3" : "Open",
                   nets[i].in_use ? "\e[1;32m[CONNECTED]\e[0m" : "Available");
        }
    }
    printf("========================================================\n\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--connect") == 0 && argc >= 3) {
        const char *ssid = argv[2];
        const char *pass = (argc >= 4) ? argv[3] : NULL;
        printf("Connecting to Wi-Fi SSID '%s'...\n", ssid);
        if (connect_wifi(ssid, pass)) {
            printf("\e[1;32m[SUCCESS]\e[0m Connected to '%s'!\n", ssid);
        } else {
            printf("\e[1;31m[FAILED]\e[0m Authentication failed or network unreachable.\n");
        }
        return 0;
    }

    print_wifi_dashboard();
    return 0;
}
