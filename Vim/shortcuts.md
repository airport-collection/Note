both **ESC** or **Ctrl + [**  enter normal mode

gi: return the cursor to the position of the last edit

=====================================================
How to enter insert mode?

I: insert before the beginning of the line

i: insert before the character

a: append after the character

A: append after the end of the line

o: open a new line below the line

O: open a new line above the line

=====================================================
Shortcuts in insert mode:

^h(H): delete a character before the cursor. but in normal mode, the cursor move front by a character.
^w(W): delete a word before the cursor.
^u(U): delete the content before the cursor of the current line.


=====================================================
Qucik move the cursor(All the operation is in the normal mode):

--> word is the word split by the non-blank character; Word is the word split by the blank character.

w(W): move the cursor to the next word(WORD)-beginning.
e(E): move the cursor to the next word(WORD)-end. 
b(B): move the cursor to the last word(WORD)-beginning.

H: move the cursor to the head(top) of the screen.
M: move the cursor to the middle of the screen.
L: move the cursor to the lower(bottom) of the screen.

^u(U): page upward
^f(F): page forward
zz: put current line in the middle of the screen.

gg: move the cursor to the first line of the file.
G: move the cursor to the last line of the file.
^o: move the cursor to the last position before a gg/G operation.

0: move the cursor to the beginning of the line.
^: move the cursor to the beginning of the first non-blank character of the line.
$: move the cursor to the end of the line.
g_: move the cursor to the end of the last non-blank character of the line.

=====================================================
Search in current line:

	enter ';' will jump to the next target. 
	enter ',' will jump to the last target.


f{Char}:
	search the target character.

k{Char}:
	like the 'f{Char}', but it searches target reversely.

t{Char}:
	like the 'f{Char}', but it will jump to the 1 character before the target instead of the target self.

T{Char}:
	search the target reversely and set the cursor point at the 1 character after the target.

=====================================================
Delete in Vim:

diw: delete the word, don't delete the blank character.
daw: delete the word and the continual blank character before the word.
dt{Char}:
	from the current character, delete until the target Char.


Edit After Deletion in Vim:

r: replace the current character.
R: replace characters until enter ESC.
s: delete the current character and enter in insert mode.
S: delete the current line and enter in insert mode.
c: conjunction with text objects, eg. w. cw will delte a word.
C: delete all characters until the end of the current line and enter in insert mode.

=====================================================
Buffer & Window & Tab

What is buffer?
	when a file is opened, vim will load the file into memory, the memory space is called buffer.

How to switch buffer?
	in the normal mode, enter :ls will show all buffers. each buffer has a id. enter :b <id> will switch from current buffer to the target buffer.
	:bfirst switch to the first buffer
	:blast switch to the last buffer
	:bpre switch to the previous buffer, if the current buffer is first, then switch to the last buffer.
	:bnext switch to the next buffer, if the current buffer is last, then switch to the first buffer.

What is window?
	a screen could be split into some different areas which are called windows. in each windows, you can open different or some buffer.

How to split window?
	enter ^ws will split window horizonally, ^wv does vertically.
	or enter :sp for horizonally, and :vs for vertically.

How to switch window?
	^ww: switch circularly.
	^w[hjkl]: the "hjkl" reprsents one direction respectively, the effects as well as move the cursor. 
	
NOTIFICATION:
	when you edit one buffer in a window, the same buffer in others will show the change instantly.
	
What is tab?
	each tab is a container to classify windows.

=====================================================
text object:
	a word, sentence and paragraph can be regarded a text object.
	w for word
	s for sentence
	p for paragraph




=====================================================
yank & paste.   registers in vim:
	there are many registers in vim, default, vim use the anonymous register.
	some other registers named from a to z. e.g.:
		enter :reg a to use the specified register a.
	some other special register:
		"0: the yank register, when enter y, the target content will be stored in this register.
		"+: this represents the system clipboard.

=====================================================
macro in vim:


=====================================================
completion in vim:

	completion for file path:
		^xf: show all the candidates, enter ^N(n) to select next candidate, ^P(a) for previous.


=====================================================
map in vim:

recrusive map:
	you can set some DIY shortcuts in vim called map.
	use nmap, vmap, imap to set map in normal, visual insert mode respectively.

no-recursive map:
	nnoremap: normal no recursive map
	inoremap: insert no recursive map
	vnoremap: visual no recursive map 

NOTIFICATION:
	ALWAYS should use no-recursive map.	

=====================================================
convert content:
	in normal mode, press v to select content, and then press U to contvert Character to Uppercase, u for lowercase.



name

["hello ('world')"]
