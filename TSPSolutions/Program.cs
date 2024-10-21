using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using System.Linq;
using System.Diagnostics;
using System.Threading;

/* This is the program I ran to test these algorithms (I choose c# because its certainly faster than python, but much nicer for debugging than c++)
    In this file has the Main, and it has serveral helper functions for managing the program and IO.
    The file you care about is Solutions, it has the algorithms I wrote.

    The other files are:
        CityMap.cs: data structure used by the algorithms for finding paths
        Point.cs: data structure of a city, used by the Space data structure to generate CityMaps
        Space.cs: The class used to generate euclidean TSP maps
        Solutions.cs: This contains all of the algorithms I made for this Project.

*/



namespace TSP;
   


class TSP {

    static void Main(string[] args) { 



        Tester();


        /*
        CityMap map1 = GenerateMap(3, 14);
        Solutions.Solution BF1 = Solutions.BruteForce(map1);
        DisplaySolution(BF1, true);
        CheckSolution(BF1, map1);

        Solutions.Solution NNA = Solutions.NearestNeighborAll(map1);
        DisplaySolution(NNA, true);
        CheckSolution(NNA, map1);

        /* //what follows is the code for testing, and the code used in the coding competition
        CityMap competitionMap = ReadCityMap("TextFiles/Size1000.graph");

        Solutions.Solution NN1S2 = Solutions.NearestNeighborAll(map2);
        */ //Competition Prep
        
        // tesing:
        /*
        //CityMap map250 = GenerateMap(3, 20);
        CityMap map250 = ReadCityMap("TextFiles/g5000.graph");
        Console.WriteLine(map250.numberOfCities);
        //DisplayMap(map1);

        Console.WriteLine("Bubble Path");
        Solutions.Solution BP1 = Solutions.BubblePath(map250);
        //DisplaySolution(BP1, true);
        CheckSolution(BP1, map250);

        //Solutions.Opt2checker(map250, BP1, 10);

        int times = 0;
        Solutions.Opt2checker(map250, BP1, 1000);

        Console.WriteLine("");
        CheckSolution(BP1, map250);
        PrintSolution("TextFiles/Solution5000r2", BP1);
        */

        



        //5k - like 30 seconds for 100 checks
        //15k - like 10 seconds
        //30k - like 
        /*
        Console.WriteLine("Nearest Neighbor 1");
        Solutions.Solution NN1S1 = Solutions.NearestNeighbor1(map250, 0);
        //DisplaySolution(NN1S1, true);
        CheckSolution(NN1S1, map250);

        
        
        //DisplayMap(map2);
        Solutions.Solution NN1S2 = Solutions.NearestNeighborAll(map250);

        Console.WriteLine("Nearest Neighbor All");
        CheckSolution(NN1S2, map250);
        
        /*
        Solutions.Solution NNAS1 = Solutions.NearestNeighborAll(map250);
        DisplaySolution(NNAS1, true);
        CheckSolution(NNAS1, map250);
        */

        
        /*
        Console.WriteLine("Nearest Insertion");
        Solutions.Solution NI1 = Solutions.NearestInsertion(map250);
        //DisplaySolution(BP1, true);
        CheckSolution(NI1, map250);

        /*
        Solutions.Opt2checker(map2, NN1S2, 1);
        CheckSolution(NN1S2, map2);

        PrintSolution("TextFiles/currentSolution.txt", NN1S2);
        */
        
        //Solutions.opt2checker(map2, NN1S2, 1000);
        //CheckSolution(NN1S2, map2);

        /*
        NN1S2.order[0] = 100;
        NN1S2.order[1] = 100;
        CheckSolution(NN1S2, map2);
        * /

        /*
        Solutions.Solution BF1 = Solutions.BruteForce(map1);
        DisplaySolution(BF1, true);
        CheckSolution(BF1, map1);
        * /

        //Console.WriteLine(solution.NearestNeighbor1(space1, 0)[0]);

        */ //Testing End

    }

    public static void DisplaySolution(Solutions.Solution solution, bool displayPath) { // this dispays a solution to the terminal (the solution is a data structure in the solutions class)
        Console.WriteLine("");
        Console.Write("Total Distance: ");
        Console.WriteLine(solution.distance);
        
        if(displayPath) { //variable for displaying node list or not
            Console.Write("Order: ");
            for(int i = 0; i < solution.order.Count; i++) { 
                Console.Write(solution.order[i]);
                Console.Write(" ");
            }
        }
        Console.WriteLine();
    }


