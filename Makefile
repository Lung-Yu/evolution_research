CPUS ?= $(shell cat /proc/cpuinfo |grep processor|wc -l || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

VPATH=include:src:objects

COMPILER=g++ -std=c++14

DEBUG=-g

OPTMZ=

CFLAGS=$(DEBUG) -Wall -fPIC -I./include -I./objects -I./src $(OPTMZ)

all: NEAT.o Innovation.o Genome.o NerveNeuron.o NerveSynapse.o NerveNetwork.o GeneLink.o GeneNode.o MeanSquaredError.o
	@echo All Compiled 

testGenome: NEAT.o Innovation.o Genome.o test_genome.o GeneLink.o GeneNode.o
	$(COMPILER) $(FOLDER_PATH) -o test_genome.out ./objects/test_genome.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/GeneLink.o ./objects/GeneNode.o

testNerveNetwork: NEAT.o Innovation.o Genome.o test_nerve_network.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o
	$(COMPILER) $(FOLDER_PATH) -o testNerveNetwork.out ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/NerveNetwork.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/MeanSquaredError.o ./objects/test_nerve_network.o

testPop:test_pop.o NEAT.o DataHelper_xor.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o
	$(COMPILER) $(FOLDER_PATH) -o testPop.out ./objects/test_pop.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_xor.o

test_genome.o: test/test_genome.cpp
	@echo Compiling test_genome class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/test_genome.o

test_nerve_network.o: test/test_nerve_network.cpp
	@echo Compiling test_nerve_network class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/test_nerve_network.o

test_pop.o: test/test_pop.cpp
	@echo Compiling test_pop class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/test_pop.o

NEAT.o: NEAT.cpp 
	@echo Compiling NEAT class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NEAT.o

Innovation.o: Innovation.cpp 
	@echo Compiling Innovation class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Innovation.o

Genome.o: Genome.cpp 
	@echo Compiling Genome class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Genome.o

NerveNeuron.o: NerveNeuron.cpp
	@echo Compiling NerveNeuron class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NerveNeuron.o

NerveSynapse.o: NerveSynapse.cpp
	@echo Compiling NerveSynapse class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NerveSynapse.o

NerveNetwork.o: NerveNetwork.cpp
	@echo Compiling NerveNetwork class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NerveNetwork.o

GeneLink.o: GeneLink.cpp
	@echo Compiling GeneLink class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneLink.o

GeneNode.o: GeneNode.cpp
	@echo Compiling GeneNode class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneNode.o

MeanSquaredError.o: MeanSquaredError.cpp
	@echo Compiling MeanSquaredError class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/MeanSquaredError.o

DataHelper_xor.o: DataHelper_xor.cpp
	@echo Compiling DataHelper_xor.cpp class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/DataHelper_xor.o

Organism.o: Organism.cpp
	@echo Compiling Organism class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Organism.o

Population.o: Population.cpp
	@echo Compiling Population class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Population.o

# GeneConnection.o: GeneConnection.cpp 
# 	@echo Compiling GeneConnection class
# 	@mkdir -p objects
# 	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneConnection.o



.PHONY:
clean:
	rm -rf objects
	rm -f *.out