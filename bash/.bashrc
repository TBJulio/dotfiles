#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto'
alias grep='grep --color=auto'
alias cdwm='nano /home/julio/.config/dwm/config.h'
alias calacritty='nano /home/julio/.config/alacritty/alacritty.toml'
alias apple='alacritty --title scratchpad --option window.dimensions.columns=96 --option window.dimensions.lines=37 --config-file /mnt/hdd3/dotfiles/scripts/bad-apple/terminal.toml -e bash -c "/mnt/hdd3/dotfiles/scripts/bad-apple/run.sh"'
PS1='[\u@\h \W]\$ '

#fastfetch
