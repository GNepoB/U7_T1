# PROJETO FINAL - Sistema Automatizado de Resposta a Emergências em UTI com RP2040

## Aluno
Gabriel Neponuceno Batista

## 📺 Demonstração + Instruções de uso

- **Simulação no Wokwi:** [Assista no YouTube](https://www.youtube.com/watch?v=pyIGGnow8kM&ab_channel=GABRIELNEPONUCENOBATISTA)

## Funcionalidades do Projeto

O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:

Simular chances do paciente internados na UTI estar sofrendo de complicações como de Bradicardia ou Choque circulatório, por meio dos aparelhos que monitoram seus sinais vitais, através da variação dos eixos X e Y do Joystick.

Caso o RP2040 identifique complicações médicas no paciente, uma bomba de infusão injetará automaticamente uma medicação para tratar a complicação detectada. Esta etapa é simulada pela ativacão de LED's

Além disso, ao identificar qualquer complicação, o RP2040 ativa também um alarme sonoro de emergência, soando como uma sirene, através do Buzzer.

Como completmentação, uma mensagem é exibida no Display como forma de acompanhar o que está acontecendo com o paciente.

## Justificativa do projeto

Este projeto visa automatizar, acelerar e facilitar a resposta as complicações geralmente detectadas nos pacientes das UTI's. Fazendo assim com que o índice de sobrevida dos pacientes internados possam aumentar consideravelmente, além de aliviar o trabalho de profissionais da saúde, que, por sua vez, é (geralmente) muito intenso.
