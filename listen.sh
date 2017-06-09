#! /bin/sh

echo "To exit: C-a C-x"

picocom --echo -b 19200 /dev/ttyUSB0 --imap lfcrlf

