#if !defined(__MCOP_H__)
#define __MCOP_H__

// MCODE operand types as used in A41
// A: no operand
// D: TEF
// E: 0 to 13 (dec)
// F: 0 to F (hex)
// G: +0 to +63 and -1 to -64 (dec)
// H: 0000 to FFFF (hex)
// I: 0000 to FFFF (hex)
// J: 1 to 31 (dec)
// K: no operand
// L: 0 to 7
// M: 000 to FFF (hex)
// N: TEF
// O: 0000 to FFFF (hex)
// P: 0 to 64 (dec)
// Q: 000 to 3FF (hex)
// R: ???
// S: 0000 to FFFF (hex)
// T: 0000 to FFFF (hex)
// U: no operand
// ?: no operand
//
#define MCODE_OP_NONE1          'A'
#define MCODE_OP_TEF1           'D'
#define MCODE_OP_0_TO_13_DEC    'E'
#define MCODE_OP_0_TO_F_HEX     'F'
#define MCODE_OP_DISPLACEMENT   'G'
#define MCODE_OP_ADDRESS1       'H'
#define MCODE_OP_ADDRESS2       'I'
#define MCODE_OP_1_TO_31_DEC    'J'
#define MCODE_OP_NONE2          'K'
#define MCODE_OP_0_TO_7         'L'
#define MCODE_OP_000_TO_FFF_HEX 'M'
#define MCODE_OP_TEF2           'N'
#define MCODE_OP_ADDRESS3       'O'
#define MCODE_OP_0_TO_64_DEC    'P'
#define MCODE_OP_000_TO_3FF_HEX 'Q'
#define MCODE_OP_UNKNOWN        'R'
#define MCODE_OP_ADDRESS4       'S'
#define MCODE_OP_ADDRESS5       'T'
#define MCODE_OP_NONE3          'U'
#define MCODE_OP_NONE4          '?'

struct mnemonic_operand_it_t {
    char operand_type;
    int index;
};

typedef struct mnemonic_operand_it_t mnemonic_operand_it;

const char *mnemonic_operand_first(mnemonic_operand_it *it, int operand_type);
const char *mnemonic_operand_next(mnemonic_operand_it *it);

#endif // !defined(__MCOP_H__)
