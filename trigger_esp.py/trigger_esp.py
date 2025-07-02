import time
import requests 

esp_ip = "http://192.168.1.39"  # IP ของ ESP32

# ฟังก์ชันควบคุม Relay
def send_command(Relay_NUM, state):
    try:
        url = f"{esp_ip}/led{Relay_NUM}/{state}"
        response = requests.get(url, timeout=5)
        print(f"🔧 Sent command to LED{Relay_NUM}: {state} → {response.status_code}")
        time.sleep(0.5) 
    except Exception as e:
        print(f"❌ Error sending to LED{Relay_NUM}: {e}")

# ฟังก์ชันส่งข้อมูลไปยังปุ่มกด
def send_B_command(ButtonN, cmd):
    try:
        url = f"{esp_ip}/button{ButtonN}/{cmd}"
        response = requests.get(url, timeout=5)
        print(f"🔧 Sent command to BUTTON{ButtonN}: {cmd} → {response.status_code}")
        time.sleep(2) 
    except Exception as e:
        print(f"❌ Error sending to BUTTON{ButtonN}: {e}")


# จำสถานะล่าสุดของแต่ละ trigger และ button      
device_state = {}

def handle_change(key, new_state, action_func, *args):
    if device_state.get(key) == new_state:
        return  # ข้ามถ้าสถานะเดิม
    device_state[key] = new_state
    print(f"{key.upper()} → {new_state.upper()}")   
    action_func(*args, new_state)

# log ไฟล์ของ Minecraft
log_path = r"C:\Users\monas\AppData\Roaming\.minecraft\logs\latest.log"

with open(log_path, "r", encoding="utf-8") as f:
    f.seek(0, 2)
    while True:
        line = f.readline()
        if not line:
            time.sleep(0.2)
            continue    

        line = line.strip()

        # Triggers
        if "trigger1:on" in line:
            handle_change("trigger1", "on", send_command, 1)
        elif "trigger1:off" in line:
            handle_change("trigger1", "off", send_command, 1)
        elif "trigger2:on" in line:
            handle_change("trigger2", "on", send_command, 2)
        elif "trigger2:off" in line:
            handle_change("trigger2", "off", send_command, 2)
        elif "trigger3:on" in line:
            handle_change("trigger3", "on", send_command, 3)
        elif "trigger3:off" in line:
            handle_change("trigger3", "off", send_command, 3)
        elif "trigger4:on" in line:
            handle_change("trigger4", "on", send_command, 4)
        elif "trigger4:off" in line:
            handle_change("trigger4", "off", send_command, 4)

        # Buttons
        elif "Button1:on" in line:
            handle_change("button1", "on", send_B_command, 1)
        elif "Button1:off" in line:
            handle_change("button1", "off", send_B_command, 1)
        elif "Button2:on" in line:
            handle_change("button2", "on", send_B_command, 2)
        elif "Button2:off" in line:
            handle_change("button2", "off", send_B_command, 2)
        elif "Button3:on" in line:
            handle_change("button3", "on", send_B_command, 3)
        elif "Button3:off" in line:
            handle_change("button3", "off", send_B_command, 3)