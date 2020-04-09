# HP-41 MCODE Syntax Highlighter

Syntax highlighting for HP-41 machine code source files. Uses a generic approach that supports different editors.

## Introduction

The [Hewlett Packard Series 41 calculators](http://www.hpmuseum.org/hp41.htm) were built from 1979 to 1990 and were probably the most sophisticated calculators available in the Eighties. The HP-41 can even be programmed in machine code (MCODE). As I'm about to learn it (you know it's never too late) I wanted to have a MCODE syntax highlighter for a convenient visual programming experience.

On my MacBook Pro I'm using the free [TextMate editor](https://macromates.com). My first approach was to write a syntax highlighter for this editor only. As I'm always in favor of generic solutions, I did a bit of research and discovered [Iro](https://medium.com/@model_train/creating-universal-syntax-highlighters-with-iro-549501698fd2). I like this system so I wrote a small program that writes an Iro grammar for MCODE.

## Limitations

I was mostly interested in having a MCODE syntax highlighter for TextMate and for publishing MCODE source files on the Internet, in particular on my [WordPress](https://wordpress.org) based blog. Therefore, I only tried the TextMate and Pygments grammars. Maybe someday I will also test with other editors. However, currently you have to figure out by yourself how to make it work for other editors (please let me know if you succeeded).

**Addendum**: I was able to build a Visual Studio Code extensions from the TextMate grammar. It's available on the Visual Studio Market Place, see:  
[HP-41 Machine Language](https://marketplace.visualstudio.com/items?itemName=JurgenKeller.hp41mcode).

## Quick Start

You can directly use the grammar and theme files, without having to go through the build process.

### Installing Grammars and Themes

#### TextMate

Installing the MCODE grammar and the MCODE Monokai theme for TextMate is easy:
* Start TextMate and choose _Bundles/Edit Bundles..._ to open the bundle editor.
* Press _Command+N_ to create your personal bundle.
* Double-click the file `grammars/mcode.tmLanguage` and import it into your personal bundle.
* In the bundle editor you should see the language grammar _hp41mcode_ in your personal bundle.
* To install the MCODE Monokai theme just double-click the file `themes/textmate/mcodeMonokai.tmTheme` and import it into your personal bundle.

Now, if you open a MCODE source file, choose the _hp41mcode_ language in the status bar and the _MCODE Monokai_ theme using command _View/Theme_ (if you don't see it you may have installed too many themes). Your display should show something like:

![MCODE on TextMate](https://calc.fjk.ch/images/textmate-mcode-sample.jpg)

#### Pygments

There is no need to install the MCODE lexer. Pygments allows for specifying custom lexers  on the command line. For styles, the situation is different. To install the MCODE Monokai style proceed as follows:
```
cd themes/pygments/
sudo python setup.py develop
```
Now, run the following command to test your setup:
```
pygmentize -O full,style=mcodemonokai \
	       -o test.html \
           -l ./grammars/mcodeLexer.py:Hp41mcodeLexer -x \
           test/test.src
```
This creates an HTML file _test.html_ using the MCODE Monokai style. If you open it in your browser it should look similar to the TextMate image above.

Further down I describe how to generate partial HTML files for use in WordPress.

## Build

This project uses the [Gradle build tool](https://gradle.org/). Just run
```
./gradlew run
```
to create the file `build/mcodeiro.txt` which contains the Iro MCODE grammar. Now open the [Iro web page](https://eeyo.io/iro/), copy the grammar into the left pane and press the Play button. Finally, choose an exporter in the top right menu to generate a syntax highlighter grammar for the system of your choice.

## Creating Themes

### Scopes

The MCODE syntax highlighter uses the following scopes:

|Scope         | TextMate / Ace               | Pygments / Rouge |
|--------------|------------------------------|------------------|
| .annotation  | comment.block.preprocessor   | Comment.Preproc  |
| .code        | constant.numeric.code        | Number.Bin       |
| .comment     | comment                      | Comment          |
| .decimal     | constant.numeric.decimal     | Number.Integer   |
| .directive   | keyword.other                | Name.Builtin     |
| .error       | message.error                | Generic.Error    |
| .hexadecimal | constant.numeric.hexadecimal | Number.Hex|      |
| .label       | variable.other               | Name.Label       |
| .mnemonic    | keyword                      | Keyword          |
| .operand     | variable.language            | Keyword.Pseudo   |
| .string      | string                       | String           |

Some of the scopes are fairly standard like _comment_, _string_ and _keyword_. Those are usually supported by most themes. However, there are a few more specific scopes like _annotation_, _directive_, _code_ etc. If you want to style them individually you most probably have to create your own theme.

### TextMate Themes

I highly recommend the [TmTheme Editor](https://tmtheme-editor.herokuapp.com/). I've used it to create my own Monokai based theme for MCODE. Proceed as follows:
* Choose a theme you like as a starting point.
* Add the missing scopes (see table above) and style them.
* Download your new theme to a file named _myThemeName_.tmTheme.
* Double-click the file and import it into your personal TextMate bundle.
* In TextMate select the theme using menu item _View/Theme_.

### Pygments Themes

You can use the Monokai style below `themes/pygments/mcodemonokai` as a template for your own style. For details, please have a look at the Pygments documentation, section [Creating Own Styles](http://pygments.org/docs/styles/#creating-own-styles). 

## Publishing MCODE on WordPress

First you have to create the additional CSS styles for your WordPress blog. Let's assume you want to use the MCODE Monokai style.
```
pygmentize -f html -S mcodemonokai -a .mcode >monokai.css
```
The -a option sets the CSS selector and thus avoids conflicts with the stylesheet of your WordPress theme. Next, add _monokai.css_ to your WordPress theme. In my blog I use the _First_ theme which has a nice customizer where I can simply add the additional CSS.

Now, create the HTML code for a MCODE source file:
```
pygmentize -O style=mcodemonokai,cssclass=mcode \
           -o test.html \
           -l ./grammars/mcodeLexer.py:Hp41mcodeLexer -x \
           test/test.src
```

Insert the contents of _test.html_ into the target page of your blog. The result should look like my [MCODE test page](https://calc.fjk.ch/test-page/). If your code is wider than the page WordPress automatically adds a horizontal scrollbar.

You see, it's not a perfect solution fully integrated into WordPress but simple and good enough for my use case.

## References
* [Introducing Iro — An Easier Way To Create Syntax Highlighters](https://medium.com/@model_train/creating-universal-syntax-highlighters-with-iro-549501698fd2) by Chris Ainsley.
* [TmTheme Editor](https://github.com/aziz/tmTheme-Editor) on GitHub.
* HP-41 Software Development Kit (SDK41) by Warren Furlow, see [The HP-41 Archive Web Site](http://www.hp41.org/).




