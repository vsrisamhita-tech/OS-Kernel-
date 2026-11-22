/*
 * CYBERLAB OS - wifi_scanner.c
 * Discovers SSIDs, signal strength, and encryption types.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_wifi_scan() {
    printf("[SCAN] Triggering 802.11 active scanning via NetworkManager...\n");
    system("nmcli device wifi rescan 2>/dev/null");
    FILE *fp = popen("nmcli -f BSSID,SSID,MODE,CHAN,RATE,SIGNAL,BARS,SECURITY device wifi list 2>/dev/null", "r");
    if (!fp) {
        printf("Error: Unable to invoke NetworkManager CLI.\n");
        return;
    }
    char buf[512];
    while (fgets(buf, sizeof(buf), fp)) {
        printf("%s", buf);
    }
    pclose(fp);
}
