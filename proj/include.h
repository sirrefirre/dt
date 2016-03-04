//made by Simon Westin and Yukki Nakamura
void init( void );
void startscreen(void);
void startmessages(void);
void gameover(void);
void victory(void);
/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
extern char textbuffer[4][16];
/* Written as part of i/o lab: getbtns, getsw */
int getbtns(void);
int getsw(void);

void display_init(void);
void display_string(int line, char *s);
void display_stringright(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from display.c */
void quicksleep(int cyc);

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Written as part of asm lab: delay */
void delay(int);
