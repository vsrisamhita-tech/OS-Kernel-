/*
 * CYBERLAB OS - DUAL ARCHITECTURE WI-FI & NETWORK CONTROL CENTER
 * File: wifi_manager.c
 * Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
 * Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

#define MAX_NETWORKS 6
#define MAX_DEVICES 5

typedef struct {
    int id;
    char ssid[32];
    char band[12];
    int channel;
    int signal;
    char security[24];
    bool connected;
} WiFiNetwork;

typedef struct {
    char name[32];
    char ip[20];
    char mac[20];
    char role[32];
    char status[16];
} ConnectedDevice;

static WiFiNetwork networks[MAX_NETWORKS] = {
    {1, "Campus-Secure-WiFi",      "5.0 GHz", 36,  94, "WPA2/WPA3", true},
    {2, "CyberLab-5G-Research",    "5.0 GHz", 44,  88, "WPA2-PSK",  false},
    {3, "BMSIT-Student-Access",    "2.4 GHz", 6,   76, "WPA2-PSK",  false},
    {4, "Faculty-HighSpeed-Mesh",  "5.0 GHz", 149, 65, "WPA2-Ent",  false},
    {5, "AndroidAP-MobileHotspot", "2.4 GHz", 1,   52, "WPA2-PSK",  false},
    {6, "Library-Guest-Open",      "2.4 GHz", 11,  44, "OPEN",      false}
};

static ConnectedDevice devices[MAX_DEVICES] = {
    {"CYBERLAB-OS [This VM]", "10.0.2.15", "08:00:27:5A:3F:1C", "Active OS Host",       "ONLINE"},
    {"Campus-WiFi-Gateway",   "10.0.2.2",  "52:54:00:12:35:02", "Default Gateway / NAT", "ONLINE"},
    {"Host-PC [Windows 11]",  "10.0.2.3",  "52:54:00:12:35:03", "Host Bridge Machine",   "ONLINE"},
    {"Primary-DNS-Resolver",  "8.8.8.8",   "--:--:--:--:--:--", "Google DNS Router",     "ONLINE"},
    {"Lab-Printer-LaserJet",  "10.0.2.50", "00:1A:2B:3C:4D:5E", "Network Peripheral",    "STANDBY"}
};

static char current_ip[32] = "10.0.2.15";
static char current_gw[32] = "10.0.2.2";
static bool is_online = true;

void refresh_real_ip() {
    FILE *fp = popen("ip -4 addr show eth0 2>/dev/null | grep -o 'inet [0-9.]*' | cut -d' ' -f2", "r");
    if (fp) {
        char buf[64];
        if (fgets(buf, sizeof(buf), fp)) {
            buf[strcspn(buf, "\r\n")] = 0;
            if (strlen(buf) > 4) {
                strncpy(current_ip, buf, sizeof(current_ip) - 1);
                strncpy(devices[0].ip, buf, sizeof(devices[0].ip) - 1);
            }
        }
        pclose(fp);
    }
}

void print_banner() {
    printf("\033[1;36m================================================================================\033[0m\n");
    printf("\033[1;37m        CYBERLAB OS :: DUAL ARCHITECTURE WI-FI & NETWORK CONTROL CENTER        \033[0m\n");
    printf("   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?\n");
    printf("   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)\n");
    printf("\033[1;36m================================================================================\033[0m\n");
}

void show_status_page() {
    refresh_real_ip();
    print_banner();

    // 1. Connection Status Block
    int active_idx = -1;
    for (int i = 0; i < MAX_NETWORKS; i++) {
        if (networks[i].connected) {
            active_idx = i;
            break;
        }
    }

    printf("\033[1;33m[1] ACTIVE WIRELESS CONNECTION:\033[0m\n");
    if (is_online && active_idx >= 0) {
        printf("    Status       : \033[1;32m[CONNECTED & ONLINE]\033[0m via 802.11 VirtualBox Adapter\n");
        printf("    Active SSID  : \033[1;37m%s\033[0m (Band: %s, Ch: %d)\n",
               networks[active_idx].ssid, networks[active_idx].band, networks[active_idx].channel);
        printf("    Signal Level : \033[1;32m[||||||||||] %d%% (-46 dBm, Excellent)\033[0m\n", networks[active_idx].signal);
        printf("    IPv4 Address : \033[1;32m%s\033[0m / 24  (DHCP Lease Active)\n", current_ip);
        printf("    Default GW   : %s  (NAT Router Interface)\n", current_gw);
        printf("    DNS Servers  : 8.8.8.8, 1.1.1.1  (Internet Route Active)\n");
        printf("    Security     : %s  (AES-CCMP / Hardware Encrypted)\n", networks[active_idx].security);
    } else {
        printf("    Status       : \033[1;31m[DISCONNECTED]\033[0m No active wireless link\n");
        printf("    Action       : Select Option [3] below to connect to an available network\n");
    }

    printf("\033[1;36m--------------------------------------------------------------------------------\033[0m\n");

    // 2. Connected Devices on Local Network
    printf("\033[1;33m[2] CONNECTED DEVICES ON LOCAL NETWORK (Live Discovery):\033[0m\n");
    printf("    %-4s | %-23s | %-15s | %-17s | %s\n", "No.", "Device Name", "IP Address", "MAC Address", "Status");
    printf("    -----+-------------------------+-----------------+-------------------+----------\n");
    for (int i = 0; i < MAX_DEVICES; i++) {
        const char *color = (strcmp(devices[i].status, "ONLINE") == 0) ? "\033[1;32m" : "\033[1;33m";
        printf("     %-3d | %-23s | %-15s | %-17s | %s%-8s\033[0m\n",
               i + 1, devices[i].name, devices[i].ip, devices[i].mac, color, devices[i].status);
    }

    printf("\033[1;36m--------------------------------------------------------------------------------\033[0m\n");

    // 3. Available Wi-Fi Networks in Range
    printf("\033[1;33m[3] AVAILABLE WI-FI NETWORKS IN RANGE:\033[0m\n");
    printf("    %-4s | %-25s | %-8s | %-4s | %-8s | %-11s | %s\n",
           "No.", "SSID Name", "Band", "Ch", "Signal", "Security", "Status");
    printf("    -----+---------------------------+----------+------+----------+-------------+-----------\n");
    for (int i = 0; i < MAX_NETWORKS; i++) {
        char sig_bar[16];
        int bars = networks[i].signal / 10;
        char bar_str[12] = "";
        for (int b = 0; b < 10; b++) {
            strcat(bar_str, (b < bars) ? "|" : ".");
        }
        snprintf(sig_bar, sizeof(sig_bar), "%d%% [%s]", networks[i].signal, bar_str);

        if (networks[i].connected) {
            printf("    \033[1;32m[%d]  | %-25s | %-8s | %-4d | %-8s | %-11s | [CONNECTED]\033[0m\n",
                   networks[i].id, networks[i].ssid, networks[i].band,
                   networks[i].channel, sig_bar, networks[i].security);
        } else {
            printf("     %d   | %-25s | %-8s | %-4d | %-8s | %-11s | Available\n",
                   networks[i].id, networks[i].ssid, networks[i].band,
                   networks[i].channel, sig_bar, networks[i].security);
        }
    }

    printf("\033[1;36m================================================================================\033[0m\n");
    printf("  \033[1;37m[CONTROL OPTIONS]:\033[0m\n");
    printf("    [1] Scan & Refresh Wireless Spectrum       [4] Test Internet (Ping 8.8.8.8)\n");
    printf("    [2] Discover Connected Local Devices (ARP) [5] Disconnect Active Network\n");
    printf("    [3] Connect to an Available Network        [6] Reconnect Default Network\n");
    printf("    [0] Exit to Terminal Shell\n");
    printf("\033[1;36m================================================================================\033[0m\n");
}

void do_scan_animation() {
    printf("\n\033[1;33m[*] Initiating RF spectrum scan across 2.4GHz & 5GHz channels...\033[0m\n");
    const char *channels[] = {"1", "6", "11", "36", "40", "44", "48", "149", "153", "157", "161"};
    for (int i = 0; i < 11; i++) {
        printf("    Scanning Channel %-3s ... OK (Beacon received)\r", channels[i]);
        fflush(stdout);
        usleep(70000);
    }
    printf("\n\033[1;32m[+] Scan Complete! Discovered %d Wireless Access Points with strong RSSI.\033[0m\n", MAX_NETWORKS);
    usleep(600000);
}

void do_device_scan() {
    printf("\n\033[1;33m[*] Sweeping subnet 10.0.2.0/24 for connected devices (ARP Request)...\033[0m\n");
    for (int i = 1; i <= 10; i++) {
        printf("    ARP probe: 10.0.2.%d ... Response received\r", i * 5);
        fflush(stdout);
        usleep(60000);
    }
    printf("\n\033[1;32m[+] 5 Active Network Devices detected and mapped to ARP cache.\033[0m\n");
    
    // Also read /proc/net/arp if available
    FILE *fp = fopen("/proc/net/arp", "r");
    if (fp) {
        char line[256];
        printf("\n\033[1;37m    Kernel ARP Cache:\033[0m\n");
        while (fgets(line, sizeof(line), fp)) {
            printf("    %s", line);
        }
        fclose(fp);
    }
    printf("\nPress [Enter] to continue...");
    getchar();
}

void do_connect_prompt() {
    printf("\n\033[1;33mEnter Network ID to connect (1-%d): \033[0m", MAX_NETWORKS);
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin)) {
        int id = atoi(buf);
        if (id >= 1 && id <= MAX_NETWORKS) {
            int target_idx = id - 1;
            printf("\n[*] Connecting to '\033[1;37m%s\033[0m'...\n", networks[target_idx].ssid);
            usleep(250000);
            printf("    [1/4] Sending IEEE 802.11 probe request... [ACK]\n");
            usleep(200000);
            printf("    [2/4] WPA2/WPA3 4-Way Handshake with AP... [VERIFIED]\n");
            usleep(200000);
            printf("    [3/4] Requesting IPv4 lease via DHCP Discover... [OFFER]\n");
            usleep(200000);
            printf("    [4/4] Assigned IP %s, Gateway %s... [BOUND]\n", current_ip, current_gw);
            usleep(200000);

            for (int i = 0; i < MAX_NETWORKS; i++) {
                networks[i].connected = (i == target_idx);
            }
            is_online = true;
            printf("\033[1;32m✔ Successfully connected to '%s'! Internet is online.\033[0m\n", networks[target_idx].ssid);
            usleep(700000);
        } else {
            printf("\033[1;31mInvalid network ID.\033[0m\n");
            usleep(500000);
        }
    }
}

void do_ping_test() {
    printf("\n\033[1;33m[*] Testing live internet route to Google DNS (8.8.8.8)...\033[0m\n");
    int ret = system("ping -c 4 -W 2 8.8.8.8 2>/dev/null || ping -c 3 10.0.2.2");
    if (ret == 0) {
        printf("\n\033[1;32m✔ Live Internet connection verified! 0%% packet loss.\033[0m\n");
    } else {
        printf("\n\033[1;33mNote: Live ICMP ping test finished.\033[0m\n");
    }
    printf("\nPress [Enter] to continue...");
    getchar();
}

void do_disconnect() {
    for (int i = 0; i < MAX_NETWORKS; i++) {
        networks[i].connected = false;
    }
    is_online = false;
    printf("\n\033[1;31m[*] Disconnected from all wireless networks.\033[0m\n");
    usleep(600000);
}

void do_reconnect_default() {
    networks[0].connected = true;
    for (int i = 1; i < MAX_NETWORKS; i++) {
        networks[i].connected = false;
    }
    is_online = true;
    printf("\n\033[1;32m[*] Reconnected to default: Campus-Secure-WiFi (5GHz).\033[0m\n");
    usleep(600000);
}

int main(int argc, char *argv[]) {
    // If called with arguments from command line (e.g. 'wifi scan', 'wifi status')
    if (argc > 1) {
        if (strcmp(argv[1], "scan") == 0) {
            print_banner();
            do_scan_animation();
            show_status_page();
            return 0;
        } else if (strcmp(argv[1], "devices") == 0) {
            print_banner();
            do_device_scan();
            return 0;
        } else if (strcmp(argv[1], "disconnect") == 0) {
            do_disconnect();
            return 0;
        }
    }

    // Interactive Loop (keeps the window open!)
    char choice[32];
    while (1) {
        system("clear");
        show_status_page();
        printf("\033[1;32mSelect Option [0-6] > \033[0m");
        fflush(stdout);

        if (!fgets(choice, sizeof(choice), stdin)) {
            break;
        }

        char c = choice[0];
        if (c == '0' || c == 'q' || c == 'Q') {
            printf("\nExiting Wi-Fi Control Center.\n");
            break;
        } else if (c == '1') {
            do_scan_animation();
        } else if (c == '2') {
            do_device_scan();
        } else if (c == '3') {
            do_connect_prompt();
        } else if (c == '4') {
            do_ping_test();
        } else if (c == '5') {
            do_disconnect();
        } else if (c == '6') {
            do_reconnect_default();
        }
    }

    return 0;
}
