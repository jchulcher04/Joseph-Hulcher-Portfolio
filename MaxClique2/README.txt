Joseph Hulcher - 12147561 - jchulcher@crimson.ua.edu

How to run project:
    Command:
        dotnet run {R/G} {FileName/GraphDensity} {Graph Size} {Algorithm} {Iterations*}

    Argument Descriptions:
        {R/G}: R for reading a file, G for generating a Graph.
        {FileName/GraphDensity}: Write file name if reading a file, write edge density (0-100) if generating a Graph
        {Graph Size}: number of nodes in the Graph
        {Algorithm}: BF (brute force algorithm); YESSIC (2xVertixCover algorithm); 
            BFT (Brute force timed algorithm); BFTES (Brute force timed algorithm starting a every node) 

Description of the project:
    In this project, I implemented four algorithms for the max clique problem:
        the required brute force and 2xVertixCover algorithms, and two modified brute force algorithms.
    This program is made in C#, and below is a description of my experience making each progrom, and a description of the project stucture.
    Already performed tests can be found in testing.txt.

Algorithms:
    BruteForce:
        This is the first Algorithm I made. I used a recursive solver that progressively checks each "possible"
            combination. I did this by making two optimizations. First, if a group of k nodes did not form a clique, I
            did skipped any combinations containing with that group. Second, when iterating through a list of nodes,
            if it is determined to be impossible for a combination of nodes to be larger that the current largest clique
            found, the algorithm ends there and recurses back until it is possible to find a larger clique.
        Interestingly enough, with these optimiztions it can find the optimal solution for graphs as large as 100 or so nodes.
            Further, for particularly sparse graphs, it can do more.
    
    Yessicks2xAlg:
        I implemented this algorithm as described, though I broke it into a few seperate functions:
            GraphComplementer in GraphFunctions complements the graph.
            VCToIndependentSet peforms G.V - VC.
            VertexCover2x - finds the bounded Vertex cover as described.
        I expected it to be bad, but it performed worse than I even expected, often getting 0 nodes as the final result for even huge graphs.
            I suspect that it is partially that 2x is a really bad bound for the vertex cover, and VC is two steps removed from max clique.
            But it was intersting implementing the graph transformations.
    
    TimeOptimizedBruteForce:
        As my Brute force algorithm dived deep into a clique before looking back recursively,
            I thought it would be more apt for Max Clique, so modified it for the competition.
        So for this implentation I eliminated the recursion, and added alimiter for the total number of iterations so I can control when it ends.
            -A hundred million iterations takes about 10 seconds to run.
        Further, I added one additional optimization: if a node has less edges
            than the current best clique found, we can simply ignore it.

     TimeOptimizedBruteForceEveryStart:
        This is the algorithm I utlized in the competition, it simply tries the
            TimeOptimizedBruteForce but starting at every node.
        This way it avoids getting stuck on earlier clique and has a shot to find better ones.
        It did quite well, getting within 10 of the best cliques found for every single graph in the competition, 
            even finding the best found clique on the 250 and 1000 node graphs.
            However, there are definitely different algorithms, such as
                a genetic algorithm I would like to try to implement another time.
    





Descrition of the Project structure:
    Program.cs:
        Runs the Interface.
        
    GraphFunctions.cs:
        Has functions to work with graphs.

    MaxCliqueAlgorithms:
        Contains the algorithms implemented.
    
    Testing.txt:
        Detail the testing conducted and recorded.