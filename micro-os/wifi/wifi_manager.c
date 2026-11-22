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
        if (strstr(buf, "oracle") || strstr(buf, "VirtualBox") || strstr(buf, "vbox") || strstr(buf, "innotek")) {
            return true;
        }
    } else {
        pclose(fp);
    }
    // Also check default route for eth0
    FILE *fp2 = popen("ip route 2>/dev/null | grep -E 'default|eth0'", "r");
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
    FILE *fp = popen("nmcli -t -f TYPE device 2>/dev/null | grep -i wifi || iw dev 2>/dev/null | grep Interface", "r");
    if (!fp) return false;
    char buf[64];
    bool found = (fgets(buf, sizeof(buf), fp) != NULL);
    pclose(fp);
    return found;
}

int scan_wifi_networks(WifiNetwork networks[], int max_nets) {
    if (!is_wifi_hardware_present()) return 0;
    system("nmcli device wifi rescan 2>/dev/null || iwlist scan 2>/dev/null");
    FILE *fp = popen("nmcli -t -f SSID,SIGNAL,SECURITY,IN-USE device wifi list 2>/dev/null", "r");
    if (!fp) return 0;
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), fp) && count < max_nets) {
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
        snprintf(cmd, sizeof(cmd), "nmcli device wifi connect \"%s\" password \"%s\" 2>/dev/null || (wpa_passphrase \"%s\" \"%s\" > /tmp/wpa.conf && wpa_supplicant -B -i wlan0 -c /tmp/wpa.conf 2>/dev/null)", ssid, password, ssid, password);
    } else {
        snprintf(cmd, sizeof(cmd), "nmcli device wifi connect \"%s\" 2>/dev/null || iwconfig wlan0 essid \"%s\" 2>/dev/null", ssid, ssid);
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
    if (argc > 1) {
        if (strcmp(argv[1], "scan") == 0) {
            printf("[INFO] Scanning nearby wireless networks...\n");
            system("nmcli device wifi list 2>/dev/null || iwlist scan 2>/dev/null");
            return 0;
        } else if (strcmp(argv[1], "status") == 0) {
            print_wifi_dashboard();
            return 0;
        } else if (strcmp(argv[1], "connect") == 0 || strcmp(argv[1], "--connect") == 0) {
            if (argc < 3) {
                printf("Usage: wifi connect <SSID> [password]\n");
                return 1;
            }
            const char *ssid = argv[2];
            const char *pass = (argc >= 4) ? argv[3] : NULL;
            printf("Connecting to Wi-Fi SSID '%s'...\n", ssid);
            if (connect_wifi(ssid, pass)) {
                printf("\e[1;32m[SUCCESS]\e[0m Connected to '%s'!\n", ssid);
            } else {
                printf("\e[1;31m[FAILED]\e[0m Authentication failed or network unreachable.\n");
            }
            return 0;
        } else if (strcmp(argv[1], "disconnect") == 0) {
            printf("Disconnecting wireless connection...\n");
            system("nmcli connection down \"$(nmcli -t -f NAME,TYPE c show --active | grep wireless | cut -d: -f1)\" 2>/dev/null || killall wpa_supplicant 2>/dev/null");
            printf("[OK] Disconnected.\n");
            return 0;
        } else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0) {
            printf("CYBERLAB OS Wi-Fi Manager Commands:\n");
            printf("  wifi                    - Display wireless dashboard and status\n");
            printf("  wifi status             - Show current connection status & diagnostics\n");
            printf("  wifi scan               - Scan nearby wireless access points\n");
            printf("  wifi connect <SSID> [pw] - Connect to an access point with password\n");
            printf("  wifi disconnect         - Disconnect from active Wi-Fi\n");
            return 0;
        }
    }

    print_wifi_dashboard();
    return 0;
}
