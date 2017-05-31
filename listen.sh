#! /bin/sh

echo "To exit: C-a C-x"

picocom --echo -b 9600 /dev/ttyUSB0 --imap lfcrlf

