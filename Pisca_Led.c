/*******************************************************************************
 FileName:     Pisca_Led - FATEC SANTO ANDRÉ
 Dependencies: Veja a seção de includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.51 + XC8 1.41
 Company:      FATEC Santo Andre
 Author:       Felipe Alves Leite Da Silva
 Date:         03/09/2020
 Software License Agreement: Somente para fins didáticos
 *******************************************************************************
 File Description: Este programa gera um pisca pisca numa saída determinada.
 Change History:
 1.0   03/09/2020  Versão inicial
 
*******************************************************************************/


//Includes do compilador

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

//Includes do projeto

#include "Config.h"

//Define's



#define RB7         LATBbits.LATB7

void delay_ms(unsigned int tempo);
void main(void) 
{
    ADCON1 = 0x0F;
    TRISB= 0x00;
    RB7=0;
      
    while(1)
    {
        RB7 = 1;
        delay_ms(500);
        RB7 = 0;
        delay_ms(500);  
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

