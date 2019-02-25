#!/bin/sh
WGET=$(which wget)
if [ -z "$WGET" ]
then
  echo "Nie wykryłem WGETa"
  exit 1
fi
rm -f bashdata.txt
$WGET http://bash.org.pl/text -O bashdata.txt