    public static void DisplayMap(CityMap map) { // used to display city maps to the terminal (useful for debuggin purposes)
        Console.WriteLine("Map: ");
        Console.Write("         ");
        for(int i = 0; i < map.numberOfCities; i++) Console.Write(i.ToString() + "     ");

        for(int i = 0; i < map.numberOfCities; i++) {
            Console.Write("\n" + i.ToString() + "      ");
            for(int j = 0; j < map.numberOfCities; j++){
                Console.Write(String.Format("{0:0.00}", map.distanceMap[i][j]) + "  ");
                Console.Write(" ");
            }
        }
    }

    public static CityMap GenerateMap(int demensions, int cityNumber){ // basic helper function to generate a map using a space of 1000^n (n being the number of dimensions)
        Space space1 = new(demensions);
        for(int i = 0; i < cityNumber; i++){
            space1.AddRandomPoint();
        }

        return space1.GetMap();

    }

    public static CityMap GenerateMap(int demensions, List<double> sizes, int cityNumber){ // basic helper function to generate a map using a space of 1000^n (n being the number of dimensions)
        Space space1 = new(demensions, sizes, cityNumber);

        return space1.GetMap();

    }

     public static CityMap GenerateMap(int demensions, double size, int cityNumber){ // basic helper function to generate a map using a space of 1000^n (n being the number of dimensions)
        List<double> sizes = new();

        for(int i = 0; i < demensions; i++) sizes.Add(size);
        
        Space space1 = new(demensions, sizes, cityNumber);

        return space1.GetMap();

    }

    public static void CheckSolution(Solutions.Solution solution, CityMap map) { // this function is used to check if a solution generated is valid, and that it reports the correct path length
        Console.WriteLine("Distance Reported: ");
        Console.WriteLine(solution.distance);
        Console.WriteLine("Distance Check: ");
        double checkDistance = 0;
        for(int i = 0; i < map.numberOfCities; i++){ // this code is used for determining the actual length of the solution (in case reporting was done badly)
            checkDistance += map.distanceMap[solution.order[i]][solution.order[(i+1) % map.numberOfCities]];
        }
        Console.WriteLine(checkDistance);
        Console.WriteLine();

        Boolean containsAll = true; //the code that follows verifies that the solution contains all cities
        for(int i = 0; i < map.numberOfCities; i++){
            bool nodeIncludedQ = false;
            for(int j = 0; j < map.numberOfCities; j++){
                if(solution.order[j] == i) {
                    nodeIncludedQ = true;
                    break;
                }   
            }
            if(!nodeIncludedQ) {
                containsAll = false;
                break;
            }
        }
        if(!containsAll) Console.WriteLine("Invalid Path");
    }

    public static double SolutionLength(Solutions.Solution solution, CityMap map) {
        double checkDistance = 0;
        for(int i = 0; i < map.numberOfCities; i++){ // this code is used for determining the actual length of the solution (in case reporting was done badly)
            checkDistance += map.distanceMap[solution.order[i]][solution.order[(i+1) % map.numberOfCities]];
        }
        return checkDistance;

    }

    public static CityMap ReadCityMap(string textFile) { // this function is for reading in a TSP map of the given specifications for the competition

        CityMap map = new(); //the data structure to be returned

        string[] lines = File.ReadAllLines(textFile);
        List<string> linesX = new(); // reads all the lines into a list, for nicer properties
        foreach(string s in lines) {
            linesX.Add(s);
        }

        map.distanceMap = new double[linesX.Count][]; // setting up the map data structure
        for(int i = 0; i < linesX.Count; i++) map.distanceMap[i] = new double[linesX.Count];
        map.numberOfCities = linesX.Count;

        int count = 0;
        foreach(string s in linesX) { //puts all the distances into the map of the data structure
            string[] subs = s.Split(' ');

            map.distanceMap[count][count] = Double.Parse(subs[count]);
            for(int i = 0; i < count; i++){
                map.distanceMap[i][count] = Double.Parse(subs[i]);
                map.distanceMap[count][i] = Double.Parse(subs[i]);
            }
            count++;

        }


        return map;
    }

    public static void PrintSolution(string textFile, Solutions.Solution solution){  //Writes a solution to file, used for the competition
        string secondLine = "";
        string space = " ";
        foreach (int city in solution.order) secondLine = secondLine + space + city;

        //Console.WriteLine( secondLine);
        File.WriteAllText(textFile, solution.distance +  "\n" + secondLine);
    }


