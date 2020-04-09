
#include "mcinstr.h"
#include "mcop.h"

const char *mnemonic_operand_first(mnemonic_operand_it *it, int operand_type)
{
    const char *mnemonic = 0;
    if (it) {
        it->operand_type = operand_type;
        it->index = -1;
        mnemonic = mnemonic_operand_next(it);
    }
    return mnemonic;
}

const char *mnemonic_operand_next(mnemonic_operand_it *it)
{
    const char *mnemonic = 0;
    if (it) {
        it->index++;
        while ((it->index < IHT_SIZE) && (inst[it->index].typ != it->operand_type)) {
            it->index++;
        }
        if (it->index < IHT_SIZE) {
            mnemonic = inst[it->index].name;
        }
    }
    return mnemonic;
}
