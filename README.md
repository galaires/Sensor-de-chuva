# 📡 Projeto de Programação Aplicada – Monitoramento Inteligente de Carga

## 📌 Descrição Geral
Este projeto tem como objetivo o **monitoramento inteligente de cargas sensíveis**, desenvolvido no contexto do Instituto Militar de Engenharia (IME).  
Nosso grupo foi responsável pela integração do **Módulo Sensor de Chuva (SS29)** ao kit de desenvolvimento **STM32MP1-DK1**, de modo a detectar **infiltração de líquidos e exposição a ambientes úmidos**, que podem comprometer a integridade da carga durante o transporte.

O sistema embarcado coleta dados do sensor, processa-os e transmite em **tempo real via protocolo UDP** para um **servidor central**, permitindo acompanhamento remoto. Além disso, foi desenvolvida uma **interface gráfica de monitoramento** no PC, que exibe os valores atuais, histórico recente e alertas visuais.

---

## 🔧 Estrutura do Projeto

```
📂 projeto-monitoramento-carga
 ├── 📂 src
 │    ├── SensorChuva.cpp      # Classe de leitura e envio do sensor
 │    ├── SensorChuva.hpp      # Definição da classe
 │    ├── main.cpp             # Programa principal (execução no kit)
 │
 ├── 📂 docs
 │    ├── Doxyfile             # Configuração Doxygen
 │    └── documentacao.html    # Documentação gerada
 │
 ├── 📂 server
 │    ├── servidor_udp.py      # Servidor UDP para recepção dos dados
 │    ├── interface_gui.py     # Interface gráfica (Python + Tkinter/Matplotlib)
 │
 ├── 📂 bin
 │    └── programa             # Executável funcional do sensor de chuva
 │
 ├── README.md                 # Este arquivo
 └── Makefile                  # Script de compilação cruzada
```

---

## 🛰️ Sensor Escolhido – **Módulo de Chuva (SS29)**
- **Tipo:** Analógico/Digital  
- **Alimentação:** 3.3V (⚠️ nunca usar 5V com STM32MP1-DK1)  
- **Sinal Analógico:** Indica nível de umidade (0–4095 no ADC de 12 bits).  
- **Saída Digital (opcional):** Indica se o nível ultrapassou um limiar definido no módulo.  

### 📋 Funcionamento:
- Em ambiente seco → valores baixos no ADC.  
- Em contato com umidade/chuva → valores altos no ADC.  
- Permite detectar infiltrações ou ambientes inadequados para transporte da carga.  

---

## ⚙️ Compilação e Execução

### 🔨 Compilação Cruzada
1. Instalar a toolchain na VM Linux:
   ```bash
   tar -xvf arm-buildroot-linux-gnueabihf_sdk-DK2.tar.gz
   ```
2. Compilar o código:
   ```bash
   source /path/to/toolchain/environment-setup
   make
   ```

### 🚀 Execução no Kit
1. Transferir o executável:
   ```bash
   scp bin/sensor_chuva root@<ip_kit>:/home/root
   ```
2. Conectar via SSH:
   ```bash
   ssh root@<ip_kit>
   ```
3. Executar aplicação:
   ```bash
   ./sensor_chuva
   ```

---

## 🚀 Programa Funcional no Kit

O repositório já contém um **executável compilado** chamado `programa`, localizado na pasta `bin/`.  
Este executável realiza a **leitura do Módulo de Chuva (SS29)** diretamente no kit de desenvolvimento **STM32MP1-DK1**.

### 📂 Estrutura recomendada
```
📂 projeto-monitoramento-carga
 ├── 📂 bin
 │    └── programa          # Executável funcional do sensor de chuva
 ├── 📂 src
 │    ├── SensorChuva.cpp
 │    ├── SensorChuva.hpp
 │    ├── main.cpp
 ...
```

### ▶️ Execução do programa
1. Transferir o executável para o kit:
   ```bash
   scp bin/programa root@<ip_kit>:/home/root
   ```

2. Acessar o kit via SSH:
   ```bash
   ssh root@<ip_kit>
   ```

3. Tornar o arquivo executável (se necessário):
   ```bash
   chmod +x programa
   ```

4. Executar o programa:
   ```bash
   ./programa
   ```

📊 Durante a execução, o programa exibirá no terminal os valores lidos do sensor de chuva em tempo real.

---

## 🌐 Comunicação UDP

- **Protocolo:** UDP simples, formato CSV  
- **Exemplo de pacote:**
  ```
  GRUPO04,SENSOR_CHUVA,valor=2350,unidade=adc,timestamp=1694368423
  ```
- **Campos:**
  - Identificador do grupo/sensor
  - Valor atual da leitura
  - Unidade de medida (ADC)
  - Timestamp (opcional)

---

## 💻 Interface Gráfica
A interface foi desenvolvida em **Python (Tkinter + Matplotlib)** e possui:

- 📊 Gráfico em tempo real (últimos 60s)  
- 🚨 Alerta visual em vermelho quando o valor ultrapassa o limite de segurança  
- 💾 Opção de salvar leituras em `.csv` para análise posterior  

Execução no PC:
```bash
python3 server/interface_gui.py
```

---

## 📚 Dependências
- **Kit de Desenvolvimento:** STM32MP1-DK1  
- **Linguagem:** C++17  
- **Ferramentas:** Doxygen, Make, Toolchain ARM  
- **Servidor/Interface:** Python 3.10+, bibliotecas `socket`, `tkinter`, `matplotlib`  

---

## 👨‍💻 Autores
- Gabriel Aires Lima  
- Victor Kauã de Sousa Viana 


