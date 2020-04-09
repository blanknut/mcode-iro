
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcinstr.h"

#define SET_HP          0x1
#define SET_JDA         0x2
#define SET_ZENCODE     0x4

#define TYPE_COUNT      20  // strlen(mnemonic_types)

static const char *mnemonic_types = "ADEFGHIJKLMNOPQRSTU?";

struct mnemonic_t {
    char *hp;
    char *jda;
    char *zencode;
};

typedef struct mnemonic_t *mnemonic;

struct mnemonic_variants_t {
    mnemonic m[TYPE_COUNT];
};

typedef struct mnemonic_variants_t *mnemonic_variants;

static mnemonic_variants mvtab[0x400];

static void clear_mnemonics()
{
    for (int code = 0; code < 0x400; code++) {
        mvtab[code] = 0;
    }
}

static int get_variant(int ch)
{
    const char *pch = strchr(mnemonic_types, ch);
    if (pch) {
        return pch - mnemonic_types;
    } else {
        return -1;
    }
}

static void add_mnemonic(char **buf, const char *name)
{
    if (*buf) {
        char *tmp = *buf;
        *buf = malloc(strlen(tmp) + strlen(name) + 2);
        if (*buf) {
            strcpy(*buf, tmp);
            strcat(*buf, "|");
            strcat(*buf, name);
        }
        free(tmp);
    } else {
        *buf = strdup(name);
    }
}

static mnemonic add_mnemonic_variant(int code, int variant)
{
    if (!mvtab[code]) {
        mvtab[code] = (mnemonic_variants) malloc(sizeof(struct mnemonic_variants_t));
        if (mvtab[code]) {
            for (int i = 0; i < TYPE_COUNT; i++) {
                mvtab[code]->m[i] = 0;
            }
        } else {
            return 0;
        }
    }
    if (!mvtab[code]->m[variant]) {
        mvtab[code]->m[variant] = (mnemonic) malloc(sizeof(struct mnemonic_t));
        if (mvtab[code]->m[variant]) {
            mvtab[code]->m[variant]->hp = 0;
            mvtab[code]->m[variant]->jda = 0;
            mvtab[code]->m[variant]->zencode = 0;
        }
    }
    return mvtab[code]->m[variant];
}

void read_mnemonics() {
    for (int i  = 0; i < IHT_SIZE; i++) {
        int code = inst[i].tyte1;
        int variant = get_variant(inst[i].typ);
        if (variant >= 0) {
            mnemonic m = add_mnemonic_variant(code, variant);
            if (m) {
                if (inst[i].set & SET_HP) {
                    add_mnemonic(&m->hp, inst[i].name);
                }
                if (inst[i].set & SET_JDA) {
                    add_mnemonic(&m->jda, inst[i].name);
                }
                if (inst[i].set & SET_ZENCODE) {
                    add_mnemonic(&m->zencode, inst[i].name);
                }
            }
        }
    }
}

static const char *get_operand(const int optype)
{
    switch (optype) {
        case 'A':
        case 'K':
        case 'U':
        case '?': return "none";
        case 'D':
        case 'N': return "TEF";
        case 'E': return "0 to 13 (dec)";
        case 'F': return "0 to F (hex)";
        case 'G': return "+0 to +63 and -1 to -64 (dec)";
        case 'H':
        case 'I':
        case 'S':
        case 'T': return "0000 to FFFF (hex)";
        case 'J': return "1 to 31 (dec)";
        case 'L': return "0 to 7";
        case 'M': return "000 to FFF (hex)";
        case 'O': return "0000 to FFFF (hex)";
        case 'P': return "0 to 64 (dec)";
        case 'Q': return "000 to 3FF (hex)";
        case 'R': return "???";
        default : return "unknown";
    }
}

static void print_string(const char *name, int last)
{
    if (name) {
        printf("%s", name);
    }
    if (!last) {
        printf(";");
    }
}

static void print_char(const char ch, int last)
{
    if (ch) {
        printf("%c", ch);
    }
    if (!last) {
        printf(";");
    }
}

static void print_hex(int num, int last)
{
    printf("0x%3.3X", num);
    if (!last) {
        printf(";");
    }
}

static void print_mnemonics() {
    printf("code;hp;jda;zencode;typ;operand\n");
    for (int i = 0; i < 0x400; i++) {
        if (mvtab[i]) {
            for (int j = 0; j < TYPE_COUNT; j++) {
                mnemonic m = mvtab[i]->m[j];
                if (m) {
                    if (m->hp || m->jda || m->zencode) {
                        print_hex(i, 0);
                        print_string(m->hp, 0);
                        print_string(m->jda, 0);
                        print_string(m->zencode, 0);
                        print_char(mnemonic_types[j], 0);
                        print_string(get_operand(mnemonic_types[j]), 1);
                        printf("\n");
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    clear_mnemonics();
    read_mnemonics();
    print_mnemonics();
    return 0;
}