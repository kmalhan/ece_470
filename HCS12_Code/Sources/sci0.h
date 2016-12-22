/* Header file for SCI0 Interface */

// Initialize SCI1
void SCI0_init(long eclock, long baud_rate);

// Place char and send
void SCI0_putchar(char c);

// Print string
void SCI0_print(char *s);

// Print String and go to new line
void SCI0_println(char *s);

// Print integer
void SCI0_printInt(int n);

// Get character from SCI1
unsigned char SCI0_getchar(void);

// print unsigned int
void SCI0_printInt(unsigned int n);

void int2str(unsigned int n, char s[]);

void reverse(char *s);
