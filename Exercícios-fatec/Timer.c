/*******************************************************************************
 FileName:     TIMER - FATEC SANTO ANDRÉ
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
 1.0   21/10/2020  Versão inicial
 
*******************************************************************************/
//includes do compilador

#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>



//includes do projeto

#include "Config.h"
#include "displayLCD.h"


//define's
#define Led_0 PORTBbits.RB0

//funções de subrotina


void delay_ms(unsigned int tempo);

//main
void main(void) {
    
    TRISB = 0x00;


    TMR0ON = 1;//liga o timer 
    T08BIT = 1;//seleciona o modo de 8bits   
    T0CS = 0; //seleciona o clock
    T0PS2 = 1; //define o prescaler para 1:256
    T0PS1 = 1;    
    T0PS0 = 1;   
    PSA = 0; //seleciona o sinal do prescaler
    TMR0L = 22; //carrega o timer com 22 para contar de 22 até 256
    
    while(1)
    {
        
        if(TMR0IF == 1)
        {
            TMR0L = 22;
            TMR0IF = 0; //seta em zero para que ocorra sempre que o contador estourar
            Led_0 = !Led_0;
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
