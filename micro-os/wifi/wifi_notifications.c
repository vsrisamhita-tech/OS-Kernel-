/*
 * CYBERLAB OS - wifi_notifications.c
 * System-Tray & Desktop Diagnostic Notifications
 */

#include <stdio.h>
#include <stdlib.h>

void send_desktop_notification(const char *summary, const char *body, const char *icon) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "notify-send -i \"%s\" \"%s\" \"%s\" 2>/dev/null || echo \"[NOTIFICATION] %s: %s\"",
             icon, summary, body, summary, body);
    system(cmd);
}

void notify_vbox_network_status() {
    send_desktop_notification(
        "Network Status",
        "Connected via VirtualBox NAT Virtual Ethernet. Physical Wi-Fi requires USB passthrough.",
        "network-wired"
    );
}
