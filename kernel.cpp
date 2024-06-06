#include <stdint.h>

// Global VGA index
int vga_index = 0xb8000;

// Define the VGA width (number of columns)
#define VGA_WIDTH 80

// putchar(character, address) writes character to address in video memory
void putchar(char c, int address, char attribute) {
    if (c == '\n') {
        // Move to the start of the next line
        int row_offset = ((vga_index - 0xb8000) / 2) / VGA_WIDTH;
        vga_index = 0xb8000 + ((row_offset + 1) * VGA_WIDTH * 2);
    } else {
        *(char*)address = c;
        *(char*)(address + 1) = attribute; // Write the attribute byte
        vga_index += 2;
    }
}

// putstr(string, address) writes string to address in video memory. Loops over the string and puts each character on the screen with putchar
int putstr(char* str, int address, int attribute) {
    int returnlen = 0x0000;
    while (*str != '\0') {
        putchar(*str, address, attribute); 
        if (*str == '\n') {
            // If a newline character is found, adjust the address to the new vga_index
            address = vga_index;
        } else {
            address += 2;
            returnlen += 2;
        }
        str++;
    }
    return returnlen;
}

// clearScreenSpace(startAddr, endAddr) clears the screen from startAddr to endAddr
void clearScreenSpace(int startAddr, int endAddr, int color = 0x01) {
    for (int i = startAddr; i < endAddr; i += 2) {
        *(char*)i = 0x00;
        *(char*)(i + 1) = color;
    }
    return;
}

// newLineManual() lets you manually add a newline to the screen without going through the putstr function
void newLineManual() {
    int row_offset = ((vga_index - 0xb8000) / 2) / VGA_WIDTH;
    vga_index = 0xb8000 + ((row_offset + 1) * VGA_WIDTH * 2);
    return;
}

// putBox(address, color) sets that address in vga video memory to use that color for the text and background. basically drawing a box
void putBox(int address, int color) {
    *(char*)(address) = 0x00;
    if (color == 0) {*(char*)(address + 1) = 0x00;}
    if (color == 1) {*(char*)(address + 1) = 0x11;}
    if (color == 2) {*(char*)(address + 1) = 0x22;}
    if (color == 3) {*(char*)(address + 1) = 0x33;}
    if (color == 4) {*(char*)(address + 1) = 0x44;}
    if (color == 5) {*(char*)(address + 1) = 0x55;}
    if (color == 6) {*(char*)(address + 1) = 0x66;}
    if (color == 7) {*(char*)(address + 1) = 0x77;}
    if (color == 8) {*(char*)(address + 1) = 0x88;}
    if (color == 9) {*(char*)(address + 1) = 0x99;}
    if (color == 10) {*(char*)(address + 1) = 0xaa;}
    if (color == 11) {*(char*)(address + 1) = 0xbb;}
    if (color == 12) {*(char*)(address + 1) = 0xcc;}
    if (color == 13) {*(char*)(address + 1) = 0xdd;}
    if (color == 14) {*(char*)(address + 1) = 0xee;}
    if (color == 15) {*(char*)(address + 1) = 0xff;}
    return;
}

extern "C" void main() {
    // Clear the screen and reset the vga index
    clearScreenSpace(0xb8000, 0xb8000 + 0x2000);
    vga_index = 0xb8000;

    // Print some text in differant colors
    char demo_red_text[] = "Hello, World!\n";
    putstr(demo_red_text, vga_index, 0x01);

    char demo_green_text[] = "Hello, World!\n";
    putstr(demo_green_text, vga_index, 0x02);

    char demo_blue_text[] = "Hello, World!\n";
    putstr(demo_blue_text, vga_index, 0x04);

    char demo_yellow_text[] = "Hello, World!\n";
    putstr(demo_yellow_text, vga_index, 0x06);

    char demo_magenta_text[] = "Hello, World!\n";
    putstr(demo_magenta_text, vga_index, 0x05);

    char demo_cyan_text[] = "Hello, World!\n";
    putstr(demo_cyan_text, vga_index, 0x03);

    // TODO:BUG
    // Triggered with: Using the newline function 3 times in a row.
    // Known Info: Commenting out the screen clearing at the top of the kernels main function it works fine.
    // newLineManual();
    // newLineManual();
    // newLineManual();

    // Smiley face with boxes
    putBox(vga_index + 0x10, 0x1); // Eyes
    putBox(vga_index + 0x16, 0x1); // Eyes
    newLineManual();
    newLineManual();
    putBox(vga_index + 0x10, 0x1); // Mouth top
    putBox(vga_index + 0x16, 0x1); // Mouth top
    newLineManual();
    putBox(vga_index + 0x10, 0x1); // Mouth bottom
    putBox(vga_index + 0x14, 0x1); // Mouth bottom
    putBox(vga_index + 0x12, 0x1); // Mouth bottom
    putBox(vga_index + 0x16, 0x1); // Mouth bottom

    while (1) {}
    return;
}
