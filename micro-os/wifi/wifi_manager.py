#!/usr/bin/env python3
"""wifi_manager.py – High level manager for Wi‑Fi panel.
Uses NetworkManager's "nmcli" CLI to query status and perform actions.
"""
import subprocess, json, sys

def run_nmcli(args):
    cmd = ["nmcli"] + args
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"nmcli error: {result.stderr.strip()}")
    return result.stdout.strip()

def list_devices():
    out = run_nmcli(["-t", "-f", "DEVICE,TYPE,STATE", "device", "status"])
    devices = []
    for line in out.splitlines():
        dev, typ, state = line.split(":")
        if typ == "wifi":
            devices.append({"device": dev, "state": state})
    return devices

def wifi_enabled():
    out = run_nmcli(["radio", "wifi"])
    return out.strip().lower() == "enabled"

def set_wifi(enabled: bool):
    run_nmcli(["radio", "wifi", "on" if enabled else "off"])

def scan_networks():
    # Force a rescan then list
    run_nmcli(["device", "wifi", "rescan"])
    out = run_nmcli(["-t", "-f", "SSID,SECURITY,SIGNAL,IN-USE", "device", "wifi", "list"])
    networks = []
    for line in out.splitlines():
        ssid, sec, signal, inuse = line.split(":")
        networks.append({"ssid": ssid, "security": sec, "signal": int(signal), "in_use": inuse == "*"})
    return networks

def connect(ssid, password=None):
    args = ["device", "wifi", "connect", ssid]
    if password:
        args += ["password", password]
    run_nmcli(args)

def disconnect():
    active = run_nmcli(["-t", "-f", "NAME,TYPE", "connection", "show", "--active"]).splitlines()
    for line in active:
        name, typ = line.split(":")
        if typ == "wifi":
            run_nmcli(["connection", "down", name])
            return

if __name__ == "__main__":
    # Simple CLI for debugging; the UI script imports this module.
    cmd = sys.argv[1] if len(sys.argv) > 1 else "status"
    if cmd == "status":
        print(json.dumps({"enabled": wifi_enabled(), "devices": list_devices()}, indent=2))
    elif cmd == "scan":
        print(json.dumps(scan_networks(), indent=2))
    else:
        print(f"Unsupported command {cmd}")