    public static void Tester() { //makes a test file displaying the results of testing the algorithm
        CityMap small100 = GenerateMap(2, 100, 100);
        CityMap medium100 = GenerateMap(2, 1000, 100);
        CityMap large100 = GenerateMap(2, 10000, 100);

        CityMap small10000 = GenerateMap(2, 100, 100);
        CityMap medium10000 = GenerateMap(2, 1000, 100);
        CityMap large10000 = GenerateMap(2, 10000, 100);

        Console.WriteLine("General Tests:       NearestNeighbor1        NearestNeighborAll     BubblePath      NearestInsertion    NearestNeighbor1+2opt  NearestNeighborAll+2opt  BubblePath+2-opt  NearestInsertion+2opt");
        Displayall(small100, "Small 100 nodes");
        Displayall(medium100, "Medium 100     ");
        Displayall(large100, "Large 100      ");

        Displayall(small10000, "Small 10000    ");
        Displayall(medium10000, "Medium 10000   ");
        Displayall(large10000, "Large 10000    ");

        

        void Displayall(CityMap toTest, string text) { //for a single test case
            Console.Write(text + ":     ");
            Console.Write(SolutionLength(Solutions.NearestNeighbor1(toTest, 0), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.NearestNeighborAll(toTest), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.BubblePath(toTest), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.NearestInsertion(toTest), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.Opt2checker(toTest, Solutions.NearestNeighbor1(toTest, 1), 10000), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.Opt2checker(toTest, Solutions.NearestNeighborAll(toTest), 10000), toTest));
            Console.Write("     ");


            Console.Write(SolutionLength(Solutions.Opt2checker(toTest, Solutions.BubblePath(toTest), 10000), toTest));
            Console.Write("     ");

            Console.Write(SolutionLength(Solutions.Opt2checker(toTest, Solutions.NearestInsertion(toTest), 10000), toTest));
            Console.Write("     \n");
            
            

        }

        Console.WriteLine(); //here is to generate average test cases

        Console.WriteLine("Average Tests:       NearestNeighbor1        NearestNeighborAll     BubblePath      NearestInsertion    NearestNeighbor1+2opt  NearestNeighborAll+2opt  BubblePath+2-opt  NearestInsertion+2opt");

        
        DisplayallAverage(2, 100, 100, 25, "Small 100 nodes");
        DisplayallAverage(2, 1000, 100, 25, "Medium 100     ");
        DisplayallAverage(2, 10000, 100, 25,  "Large 100      ");

        DisplayallAverage(2, 100, 1000, 5,  "Small 1000    ");
        DisplayallAverage(2, 1000, 1000, 5,  "Medium 1000   ");
        DisplayallAverage(2, 10000, 1000, 5, "Large 1000    ");
        
        
        void DisplayallAverage(int demsnions, double size, int nodes, int times, string text) { //this tests all of the algorithms on one graph
            Stopwatch stopWatch = new Stopwatch();
            

            List<CityMap> maps = new();
            for(int i = 0; i < times; i++) maps.Add(GenerateMap(demsnions, size, nodes));

            Console.Write(text + ":     ");

            stopWatch.Start();
            double[] runs = new double[times];
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.NearestNeighbor1(maps[i], 0), maps[i]);
            }
            stopWatch.Stop();
            double average = Queryable.Average(runs.AsQueryable());
            
            
            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();

            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.NearestNeighborAll(maps[i]), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
        
            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();


            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.BubblePath(maps[i]), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            
            


            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();

            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.NearestInsertion(maps[i]), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            
            


            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();

            /*
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.NearestInsertion(maps[i]), maps[i]);

                Boolean containsAll = true;
                for(int k = 0; k < maps[i].numberOfCities; k++){
                    bool nodeIncludedQ = false;
                    for(int j = 0; j < maps[i].numberOfCities; j++){
                        if(Solutions.NearestInsertion(maps[i]).order[j] == i) {
                            nodeIncludedQ = true;
                            break;
                        }   
                    }
                    if(!nodeIncludedQ) {
                        containsAll = false;
                        break;
                    }
                }
                if(!containsAll) Console.Write("INVALID   ");
            } */

            
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.Opt2checker(maps[i], Solutions.NearestNeighbor1(maps[i], 1), 10000), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            


            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();
            
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.Opt2checker(maps[i], Solutions.NearestNeighborAll(maps[i]), 10000), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            

            

            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();
            
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.Opt2checker(maps[i], Solutions.BubblePath(maps[i]), 10000), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            



            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  ");

            stopWatch.Reset();
            stopWatch.Start();
            
            for(int i = 0; i < times; i++){
                runs[i] = SolutionLength(Solutions.Opt2checker(maps[i], Solutions.NearestInsertion(maps[i]), 10000), maps[i]);
            }
            stopWatch.Stop();
            average = Queryable.Average(runs.AsQueryable());
            


            Console.Write(Math.Round(average, 2));
            Console.Write(" ");
            Console.Write(stopWatch.ElapsedMilliseconds );
            Console.Write("ms  \n");

            stopWatch.Reset();
            //stopWatch.Start();
            
            


            

        }

    }

}