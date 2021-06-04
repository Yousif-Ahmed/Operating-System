
#include "FirstComeFirstServe.h"
#include "ShortestJobFirst.h"
#include "ShortestRemainingTimeNext.h"
#include "Preemptive_HPF.h"
#include "RoundRobin.h"
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

enum Algorithms {
		FirstComeFirstServe,
		ShortestJobFirst,
		PreemptiveHPF,	 	
		ShortestRemainingTimeNext,
		RoundRobin
}Algo;

#endif