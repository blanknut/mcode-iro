/**********************************************************************
 * MCODE Syntax Hightlighing
 * 2019 by Jurgen Keller <jkeller@gmx.ch>
 *
 * This small program generates a HP-41 MCODE syntax hightlighting
 * grammar for Iro (https://eeyo.io/iro/). Iro allows to export the
 * syntax highlighter for different editors.
 *
 * I took the liberty to copy the MCODE instruction inventory from
 * the HP-41 Software Development Kit (SDK41), see http://hp41.org.
 *********************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcop.h"

#define MNEMONIC_COUNT              0x400
#define MAX_MNEMONIC_SIZE           20
#define MAX_MNEMONIC_PATTERN_SIZE   2*MAX_MNEMONIC_SIZE

#define INDENT      "   "
#define EQUAL_POS   30

// style identifiers
#define STYLE_COMMENT       "comment"
#define STYLE_ANNOTATION    "annotation"
#define STYLE_ERROR         "error"
#define STYLE_DIRECTIVE     "directive"
#define STYLE_LABEL         "label"
#define STYLE_MNEMONIC      "mnemonic"
#define STYLE_OPERAND       "operand"
#define STYLE_STRING        "string"
#define STYLE_DECIMAL       "decimal"
#define STYLE_HEXADECIMAL   "hexadecimal"
#define STYLE_CODE          "code"

struct styledef_t {
    const char *name;
    const char *color;
    const char *textmateScope;
    const char *pygmentsScope;
} style_definitions[] = {
    { STYLE_COMMENT,     "grey",       "comment",                      "Comment"         },
    { STYLE_ANNOTATION,  "brown",      "comment.block.preprocessor",   "Comment.Preproc" },
    { STYLE_ERROR,       "red",        "message.error",                "Generic.Error"   },
    { STYLE_DIRECTIVE,   "violet",     "keyword.other",                "Name.Builtin"    },
    { STYLE_LABEL,       "green",      "variable.other",               "Name.Label"      },
    { STYLE_MNEMONIC,    "purple",     "keyword",                      "Keyword"         },
    { STYLE_OPERAND,     "gold",       "variable.language",            "Keyword.Pseudo"  },
    { STYLE_STRING,      "yellow",     "string",                       "String"          },
    { STYLE_DECIMAL,     "cyan",       "constant.numeric.decimal",     "Number.Integer"  },
    { STYLE_HEXADECIMAL, "light_blue", "constant.numeric.hexadecimal", "Number.Hex"      },
    { STYLE_CODE,        "orange",     "constant.numeric.code",        "Number.Bin"      },
    { 0,                 0,            0,                              0                 }
};

// context identifiers
#define CTX_MAIN                    "main"
#define CTX_COMMENT                 "comment"
#define CTX_ANNOTATION              "annotation"
#define CTX_DATA                    "data"
#define CTX_STRING                  "string"
#define CTX_DEC_NUMBER              "dec_number"
#define CTX_HEX_NUMBER              "hex_number"
#define CTX_ADDRESS                 "address"
#define CTX_CODE                    "code"
#define CTX_LOCAL_LABEL             "local_label"
#define CTX_GLOBAL_LABEL            "global_label"
#define CTX_SIMPLE_DIRECTIVE        "simple_directive"
#define CTX_STRING_DIRECTIVE        "string_directive"
#define CTX_NUMBER_DIRECTIVE        "number_directive"
#define CTX_ADDRESS_DIRECTIVE       "address_directive"
#define CTX_SYMBOL_DIRECTIVE        "symbol_directive"
#define CTX_CODE_LITERAL            "code_literal"
#define CTX_INSTRUCTION_NONE        "instruction_none"
#define CTX_INSTRUCTION_NUMBER      "instruction_number"
#define CTX_INSTRUCTION_ADDRESS     "instruction_address"
#define CTX_INSTRUCTION_REGISTER    "instruction_register"
#define CTX_INSTRUCTION_CLASS2      "instruction_class2"
#define CTX_INSTRUCTION_CLASS3      "instruction_class3"
#define CTX_INSTRUCTION_SPECIAL1    "instruction_special1"
#define CTX_INSTRUCTION_SPECIAL2    "instruction_special2"
#define CTX_TEF                     "tef"
#define CTX_REGISTER                "register"
#define CTX_POS_DISPLACEMENT        "positive_displacement"
#define CTX_NEG_DISPLACEMENT        "negative_displacement"

static const char *indent[] = {
    // only 4 levels of indenting needed
    INDENT,
    INDENT INDENT,
    INDENT INDENT INDENT,
    INDENT INDENT INDENT INDENT
};

static const int fw[] = {
    EQUAL_POS,
    EQUAL_POS - sizeof(INDENT),
    EQUAL_POS - 2*sizeof(INDENT),
    EQUAL_POS - 3*sizeof(INDENT),
    EQUAL_POS - 4*sizeof(INDENT),
};

// temporary buffer for collecting mnemonics
static const char *mnemonics[MNEMONIC_COUNT];


static void print_iro_context_begin(const char *name)
{
    printf("%s%s : context {\n", indent[0], name);
}

static void print_iro_context_end(void)
{
    printf("%s}\n\n", indent[0]);
}

static void print_iro_context_include(const char *name, const char *indent)
{
    printf("%s: include \"%s\";\n", indent, name);
}

static void print_iro_context_includes(const char **names, const char *indent)
{
    while (*names) {
        printf("%s: include \"%s\";\n", indent, *names);
        names++;
    }
}

static void print_iro_context_push_begin(void)
{
    printf("%s: inline_push {\n", indent[1]);
}

static void print_iro_context_push_end(void)
{
    printf("%s}\n", indent[1]);
}

static void print_iro_style_begin(const char *name)
{
    printf("%s.%s : style {\n", indent[0], name);
}

static void print_iro_style_end(void)
{
    printf("%s}\n\n", indent[0]);
}

static void print_iro_style_color(const char *name)
{
    printf("%s%-*s  = %s\n", indent[1], fw[2], "color", name);
}

static void print_iro_style_scopes(
    const char *ace_scope,
    const char *textmate_scope,
    const char *pygments_scope)
{
    printf("%s%-*s  = %s\n", indent[1], fw[2], "ace_scope", ace_scope);
    printf("%s%-*s  = %s\n", indent[1], fw[2], "textmate_scope", textmate_scope);
    printf("%s%-*s  = %s\n", indent[1], fw[2], "pygments_scope", pygments_scope);
}

static void print_iro_header(void)
{
    printf("%-*s  = hp41mcode\n", fw[0], "name");
    printf("%-*s  = src;\n", fw[0], "file_extensions []");
    printf("%-*s  = \"HP-41 MCODE Syntax Highlighter\";\n", fw[0], "description");
    printf("%-*s  = \"ec78fc6d-d744-485c-b450-ad86e83e4405\";\n", fw[0], "textmate_uuid");
    printf("\n");
}

static void print_iro_styles(struct styledef_t *styles)
{
    struct styledef_t *style = styles;
    printf("#-------------------------------------------------\n");
    printf("# Styles\n");
    printf("#-------------------------------------------------\n\n");
    printf("styles [] {\n");
    while (style->name) {
        print_iro_style_begin(style->name);
        print_iro_style_color(style->color);
        print_iro_style_scopes(
            style->textmateScope,
            style->textmateScope,
            style->pygmentsScope);
        print_iro_style_end();
        style++;
    }
    printf("}\n\n");
}

static void print_iro_pattern_context_multistyle(
    const char *name,
    const char *regex,
    const char **styles)
{
    print_iro_context_begin(name);
    printf("%s: pattern {\n", indent[1]);
    printf("%s%-*s \\= %s\n", indent[2], fw[3], "regex", regex);
    printf("%s%-*s  = .%s", indent[2], fw[3], "styles []", *styles++);
    while (*styles) {
        printf(", .%s", *styles++);
    } 
    printf(";\n%s}\n", indent[1]);
    print_iro_context_end();
}

static void print_iro_pattern_context(
    const char *name,
    const char *regex,
    const char *style)
{
    const char *styles[] = { style, 0 };
    print_iro_pattern_context_multistyle(name, regex, styles);
}

static void print_iro_push_context(
    const char *name,
    const char *regex_start,
    const char *regex_end,
    const char *style)
{
    print_iro_context_begin(name);
    print_iro_context_push_begin();
    printf("%s%-*s \\= (%s)\n", indent[2], fw[3], "regex", regex_start);
    printf("%s%-*s  = .%s;\n", indent[2], fw[3], "styles []", style);
    printf("%s%-*s  = .%s\n", indent[2], fw[3], "default_style", style);
    printf("%s: pop {\n", indent[2]);
    printf("%s%-*s \\= (%s)\n", indent[3], fw[4], "regex", regex_end);
    printf("%s%-*s  = .%s;\n", indent[3], fw[4], "styles []", style);
    printf("%s}\n", indent[2]);
    print_iro_context_push_end();
    print_iro_context_end();
}

static void print_iro_push_eol_context(
    const char *name,
    const char *regex,
    const char *style,
    const char **includes)
{
    print_iro_context_begin(name);
    print_iro_context_push_begin();
    printf("%s%-*s \\= (%s)\n", indent[2], fw[3], "regex", regex);
    printf("%s%-*s  = .%s;\n", indent[2], fw[3], "styles []", style);
    printf("%s: eol_pop {}\n", indent[2]);
    print_iro_context_includes(includes, indent[2]);
    print_iro_context_push_end();
    print_iro_context_end();
}

static void print_iro_context_comment(void)
{
    print_iro_pattern_context(CTX_COMMENT, "(;.*)", STYLE_COMMENT);
}

static void print_iro_context_annotation(void)
{
    const char *styles[] = {
        STYLE_ERROR,
        STYLE_ANNOTATION,
        0  
    };
    print_iro_pattern_context_multistyle(
        CTX_ANNOTATION,
        "(\\*\\*\\* ERROR.*)|(\\*.*)",
        styles);
}

static void print_iro_context_string(void)
{
    print_iro_pattern_context(CTX_STRING, "(\\\"[^\\\"]*\\\")", CTX_STRING);
}

static void print_iro_context_number(void)
{
    print_iro_pattern_context(CTX_DEC_NUMBER, "(\\d+\\b)", STYLE_DECIMAL);
    print_iro_pattern_context(CTX_HEX_NUMBER, "([0-9A-F]+\\b)", STYLE_HEXADECIMAL);
}

static void print_iro_context_address(void)
{
    print_iro_pattern_context(CTX_ADDRESS, "([0-9A-F]{4}\\b)", STYLE_HEXADECIMAL);
}

static void print_iro_context_code(void)
{
    print_iro_pattern_context(CTX_CODE, "([0-3][0-9A-F]{2}\\b)", STYLE_CODE);
}

static void print_iro_context_data(void)
{
    print_iro_context_begin(CTX_DATA);
    printf("%s: pattern {\n", indent[1]);
    printf("%s%-*s \\= ([0-9A-F]{4}\\s+)((?:[0-3][0-9A-F]{2}){1,3})\n", indent[2], fw[3], "regex");
    printf("%s%-*s  = .%s, .%s;\n", indent[2], fw[3], "styles []", STYLE_HEXADECIMAL, STYLE_CODE);
    printf("%s}\n", indent[1]);
    print_iro_context_end();
}

static void print_iro_context_tef(void)
{
    print_iro_pattern_context(
        CTX_TEF,
        "([P[QT^-]|XS?|W(PT)?|MS?|S(&X)?|ALL|@R|R<|P-Q)",
        STYLE_OPERAND);
}

static void print_iro_context_register(void)
{
    print_iro_pattern_context(
        CTX_REGISTER,
        "((\\d{1,2}|[0-9A-F])(\\([TZYXLMNOPQabcde]\\))?(/[TZYXLMNOPQabcde])?)",
        STYLE_OPERAND);
}

static void print_iro_context_positive_displacement(void)
{
    print_iro_pattern_context(
        CTX_POS_DISPLACEMENT,
        "(\\+(?:(?:[1-5]\\d)|(?:6[0-3])|\\d))",
        STYLE_OPERAND);
}

static void print_iro_context_negative_displacement(void)
{
    print_iro_pattern_context(
        CTX_NEG_DISPLACEMENT,
        "(\\-(?:(?:[1-5]\\d)|(?:6[0-4])|[1-9]))",
        STYLE_OPERAND);
}

static void print_iro_context_local_label(void)
{
    print_iro_pattern_context(
        CTX_LOCAL_LABEL,
        "(\\([^\\)]+\\))",
        STYLE_LABEL);
}

static void print_iro_context_global_label(void)
{
    print_iro_pattern_context(
        CTX_GLOBAL_LABEL,
        "(\\[[^\\]]+\\])",
        STYLE_LABEL);
}

static void print_iro_context_simple_directive(void)
{
    const char *includes[] = {
        CTX_COMMENT,
        0
    };
    print_iro_push_eol_context(
        CTX_SIMPLE_DIRECTIVE,
        "\\.(HP|JDA|ZENCODE)",
        STYLE_DIRECTIVE,
        includes);
}

static void print_iro_context_string_directive(void)
{
    const char *includes[] = {
        CTX_STRING,
        CTX_COMMENT,
        0
    };
    print_iro_push_eol_context(
        CTX_STRING_DIRECTIVE,
        "\\.(TITLE|TEXT|NAME|MESSL)",
        STYLE_DIRECTIVE,
        includes);
}

static void print_iro_context_number_directive(void)
{
    const char *includes[] = {
        CTX_DEC_NUMBER,
        CTX_COMMENT,
        0
    };
    print_iro_push_eol_context(
        CTX_NUMBER_DIRECTIVE,
        "\\.BSS",
        STYLE_DIRECTIVE,
        includes);
}

static void print_iro_context_address_directive(void)
{
    const char *includes[] = {
        CTX_ADDRESS,
        CTX_COMMENT,
        0
    };
    print_iro_push_eol_context(
        CTX_ADDRESS_DIRECTIVE,
        "\\.(FILLTO|ORG)",
        STYLE_DIRECTIVE,
        includes);
}

static void print_iro_context_symbol_directive(void)
{
    const char *includes[] = {
        CTX_LOCAL_LABEL,
        CTX_GLOBAL_LABEL,
        CTX_ADDRESS,
        CTX_COMMENT,
        0
    };
    print_iro_push_eol_context(
        CTX_SYMBOL_DIRECTIVE,
        "\\.EQU",
        STYLE_DIRECTIVE,
        includes);
}

static void print_iro_context_code_literal(void)
{
    const char *includes[] = {
        CTX_CODE, CTX_COMMENT, 0
    };
    print_iro_push_eol_context(
        CTX_CODE_LITERAL,
        "#",
        STYLE_DIRECTIVE,
        includes);
}

static const char *mnemonic_to_pattern(const char *mnemonic, char *pattern)
{
    char *p = pattern;
    if (pattern) {
        while (mnemonic && *mnemonic) {
            switch (*mnemonic) {
                case '?':
                case '+':
                case '-':
                case '.':
                    *p++ = '\\';
                default:
                    *p++ = *mnemonic++;
                    break;
            }
        }
        *p++ = '\0';
    }
    return pattern;
}

static int compare_mnemonics(const void *a, const void *b) {
    const char **ps1 = (const char **) a;
    const char **ps2 = (const char **) b;
    int len1 = strlen(*ps1);
    int len2 = strlen(*ps2);
    if (len1 == len2) {
        return strcmp(*ps1, *ps2);
    } else {
        return len2-len1;
    }
}

static int collect_mnemonics(const int *operand_type)
{
    int i;
    mnemonic_operand_it it;
    for (i = 0; i < MNEMONIC_COUNT; i++) {
        mnemonics[i] = 0;
    }
    i = 0;
    if (operand_type) {
        while (*operand_type) {
            const char *m = mnemonic_operand_first(&it, *operand_type++);
            while (m) {
                mnemonics[i++] = m;
                m = mnemonic_operand_next(&it);
            }
        }
        qsort(mnemonics, i, sizeof(char *), compare_mnemonics);
    }
    return i;
}

static void print_mnemonics_pattern(int *operand_type)
{
    if (collect_mnemonics(operand_type) > 0) {
        int i = 0;
        char pattern[MAX_MNEMONIC_PATTERN_SIZE];
        printf("%s", mnemonic_to_pattern(mnemonics[i++], pattern));
        while (mnemonics[i]) {
            printf("|%s", mnemonic_to_pattern(mnemonics[i++], pattern));
        }
    }
}

static void print_iro_context_instruction_aux(
    const char *name,
    const int *operand_type,
    const char **includes)
{
    print_iro_context_begin(name);
    print_iro_context_push_begin();
    printf("%s%-*s \\= (", indent[2], fw[3], "regex");
    print_mnemonics_pattern(operand_type);
    printf(")\n");
    printf("%s%-*s  = .%s;\n", indent[2], fw[3], "styles []", STYLE_MNEMONIC);
    printf("%s: eol_pop {}\n", indent[2]);
    print_iro_context_includes(includes, indent[2]);
    print_iro_context_push_end();
    print_iro_context_end();
}

static void print_iro_context_instruction_none(void)
{
    const int operands[] = {
        MCODE_OP_NONE1,
        MCODE_OP_NONE2,
        MCODE_OP_NONE3,
        0
    };
    const char *includes[] = {
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_NONE, operands, includes);
}

static void print_iro_context_instruction_number()
{
    const int operands[] = {
        MCODE_OP_0_TO_7,
        MCODE_OP_0_TO_13_DEC,
        MCODE_OP_1_TO_31_DEC,
        MCODE_OP_0_TO_64_DEC,
        0
    };
    const char *includes[] = {
        CTX_DEC_NUMBER,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_NUMBER, operands, includes);
}

static void print_iro_context_instruction_address()
{
    const int operands[] = {
        MCODE_OP_ADDRESS1,
        MCODE_OP_ADDRESS2,
        MCODE_OP_ADDRESS3,
        MCODE_OP_ADDRESS4,
        0
    };
    const char *includes[] = {
        CTX_ADDRESS,
        CTX_LOCAL_LABEL,
        CTX_GLOBAL_LABEL,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_ADDRESS, operands, includes);
}

static void print_iro_context_instruction_class2()
{
    const int operands[] = {
        MCODE_OP_TEF1,
        MCODE_OP_TEF2,
        0
    };
    const char *includes[] = {
        CTX_TEF,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_CLASS2, operands, includes);
}

static void print_iro_context_instruction_class3()
{
    const int operands[] = {
        MCODE_OP_DISPLACEMENT,
        0
    };
    const char *includes[] = {
        CTX_POS_DISPLACEMENT,
        CTX_NEG_DISPLACEMENT,
        CTX_LOCAL_LABEL,
        CTX_GLOBAL_LABEL,
        CTX_ADDRESS,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_CLASS3, operands, includes);
}

static void print_iro_context_instruction_register()
{
    const int operands[] = {
        MCODE_OP_0_TO_F_HEX,
        0
    };
    const char *includes[] = {
        CTX_REGISTER,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_REGISTER, operands, includes);
}

static void print_iro_context_instruction_special1()
{
    const int operands[] = {
        MCODE_OP_000_TO_FFF_HEX,
        MCODE_OP_000_TO_3FF_HEX,
        0
    };
    const char *includes[] = {
        CTX_HEX_NUMBER,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_SPECIAL1, operands, includes);
}

static void print_iro_context_instruction_special2()
{
    const int operands[] = {
        MCODE_OP_UNKNOWN, // CON (not really an instruction)
        0
    };
    const char *includes[] = {
        CTX_CODE,
        CTX_LOCAL_LABEL,
        CTX_GLOBAL_LABEL,
        CTX_COMMENT,
        0
    };
    print_iro_context_instruction_aux(
        CTX_INSTRUCTION_SPECIAL2, operands, includes);
}

static void print_iro_main_context(void)
{
    const char *includes[] = {
        CTX_COMMENT,
        CTX_ANNOTATION,
        CTX_DATA,
        CTX_SIMPLE_DIRECTIVE,
        CTX_STRING_DIRECTIVE,
        CTX_NUMBER_DIRECTIVE,
        CTX_ADDRESS_DIRECTIVE,
        CTX_SYMBOL_DIRECTIVE,
        CTX_CODE_LITERAL,
        CTX_INSTRUCTION_NONE,
        CTX_INSTRUCTION_NUMBER,
        CTX_INSTRUCTION_ADDRESS,
        CTX_INSTRUCTION_REGISTER,
        CTX_INSTRUCTION_CLASS2,
        CTX_INSTRUCTION_CLASS3,
        CTX_INSTRUCTION_SPECIAL1,
        CTX_INSTRUCTION_SPECIAL2,
        CTX_LOCAL_LABEL,
        CTX_GLOBAL_LABEL,
        0
    };
    print_iro_context_begin(CTX_MAIN);
    print_iro_context_includes(includes, indent[1]);
    print_iro_context_end();
}

static void print_iro_basic_contexts(void)
{
    print_iro_context_comment();
    print_iro_context_annotation();
    print_iro_context_data();
    print_iro_context_string();
    print_iro_context_number();
    print_iro_context_address();
    print_iro_context_code();
    print_iro_context_local_label();
    print_iro_context_global_label();
    print_iro_context_tef();
    print_iro_context_register();
    print_iro_context_positive_displacement();
    print_iro_context_negative_displacement();
}

static void print_iro_directive_contexts(void)
{
    print_iro_context_simple_directive();
    print_iro_context_string_directive();
    print_iro_context_number_directive();
    print_iro_context_address_directive();
    print_iro_context_symbol_directive();
    print_iro_context_code_literal();
}

static void print_iro_instruction_contexts(void)
{
    print_iro_context_instruction_none();
    print_iro_context_instruction_number();
    print_iro_context_instruction_address();
    print_iro_context_instruction_class2();
    print_iro_context_instruction_class3();
    print_iro_context_instruction_register();
    print_iro_context_instruction_special1();
    print_iro_context_instruction_special2();
}

static void print_iro_contexts(void)
{
    printf("#-------------------------------------------------\n");
    printf("# Main Context\n");
    printf("#-------------------------------------------------\n\n");
    printf("contexts [] {\n");
    print_iro_main_context();
    printf("#-------------------------------------------------\n");
    printf("# Auxiliary Contexts\n");
    printf("#-------------------------------------------------\n\n");
    print_iro_basic_contexts();
    print_iro_directive_contexts();
    print_iro_instruction_contexts();
    printf("}\n");
}

static void print_iro_mcode_syntax(void)
{
    printf("#-------------------------------------------------\n");
    printf("# HP-41 MCODE Syntax Highlighter\n");
    printf("# 2019 by Jurgen Keller <jkeller@gmx.ch>\n");
    printf("#-------------------------------------------------\n\n");
    print_iro_header();
    print_iro_styles(style_definitions);
    print_iro_contexts();
}

int main(int argc, char *argv[])
{
    print_iro_mcode_syntax();
    return 0;
}
