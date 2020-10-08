/*******************************************************************************
 FileName:     Leitura de entrada analógica e conversor AD - FATEC SANTO ANDRÉ
 Dependencies: Veja a seção de includes
 Processor:    PIC18F4550
 Compiler:     MPLAB X v3.65 + XC8 1.40
 Company:      FATEC Santo Andre
 Author:       Prof. Edson Kitani
 Date:         07/10/2020
 Software License Agreement: Somente para fins didáticos
 *******************************************************************************
 File Description: O programa parametriza as entradas analógicas, faz a leitura delas e converte o valor de 
 tensão e devolve no display. O programa está configurado para trabalhar no PicsimLab (simulador). E sua saída
 é configurada para um display de 16x2 * 
 Change History:
 1.0   07/10/2020  Versão inicial
 2.0   08/10/2020  Medidor de Tensao(Voltímetro)
 
*******************************************************************************/

// Includes do Compilador

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

// Includes do Projeto

#include"Config.h"
#include"displayLCD.h"

#define B1 PORTBbits.RB0
#define servo PORTBbits.RB7

//Protótipos das Funções

void delay_ms(unsigned int tempodeatraso);

void main(void)
{    
    //constantes do projeto
    const unsigned char texto_0[] = "FATEC STO. ANDRE";
    const unsigned char texto_1[] = "Entr.  Analogica";
    const unsigned char texto_2[] = "ADRESH=         ";
    const unsigned char texto_3[] = "ADRESL=   |     ";
    const unsigned char texto_4[] = "|AN0=           ";
    
    float valor_AN0;
   
    //parametrizacao
    ADCON0 = 0b00000001;
    ADCON1 = 0x0E;
    ADCON2 = 0x8E;
    TRISB = 0b00001111;
    
    
    

    
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
        while(ADCON0bits.DONE)//Enquanto a leitura não terminar no display nao aparecera        
            {     
            } 
       

        
/*
        //Escreve ADRESH e ADRESL respectivamente
        PosicaoCursorLCD(1,1);
        EscreveFraseRamLCD(texto_2);
        PosicaoCursorLCD(1,8);
        EscreveInteiroLCD(ADRESH);
        PosicaoCursorLCD(2,1);
        EscreveFraseRamLCD(texto_3);
        PosicaoCursorLCD(2,8);
        EscreveInteiroLCD(ADRESL);
*/  
        // monta uma word       
        /*LimpaDisplay();
        PosicaoCursorLCD(1,1);
        EscreveFraseRamLCD(texto_4);
        PosicaoCursorLCD(1,6);
        EscreveInteiroLCD(AN0);
        delay_ms(2000);*/  
        
        
        
        
        //Escreve a tensao convertida para binario em decimal e os separa no display
        valor_AN0 = ((ADRESH << 8) + ADRESL)*5;
        valor_AN0 /= 1023;
        PosicaoCursorLCD(1,1);
        LimpaDisplay();
        EscreveFloatLCD(valor_AN0,'2');
        PosicaoCursorLCD(1,8);
        EscreveCaractereLCD('V');
        delay_ms(1000); 
        

        
        
        
    }
    
    
    return;
    
}




/******************************************************************************               
* Funcao:   delay_ms(unsigned int delaytime)
* Entrada:  unsigned int delaytime - recebe valores de 0 a 65536
* Saida:    Nenhuma (void)
* Descricao: Gera um atraso de tempo conforme o valor recebido de delaytime
             O tempo interno é baseado no delay 1 ms. Assim, a cada loop temos o
 *           decremento da variável delaytime, até que ele atinja zero. Exemplo:
 *           se o delyatime = 500 a função terminará e retornará para o programa
 *           principal após 500 ms.
/******************************************************************************/

void delay_ms(unsigned int tempodeatraso)
{
    while(--tempodeatraso)
    {
        __delay_ms(1); //Gera um atraso de 1 ms
    }
}
