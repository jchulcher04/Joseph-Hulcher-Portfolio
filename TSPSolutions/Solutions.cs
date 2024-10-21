using System.Security.Cryptography.X509Certificates;

namespace TSP;

public class Solutions {

    public struct Solution { // simply data stucture for storing solutions
        public List<int> order;
        public double distance;
    }
    public static Solution NearestNeighbor1(CityMap map, int start){ // The classic nearest neighbor O(n^2) time, I sketched this program myself (though unoriginal idea obviously) 
        //int count = theList.Count;
        double SolutionDistance = 0;
        List<int> traveled = new(); // the path

        List<int> left = new(); //nodes yet to be included
        for(int i = 0; i < map.numberOfCities; i++) left.Add(i);
        traveled.Add(start); //arbitrarily start with the node designated by "start"
        left.Remove(start);
        

        int current = start;
        int next = start;
        for(int i = 1; i < map.numberOfCities; i++){  //simply add the next nearest city until 
            double distance = (double.MaxValue / map.numberOfCities) / 3;
            for(int j = 0; j < map.numberOfCities - i; j++) { //find the nearest city
                if(distance > map.distanceMap[left[j]][current] ){
                    next = left[j];
                    distance = map.distanceMap[left[j]][current];
                }
            }

            current = next; // add the nearest city
            traveled.Add(current);
            left.Remove(current);
            SolutionDistance += distance;
            //Console.WriteLine(SolutionDistance);
        }
        SolutionDistance += map.distanceMap[start][current]; //add the distance from the end to beginning
        //Console.WriteLine(SolutionDistance);

        Solution solution = new(); //create and return the solution
        solution.order = traveled;
        solution.distance = SolutionDistance; 
        return solution;
    }


    public static Solution NearestNeighborAll(CityMap map) { //O(n^3) this simply runs the nearest neighbor algorithm on all of the nodes, taking the shortest path 
        Solution currentSolution = NearestNeighbor1(map, 0);

        for(int i = 1; i < map.numberOfCities; i++){
            Solution compare = NearestNeighbor1(map, i);
            if(currentSolution.distance > compare.distance) currentSolution = compare;
        }

        return currentSolution;
    }

    public static Solution BruteForce(CityMap map){ //O((n-1)!) this is my brute force algorithms, I sketched it up, and it is unoptimized (14 nodes took about 6 minutes)
        Solution solution = new(); //prepares the solutions and the lists used by the recursive solver
        double solutionDistance = double.MaxValue / 2;
        List<int> path = new(); 
        List<int> left = new();
        for(int i = 0; i < map.numberOfCities; i++) { 
            path.Add(0);
            left.Add(i);
        }

        

        void recursiveSolver(List<int> currentPath, List<int> left, double currentDistance) {
            if(currentPath.Count >= 2)    currentDistance += map.distanceMap[currentPath[currentPath.Count -1]][currentPath[currentPath.Count - 2]];  //add the distance for the last node added

            if(left.Count == 1) { // if at the end of the current rendition, check if it is shorter than our current best path, if so store, if not don't
                currentDistance += map.distanceMap[currentPath[0]][left[0]] + map.distanceMap[currentPath[currentPath.Count - 1]][left[0]]; 
                currentPath.Add(left[0]);
                if(solutionDistance > currentDistance) { // if we have found a better solution, store it
                    solutionDistance = currentDistance;
                    for(int i = 0; i < currentPath.Count; i++){
                        path[i] = currentPath[i];
                    }
                }
                currentPath.RemoveAt(currentPath.Count - 1); // remove the last node for the other recusions as it works off of one list
                //Console.WriteLine(currentDistance);
                return;
            }
            
            for(int i = 0; i < left.Count; i++){ // if not at the end of the current rendition, go through each possibilty calling recursively calling this algorithms on a path of one new node.
                currentPath.Add(left[i]);
                left.RemoveAt(i);
                recursiveSolver(currentPath, left, currentDistance);
                left.Insert(i ,currentPath[currentPath.Count - 1]);
                currentPath.RemoveAt(currentPath.Count - 1);
            }

        
        }

        List<int> currentPath = new(); //we start at node 0, becuase the optimal path includes it
        currentPath.Add(0);
        left.RemoveAt(0);
        double cDistance = 0;
        recursiveSolver(currentPath, left, cDistance); //then we call the recursive solver on the remaining nodes

        solution.distance = solutionDistance;
        solution.order = path;
        return solution; // return the solution
    }

