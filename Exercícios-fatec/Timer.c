/*******************************************************************************
 FileName:     Leitura de entrada anal�gica e conversor AD - FATEC SANTO ANDR�
 Dependencies: Veja a se��o de includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.65 + XC8 1.40
 Company:      FATEC Santo Andre
 Author:       Prof. Edson Kitani
 Date:         07/10/2020
 Software License Agreement: Somente para fins did�ticos
 *******************************************************************************
 File Description: Este programa simula um cluster veicular, monitorando entrada de conversor AD 
 * informando se o tanque est� na reserva ou n�o, e simula de acordo com os bot�es B0 e B1 o controle
 * de velocidade ou aumento de rota��es.
 * 
 Change History:
 1.0   21/11/2020  Vers�o inicial
 
*******************************************************************************/

// Includes do Compilador

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

// Includes do Projeto

#include"Config.h"
#include"displayLCD.h"

#define B0 PORTBbits.RB0
#define B1 PORTBbits.RB1
#define B2 PORTBbits.RB2
#define LED_RB7 PORTBbits.RB7

//Prot�tipos das Fun��es

void delay_ms(unsigned int tempodeatraso);
void chave_comando();


//vari�veis globais
unsigned char flag_controle;
unsigned int incremento_rpm;
unsigned int incremento_velocidade;
unsigned int tanque;

void main(void)
{    
    //constantes do projeto
    const unsigned char texto_0[] = "FATEC STO. ANDRE";
    const unsigned char texto_1[] = "CLUSTER VEICULAR";
    const unsigned char texto_4[] = "KM/h";
    const unsigned char texto_3[] = "RPM";
    
    unsigned int AN0;
   
    //parametrizacao
    ADCON0 = 0b00000001;
    ADCON1 = 0x0E;
    ADCON2 = 0x8E;
    TRISB = 0x00;
    
    
    

    
    //saudacao do programa
    ConfiguraLCD();
    DesligaCursor();
    PosicaoCursorLCD(1,1);
    EscreveFraseRamLCD(texto_0);
    PosicaoCursorLCD(2,1);
    EscreveFraseRamLCD(texto_1);
    delay_ms(1000);
    
    
    while(1)
    
    {      
        ADCON0 = 0b00000011;//Ativa a leitura
        while(ADCON0bits.DONE)//Enquanto a leitura n�o terminar no display nao aparecera        
        {     
        }
        tanque = (ADRESH<<8) + ADRESL;//
        if(tanque >= 308){
            LED_RB7 = !LED_RB7;
            delay_ms(100);
        }
        
        
        if(!B1)
        {
            flag_controle = 1;
            chave_comando();
        }
        
        if(!B0){
            if(flag_controle == 1){
                if(incremento_velocidade != 200){
                    incremento_velocidade = incremento_velocidade + 100;                    
                }
                if(incremento_rpm != 8000){
                    incremento_rpm = incremento_rpm + 50;                   
                }

            }else{
                if(incremento_velocidade != 0){
                    incremento_velocidade = incremento_velocidade - 100;
                }
                incremento_rpm = incremento_rpm + 50;
            } 
                PosicaoCursorLCD(1,1);
                EscreveInteiroLCD(incremento_velocidade);
                PosicaoCursorLCD(2,1);
                EscreveInteiroLCD(incremento_rpm);
        }else{
                if(incremento_velocidade != 0){
                    incremento_velocidade = incremento_velocidade - 100;
                }
                if(incremento_rpm != 700){
                    incremento_rpm = incremento_rpm - 50;
                } 
                
                PosicaoCursorLCD(1,1);
                EscreveInteiroLCD(incremento_velocidade);
                PosicaoCursorLCD(2,1);
                EscreveInteiroLCD(incremento_rpm);
        }

        
        
    }
    
    
    return;
    
}
/******************************************************************************               
* Funcao:   void chave_comando ()
* Entrada:  void
* Saida:    Nenhuma (void)
* Descricao: Altera o flag como se fosse o ponto morto do carro
/******************************************************************************/





void chave_comando(){
    switch(flag_controle){
        case 1:
            flag_controle = !flag_controle;
        break;
        
        case 0:
            flag_controle = !flag_controle;
        break;       
    }
    
    
    
}
    
/******************************************************************************               
* Funcao:   delay_ms(unsigned int delaytime)
* Entrada:  unsigned int delaytime - recebe valores de 0 a 65536
* Saida:    Nenhuma (void)
* Descricao: Gera um atraso de tempo conforme o valor recebido de delaytime
             O tempo interno � baseado no delay 1 ms. Assim, a cada loop temos o
 *           decremento da vari�vel delaytime, at� que ele atinja zero. Exemplo:
 *           se o delyatime = 500 a fun��o terminar� e retornar� para o programa
 *           principal ap�s 500 ms.
/******************************************************************************/
void delay_ms(unsigned int tempodeatraso)
{
    while(--tempodeatraso)
    {
        __delay_ms(1); //Gera um atraso de 1 ms
    }
}