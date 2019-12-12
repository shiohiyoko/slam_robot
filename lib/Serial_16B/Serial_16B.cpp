#include "Serial_16B.h"
#include<vector>
Serial_16B::Serial_16B(PinName tx, PinName rx) : serial_mbed(tx,rx)
{
    UART_tx = tx;
    UART_rx = rx;
}

void Serial_16B::write(vector<unsigned char>& buffer, unsigned int length)
{
    if(UART_tx == p13) { //p13    p0.15   TXD1
        for(int i=0; i < length ; i++) UART1_RBR_THR = buffer[i];
    } else if(UART_tx == p28) { //p28  p0.10   TXD2
        for(int i=0; i < length ; i++) UART2_RBR_THR = buffer[i];
    } else if(UART_tx == p9) { //p9   p0.0    TXD3
        for(int i=0; i < length ; i++) UART3_RBR_THR = buffer[i];
    } else if(UART_tx == USBTX) { //USBTX   TXD0
        for(int i=0; i < length ; i++) UART0_RBR_THR = buffer[i];
    }
}
unsigned char Serial_16B::get( void )
{
    if(UART_rx == p14) { //p14    p0.16   RXD1
        if((UART1_LSR & 0x01) == 0x01) {
            return UART1_RBR_THR;
        }
    } else if(UART_rx == p27) { //p27  p0.11   RXD2
        if((UART2_LSR & 0x01) == 0x01) {
            return UART2_RBR_THR;
        }
    } else if(UART_rx == p10) { //p10  p0.1    RXD3
        if((UART3_LSR & 0x01) == 0x01) {
            return UART3_RBR_THR;
        }
    } else if(UART_rx == USBRX) { //USBRX   RXD0
        if((UART0_LSR & 0x01) == 0x01) {
            return UART0_RBR_THR;
        }
    }
    return 0;
}
bool Serial_16B::readable( void )
{
    if(UART_rx == p14) { //p14    p0.16   RXD1
        if((UART1_LSR & 0x01) == 0x01) {
            return true;
        }
    } else if(UART_rx == p27) { //p27  p0.11   RXD2
        if((UART2_LSR & 0x01) == 0x01) {
            return true;
        }
    } else if(UART_rx == p10) { //p10  p0.1    RXD3
        if((UART3_LSR & 0x01) == 0x01) {
            return true;
        }
    } else if(UART_rx == USBRX) { //USBRX   RXD0
        if((UART0_LSR & 0x01) == 0x01) {
            return true;
        }
    }
    return false;
}
void Serial_16B::baud(int baudrate)
{
    serial_mbed.baud(baudrate);
}