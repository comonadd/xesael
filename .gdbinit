set pagination off
set logging file tmp/gdb.log
set logging on
info breakpoints
source /home/wrongway4you/Git/voltron/voltron/entry.py
r
set logging off
quit
