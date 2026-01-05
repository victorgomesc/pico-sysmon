# Pico SysMon

Projeto de driver Linux desenvolvido para fins acadêmicos, utilizando
um periférico USB genérico (Raspberry Pi Pico 2) como gatilho de
ativação do driver.

## Objetivo
Demonstrar o desenvolvimento de um driver Linux com:
- Operações `read()` e `write()`
- Integração com o subsistema USB
- Coleta de métricas do kernel Linux
- Comunicação com aplicação em espaço de usuário

## Arquitetura
pico_sysmon/
├── device/ # Firmware do Pico 2 (USB Vendor)
├── driver/ # Driver Linux (kernel module)
└── user_app/ # Aplicação de usuário

markdown
Copiar código

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
- Estatísticas de CPU por núcleo
- Último comando recebido

## Compilação

### Driver
```bash
cd driver
make
sudo insmod pico_sysmon.ko
User App
bash
Copiar código
cd user_app
gcc sysmon_cli.c -o sysmon
./sysmon
Testes
bash
Copiar código
cat /dev/pico_sysmon
./sysmon "CPU?"
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