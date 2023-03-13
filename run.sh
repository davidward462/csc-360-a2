#/usr/bin/bash/

./data_av > seq.txt
./data_av > multi.txt
diff --color seq.txt multi.txt
