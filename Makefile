build:
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out Preemptive_HPF.c FirstComeFirstServe.c ShortestJobFirst.c ShortestRemainingTimeNext.c RoundRobin.c
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out
	touch keyfile
clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out
