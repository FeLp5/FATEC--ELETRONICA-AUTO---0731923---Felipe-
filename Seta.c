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
    unsigned int i = 16, d=8, a=0, m=0;
    /*unsigned int inc[4] = {16,32,64,128};
    unsigned int dec[4] = {8,4,2,1};*/
    ADCON1 = 0x0F;
    TRISB = 0b11111111;
    TRISD = 0b00000000;
    LATD = 0;
    delay_ms(200);
    while(1)
    {
    /*    if(!B1 && i<4){
            seta = inc[i];
            delay_ms(0);
            i++;
        }else if(i>3){
            i=0;
            seta = inc[i];
        }else if(B1){
            i=0;
            seta = 0;
        }
        
         if(!B2 && d<4){
            seta = dec[d];
            delay_ms(50);
            d++;
        }else if(d>3){
            d=0;
            seta = dec[d];
        }else if(B2){
            seta = 0;
        }*/
        
        if(!B1){
            seta = i<<a;
            delay_ms(50);
            a++;
            if(a==4){
                a=0;
            }
        }else{
            a=0;
            seta=0;
        }
        
        if(!B2){
            seta = d>>m;
            delay_ms(50);
            m++;
            if(m==4){
                m=0;
            }
        }else{
            m=0;
            seta=0;
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

void delay_ms(unsigned int tempo){
    
    while(--tempo)
    {
        __delay_ms(1);
    }
    
    return;
}