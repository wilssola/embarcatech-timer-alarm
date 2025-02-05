# 🚦 Projeto Embarcatech Timer Alarm 🚨

Este projeto implementa um sistema de temporização utilizando o Raspberry Pi Pico W, que alterna entre dois modos: semáforo e alarme. O sistema utiliza LEDs e um botão para demonstrar o funcionamento dos temporizadores periódicos e de um disparo (One Shot).

## 🛠️ Componentes Necessários

1. 🖥️ Microcontrolador Raspberry Pi Pico W
2. 🔴🟡🟢 3 LEDs (vermelho, amarelo e verde)
3. 🔧 3 Resistores de 330 Ω
4. 🔘 Botão (Pushbutton)

## 📋 Descrição do Projeto

### 🚦 Modo Semáforo

- O sistema inicia no modo semáforo.
- Os LEDs alternam entre vermelho, amarelo e verde a cada 3 segundos.
- O estado inicial é o LED vermelho aceso.

### 🚨 Modo Alarme

- Ao pressionar o botão, o sistema alterna para o modo alarme.
- No modo alarme, todos os LEDs são acesos inicialmente.
- Após 3 segundos, o LED amarelo é desligado.
- Após mais 3 segundos, o LED vermelho é desligado.
- Após mais 3 segundos, o LED verde é desligado.
- O botão só pode alterar o estado dos LEDs quando o último LED for desligado.

### 🔄 Alternância entre Modos

- Um clique único no botão alterna para o modo alarme.
- Um duplo clique no botão alterna de volta para o modo semáforo.

## 🗂️ Estrutura do Código

- `main()`: Função principal que inicializa os pinos GPIO e configura os temporizadores.
- `repeating_timer_callback()`: Função de callback para o temporizador repetitivo no modo semáforo.
- `turn_off_green_callback()`, `turn_off_red_callback()`, `turn_off_blue_callback()`: Funções de callback para desligar os LEDs no modo alarme.
- `is_button_pressed()`: Função para verificar o estado do botão com debounce.

## ▶️ Como Executar

1. Conecte os LEDs e o botão aos pinos GPIO do Raspberry Pi Pico W conforme definido no código.
2. Compile e carregue o código no Raspberry Pi Pico W.
3. Abra um monitor serial caso queira visualizar as mensagens de status.
4. Pressione o botão para alternar entre os modos semáforo e alarme.

## 🔗 Links Úteis

- [📄 Documentação do Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [🔧 Simulação sugerida no Wokwi](https://wokwi.com/)

## 📜 Licença

Este projeto está licenciado sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.