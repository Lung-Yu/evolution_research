CPUS ?= $(shell cat /proc/cpuinfo |grep processor|wc -l || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

VPATH=include:src:objects

COMPILER=g++ -std=c++14

DEBUG=-g

OPTMZ=

CFLAGS=$(DEBUG) -Wall -fPIC -I./include -I./objects -I./src $(OPTMZ)

all: NEAT.o Innovation.o Genome.o NerveNeuron.o NerveSynapse.o NerveNetwork.o GeneLink.o GeneNode.o MeanSquaredError.o GeneInfoController.o GeneModelSaver.o
	@echo All Compiled 

testGenome: NEAT.o Innovation.o Genome.o test_genome.o GeneLink.o GeneNode.o
	$(COMPILER) $(FOLDER_PATH) -o test_genome.out ./objects/test_genome.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/GeneLink.o ./objects/GeneNode.o 

test_load:bmp_reader.o mnist_load.o NEAT.o DataHelper_xor.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o GeneInfoController.o
	$(COMPILER) $(FOLDER_PATH) -o test_load.out ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_xor.o ./objects/bmp_reader.o ./objects/mnist_load.o ./objects/GeneInfoController.o -fopenmp 

testNerveNetwork: NEAT.o Innovation.o Genome.o test_nerve_network.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o
	$(COMPILER) $(FOLDER_PATH) -o testNerveNetwork.out ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/NerveNetwork.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/MeanSquaredError.o ./objects/test_nerve_network.o

testPop:GeneModelSaver.o test_pop.o NEAT.o DataHelper_mnist.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o GeneInfoController.o
	$(COMPILER) $(FOLDER_PATH) -o testPop.out ./objects/GeneModelSaver.o ./objects/test_pop.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_mnist.o ./objects/GeneInfoController.o -fopenmp

testPop_f:test_pop.o NEAT.o DataHelper_f.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o GeneInfoController.o
	$(COMPILER) $(FOLDER_PATH) -o testPop.out ./objects/test_pop.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_f.o ./objects/GeneInfoController.o -fopenmp

testPop_mnist:test_pop.o NEAT.o DataHelper_mnist.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o GeneInfoController.o
	$(COMPILER) $(FOLDER_PATH) -o testPop.out ./objects/test_pop.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_mnist.o ./objects/GeneInfoController.o -fopenmp

testPop_xor:test_pop.o NEAT.o DataHelper_xor.o Population.o Organism.o Innovation.o Genome.o GeneLink.o GeneNode.o NerveNetwork.o NerveNeuron.o NerveSynapse.o MeanSquaredError.o GeneInfoController.o
	$(COMPILER) $(FOLDER_PATH) -o testPop.out ./objects/test_pop.o ./objects/NEAT.o ./objects/Innovation.o ./objects/Genome.o ./objects/NerveNeuron.o ./objects/NerveSynapse.o ./objects/NerveNetwork.o ./objects/GeneLink.o ./objects/GeneNode.o ./objects/MeanSquaredError.o ./objects/Organism.o ./objects/Population.o ./objects/DataHelper_xor.o ./objects/GeneInfoController.o -fopenmp

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
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NerveNeuron.o -fopenmp

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

MeanSquaredError.o: CrossEntropyError.cpp
	@echo Compiling CrossEntropyError class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/MeanSquaredError.o

# MeanSquaredError.o: MeanSquaredError.cpp
# 	@echo Compiling MeanSquaredError class
# 	@mkdir -p objects
# 	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/MeanSquaredError.o

DataHelper_xor.o: DataHelper_xor.cpp
	@echo Compiling DataHelper_xor.cpp class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/DataHelper_xor.o

DataHelper_mnist.o: DataHelper_mnist.cpp
	@echo Compiling DataHelper_mnist.cpp class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/DataHelper_mnist.o

DataHelper_f.o: DataHelper_f.cpp
	@echo Compiling DataHelper_f.cpp class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/DataHelper_f.o

Organism.o: Organism.cpp
	@echo Compiling Organism class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Organism.o

Population.o: Population.cpp
	@echo Compiling Population class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Population.o -fopenmp

GeneInfoController.o: GeneInfoController.cpp
	@echo Compiling GeneInfoController class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneInfoController.o

GeneModelSaver.o: GeneModelSaver.cpp
	@echo Compiling GeneModelSaver class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneModelSaver.o


# GeneConnection.o: GeneConnection.cpp 
# 	@echo Compiling GeneConnection class
# 	@mkdir -p objects
# 	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GeneConnection.o


bmp_reader.o: bmp_reader.cpp
	@echo Compiling bmp_reader class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/bmp_reader.o

mnist_load.o: test/mnist_load.cpp
	@echo Compiling bmp_reader class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/mnist_load.o


.PHONY:
clean:
	rm -rf objects
	rm -f *.out