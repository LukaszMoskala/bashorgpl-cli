#!/bin/sh
WGET=$(which wget)
if [ -z "$WGET" ]
then
  echo "Nie wykryłem WGETa"
  exit 1
fi
mv -v "$HOME/.bashdata.txt" "$HOME/.bashdata.txt.old"
$WGET "http://bash.org.pl/text" -O "$HOME/.bashdata.txt"
