#!/bin/bash
set -e

#user input
echo "1 - Arch
2 - Debian"
read -p "Select the number for the OS you are currently using for this script: " OS_NUM
echo ""
case "$OS_NUM" in
	1)
		echo "Running Arch postinstallation..."
		OS=arch
		;;
	2)
		echo "Running Debian postinstallation..."
		OS=debian
		;;
	*)
		echo "Invalid option"
		echo "Currently only supporting Arch and Debian"
		exit 1
		;;
esac
echo ""
echo "1 - Agnes Digital
2 - Daiwa Scarlet
3 - Haru Urara
4 - Mambo
5 - Meisho Doto
6 - Oguri Cap
7 - Special Week
8 - Daitaku Helios"
read -p "Select a number to apply the cursor theme from the list above: " THEME_NUM

themes=(agnes-digital daiwa-scarlet haru-urara mambo meisho-doto oguri-cap special-week wei)
if [ "$THEME_NUM" -ge 1 ] && [ "$THEME_NUM" -le 8 ]; then
	THEME=${themes[$((THEME_NUM-1))]}
else
	echo "Invalid selection"
	exit 1
fi

DOTFILES="$HOME/.dotfiles"

#packages
case "$OS" in
	arch)
		echo "Instaling packages"
		sudo pacman -Syu --noconfirm
		sudo pacman -S --noconfirm --needed - < $DOTFILES/lists/pkg-arch.txt
		;;
	debian)
		sudo DEBIAN_FRONTEND=noninteractive apt-get install -y $(cat $DOTFILES/lists/pkg-debian.txt)
		;;
	*)
		echo "Invalid option"
		echo "Currently only supporting Arch and Debian"
		exit 1
		;;
esac

mkdir ~/.config

#alacritty
ln -s $DOTFILES/alacritty ~/.config/alacritty

#bash
if [ "$OS" = "arch" ]; then
	rm ~/.bash_profile
fi
ln -s $DOTFILES/bash/.bash_profile ~/.bash_profile

#cursor
case "$OS" in
	arch)
		mkdir -p ~/.local/share/icons
		ln -s $DOTFILES/cursors/$THEME ~/.local/share/icons/$THEME
		ln -s $DOTFILES/cursors/$THEME ~/.local/share/icons/default
		;;
	debian)
		mkdir ~/.icons
		ln -s $DOTFILES/cursors/$THEME ~/.icons/$THEME
		ln -s $DOTFILES/cursors/$THEME ~/.icons/default
		;;
	*)
		echo "Invalid option"
		echo "Currently only supporting Arch and Debian"
		exit 1
		;;
esac

#dunst
ln -s $DOTFILES/dunst ~/.config/dunst

#fastfetch
ln -s $DOTFILES/fastfetch ~/.config/fastfetch

#fonts
case "$OS" in
	arch)
		ln -s $DOTFILES/fonts ~/.local/share/fonts
		;;
	debian)
		mkdir -p /usr/share/fonts/truetype
		ln -s $DOTFILES/fonts/LessPerfectDOSVGA.ttf /usr/share/fonts/truetype/LessPerfectDOSVGA.ttf
		ln -s $DOTFILES/fonts/W95F.otf /usr/share/fonts/truetype/W95F.otf
		;;
	*)
		echo "Invalid option"
		echo "Currently only supporting Arch and Debian"
		exit 1
		;;
esac

#gtk
case "$OS" in
	arch)
		ln -s $DOTFILES/icons ~/.icons
		;;
	debian)
		ln -s $DOTFILES/icons/classic95 ~/.icons/classic95
		;;
	*)
		echo "Invalid option"
		echo "Currently only supporting Arch and Debian"
		exit 1
		;;
esac
ln -s $DOTFILES/themes ~/.themes

ln -s $DOTFILES/gtk-3.0 ~/.config/gtk-3.0
sed -i "2s/.*/gtk-cursor-theme-name = $THEME/" ~/.config/gtk-3.0/settings.ini

#picom
ln -s $DOTFILES/picom ~/.config/picom

#autologin
sudo mkdir -p /etc/systemd/system/getty@tty1.service.d
sudo tee /etc/systemd/system/getty@tty1.service.d/override.conf > /dev/null <<EOF
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin $USER --noclear %I \$TERM
EOF
sudo systemctl daemon-reexec

#suckless
git clone https://git.suckless.org/dwm ~/.config/dwm
mkdir ~/.config/dwm/patches
rm ~/.config/dwm/dwm.c
ln -s $DOTFILES/suckless/dwm/patches ~/.config/dwm/patches
ln -s $DOTFILES/suckless/dwm/autostart.sh ~/.config/dwm/autostart.sh
mkdir ~/.dwm
ln -s $DOTFILES/suckless/dwm/autostart.sh ~/.dwm/autostart.sh
ln -s $DOTFILES/suckless/dwm/config.h ~/.config/dwm/config.h
ln -s $DOTFILES/suckless/dwm/dwm.c ~/.config/dwm/dwm.c
sudo chmod +x ~/.config/dwm/autostart.sh
cd ~/.config/dwm/
sudo make clean install

git clone https://git.suckless.org/slstatus ~/.config/slstatus
ln -s $DOTFILES/suckless/slstatus/config.h ~/.config/slstatus/config.h
cd ~/.config/slstatus/
sudo make clean install

#x
ln -s $DOTFILES/x/.Xresources ~/.Xresources
sed -i "1s/.*/Xcursor.theme: $THEME/" ~/.Xresources
ln -s $DOTFILES/x/.xinitrc ~/.xinitrc

sudo reboot
