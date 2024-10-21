namespace TSP;

public class Point { //this is the basic data structure to respresent a city in a space
    public int demensions; // 

    public List<double> location = new(); //data holding location of city in each dimension

    public Point(int demension, List<double> size, Random RND){ //generated a city's location randomly given dimensional constraints
        for(int i = 0; i < demension; i++){
            location.Add(RND.NextDouble() * size[i]);
        }
    }

}