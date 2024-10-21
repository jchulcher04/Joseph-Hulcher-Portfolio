using System;
using System.Collections.Generic;
using System.Timers;


    // This file houses the 4 implemented algorithms in the following order:
    //Brute Force
    //Required 2x function
    //Non-recursive timed Brute Force
    //Non-recursive timed Brute Force with starting at every node

public class MaxCliqueAlgorithms
{
        //brute force solution - has optimizations to not check "dead" paths
    public static List<int> BruteForce(int [,] graph, int size){
        List<int> maxClique = new(); 

        //Recursive function to try adding a node to the current clique
        void MaxCliqueHelper(List<int> currentClique, int place) {

            //This prevents checking cliques that won't be larger than the max clique
            if(maxClique.Count >= currentClique.Count + size - place) return; 

            // if we can add the node, recusively try adding all the nodes remaining nodes, then pop the added node when done
            if(CheckExpansion(currentClique, place)){ 
                currentClique.Add(place);
                for(int i = place + 1; i < size; i++){
                    MaxCliqueHelper(currentClique, i);
                }
                
                if(currentClique.Count > maxClique.Count) maxClique = new List<int>(currentClique);
                currentClique.RemoveAt(currentClique.Count - 1); 
            }   
        }

        //helper function to see if we can add a new node to the current clique
        bool CheckExpansion(List<int> clique, int node){ 
            for(int i = 0; i < clique.Count; i++){ 
                if(graph[clique[i], node] == 0) return false;
            }
            return true;
        }

        List<int> startingClique = new(); 

        // It begins with the recursive solver on each node
        for(int i = 0; i < size; i++){
            MaxCliqueHelper(startingClique, i);
        }

        return maxClique;
    }

        //2x optimal vertix cover algorithm as described by Dr. Yessick
    public static List<int> VertexCover2x(int[,] graph, int size){ 
        List<int> cover = new();
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(graph[i,j] == 1 && !cover.Contains(i) && !cover.Contains(j)) {
                    cover.Add(i);
                    cover.Add(j);
                }
            }
        }
        return cover;
    }

        //Second required Algotithm
    public static List<int> Yessick2xAlg(int[,] graph, int size){
        return VCtoIndependentSet(
                    VertexCover2x(
                        GraphFunctions.GraphComplementer(graph, size), 
                    size), 
                size);
    }

        //Converts a vertex cover to an independent set by taking G.V - VC(G)
    public static List<int> VCtoIndependentSet(List<int> cover, int size) { // 
        List<int> independentSet = new();
        for(int i = 0; i < size; i++){
            if(!cover.Contains(i)) independentSet.Add(i);
        }
        return independentSet;
    }

        //non-recursive bruteforce algorithm that has a built in ending timer
    public static List<int> TimedOptimizedBruteForce(int [,] graph, int size, long iterations){ 
        List<int> maxClique = new(); 
        List<int> currentClique = new();

            //note the number of edges for each node for a check later on
        List<int> edgesCount = new();
        for(int i = 0; i < size; i++){
            edgesCount.Add(0);
            for(int j = 0; j < size; j++){
                if(graph[i,j] == 1) edgesCount[i]++;
            }
        }


        int place = 0;  
        int times = 0;

        //while loop for the brute force algorithm that terminates when the number of specified loops is reached
        while(times < iterations){
            if(place == size || maxClique.Count >= currentClique.Count + size - place){
                if(currentClique.Count == 0) return maxClique;
                else {
                    place = currentClique[currentClique.Count - 1];
                    currentClique.RemoveAt(currentClique.Count - 1);   
                }
            } 
                //here is the only new check: if a nodes edges < size of the biggest found clique, we don't need to check it.
            else if (maxClique.Count < edgesCount[place] && CheckExpansion(currentClique, place)){ 
                currentClique.Add(place);
                if(currentClique.Count > maxClique.Count) maxClique = new List<int>(currentClique);
            } 
            place++;
            times++;
        }
        
        return maxClique;

            //same helper function as in Brute Force to check adding new nodes to a clique
        bool CheckExpansion(List<int> clique, int node){ 
            for(int i = 0; i < clique.Count; i++){ 
                if(graph[clique[i], node] == 0) return false;
            }
            return true;
        }
    }

        //non-recursive bruteforce algorithm that has a built in ending timer, and the addition of starting at every node
    public static List<int> TimedOptimizedBruteForceEveryStart(int [,] graph, int size, long iterations){ 
        List<int> maxClique = new(); 
        List<int> currentClique = new();

        List<int> edgesCount = new();
        for(int i = 0; i < size; i++){
            edgesCount.Add(0);
            for(int j = 0; j < size; j++){
                if(graph[i,j] == 1) edgesCount[i]++;
            }
        }

        //this for loop is the only change, it trys the algorithm starting at each node, splitting the loops amoung all.
        for(int i = 0; i < size; i++){
            int place = i;  
            int times = i;
            while(times < iterations / size){
                if(place == size || maxClique.Count >= currentClique.Count + size - place){
                    if(currentClique.Count == 0) return maxClique;
                    else {
                        place = currentClique[currentClique.Count - 1];
                        currentClique.RemoveAt(currentClique.Count - 1);
                        
                    }
                } else if (maxClique.Count < edgesCount[place] && CheckExpansion(currentClique, place)){
                    currentClique.Add(place);
                    if(currentClique.Count > maxClique.Count) maxClique = new List<int>(currentClique);
                } 
                place++;
                times++;
            }
            currentClique.Clear();
        }
        
        return maxClique;

        bool CheckExpansion(List<int> clique, int node){ 
            for(int i = 0; i < clique.Count; i++){ 
                if(graph[clique[i], node] == 0) return false;
            }
            return true;
        }
    }

}