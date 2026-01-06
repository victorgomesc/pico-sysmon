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
pico_sysmon/
├── device/ # Firmware do Pico 2 (USB Vendor)
├── driver/ # Driver Linux (kernel module)
└── user_app/ # Aplicação de usuário


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
📋 Pré-requisitos

Linux (Ubuntu 22.04+ recomendado)

Kernel headers instalados

Raspberry Pi Pico 2

Pico SDK configurado

GCC

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
    <li>Segure o botão BOOTSEL<li>
    <li>Conecte o USB</li>
    <li>Solte o botão<li>
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

Reconecte a placa sem BOOTSEL



Observações
O projeto possui um modo simulado para permitir testes mesmo sem o
hardware conectado. Quando o Raspberry Pi Pico 2 é conectado, o driver
migra automaticamente para modo USB.

Autor
Victor Gomes

yaml
Copiar código

---

# 4️⃣ Texto explicativo para o relatório (PRONTO)

Você pode colar **literalmente**:

> Este trabalho apresenta o desenvolvimento de um driver Linux para monitoramento de recursos do sistema, utilizando um periférico USB genérico como mecanismo de ativação. O driver foi implementado como um módulo do kernel e integra-se ao subsistema USB do Linux, criando dinamicamente um dispositivo de caractere acessível via `/dev/pico_sysmon`.
>
> O driver implementa operações de leitura e escrita. A operação de leitura retorna métricas do próprio kernel Linux, como tempo de atividade do sistema, carga média, uso de memória e estatísticas de CPU por núcleo. A operação de escrita permite o envio de comandos simples, que são registrados pelo driver e, quando o hardware está conectado, encaminhados ao dispositivo USB.
>
> Para facilitar o desenvolvimento e a validação do projeto, foi implementado um modo simulado, no qual o dispositivo de caractere permanece disponível mesmo sem o hardware conectado. Quando o Raspberry Pi Pico 2 é conectado, o driver alterna automaticamente para o modo USB real, mantendo a mesma interface para a aplicação de usuário.
>
> A aplicação em espaço de usuário foi desenvolvida em linguagem C e demonstra o uso das chamadas de sistema `open`, `read` e `write` para interação com o driver.

---

# 5️⃣ Status final do seu projeto (importante)

Você agora tem:

✔ Driver Linux real  
✔ `read()` e `write()` funcionando  
✔ CPU por core (kernel-level)  
✔ Saída profissional  
✔ README pronto  
✔ Texto de relatório pronto  
✔ Pode gravar o vídeo **sem o Pico**