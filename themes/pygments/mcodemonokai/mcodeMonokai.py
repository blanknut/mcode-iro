from pygments.style import Style
from pygments.token import Keyword, Name, Comment, String, Error, \
     Number, Operator, Generic

BACKGROUND = "#272822"
FOREGROUND = "#F8F8F2"
SELECTION  = "#49483E"

class McodeMonokaiStyle(Style):

    default_style = ""

    background_color = BACKGROUND
    highlight_color = SELECTION

    styles = {
        Comment:            '#A89880',
        Comment.Preproc:    '#808080',
        Generic.Error:      'italic #888',
        Keyword:            '#F92672',
        Keyword.Pseudo:     '#FD971F',
        String:             '#E6DB74',
        Name.Builtin:       'italic #F92672',
        Name.Label:         '#A6E22E',
        Number.Integer:     '#A080F0',
        Number.Hex:         '#A080F0',
        Number.Bin:         '#8060D0'
    }
