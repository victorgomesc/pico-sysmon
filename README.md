# Pico SysMon

## 📌 Resumo do Projeto

O **Pico SysMon** é um projeto de monitoramento de recursos do sistema desenvolvido em nível de kernel Linux, utilizando um **driver USB** e um **dispositivo físico real** (Raspberry Pi Pico 2).  
O projeto demonstra a integração completa entre **hardware USB**, **driver Linux** e **aplicação em espaço de usuário**, coletando e exibindo métricas como uso de CPU por núcleo, uso de memória RAM e temperatura do processador.

O objetivo principal é demonstrar conceitos de **desenvolvimento de drivers**, **comunicação USB**, **interação kernel ↔ hardware**, e **arquitetura de sistemas operacionais**.

---

## 🎯 O que o projeto faz

O Pico SysMon fornece:

- Uso de **CPU por core (%)**
- Uso total de **memória RAM (%)**
- **Temperatura real da CPU** (quando disponível)
- Comunicação via **dispositivo USB Vendor**
- Interface de leitura via `/dev/pico_sysmon`

A saída é exibida de forma organizada e legível diretamente no terminal.

Exemplo de saída:

```text
====================================
           PICO SYSMON
====================================
Mode        : USB
Uptime      : 1956 s
Load Avg    : 0 0 0

MEMORY
------------------------------------
RAM Usage   : 7307 / 17826 MB (40 %)

TEMPERATURE
------------------------------------
CPU Temp    : 41.5 °C

CPU USAGE (per core)
------------------------------------
CPU0  :   1 %
CPU1  :   0 %
CPU2  :   0 %
...
```

## Arquitetura
pico_sysmon/ <br>
├── device/     # Firmware da Pico 2 <br>
├── driver/     # Driver Linux <br>
└── user_app/   # Aplicação de usuário (opcional) <br>


## Funcionamento
- O driver cria o dispositivo `/dev/pico_sysmon` no carregamento.
- Em modo **SIMULATED**, as métricas são coletadas diretamente do kernel.
- Quando o Pico 2 é conectado, o driver entra em modo **USB**.
- A leitura (`read`) retorna estatísticas do sistema.
- A escrita (`write`) aceita comandos simples e registra o último comando.

## Métricas exibidas
- Uptime do sistema
- Load average
- Memória RAM total e livre
- Temperatura da CPU
- Estatísticas de CPU por núcleo
- Último comando recebido

## ▶️ Como rodar o projeto

### Pré-requisitos do sistema

Atualizar o sistema

```bash
sudo apt update && sudo apt upgrade -y
```

Instalar dependências básicas:

```bash
sudo apt install -y \
  build-essential \
  cmake \
  git \
  linux-headers-$(uname -r)
```

## Configurar o Pico SDK (se já não estiver configurado)

```bash
cd ~
git clone https://github.com/raspberrypi/pico-sdk.git
```

Inicializar submódulos:

```bash
cd pico-sdk
git submodule update --init
```

Exportar variável de ambiente:

```bash
echo 'export PICO_SDK_PATH=$HOME/pico-sdk' >> ~/.bashrc
source ~/.bashrc
```

Verifique:

```bash
echo $PICO_SDK_PATH
```

## Estrutura do projeto

Entre no projeto:

```bash
cd ~/pico_sysmon
```

Estrutura esperada:

```bash
pico_sysmon/
├── device/     # Firmware da Pico 2
├── driver/     # Driver Linux
└── user_app/   # Aplicação de usuário (opcional)
```

## Compilar e gravar o firmware da Pico 2

### Compilar o firmware
```bash
cd ~/pico_sysmon/device
rm -rf build
mkdir build
cd build
cmake ..
make -j
```
Comfirme que existe:

```bash
ls pico_sysmon_device.uf2
```

### Gravar o firmware na Pico 2

<ol>
    <li>Desconecte a Pico</li>
    <li>Segure o botão BOOTSEL</li>
    <li>Conecte o USB</li>
    <li>Solte o botão</li>
</ol>

Agora descubra o ponto de montagem:

```bash
ls /media/$USER
```
Normalmente aparecerá:

```bash
RP2350
```

Grave o firmware:

```bash
cp pico_sysmon_device.uf2 /media/$USER/RP2350/
sync
```

A Pico vai reiniciar automaticamente.
Desconecte o USB
Reconecte a placa sem BOOTSEL

Verifique:

```bash
lsusb
```
Você deve ver algo como:

```bash
Pico SysMon Device
```

## Compilar e carregar o driver Linux

### Compilar o driver

```bash
cd ~/pico_sysmon/driver
make clean
make
```

### Carregar o módulo

```bash
sudo insmod pico_sysmon.ko
```

Verifique:

```bash
lsmod | grep pico_sysmon
```

## Verificar criação do device

```bash
ls -l /dev/pico_sysmon
```

Deve existir algo como:

```bash
crw------- 1 root root ...
```

## Rodar o projeto (final)

```bash
sudo cat /dev/pico_sysmon
```

Observações
O projeto possui um modo simulado para permitir testes mesmo sem o
hardware conectado. Quando o Raspberry Pi Pico 2 é conectado, o driver
migra automaticamente para modo USB.

Autor
Victor Gomes da costa
Gabriel Braga

