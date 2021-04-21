/*******************************************************************************
 FileName:     tacometro
 Dependencies: Veja os includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.61 + XC8 1.41
 Company:      FATEC Santo Andre
 Author:       Felipe Alves 
 Date:         21/04/2021
 Software License Agreement: Somente para fins didaticos
 *******************************************************************************
 File Description: O programa realiza a leitura de frequência. Seu principio de 
 funcionamento se baseia na contagem de pulsos a cada 0.1ms que é o tempo de interrupção do timer. 
 Cada pulso tem 0.1ms e é feito a contagem da duração do sinal até que haja uma interrupção que interrompa 
 a contagem. A contagem é encerrada por uma interrupção externa e na mesma função é salvo o número de pulsos
 em outra variável "frequenciadoRPM" e reseta-se a contagem "tempodoRPM" para que se inicie novamente, e também
 na mesma rotina é acionado o led RB7, e após N1 pulsos de interrupção do timer, o que é equivalente a 0,N1 ms
 o led é então desligado. Pois o meu RA modificado: 164156377, o N1 é então 7; o que significa 
 que a duração do pulso no led é de 0,7ms.

 Change History:
 1.0   21/04/2021  Versão inicial
 
*******************************************************************************/

// Includes do Compilador

#include <p18f4550.h>
#include <stdio.h>
#include <xc.h>
#include <pic18f4550.h>

// Includes do Projeto

#include"Config.h"
#include"displayLCD.h"
#include "config_4550.h"

//defines
#define Ent_PIP PORTBbits.RB0

#define botaoB1 PORTEbits.RE0
#define botaoB2 PORTEbits.RE1
#define botaoB3 PORTEbits.RE2

//Protótipos de funções

void delay_ms(unsigned int tempodeatraso);
void interrupt high_priority interrupcoesAlta(void);


//variáveis globais
unsigned long int ticktime = 0;
unsigned char basede1ms = 10;
unsigned char basede10ms= 100;
unsigned int basede100ms = 1000;
unsigned int basede1s = 10000;

unsigned long int tempodoRPM = 0; //Contador de intervalo de tempo da Frequência
unsigned long int frequenciadoRPM = 0; // Valor calculado da frequência
unsigned long int rotacao = 0; //Rotação do motor

    
void main(void)
{    
    //Texto das tela
    const unsigned char texto_0[] = "FATEC STO. ANDRE";
    const unsigned char texto_1[] = "  *TACOMETRO*   ";
    const unsigned char texto_2[] = "RPM =           ";
    const unsigned char texto_3[] = "TEMPO =         ";
    
    //variáveis do programa

    
    //parametrizacao I/O's
    LATA = 0x00;
    LATB = 0x00;
    TRISE = 0xFF;
    TRISB = 0x03;
    TRISC = 0x00;


    //parametrizacao ADC
    ADCON0 = 0b00000001;
    ADCON1 = 0x0D;
    ADCON2 = 0x8E;

    //parametrização TIMER0
    T0CON = 0b11000011; //parametrização
    TMR0 = 0x6A; //valor de estouro do contador
//    T0CONbits.TMR0ON = 1;
//    T0CONbits.T08BIT = 1;
//    T0CONbits.T0CS = 0;
//    T0CONbits.T0SE = 0;
//    T0CONbits.PSA = 0;
//    T0CONbits.T0PS2 = 0;
//    T0CONbits.T0PS0 = 1;
//    T0CONbits.T0PS1 = 1;


   //parametrização interrupção externa
    INTCONbits.RBIE = 0; //interrupções mascaradas desativadas
    INTCONbits.GIE =1;
    INTCONbits.INT0E = 1; // liga interrupção externa RB0
    INTCON2bits.INTEDG0 = 1; //interrupção por borda de subida
    INTCON3bits.INT1IE = 1; // liga a interrupção externa RB1
    INTCONbits.TMR0IE = 1; //Interrupção do timer ativada


    //Configuração do CCP - modo Captura de tempo com Timer 1
    PIE1bits.CCP1IE = 1; //Habilita interrupção do CCP1
    PIR1bits.CCP1IF = 0; //Apaga flag de interrupção
    CCP1CON = 0x05; //modo captura com borda de subida
    CCPR1 = 0x00; // inicializa o CCPR1
    TMR1IF = 0; //reseta o flag
    T1CON = 0x80; // Modo 16-bit no TMR1
    TMR1 = 0; //seta o valor do TIMER para 0
    TMR1ON = 1; // Liga o timer 1
    
    //inicia display faz saudação
    ConfiguraLCD();

    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(texto_0);

    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(texto_1);
    DesligaCursor();
    delay_ms(3000);
    LimpaDisplay();
    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(texto_2);
    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(texto_3);    
    //termino da saudação  
    
    
    
    
    //deispara o timer


    while(1)
    {
        delay_ms(100);
        rotacao = frequenciadoRPM/10;
        rotacao = 30000/rotacao;
        PosicaoCursorLCD(1,9);
        EscreveFraseRamLCD("       ");
        PosicaoCursorLCD(1,9);
        EscreveInteiroLCD(rotacao);
        PosicaoCursorLCD(2,9);
        EscreveFraseRamLCD("       ");
        PosicaoCursorLCD(2,9);
        EscreveInteiroLCD(frequenciadoRPM);
    
    }
    
    
    return;
}


/******************************************************************************               
* Funcao:  void interrupt high_priority interruption()
* Entrada:  void
* Saida:    Nenhuma (void)
* Descricao: Rotina de interrupção do pic
/******************************************************************************/
void interrupt high_priority interrupcoesAlta()
{
    if(TMR0IF)
    {
    if(INTCONbits.TMR0IF == 1)
     {
        INTCONbits.TMR0IF = 0;  //Desligando o flag para próxima interrupção
        tempodoRPM++;
        if(tempodoRPM >= 7)
        {
            LATBbits.LATB7 = 0;
        }
        TMR0L = 0x6A + TMR0L;     //Desconta o valor do resíduo
     }
     
// Área das interrupções Externas 0 e 1
     
     if(INT0IE && INT0IF)
    {
        INT0IF = 0;  //Libera interrupção
        frequenciadoRPM = tempodoRPM;
        tempodoRPM = 0;
        LATBbits.LATB7 = 1;
    }
 }

}
    
/******************************************************************************               
* Funcao:   delay_ms(unsigned int delaytime)
* Entrada:  unsigned int delaytime - recebe valores de 0 a 65536
* Saida:    Nenhuma (void)
* Descricao: Gera um atraso de tempo conforme o valor recebido de delaytime
             O tempo interno ? baseado no delay 1 ms. Assim, a cada loop temos o
 *           decremento da vari?vel delaytime, at? que ele atinja zero. Exemplo:
 *           se o delyatime = 500 a fun??o terminar? e retornar? para o programa
 *           principal ap?s 500 ms.
/******************************************************************************/
void delay_ms(unsigned int tempodeatraso)
{
    while(--tempodeatraso)
    {
        __delay_ms(1); //Gera um atraso de 1 ms
    }
}