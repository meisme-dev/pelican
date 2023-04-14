#include <device/display/terminal.h>
#include <device/serial/serial.h>

bool init_serial(uint16_t port) {
    trace();
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x0C); // 9600 baud
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7); 
    outb(port + 4, 0x0B);
    outb(port + 4, 0x1E);
    outb(port, 0xFF);

    if(inb(port) != 0xFF) {
        return false;
    }
    outb(port + 4, 0x0F);
    return true;
}

void serial_send(uint16_t port, const char c) {
    while((inb(port + 5) & 0x20) == 0);
    outb(port, c);
}
