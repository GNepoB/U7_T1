/* 
Código para identificar complicações médicas em UTI, e, a partir disto,
simular a injeção automática do medicamento, por meio de uma bomba de infusão
para combater a complicação identificada (processo simulado pelo acionamento do LED)
e, além disso, acionar uma sirene de emergência para acionar a equipe médica do
hospital. Como também o acompanahmento do processo de aplicações de medicações através
do display.

Código criado para a placa BitDogLab, da Raspberry Pi Pico W (RP2040).

Autor: Gabriel Neponuceno Batista
*/

// Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

// Configurações para a comunicação serial I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define Joystick_X 26  // GPIO para eixo X
#define Joystick_Y 27  // GPIO para eixo Y
#define botao_Joystick 22 // GPIO para botão do Joystick
#define botao_A 5 // GPIO para botão A
#define LED_verde 11 // GPIO para LED verde
#define LED_azul 12 // GPIO para LED azul
#define LED_vermelho 13 // GPIO para LED vermelho
#define buzzer_A 21 // GPIO para BUZZER A

bool LED_off = false; // Costante booleana para desligar LED
bool LED_on = true; // Costante booleana para ligar LED

ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;

// Função para emitir som de uma sirene de emergências no buzzer
// e ligar LED representando a aplicação da medicação contra Bradicardia
void sirene_buzzer_1(int duracao_ms) {
    gpio_init(buzzer_A); // Inicializando Buzzer
    gpio_set_dir(buzzer_A, GPIO_OUT); //Definindo Buzzer como saída

    int tempo_total = duracao_ms; 
    int periodo = 300; // Tempo de cada tom da sirene (em ms)

    while (tempo_total > 0) {
        gpio_put(LED_vermelho, LED_on);  // LED acende
        gpio_put(LED_azul, LED_on); // LED acende
        // Tom alto da sirene
        for (int i = 0; i < periodo * 1000 / 1000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(600);
            gpio_put(buzzer_A, 0);
            sleep_us(600);
        }
        sleep_ms(50); // Pequena pausa para variação do tom

        gpio_put(LED_vermelho, LED_off); // LED apaga
        gpio_put(LED_azul, LED_off);
        // Tom baixo da sirene
        for (int i = 0; i < periodo * 1000 / 2000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(1200);
            gpio_put(buzzer_A, 0);
            sleep_us(1200);
        }
        sleep_ms(50); // Pausa entre os tons

        tempo_total -= (2 * periodo);
    }
}
// Função para emitir som de uma sirene de emergências no buzzer
// e ligar LED representando a aplicação da medicação contra Choque Circulatório
void sirene_buzzer_2(int duracao_ms) {
    gpio_init(buzzer_A);
    gpio_set_dir(buzzer_A, GPIO_OUT);

    int tempo_total = duracao_ms; 
    int periodo = 300; // Tempo de cada tom da sirene (em ms)

    while (tempo_total > 0) {
        // Tom alto da sirene
        gpio_put(LED_azul, LED_on);  // LED acende
        for (int i = 0; i < periodo * 1000 / 1000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(600);
            gpio_put(buzzer_A, 0);
            sleep_us(600);
        }
        sleep_ms(50); // Pequena pausa para variação do tom

        // Tom baixo da sirene
        gpio_put(LED_azul, LED_off); // LED apaga
        for (int i = 0; i < periodo * 1000 / 2000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(1200);
            gpio_put(buzzer_A, 0);
            sleep_us(1200);
        }
        sleep_ms(50); // Pausa entre os tons

        tempo_total -= (2 * periodo);
    }
}
// Função para emitir som de uma sirene de emergências no buzzer
// e ligar LED representando a aplicação da medicação contra Bradicardia e Choque Circulatório
void sirene_buzzer_3(int duracao_ms) {
    gpio_init(buzzer_A);
    gpio_set_dir(buzzer_A, GPIO_OUT);

    int tempo_total = duracao_ms; 
    int periodo = 300; // Tempo de cada tom da sirene (em ms)

    while (tempo_total > 0) {
        // Tom alto
        gpio_put(LED_vermelho, LED_on);  // LED acende
        for (int i = 0; i < periodo * 1000 / 1000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(600);
            gpio_put(buzzer_A, 0);
            sleep_us(600);
        }
        sleep_ms(50); // Pequena pausa para variação do tom

        // Tom baixo
        gpio_put(LED_vermelho, LED_off); // LED apaga
        for (int i = 0; i < periodo * 1000 / 2000; i++) {
            gpio_put(buzzer_A, 1);
            sleep_us(1200);
            gpio_put(buzzer_A, 0);
            sleep_us(1200);
        }
        sleep_ms(50); // Pausa entre os tons

        tempo_total -= (2 * periodo);
    }
}

