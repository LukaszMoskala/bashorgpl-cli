#!/bin/sh
WGET=$(which wget)
if [ -z "$WGET" ]
then
  echo "Nie wykryłem WGETa"
  exit 1
fi
rm -f "$HOME/.bashdata.txt"
$WGET "http://bash.org.pl/text" -O "$HOME/.bashdata.txt"
