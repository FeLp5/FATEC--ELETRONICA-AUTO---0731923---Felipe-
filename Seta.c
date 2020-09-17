/*******************************************************************************
 FileName:     Seta - FATEC SANTO ANDRÉ
 Dependencies: Veja a seção de includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.51 + XC8 1.41
 Company:      FATEC Santo Andre
 Author:       Felipe Alves Leite Da Silva
 Date:         17/09/2020
 Software License Agreement: Somente para fins didáticos
 *******************************************************************************
 File Description: Este programa simula a seta do carro, fazendo o led piscar em loop em função de um botão.
 Change History:
 1.0   17/09/2020  Versão inicial
 
*******************************************************************************/


//Includes do compilador

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

//Includes do projeto

#include "Config.h"

//Define's



#define seta LATD
#define B1 PORTBbits.RB0
#define B2 PORTBbits.RB1


void delay_ms(unsigned int tempo);
void main(void) 
{  
    ADCON1 = 0x0F;
    TRISB = 0b11111111;
    TRISD = 0b00000000;
    delay_ms(100);
    while(1)
    {
        if(B1==0)
        {
            seta = 240;
            delay_ms(500);
            seta = 0;
            delay_ms(500);
        }else if(B2==0)
        {
            seta = 15;
            delay_ms(500);
            seta = 0;
            delay_ms(100);
        }else{
            seta = 0;
            
        }
    }
    
    
    return;
}

/******************************************************************************               
* Funcao:   delay_ms(unsigned int tempo)
* Entrada:  unsigned int tempo - recebe valores de 0 a 65536
* Saida:    Nenhuma (void)
* Descricao: Gera um atraso de tempo conforme o valor recebido de delaytime
             O tempo interno é baseado no delay 1 ms. Assim, a cada loop temos o
 *           decremento da variável delaytime, até que ele atinja zero. Exemplo:
 *           se o delyatime = 500 a função terminará e retornará para o programa
 *           principal após 500 ms.
/******************************************************************************/
void delay_ms(unsigned int tempo)
{
    while(--tempo)
    {
        __delay_ms(1);//gera um atraso de 1ms
    }  
}