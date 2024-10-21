﻿using System;

using System.Collections.Generic;

// This file houses the command line interface and a few display functions.


public class Program {
    static void Main(string[] args) {
        //CompetitionRunner(args); used for the comepetition
        
        //runs interface
        MainRunner(args);
    }

        //function to print out solutions generated
    public static void PrintSolution(int[,] graph, List<int> solution){ 
        Console.Write("\nSolution Size: " + solution.Count);
        Console.Write("\nSolution Status: ");
        CheckSolution(graph, solution);
        Console.Write("Solution: ");
        for(int i = 0; i < solution.Count; i++){
            Console.Write(solution[i] + " ");
        }
        Console.WriteLine();
    }

        //helper function that checks whether a given solution is correct for a given graph
    public static void CheckSolution(int[,] graph, List<int> solution){
        for(int i = 0; i < solution.Count; i++){
            for(int j = i + 1; j < solution.Count; j++){
                if(graph[solution[i],solution[j]] == 0) {
                    Console.WriteLine("Bad Solution.");
                    return;
                }
            }
        }
        Console.WriteLine("Good Solution");
    }


        //Interface used to run program in the competition
    public static void CompetitionRunner(string[] args){
        int size = int.Parse(args[1]);
        int [,] graph = GraphFunctions.ReadAdjmatGraph(args[0], size);
        List<int> solution = new();
        if(args[2] == "BFT"){
            solution = MaxCliqueAlgorithms.TimedOptimizedBruteForce(graph, size, long.Parse(args[3]));
        } else if (args[2] == "BFTES") {
            solution = MaxCliqueAlgorithms.TimedOptimizedBruteForceEveryStart(graph, size, long.Parse(args[3]));
        } else if (args[2] == "BF") {
            solution = MaxCliqueAlgorithms.BruteForce(graph, size);
        } else if (args[2] == "YESSICK"){
            solution = MaxCliqueAlgorithms.Yessick2xAlg(graph, size);
        }

        PrintSolution(graph, solution);
    }

        //Main interface for utilizing the different algorithms
    public static void MainRunner(string[] args){ 
        int size = int.Parse(args[2]);
        int [,] graph = {{}};

        if(args[0] == "G"){ // Determines whether we generate or read in the grpah
            graph = GraphFunctions.GenerateRandomGraph(size, int.Parse(args[1]));
        } else if (args[0] == "R") {
            graph = GraphFunctions.ReadAdjmatGraph(args[1], size);
        } else {
            Console.WriteLine("Must enter G (for generated graph) or R (for reading in a graph)");
            return;
        }

        //runs the specified algorithm
        List<int> solution = new();
        if(args[3] == "BFT"){
            solution = MaxCliqueAlgorithms.TimedOptimizedBruteForce(graph, size, long.Parse(args[4]));
        } else if (args[3] == "BFTES") {
            solution = MaxCliqueAlgorithms.TimedOptimizedBruteForceEveryStart(graph, size, long.Parse(args[4]));
        } else if (args[3] == "BF") {
            solution = MaxCliqueAlgorithms.BruteForce(graph, size);
        } else if (args[3] == "YESSIC"){
            solution = MaxCliqueAlgorithms.Yessick2xAlg(graph, size);
        }

        PrintSolution(graph, solution);
    }

}