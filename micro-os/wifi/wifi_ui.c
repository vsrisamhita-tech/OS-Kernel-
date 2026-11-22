/*
 * CYBERLAB OS - wifi_ui.c
 * Terminal & Graphical Interface Rendering for Quick Settings
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void render_quick_settings_frame(bool wifi_on, const char *connected_ssid) {
    printf("+------------------------------------------------------+\n");
    printf("|       CYBERLAB OS :: WIRELESS QUICK SETTINGS         |\n");
    printf("+------------------------------------------------------+\n");
    printf("|  Wi-Fi Radio:           [%s]                           |\n", wifi_on ? "ON " : "OFF");
    if (connected_ssid) {
        printf("|  Current Network:       \e[1;32m%-28s\e[0m |\n", connected_ssid);
    } else {
        printf("|  Current Network:       Not Connected                |\n");
    }
    printf("+------------------------------------------------------+\n");
}
