#!/usr/bin/env bash

if [[ "$1" == "remove" ]]; then
  git reset --hard;
  exit 0;
fi;

if [[ "$1" == "open" ]]; then
  qutebrowser "https://dwm.suckless.org/patches/$2/";
  exit 0;
fi;

[[ -z "$1" ]] && exit 1;
[[ -z "$2" ]] && exit 1;

rm -f *.orig *.rej *.o config.h;

curl $1 > patches/$2.diff && \
  git add patches/$2.diff && \
  patch -i patches/$2.diff;

