import socket
import threading
import tkinter as tk
import re
import time
import csv
from collections import deque
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt


UDP_IP = "192.168.42.10"
UDP_PORT = 5005
THRESHOLD = 5.0
UPDATE_INTERVAL = 500
HISTORY_SECONDS = 60


last_voltage = 0.0
data_history = deque(maxlen=HISTORY_SECONDS * 2)


def udp_listener():
    global last_voltage
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    print(f"Escutando pacotes UDP na porta {UDP_PORT}...")
    while True:
        data, _ = sock.recvfrom(1024)
        msg = data.decode("utf-8").strip()
        match = re.search(r"Rain=([\d\.]+)V", msg)
        if match:
            last_voltage = float(match.group(1))
            data_history.append((time.time(), last_voltage))


def update_gui():
    voltage_label.config(text=f"Tensão: {last_voltage:.2f} V")

    if last_voltage < THRESHOLD:
        status_label.config(text="🌧️ Chovendo", bg="#3BAFDA", fg="white")
        root.configure(bg="#3BAFDA")
    else:
        status_label.config(text="☀️ Sem chuva", bg="#A0CFA3", fg="black")
        root.configure(bg="#A0CFA3")

    if data_history:
        times, voltages = zip(*data_history)
        t0 = times[-1]
        rel_times = [t - t0 for t in times]
        line.set_data(rel_times, voltages)
        ax.set_xlim(-HISTORY_SECONDS, 0)
        ax.set_ylim(0, max(10, max(voltages) + 1))
        ax.figure.canvas.draw_idle()

    root.after(UPDATE_INTERVAL, update_gui)


def salvar_log():
    if not data_history:
        print("Nenhum dado para salvar.")
        return

    nome_arquivo = time.strftime("log_chuva_%Y-%m-%d_%H-%M-%S.csv")
    with open(nome_arquivo, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Tempo Relativo (s)", "Tensão (V)"])
        t0 = data_history[-1][0]
        for t, v in data_history:
            writer.writerow([round(t - t0, 2), v])

    print(f"Log salvo em {nome_arquivo}")
    status_label.config(text=f"💾 Log salvo: {nome_arquivo}", bg="#F7D358", fg="black")
    root.configure(bg="#F7D358")
    root.after(2000, update_gui)  

root = tk.Tk()
root.title("Monitor de Sensor de Chuva (UDP)")
root.geometry("500x420")
root.resizable(False, False)
root.configure(bg="#A0CFA3")

voltage_label = tk.Label(root, text="Tensão: --.- V", font=("Arial", 16), bg=root["bg"])
voltage_label.pack(pady=10)

status_label = tk.Label(root, text="Aguardando dados...", font=("Arial", 18, "bold"), bg=root["bg"])
status_label.pack(pady=5)


salvar_button = tk.Button(root, text="Salvar Log", command=salvar_log, bg="#D9EAD3", font=("Arial", 12, "bold"))
salvar_button.pack(pady=5)


fig, ax = plt.subplots(figsize=(5, 2.5), dpi=100)
ax.set_title("Histórico de Tensão (últimos 60s)")
ax.set_xlabel("Tempo (s)")
ax.set_ylabel("Tensão (V)")
ax.grid(True)

line, = ax.plot([], [], lw=2)
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(pady=10)


listener_thread = threading.Thread(target=udp_listener, daemon=True)
listener_thread.start()


update_gui()
root.mainloop()
