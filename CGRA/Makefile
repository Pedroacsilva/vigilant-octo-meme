all: e1 e2 e3 e4 e5 e6
e1: e1_seq e1_acc
e2: e2_seq e2_acc
e3: e3_acc
e4: e4_acc
e5: e5_seq e5_acc
e6: e6_seq e6_acc

e1_seq: e1_seq.c
	gcc -o e1_seq.out e1_seq.c -lrt -lm

e1_acc: e1_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e1_acc.out e1_acc.c

e2_seq: e2_seq.c
	gcc -o e2_seq.out e2_seq.c -lrt -lm

e2_acc: e2_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e2_acc.out e2_acc.c

e3_acc: e3_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e3_acc.out e3_acc.c

e4_acc: e4_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e4_acc.out e4_acc.c

e5_seq: e5_seq.c
	gcc -o e5_seq.out e5_seq.c -lrt -lm

e5_acc: e5_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e5_acc.out e5_acc.c

e6_seq: e6_seq.c
	gcc -o e6_seq.out e6_seq.c -lrt -lm

e6_acc: e6_acc.c
	pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o e6_acc.out e6_acc.c

clean:
	rm *.o *.out