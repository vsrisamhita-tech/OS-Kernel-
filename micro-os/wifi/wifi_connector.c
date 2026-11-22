/*
 * CYBERLAB OS - wifi_connector.c
 * Secure 4-Way WPA2/WPA3 Handshake and Profile Generation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool authenticate_and_connect(const char *ssid, const char *password) {
    if (!ssid || strlen(ssid) == 0) return false;
    char buffer[512];
    if (password && strlen(password) > 0) {
        snprintf(buffer, sizeof(buffer),
                 "nmcli device wifi connect \"%s\" password \"%s\" >/dev/null 2>&1",
                 ssid, password);
    } else {
        snprintf(buffer, sizeof(buffer),
                 "nmcli device wifi connect \"%s\" >/dev/null 2>&1", ssid);
    }
    int rc = system(buffer);
    return (rc == 0);
}

bool disconnect_active() {
    return (system("nmcli connection down id \"$(nmcli -t -f NAME,TYPE connection show --active | grep ':802-11-wireless' | cut -d: -f1)\" >/dev/null 2>&1") == 0);
}
