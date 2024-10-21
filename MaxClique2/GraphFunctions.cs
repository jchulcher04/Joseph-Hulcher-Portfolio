// THIS PROGRAM WAS MADE WITH ASSISTANCE FROM CHAT GPT, which functions are marked

using System;
using System.Collections.Generic;
using System.IO;

public class GraphFunctions
{
    static Random random = new Random();

    //CHAT GPT ASSISTED function to print a graph - used for veryfing graphs 
    public static void PrintGraph(int[,] graph, int n){
        Console.WriteLine("Generated Graph:");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                Console.Write(graph[i, j] + " ");
            }
            Console.WriteLine();
        }
    }

    //CHAT GPT GENERATED - makes a random graph 
    public static int[,] GenerateRandomGraph(int n, int density) //n is the number of nodes, and density is a percentage of 0 - 100 of edges filled
    {
        int[,] graph = new int[n, n];

        // Fill the upper triangular part of the matrix with random edges based on density
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (random.Next(100) < density)
                {
                    graph[i, j] = 1;
                    graph[j, i] = 1; // Since the graph is undirected
                }
            }
        }

        return graph;
    }

    //this function was made by me: reads in the file of the format .adjmat
    public static int[,] ReadAdjmatGraph(string file, int size) {
        int[,] graph = new int[size, size];
        string[] lines = File.ReadAllLines(file);

        for(int i = 0; i < size; i++){
            graph[i,i] = 0;
            string[] subs = lines[i].Split(' ');
            for(int j = 0; j < i; j++){
                int.TryParse(subs[j+1], out graph[i,j]);
                int.TryParse(subs[j+1], out graph[j,i]);
            }
        }

        return graph;
    }

    //Not implemented
    public static int[,] ReadAdjlisGraph(string file, int size) {
        int[,] graph = new int[size, size];

        return graph;
    }

     //this function was made by me: it reverses a graph, i.e. GraphComplementer(G) = G'
    public static int[,] GraphComplementer(int [,] inGraph, int size){
        int[,] outGraph = new int[size, size];
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(i == j) continue;
                outGraph[i,j] = (inGraph[i,j] + 1) % 2;
            }
        }
        return outGraph;
    }
}