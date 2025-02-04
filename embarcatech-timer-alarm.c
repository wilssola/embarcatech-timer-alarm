#include <stdio.h>
#include "pico/stdlib.h"

// Definir pinos GPIO para os LEDs
#define RED_LED_PIN 11
#define YELLOW_LED_PIN 12
#define GREEN_LED_PIN 13

// Enum para representar os estados do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} traffic_light_state_t;

traffic_light_state_t current_state = RED;

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

int main() {
    stdio_init_all();

    // Inicializar pinos GPIO
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_init(YELLOW_LED_PIN);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    // Ligar o LED vermelho inicialmente
    gpio_put(RED_LED_PIN, 1);

    // Adicionar temporizador repetitivo
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        printf("Sistema de semáforo em execução...\n");
        sleep_ms(1000);
    }
}