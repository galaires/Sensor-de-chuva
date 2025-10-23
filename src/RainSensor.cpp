#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    const std::string adc_path = "/sys/bus/iio/devices/iio:device0/in_voltage13_raw";
    const char* dest_ip = "192.168.42.10";
    const int dest_port = 5005;
    const int max_envios = 100;
    const int intervalo_seg = 1; // intervalo entre envios (segundos)

    // Criação do socket UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Erro ao criar socket." << std::endl;
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(dest_port);
    inet_pton(AF_INET, dest_ip, &addr.sin_addr);

    for (int i = 0; i < max_envios; ++i) {
        std::ifstream f(adc_path);
        if (!f.is_open()) {
            std::cerr << "Erro ao abrir " << adc_path << std::endl;
            break;
        }

        int val = 0;
        f >> val;
        f.close();

        float voltage = (3.3f * val) / 4096.0f;
        std::string msg = "Rain=" + std::to_string(voltage) + "V";

        ssize_t sent = sendto(sock, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
        if (sent < 0) {
            std::cerr << "Erro ao enviar dados via UDP." << std::endl;
            break;
        }

        std::cout << "[" << i + 1 << "/" << max_envios << "] "
                  << "Enviado para " << dest_ip << ":" << dest_port
                  << " -> " << msg << std::endl;

        sleep(intervalo_seg);
    }

    close(sock);
    std::cout << "Envio concluído após " << max_envios << " transmissões." << std::endl;
    return 0;
}