// Função para piscar o LED verde
void pisca_LED_verde(int duracao_ms) {
    int tempo_total = duracao_ms;
    int intervalo = 500; // Tempo entre cada alternância do LED
    //loop para piscar o LED verde a cada 0,5 segundos
    while (tempo_total > 0) {
        gpio_put(LED_verde, LED_on);
        sleep_ms(intervalo);
        gpio_put(LED_verde, LED_off);
        sleep_ms(intervalo);
        tempo_total -= (2 * intervalo); // Reduz o tempo total
    }
}

int main(){
    stdio_init_all();
    
    // Comandos para inicializar os LEDs e defini-los como saída
    gpio_init(LED_azul);
    gpio_set_dir(LED_azul, GPIO_OUT);
    gpio_init(LED_vermelho);
    gpio_set_dir(LED_vermelho, GPIO_OUT);
    gpio_init(LED_verde);
    gpio_set_dir(LED_verde, GPIO_OUT);

    gpio_init(buzzer_A); //Inicializando o Buzzerr
    gpio_set_dir(buzzer_A, GPIO_OUT); // Definindo o Buzzer como saída
    // Comandos para inicializar os Eixos X e Y do joystick, além do adc do RP2040
    adc_init();
    gpio_init(Joystick_X);
    gpio_init(Joystick_Y);

    // Variável para armazenar valores da conversao AD dos eixos X e Y do Joystick
    uint16_t adc_value_X;
    uint16_t adc_value_Y;
    
    // Inicialização do I2C, usando 400Khz
    i2c_init(I2C_PORT, 400 * 1000);
    // Comandos para configurar a comunicação serial I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Loop princial
    while(1){
        adc_select_input(0); // Selecionando canal do eixo X do joystick
        adc_value_X = adc_read(); // Lendo valor do ADC do eixo X
        
        adc_select_input(1); // Selecionando canal do eixo Y do joystick
        adc_value_Y = adc_read(); // Lendo valor do ADC do eixo Y

        if(adc_value_X >= 3000 && adc_value_Y < 3000){
            sleep_ms(40);
            ssd1306_fill(&ssd, !cor); // Limpa o display
            // Escrevendo ATROPINA APLICADA no display
            // (medicação contra Bradicardia)
            ssd1306_draw_string(&ssd, "ATROPINA", 30, 18);
            ssd1306_draw_string(&ssd, "APLICADA", 30, 38);
            ssd1306_send_data(&ssd); // Atualiza o display
            sirene_buzzer_1(2000); // Aciona função que vai iniciar uma sirene de
                                  // emergência e acionar LED roxo (injeção automática)
        }
        if(adc_value_Y >= 3000 && adc_value_X < 3000){
            sleep_ms(40);
            ssd1306_fill(&ssd, !cor); // Limpa o display
        // Escrevendo NORADRENALINA APLICADA no display
        // (medicação contra Choque Circulatório)
            ssd1306_draw_string(&ssd, "NORADRENALINA", 15, 18); 
            ssd1306_draw_string(&ssd, "APLICADA", 30, 38);
            ssd1306_send_data(&ssd); // Atualiza o display
            sirene_buzzer_2(2000); // Aciona função que vai iniciar uma sirene de
                                  // emergência e acionar LED azul (injeção automática)
        }

        if(adc_value_X >= 3000 && adc_value_Y >= 3000){
            sleep_ms(40);
            ssd1306_fill(&ssd, !cor); // Limpa o display
            // Escrevendo ATROPINA E NORADRENALINA APLICADA no display
            // (medicação contra Bradicardia e Choque Circulatório)
            ssd1306_draw_string(&ssd, "ATROPINA E", 25, 8); 
            ssd1306_draw_string(&ssd, "NORADRENALINA", 15, 28); 
            ssd1306_draw_string(&ssd, "APLICADA", 35, 48);
            ssd1306_send_data(&ssd); // Atualiza o display
            sirene_buzzer_3(2000); // Aciona função que vai iniciar uma sirene de
                                  // emergência e acionar LED vermelho (injeção automática)
        }

        if(adc_value_X <= 3000 && adc_value_Y <= 3000){
            sleep_ms(40);
            ssd1306_fill(&ssd, !cor); // Limpa o display
            // Escrevendo PACIENTE ESTAVEL no display (sem nenhuma complicação médica)
            ssd1306_draw_string(&ssd, "PACIENTE", 30, 18); 
            ssd1306_draw_string(&ssd, "ESTAVEL", 34, 38);
            ssd1306_send_data(&ssd); // Atualiza o display
            gpio_put(LED_vermelho, LED_off); // Garante que os LED vermelho esteja apagado
            gpio_put(LED_azul, LED_off); // Garante que os LED azul esteja apagado
            pisca_LED_verde(2000); // Pisca o LED verde por 2 segundos (a cada 0,5s)
        }
    }
}
