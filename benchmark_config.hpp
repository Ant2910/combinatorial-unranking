#ifndef BENCHMARK_CONFIG_HPP
#define BENCHMARK_CONFIG_HPP

/*
Alle Benchmarks benoetigen zusaetzlich discreture.
/https://github.com/mraggi/discreture

Fuer die Laufzeitbenchmarks wird zusaetzlich Catch2 benoetigt.
https://github.com/catchorg/Catch2

FULL_ITERATION Kompilierung und Ausfuerhung:
g++ -std=c++20 -O3 -march=native iteration_runtime_benchmark.cpp urn.cpp -o benchmark -lCatch2Main -lCatch2
./benchmark --benchmark-samples 500

SPECIFIC_DRAW Kompilierung und Ausfuerhung:
g++ -std=c++20 -O3 -march=native specific_runtime_benchmark.cpp urn.cpp -o benchmark -lCatch2Main -lCatch2
./benchmark --benchmark-samples 500


Zur Auswertung der Speicherbenchmarks wurde Massif des Valgrind Frameworks verwendet.
https://valgrind.org/docs/manual/ms-manual.html
sudo apt install valgrind
sudo apt install massif-visualizer //Zur Visualisierung

MEMOERY Kompilierung und Ausfuerhung:
g++ -std=c++20 -O3 urn.cpp memory_benchmark.cpp -o memory_benchmark
valgrind --tool=massif --time-unit=ms --stacks=yes --massif-out-file=memory.out ./memory_benchmark
massif-visualizer memory.out
*/


/*
Welche Benchmarks sollen enthalten sein.
Benchmark soll enthalten sein       => 1
Benchmark soll nicht enthalten sein => 0
HINWEIS: Fuer die Speicherbenchmarks kann immer nur ein Ansatz gleichzeitig ausgewaehlt werden
*/

//Volle Iteration
#define URN_OR_FULL_ITERATION 0
#define URN_O_FULL_ITERATION 0
#define URN_R_FULL_ITERATION 0
#define URN_FULL_ITERATION 0

//Spezifische Ziehung
#define URN_OR_SPECIFIC_DRAW 0
#define URN_O_SPECIFIC_DRAW 0 
#define URN_R_SPECIFIC_DRAW 0
#define URN_SPECIFIC_DRAW 0

//Speicher fuer Urn
#define URN_MEMORY 0
#define NAIVE_APPROACH_MEMORY 0
#define DISCRETURE_MEMORY 0


//CONFIG AUSWERTUNGEN
#if URN_OR_FULL_ITERATION
    #define URN_OR_FULL_ITERATION_ENABLED
#endif //URN_OR_FULL_ITERATION 

#if URN_OR_SPECIFIC_DRAW 
    #define URN_OR_SPECIFIC_DRAW_ENABLED
#endif //URN_OR_SPECIFIC_DRAW

#if URN_OR_MEMORY_BENCHMARK
    #define URN_OR_MEMORY_ENABLED
#endif //URN_OR_MEMORY_BENCHMARK


#if URN_O_FULL_ITERATION
    #define URN_O_FULL_ITERATION_ENABLED
#endif //URN_O_FULL_ITERATION 

#if URN_O_SPECIFIC_DRAW 
    #define URN_O_SPECIFIC_DRAW_ENABLED
#endif //URN_O_SPECIFIC_DRAW


#if URN_R_FULL_ITERATION
    #define URN_R_FULL_ITERATION_ENABLED
#endif //URN_R_FULL_ITERATION 

#if URN_R_SPECIFIC_DRAW 
    #define URN_R_SPECIFIC_DRAW_ENABLED
#endif //URN_R_SPECIFIC_DRAW


#if URN_FULL_ITERATION
    #define URN_FULL_ITERATION_ENABLED
#endif //URN_FULL_ITERATION 

#if URN_SPECIFIC_DRAW 
    #define URN_SPECIFIC_DRAW_ENABLED
#endif //URN_SPECIFIC_DRAW


#if URN_MEMORY
    #define URN_MEMORY_ENABLED
#endif //URN_MEMORY

#if NAIVE_APPROACH_MEMORY
    #define NAIVE_APPROACH_MEMORY_ENABLED
#endif //NAIVE_APPROACH_MEMORY

#if DISCRETURE_MEMORY
    #define DISCRETURE_MEMORY_ENABLED
#endif //DISCRETURE_MEMORY

#endif //BENCHMARK_CONFIG_HPP