    public static Solution BubblePath(CityMap map){ //This is also know as "random insertion" after looking it up on the webs
        Solution solution = new();          //it works by creating a subtour and then inserting new nodes one at a time, going in order. where 
                                            //you insert the new node in the way that minimizes the length of the next subtour
        double SolutionDistance = 0;
        List<int> path = new(); //insert first node in path
        path.Add(0);
        

        for(int i = 1; i < map.numberOfCities; i++){ // then in order, insert the rest of the nodes

            int firstConnection = i - 1;
            double tryDistance  = map.distanceMap[path[i-1]][i] + map.distanceMap[i][path[0]] - map.distanceMap[path[i-1]][path[0]];
            for(int j = 0; j < i - 1; j++) { //find the insertion place that minimizes the new subtour length
                if(tryDistance > map.distanceMap[path[j]][i] + map.distanceMap[path[j+1]][i] - map.distanceMap[path[j]][path[j+1]]){
                    firstConnection = j;
                    tryDistance = map.distanceMap[path[j]][i] + map.distanceMap[path[j+1]][i] - map.distanceMap[path[j]][path[j+1]];
                } 
            }
            path.Insert(firstConnection + 1, i); //insert the node
            SolutionDistance += tryDistance;
        }

        solution.distance = SolutionDistance;
        solution.order = path;

        return solution;
    }

    public static Solution NearestInsertion(CityMap map){ // I made this after seeing that it has a worst case of 2 * the optimal tour length. 
        Solution solution = new();                        // It works exactly like BubblePath, except it instead inserts the next node nearest the subtour
        
        double SolutionDistance = 0;
        List<int> path = new(); //first node
        path.Add(0);
            //create list of distances to current subtour
        List<(int, double)> remainingNodes = new(); // the extra steps works by tracking the distance between the rest of th cities and the current subtour
        for(int i = 1; i < map.numberOfCities; i++) remainingNodes.Add((i, double.MaxValue / 3));  // it is only O(n^2) as there are only n operation to update and then find the node nearest the current subtour

        for(int i = 1; i < map.numberOfCities; i++){ // insert each city
            
            //first find next nearest node
            for(int j = 0; j < remainingNodes.Count; j++){ // update distances to the current subtour
                if(map.distanceMap[path[i-1]][remainingNodes[j].Item1] < remainingNodes[j].Item2) 
                    remainingNodes[j] = (remainingNodes[j].Item1, map.distanceMap[path[i-1]][remainingNodes[j].Item1]); 
            }

            int nextNodeIndecy = 0;
            int nextNode = remainingNodes[0].Item1; // choose next lowest node
            double nextNodeDistance = remainingNodes[0].Item2;

            for(int j = 1; j < remainingNodes.Count; j++) {  // choose the node that is closest to the current subtour
                if(nextNodeDistance > remainingNodes[j].Item2){
                    nextNodeIndecy = j;
                    nextNode = remainingNodes[j].Item1;
                    nextNodeDistance = remainingNodes[j].Item2;
                }
            }
            remainingNodes.RemoveAt(nextNodeIndecy); // update the list of nodes remaining


            int firstConnection = i - 1; //from here on it is the same as BubblePath
            double tryDistance  = map.distanceMap[path[i-1]][nextNode] + map.distanceMap[nextNode][path[0]] - map.distanceMap[path[i-1]][path[0]];
            for(int j = 0; j < i - 1; j++) {
                if(tryDistance > map.distanceMap[path[j]][nextNode] + map.distanceMap[path[j+1]][nextNode] - map.distanceMap[path[j]][path[j+1]]){
                    firstConnection = j;
                    tryDistance = map.distanceMap[path[j]][nextNode] + map.distanceMap[path[j+1]][nextNode] - map.distanceMap[path[j]][path[j+1]];
                } 
            }

            path.Insert(firstConnection + 1, nextNode);
            SolutionDistance += tryDistance;
        }

        solution.distance = SolutionDistance;
        solution.order = path;

        return solution;
    }

    public static Solution Opt2checker(CityMap map, Solution solution, int times) { // this switches 2 edges to see if it strinks the distance until either it completes the number of specified cycles (times) or there are no improvements made
        bool cont = false;
        double bestDistance = solution.distance;
        do {
            cont = false;
            times--;
            for (int i = 0; i < solution.order.Count-2; i++) { // runs through switching all of the edges with each other
                for(int j = i + 2; j < solution.order.Count; j++){
                    double oldDistance = map.distanceMap[solution.order[i]][solution.order[i + 1]];  //find the distance of the old edges and the distance of the new ones
                    oldDistance += map.distanceMap[solution.order[j]][solution.order[(j + 1) % solution.order.Count]];

                    double newDistance = map.distanceMap[solution.order[i+1]][solution.order[(j + 1) % solution.order.Count]];
                    newDistance += map.distanceMap[solution.order[j]][solution.order[i]];
                    

                    if(newDistance < oldDistance){ // if the new distance is less, switch
                        //Console.Write("l");
                        solution.distance += newDistance;
                        solution.distance -= oldDistance;    
                        
                        for(int m = 0; m < (j - i ) / 2; m++){ // the switching happens by reversing the middle part between the two edges
                            int temp = solution.order[i + m + 1];
                            solution.order[i + m + 1] = solution.order[j - m];
                            solution.order[j - m] = temp;
                        }

                        cont = true;
                    };
                }
            }
        } while (cont && times > 0);

        double finalDistance = 0; //update the final distance
        for(int i = 0; i < solution.order.Count - 1; i++){
            finalDistance += map.distanceMap[solution.order[i]][solution.order[i+1]];
        }
        //finalDistance += map.distanceMap[solution.order[0]][solution.order[solution.order.Count - 1]];

        return solution;
    }

    

}