#include <stdio.h>
#include "pico/stdlib.h"

// Definir pinos GPIO para os LEDs
#define RED_LED_PIN 11
#define YELLOW_LED_PIN 12
#define GREEN_LED_PIN 13

// Definir pino GPIO para o botão
#define BUTTON_PIN 5

// Enum para representar os modos do sistema
typedef enum {
    MODE_TRAFFIC_LIGHT,
    MODE_ALARM
} system_mode_t;

// Enum para representar os estados do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} traffic_light_state_t;

system_mode_t current_mode = MODE_TRAFFIC_LIGHT;
traffic_light_state_t current_state = RED;

bool timer_running = false;
bool button_pressed = false;
uint32_t last_button_press_time = 0;

// Função de callback para o temporizador repetitivo
bool repeating_timer_callback(struct repeating_timer *t) {
    // Alterar o estado do semáforo
    switch (current_state) {
        case RED:
            gpio_put(RED_LED_PIN, 0);
            gpio_put(YELLOW_LED_PIN, 1);
            current_state = YELLOW;
            break;
        case YELLOW:
            gpio_put(YELLOW_LED_PIN, 0);
            gpio_put(GREEN_LED_PIN, 1);
            current_state = GREEN;
            break;
        case GREEN:
            gpio_put(GREEN_LED_PIN, 0);
            gpio_put(RED_LED_PIN, 1);
            current_state = RED;
            break;
    }
    return true; // Continuar repetindo
}

// Função de callback para desligar o LED verde (modo alarme)
int64_t turn_off_green_callback(alarm_id_t id, void *user_data) {
    gpio_put(GREEN_LED_PIN, 0);
    timer_running = false;
    return 0; // Não repetir
}

// Função de callback para desligar o LED vermelho (modo alarme)
int64_t turn_off_red_callback(alarm_id_t id, void *user_data) {
    gpio_put(RED_LED_PIN, 0);
    add_alarm_in_ms(3000, turn_off_green_callback, NULL, false);
    return 0; // Não repetir
}

// Função de callback para desligar o LED azul (modo alarme)
int64_t turn_off_blue_callback(alarm_id_t id, void *user_data) {
    gpio_put(YELLOW_LED_PIN, 0);
    add_alarm_in_ms(3000, turn_off_red_callback, NULL, false);
    return 0; // Não repetir
}

// Função para verificar o estado do botão com debounce
bool is_button_pressed() {
    static uint32_t last_press_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (gpio_get(BUTTON_PIN) == 0) {
        if (current_time - last_press_time > 200) { // Debounce de 200ms
            last_press_time = current_time;
            return true;
        }
    }
    return false;
}

int main() {
    stdio_init_all();

    // Inicializar pinos GPIO
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_init(YELLOW_LED_PIN);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Inicializar temporizador repetitivo para o modo semáforo
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        if (is_button_pressed()) {
            uint32_t current_time = to_ms_since_boot(get_absolute_time());
            if (current_time - last_button_press_time < 500) {
                // Duplo clique, alternar para o modo semáforo
                current_mode = MODE_TRAFFIC_LIGHT;
                gpio_put(RED_LED_PIN, 1);
                gpio_put(YELLOW_LED_PIN, 0);
                gpio_put(GREEN_LED_PIN, 0);
                current_state = RED;
                timer_running = false;
            } else if (!timer_running) {
                // Clique único, alternar para o modo alarme
                current_mode = MODE_ALARM;
                gpio_put(RED_LED_PIN, 1);
                gpio_put(YELLOW_LED_PIN, 1);
                gpio_put(GREEN_LED_PIN, 1);
                timer_running = true;
                add_alarm_in_ms(3000, turn_off_blue_callback, NULL, false);
            }
            last_button_press_time = current_time;
        }

        if (current_mode == MODE_TRAFFIC_LIGHT) {
            printf("Modo semáforo em execução...\n");
        } else if (current_mode == MODE_ALARM) {
            printf("Modo alarme em execução...\n");
        }

        sleep_ms(100);
    }
}