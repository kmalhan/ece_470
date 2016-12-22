/* Header file for SCI1 Interface */

// Initialize SCI1
void SCI1_init(long eclock, long baud_rate);

// Place char and send
void SCI1_putchar(char c);

// Print string
void SCI1_print(char *s);

// Print String and go to new line
void SCI1_println(char *s);

// Print integer
void SCI1_printInt(int n);

// Get character from SCI1
unsigned char SCI1_getchar(void);