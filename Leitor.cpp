#include <iostream>
#include <fstream>
#include <string>

/**
 * @class RainSensor
 * @brief Classe que representa um sensor de chuva conectado via IIO.
 * 
 * Esta classe encapsula a leitura de um sensor analógico de chuva
 * disponível em /sys/bus/iio/devices/iio:deviceX/.
 */
class RainSensor {
private:
    std::string sensorPath; ///< Caminho do arquivo do sensor
    int threshold;          ///< Valor acima do qual consideramos que está chovendo

public:
    /**
     * @brief Construtor da classe RainSensor
     * @param path Caminho do arquivo do sensor (ex: "/sys/bus/iio/devices/iio:device0/in_voltage19_raw")
     * @param thresh Valor do limiar para considerar chuva
     */
    RainSensor(const std::string& path, int thresh)
        : sensorPath(path), threshold(thresh) {}

    /**
     * @brief Lê o valor bruto do sensor
     * @return Valor inteiro do sensor, ou -1 em caso de erro
     */
    int readValue() const {
        std::ifstream file(sensorPath);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir sensor em " << sensorPath << std::endl;
            return -1;
        }
        int value;
        file >> value;
        return value;
    }

    /**
     * @brief Verifica se está chovendo com base no valor do sensor
     * @return true se o valor do sensor for maior que o threshold, false caso contrário
     */
    bool isRaining() const {
        int value = readValue();
        if (value == -1) return false; // erro de leitura
        return value > threshold;
    }
};

/**
 * @brief Função principal do programa
 * 
 * Cria um objeto RainSensor, lê o valor do sensor e informa se está chovendo.
 * @return int Código de saída do programa
 */
int main() {
    // Caminho do sensor e threshold arbitrário
    RainSensor sensor("/sys/bus/iio/devices/iio:device0/in_voltage19_raw", 1000);

    int valor = sensor.readValue();
    if (valor != -1) {
        std::cout << "Valor do sensor: " << valor << std::endl;
        if (sensor.isRaining()) {
            std::cout << "Está chovendo!" << std::endl;
        } else {
            std::cout << "Não está chovendo." << std::endl;
        }
    }

    return 0;
}
