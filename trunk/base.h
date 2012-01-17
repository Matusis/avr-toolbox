#ifndef __BASE
#define __BASE

#define cbi(byt, bit)   (byt &= ~_BV(bit))
#define sbi(byt, bit)   (byt |= _BV(bit))
#define chb(byt, bit)   (byt & _BV(bit))
#define inb(byt, bit)   (byt ^= _BV(bit))

#define hi8(wrd) (wrd >> 8)
#define lo8(wrd) (wrd & 0xff)

#define bsd2as_hi(bsd)	( ((bsd & 0xf0) >> 4) + 0x30)
#define bsd2as_lo(bsd)	( (bsd & 0x0f) + 0x30)

#endif
