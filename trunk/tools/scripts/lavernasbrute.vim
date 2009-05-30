" Justin Workman <xtagon@xinja.net>
"
" The settings in this script help contributors to the lavernasbrute project
" follow the code formatting guidlines.
"
" Usage:
"       Place this script in your ~/.vim directory and add the following
"       command to your .vimrc:
"
"       au BuffRead ~/path/to/lavernasbrute/* source ~/.vim/lavernasbrute.vim
"
" Links:
"       http://code.google.com/p/lavernasbrute/
"       http://code.google.com/p/lavernasbrute/wiki/CodeFormatting


" Indentation and whitespace
"
set expandtab           " Use spaces to insert a tab
set linebreak           " Don't break lines by characters
set shiftwidth=4        " Number of spaces for autoindent
set tabstop=4           " Number of spaces that a tab counts for
set textwidth=80        " Keep lines 80 columns or fewer
