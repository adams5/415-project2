415-project2

Bryan Adams and Josh Schmidt
============================================================


============================================================
		Submitted Source Files
============================================================

makefile

Header Files
------------
global.h
jobs.h
process_hash.h
shellFunc.h
shell_signals.h
tokenizer.h

Source Files
------------
utils.c ---string compare, data structures 
jobs.c  ---handle job control
process_hash.c
shellFunc.c ---shell functions
shell_signals.c --the signal handler
tokenizer.c  ---tokenizer	
project2.c ---main shell loop

============================================================
		Extra Credit
============================================================


============================================================
		Compilation Instructions
============================================================
1) Run the makefile
2) ./project2


============================================================
		Overview of Work 
============================================================

Overview of work accomplished, including who primarily wrote which parts of the code, if you are working in a group

utils.h/c ---josh for cmp, bryan for data structures
jobs.h/c  ---josh for layout nad design, byran for immplementation
shellFunc.h/c ---bryan
shell_signals.h/c --josh
tokenizer.h/c  ---provided	
project2.h/c ---josh/bryan

============================================================
		Description of code and code layout
============================================================

The code is layed out in the files listed above. project2.c contains the main shell loop. From there a check is made for special commands {exit, bg, fg}. If there are no special commands then procede to fork() and chekc for a pipe, if found break the cmd into two parts (left of pipe & right of pipe), then execute each part. If no pipe, execute the cmd as a single process. The execute function located in shellFunc.c from there the cmd passed in will be handled for any redirection and arguments passed into execvp.

============================================================
		General Comments
============================================================


============================================================
		Number of Grace Days
============================================================

Zero

============================================================
		Attribution
============================================================
http://www.linuxprogrammingblog.com/code-examples/sigaction

http://man7.org/linux/man-pages/man7/signal.7.html

http://codingfreak.blogspot.com/2009/10/signals-in-linux-blocking-signals.html

http://www.win.tue.nl/~aeb/linux/lk/lk-5.html

man files for all the used system calls
