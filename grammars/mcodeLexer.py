from pygments.lexer import RegexLexer, bygroups
from pygments.token import *

import re

__all__=['Hp41mcodeLexer']

class Hp41mcodeLexer(RegexLexer):
    name = 'Hp41mcode'
    aliases = ['hp41mcode']
    filenames = ['*.src']
    flags = re.MULTILINE | re.UNICODE

    tokens = {
        'root' : [
            (u'(;.*)', bygroups(Comment)),
            (u'(\\*\\*\\* ERROR.*)|(\\*.*)', bygroups(Generic.Error, Comment.Preproc)),
            (u'([0-9A-F]{4}\\s+)((?:[0-3][0-9A-F]{2}){1,3})', bygroups(Number.Hex, Number.Bin)),
            (u'(\\.(HP|JDA|ZENCODE))', bygroups(Name.Builtin), 'simple_directive__1'),
            (u'(\\.(TITLE|TEXT|NAME|MESSL))', bygroups(Name.Builtin), 'string_directive__1'),
            (u'(\\.BSS)', bygroups(Name.Builtin), 'number_directive__1'),
            (u'(\\.(FILLTO|ORG))', bygroups(Name.Builtin), 'address_directive__1'),
            (u'(\\.EQU)', bygroups(Name.Builtin), 'symbol_directive__1'),
            (u'(#)', bygroups(Name.Builtin), 'code_literal__1'),
            (u'(FETCHS&X|PRPHSLCT|READDATA|WRITDATA|\\?LOWBAT|A=B=C=0|C=CANDA|ENBANK1|ENBANK2|ENBANK3|ENBANK4|ENDREAD|ENDWRIT|GOTOADR|PERSLCT|PUSHADR|RAMSLCT|RDABC1L|RDABC1R|RDABC4L|RTIMEST|TIMER=A|TIMER=B|WKUPOFF|WRABC1L|WRABC1R|WRABC4L|WRABC4R|\\?F10=1|\\?F11=1|\\?F12=1|\\?F13=1|\\?NCRTN|\\?S10=1|\\?S11=1|\\?S12=1|\\?S13=1|\\?TFAIL|ALARM\\?|ALMOFF|C=CORA|C=DATA|C=KEYS|CLRABC|CLRKEY|CLRRTN|CRDEXF|CRDFLG|CRDINF|CRDOHF|CRDWPF|DADD=C|DATA=C|DISOFF|DISTOG|DSPOFF|DSPTOG|DSWKUP|ENREAD|ENROM1|ENROM2|ENROM3|ENROM4|ENWKUP|ENWRIT|ERROR\\?|FLLABC|FLLDAB|FLSDAB|FRSABC|FRSDAB|GOKEYS|GTOKEY|PFAD=C|POPADR|POWOFF|POWON\\?|RCTIME|RDA12L|RDAB1L|RDAB1R|RDAB6L|RDB12L|RDC12L|RDTIME|READAN|READEN|RTNCPU|SETCTF|SETDEC|SETHEX|SLLABC|SLLDAB|SLSABC|SLSDAB|SPOPND|SRLABC|SRLDAB|SRSABC|SRSDAB|STARTC|STATUS|STPINT|STREAD|STWRIT|TCLCRD|TCLCTF|TRPCRD|TSTBUF|WDTIME|WINTST|WKUPON|WRA12L|WRAB1L|WRAB1R|WRAB6L|WRAB6R|WRB12L|WRC12L|WRITAN|WRTIME|WTIME\\-|\\?CRDR|\\?CRTN|\\?EDAV|\\?F0=1|\\?F1=1|\\?F2=1|\\?F3=1|\\?F4=1|\\?F5=1|\\?F6=1|\\?F7=1|\\?F8=1|\\?F9=1|\\?FRAV|\\?FRNS|\\?IFCR|\\?ORAV|\\?PBSY|\\?S0=1|\\?S1=1|\\?S2=1|\\?S3=1|\\?S4=1|\\?S5=1|\\?S6=1|\\?S7=1|\\?S8=1|\\?S9=1|\\?SERV|\\?SRQR|\\?WNDB|ABC=0|ALMON|BUSY\\?|C<>ST|C=C!A|C=C&A|C=C\\.A|C=KEY|C=STK|CHKKB|CLRST|CSTEX|CXISA|DECPT|DSALM|ENALM|FEXSB|FLLDA|FLLDB|FLLDC|FLSDA|FLSDB|FLSDC|FRAV\\?|FRNS\\?|FRSDA|FRSDB|FRSDC|GOTOC|IFCR\\?|INCPT|NCRTN|ORAV\\?|PRINT|R=R\\+1|R=R\\-1|RABCL|RABCR|RDA1L|RDA1R|RDALM|RDATA|RDB1L|RDB1R|RDC1L|RDC1R|RDINT|RDROM|RDSCR|RDSTS|RSTKB|RTIME|RTNNC|SLCTP|SLCTQ|SLSDA|SLSDB|SRLDA|SRLDB|SRLDC|SRQR\\?|SRSDA|SRSDB|SRSDC|ST<>F|ST<>T|STK=C|STOPC|WDATA|WMLDL|WRA1L|WRA1R|WRALM|WRB1L|WRB1R|WRC1L|WRSCR|WRSTS|WRTEN|WSINT|WTIME|XQ>GO|\\?ALM|\\?BAT|\\?KEY|\\?LLD|\\?P=Q|C<>G|C<>M|C<>N|C=ST|CGEX|CMEX|CNEX|CRTN|F=SB|F=ST|GTOC|LLD\\?|M<>C|MCEX|N<>C|NCEX|P=Q\\?|PT=A|PT=B|PT=P|PT=Q|RALM|RINT|RSCR|RSTS|RTNC|S10=|S11=|S12=|S13=|SB=F|SELP|SELQ|ST=0|ST=C|ST=F|ST=T|T=ST|WALM|WROM|WSCR|WSTS|\\+PT|\\-PT|C=G|C=M|C=N|G=C|M=C|N=C|NOP|RTN|S0=|S1=|S2=|S3=|S4=|S5=|S6=|S7=|S8=|S9=)', bygroups(Keyword), 'instruction_none__1'),
            (u'(FLG=1\\?|HPIL=C|\\?FSET|ST=1\\?|\\?FI=|\\?PT=|CLRF|FCNS|PT=\\?|SETF|ST=1|XROM|\\?FS|\\?PF|\\?R=|PT=|RCR|CF|R=|SF)', bygroups(Keyword), 'instruction_number__1'),
            (u'(\\?NCGOREL|\\?NCXQREL|NCGOREL|NCXQREL|DEFP4K|DEFR4K|GOL41C|GOLONG|GSB41C|GSUBNC|U4KDEF|\\?NCGO|\\?NCXQ|GOLNC|GOSUB|GSUBC|\\?CGO|\\?CXQ|GOLC|NCGO|NCXQ|CGO|CXQ)', bygroups(Keyword), 'instruction_address__1'),
            (u'(C=REGN|HPL=CH|PERTCT|REGN=C|C=REG|REG=C|SELPF|LD@R|READ|WRIT|LC)', bygroups(Keyword), 'instruction_register__1'),
            (u'(C=\\-C\\-1|A=A\\+1|A=A\\+B|A=A\\+C|A=A\\-1|A=A\\-B|A=A\\-C|C=0\\-C|C=A\\+C|C=A\\-C|C=C\\+1|C=C\\+A|C=C\\+C|C=C\\-1|LSHFA|RSHFA|RSHFB|RSHFC|\\?A#0|\\?A#C|\\?A<B|\\?A<C|\\?B#0|\\?C#0|A#0\\?|A#C\\?|A<>B|A<>C|A<B\\?|A<C\\?|ABEX|ACEX|B#0\\?|B<>A|B<>C|BAEX|BCEX|C#0\\?|C<>A|C<>B|C=\\-C|CAEX|CBEX|A=0|A=B|A=C|ASL|ASR|B=0|B=A|B=C|BSR|C=0|C=A|C=B|CSR)', bygroups(Keyword), 'instruction_class2__1'),
            (u'(GONC|GOTO|GOC|JNC|JC)', bygroups(Keyword), 'instruction_class3__1'),
            (u'(LDIS&X|LD@R3|LC3|LDI)', bygroups(Keyword), 'instruction_special1__1'),
            (u'(CON)', bygroups(Keyword), 'instruction_special2__1'),
            (u'(\\([^\\)\\n\\r]+\\))', bygroups(Name.Label)),
            (u'(\\[[^\\]\\n\\r]+\\])', bygroups(Name.Label)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'address_directive__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([0-9A-F]{4}\\b)', bygroups(Number.Hex)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'code_literal__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([0-3][0-9A-F]{2}\\b)', bygroups(Number.Bin)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_address__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([0-9A-F]{4}\\b)', bygroups(Number.Hex)),
            (u'(\\([^\\)\\n\\r]+\\))', bygroups(Name.Label)),
            (u'(\\[[^\\]\\n\\r]+\\])', bygroups(Name.Label)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_class2__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([P[QT]|XS?|W(PT)?|MS?|S(&X)?|ALL|@R|R<|P\\-Q)', bygroups(Keyword.Pseudo)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_class3__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(\\+(?:(?:[1-5]\\d)|(?:6[0-3])|\\d))', bygroups(Keyword.Pseudo)),
            (u'(\\-(?:(?:[1-5]\\d)|(?:6[0-4])|[1-9]))', bygroups(Keyword.Pseudo)),
            (u'(\\([^\\)\\n\\r]+\\))', bygroups(Name.Label)),
            (u'(\\[[^\\]\\n\\r]+\\])', bygroups(Name.Label)),
            (u'([0-9A-F]{4}\\b)', bygroups(Number.Hex)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_none__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_number__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(\\d+\\b)', bygroups(Number.Integer)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_register__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'((\\d{1,2}|[0-9A-F])(\\([TZYXLMNOPQabcde]\\))?(/[TZYXLMNOPQabcde])?)', bygroups(Keyword.Pseudo)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_special1__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([0-9A-F]+\\b)', bygroups(Number.Hex)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'instruction_special2__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'([0-3][0-9A-F]{2}\\b)', bygroups(Number.Bin)),
            (u'(\\([^\\)\\n\\r]+\\))', bygroups(Name.Label)),
            (u'(\\[[^\\]\\n\\r]+\\])', bygroups(Name.Label)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'number_directive__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(\\d+\\b)', bygroups(Number.Integer)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'simple_directive__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'string_directive__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(\\\"[^\\\"\\n\\r]*\\\")', bygroups(String)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ], 
        'symbol_directive__1' : [
            (u'(\n|\r|\r\n)', bygroups(Comment), '#pop'),
            (u'(\\([^\\)\\n\\r]+\\))', bygroups(Name.Label)),
            (u'(\\[[^\\]\\n\\r]+\\])', bygroups(Name.Label)),
            (u'([0-9A-F]{4}\\b)', bygroups(Number.Hex)),
            (u'(;.*)', bygroups(Comment)),
            ('(\n|\r|\r\n)', String),
            ('.', String),
        ]
    }

