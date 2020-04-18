""""""""""""""""""""""""""""""""""""""""
"        theme releated stuff          "
""""""""""""""""""""""""""""""""""""""""
set background=dark
colorscheme default


""""""""""""""""""""""""""""""""""""""""
"             key mappings             "
""""""""""""""""""""""""""""""""""""""""
"shift space to scape insert mode
imap <S-Space> <Esc>

"control tab and shifttab  to move tabs
nmap <C-Tab> :tabn<CR>
nmap <S-Tab> :tabp<CR>

nmap <Tab>p :tabn<CR>
nmap <Tab>o :tabp<CR>

"delete all white spaces
:nnoremap <silent> <F5> :let _s=@/ <Bar> :%s/\s\+$//e <Bar> :let @/=_s <Bar> :nohl <Bar> :unlet _s <CR>

"indent with tab and shift tab in visual mode
vmap <Tab> >gv
vmap <S-Tab> <gv

"create line before and after but return to normal mode
nnoremap o o<Esc>
nnoremap O O<Esc>

"delete word and line in insert without loosing undo history
inoremap <c-u> <c-g>u<c-u>
inoremap <c-w> <c-g>u<c-w>



""""""""""""""""""""""""""""""""""""""""
"         environment stuff            "
""""""""""""""""""""""""""""""""""""""""
"cd to file's working directory
command! Cd cd %:h

"Easy align by equal sign
command! -range EL <line1>,<line2> EasyAlign *=



""""""""""""""""""""""""""""""""""""""""
"          custom commands             "
""""""""""""""""""""""""""""""""""""""""
"environment related stuff
set tabstop=2 softtabstop=0 expandtab shiftwidth=2 smarttab
syntax enable
filetype plugin on
cd /home/bruno2
set number
set foldmethod=indent

"for browsing
set path+=**
set wildmenu

let g:netrw_banner=0       " annoying banner
let g:netrw_browse_split=4 " open in prior window
let g:netrw_altv=1         " opens splits to the right
let g:netrw_liststyle=3    " tree view
let g:netrw_list_hide=netrw_gitignore#Hide()



""""""""""""""""""""""""""""""""""""""""
"          non Plug pluggins           "
""""""""""""""""""""""""""""""""""""""""




""""""""""""""""""""""""""""""""""""""""
"         Plug pluggins stuff          "
""""""""""""""""""""""""""""""""""""""""
call plug#begin('~/.vim/plugged')
  Plug 'vim-airline/vim-airline'
  Plug 'vim-airline/vim-airline-themes'
  Plug 'chrisbra/csv.vim'
  Plug 'andymass/vim-matchup'
  Plug 'MortenStabenau/matlab-vim'
  Plug 'junegunn/vim-easy-align'
  Plug 'tpope/fugitive'
call plug#end()



""""""""""""""""""""""""""""""""""""""""
"         matchit or matchup           "
""""""""""""""""""""""""""""""""""""""""
let g:loaded_matchit = 1



""""""""""""""""""""""""""""""""""""""""
"      Airline config and theme        "
""""""""""""""""""""""""""""""""""""""""
"let g:airline_extensions = ['branch', 'tabline', 'csv']
"let g:airline_extensions = ['branch', 'wordcount', 'capslock', 'csv']

let g:airline#extensions#csv#enabled = 1
let g:airline#extensions#csv#column_display = 'Name'

let g:airline_theme = 'solarized'
let g:airline_solarized_bg='dark'
let g:airline_left_sep= '»'
let g:airline_right_sep= '«'
let g:airline_detect_modified=1
let g:airline_detect_paste=1
"let g:airline_detect_crypt=1
"let g:airline_detect_spell=1
"let g:airline_detect_spelllang=1
"let g:airline_detect_iminsert=0
let g:airline_inactive_collapse=1
let g:airline_inactive_alt_sep=1

"let g:airline_section_z = airline#section#create(['linenr'])
let g:airline_section_z = airline#section#create(['%3p%%', ' ', 'linenr', '/', '%L', '  ', ':%c'])



""""""""""""""""""""""""""""""""""""""""
"           tabline stuff              "
""""""""""""""""""""""""""""""""""""""""
if exists("+showtabline")
  " Rename tabs to show tab number.
  " (Based on http://stackoverflow.com/questions/5927952/whats-implementation-of-vims-default-tabline-function)

  function! MyTabLine()
    let s = ''
    let t = tabpagenr()
    let i = 1
    while i <= tabpagenr('$')
      let buflist = tabpagebuflist(i)
      let winnr = tabpagewinnr(i)
      let s .= '%' . i . 'T'
      let s .= (i == t ? '%1*' : '%2*')

      " let s .= (i == t ? '%#TabLineSel#' : '%#TabLine#')
      " let s .= ' '
      let s .= (i == t ? '%#TabNumSel#' : '%#TabNum#')
      let s .= ' ' . i . ' '
      let s .= (i == t ? '%#TabLineSel#' : '%#TabLine#')

      let bufnr = buflist[winnr - 1]
      let file = bufname(bufnr)
      let buftype = getbufvar(bufnr, '&buftype')

      if buftype == 'help'
        let file = 'h:' . fnamemodify(file, ':t:r')

      elseif buftype == 'quickfix'
        let file = 'quickfix'

      elseif buftype == 'nofile'
        if file =~ '\/.'
          let file = substitute(file, '.*\/\ze.', '', '')
        endif

      else
        let file = pathshorten(fnamemodify(file, ':p:~:.'))
        if getbufvar(bufnr, '&modified')
          let file = '+' . file
        endif

      endif

      if file == ''
        let file = '[no-name]'
      endif

      let s .= ' ' . file

      let nwins = tabpagewinnr(i, '$')
      if nwins > 1
        let modified = ''
        for b in buflist
          if getbufvar(b, '&modified') && b != bufnr
            let modified = '*'
            break
          endif
        endfor
        let hl = (i == t ? '%#WinNumSel#' : '%#WinNum#')
        let nohl = (i == t ? '%#TabLineSel#' : '%#TabLine#')
        let s .= ' ' . modified . '(' . hl . winnr . nohl . '/' . nwins . ')'
      endif

      if i < tabpagenr('$')
        let s .= ' %#TabLine#|'
      else
        let s .= ' '
      endif

      let i = i + 1

    endwhile

    let s .= '%T%#TabLineFill#%='
    let s .= (tabpagenr('$') > 1 ? '%999XX' : 'X')
    return s
  endfunction

   set showtabline=1
  highlight! TabNum term=bold,underline cterm=bold,underline ctermfg=1 ctermbg=7 gui=bold
  highlight! TabNumSel term=bold,reverse cterm=bold,reverse ctermfg=1 ctermbg=7 gui=bold
  highlight! WinNum term=bold,underline cterm=bold,underline ctermfg=11 ctermbg=7 guifg=DarkBlue guibg=LightGrey
  highlight! WinNumSel term=bold cterm=bold ctermfg=7 ctermbg=14 guifg=DarkBlue guibg=LightGrey

  set tabline=%!MyTabLine()
endif " exists("+showtabline")


""""""""""""""""""""""""""""""""""""""""
"             other stuff              "
""""""""""""""""""""""""""""""""""""""""
set sw=2
