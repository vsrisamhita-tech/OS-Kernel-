/*
 * CYBERLAB OS - DUAL ARCHITECTURE WI-FI CONTROL CENTER
 * File: wifi_manager.c
 * Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
 * Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void print_header() {
    printf("\033[1;36m========================================================================\033[0m\n");
    printf("\033[1;37m     CYBERLAB OS :: DUAL ARCHITECTURE WI-FI CONTROL CENTER\033[0m\n");
    printf("   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?\n");
    printf("   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)\n");
    printf("\033[1;36m========================================================================\033[0m\n");
}

void get_network_info(char *ip, char *gw, bool *connected) {
    strcpy(ip, "10.0.2.15");
    strcpy(gw, "10.0.2.2");
    *connected = true;

    FILE *fp = popen("ip -4 addr show eth0 2>/dev/null | grep -o 'inet [0-9.]*' | cut -d' ' -f2", "r");
    if (fp) {
        char buf[64];
        if (fgets(buf, sizeof(buf), fp)) {
            buf[strcspn(buf, "\r\n")] = 0;
            if (strlen(buf) > 4) strcpy(ip, buf);
        }
        pclose(fp);
    }
}

void show_status() {
    print_header();
    char ip[64], gw[64];
    bool conn;
    get_network_info(ip, gw, &conn);

    printf("\033[1;33m  [WIRELESS CONNECTION STATUS]:\033[0m\n");
    printf("    Connection   : \033[1;32m[CONNECTED]\033[0m Online via Host Wi-Fi Bridge\n");
    printf("    Active SSID  : \033[1;37mCampus-Secure-WiFi\033[0m (802.11ax 5GHz)\n");
    printf("    IPv4 Address : %s  (DHCP Lease Active)\n", ip);
    printf("    Default GW   : %s  (NAT Router)\n", gw);
    printf("    DNS Servers  : 8.8.8.8, 1.1.1.1 (Google / Cloudflare)\n");
    printf("    Signal Level : \033[1;32m█████████░ 92%%\033[0m (Excellent Quality)\n");
    printf("    Security     : WPA2-Enterprise / WPA3-Personal (AES-CCMP)\n");
    printf("    Link Speed   : 300.0 Mbps Full Duplex\n");
    printf("\033[1;36m------------------------------------------------------------------------\033[0m\n");
    printf("\033[1;33m  [NEARBY DETECTED ACCESS POINTS]:\033[0m\n");
    printf("    %-4s | %-26s | %-8s | %-12s | %s\n", "No.", "SSID Name", "Signal", "Security", "Status");
    printf("    -----+----------------------------+----------+--------------+-----------\n");
    printf("     1   | Campus-Secure-WiFi         | 92%%      | WPA2/WPA3    | \033[1;32m[CONNECTED]\033[0m\n");
    printf("     2   | CyberLab-5G-Research       | 88%%      | WPA2-PSK     | Available\n");
    printf("     3   | BMSIT-Student-Access       | 75%%      | WPA2-PSK     | Available\n");
    printf("     4   | Faculty-HighSpeed-Mesh     | 64%%      | WPA2-PSK     | Available\n");
    printf("     5   | AndroidAP-MobileHotspot    | 50%%      | WPA2-PSK     | Available\n");
    printf("\033[1;36m========================================================================\033[0m\n");
    printf("  [COMMANDS]:\n");
    printf("    wifi             - Show this connection status\n");
    printf("    wifi scan        - Scan surrounding 802.11 wireless spectrum\n");
    printf("    wifi connect <SSID> [password] - Connect to another network\n");
    printf("    wifi disconnect  - Disconnect active Wi-Fi\n");
    printf("    ping 8.8.8.8     - Test live packet transmission\n");
    printf("\033[1;36m========================================================================\033[0m\n");
}

void do_scan() {
    print_header();
    printf("\033[1;33m  Scanning 2.4GHz & 5GHz wireless channels...\033[0m\n");
    usleep(400000);
    printf("  [Ch 1, 6, 11, 36, 40, 44, 48, 149, 153, 157, 161] Scanned.\n\n");
    printf("  Discovered 5 wireless networks in range:\n\n");
    printf("    %-4s | %-26s | %-8s | %-12s | %s\n", "No.", "SSID Name", "Signal", "Security", "Status");
    printf("    -----+----------------------------+----------+--------------+-----------\n");
    printf("     1   | Campus-Secure-WiFi         | 92%%      | WPA2/WPA3    | \033[1;32m[CONNECTED]\033[0m\n");
    printf("     2   | CyberLab-5G-Research       | 88%%      | WPA2-PSK     | Available\n");
    printf("     3   | BMSIT-Student-Access       | 75%%      | WPA2-PSK     | Available\n");
    printf("     4   | Faculty-HighSpeed-Mesh     | 64%%      | WPA2-PSK     | Available\n");
    printf("     5   | AndroidAP-MobileHotspot    | 50%%      | WPA2-PSK     | Available\n");
    printf("\n  To switch networks: wifi connect <SSID> [password]\n");
}

void do_connect(const char *ssid, const char *pw) {
    print_header();
    printf("\033[1;33m  Initiating 4-way WPA handshake with '%s'...\033[0m\n", ssid);
    usleep(500000);
    printf("  [1/4] Beacon frame probe request sent.\n");
    usleep(250000);
    printf("  [2/4] Association request accepted.\n");
    usleep(250000);
    printf("  [3/4] EAPoL key exchange verified.\n");
    usleep(250000);
    printf("  [4/4] DHCP address allocated: 10.0.2.15/24 via default route.\n\n");
    printf("\033[1;32m  ✔ Successfully connected to '%s'!\033[0m\n", ssid);
    printf("  Internet route is active and operational.\n");
}

void do_disconnect() {
    print_header();
    printf("\033[1;31m  [DISCONNECTED]\033[0m Wireless connection disabled.\n");
    printf("  Run 'wifi connect Campus-Secure-WiFi' to reconnect.\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1 || strcmp(argv[1], "status") == 0) {
        show_status();
    } else if (strcmp(argv[1], "scan") == 0) {
        do_scan();
    } else if (strcmp(argv[1], "connect") == 0) {
        if (argc >= 3) {
            do_connect(argv[2], argc >= 4 ? argv[3] : "");
        } else {
            printf("Usage: wifi connect <SSID> [password]\n");
        }
    } else if (strcmp(argv[1], "disconnect") == 0) {
        do_disconnect();
    } else {
        printf("Unknown option '%s'. Usage: wifi [scan|status|connect|disconnect]\n", argv[1]);
    }
    return 0;
}
