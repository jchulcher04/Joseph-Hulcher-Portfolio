using System.Runtime.CompilerServices;

namespace TSP;


public class Space { //this class was used for generating maps that the algorithms could then run on
    Random RND = new();
    int demensions;
    List<double> size; //dimensions of the space
    public List<Point> points; //the list of cities

    public Space(int demensions) {  //this is a constuctor for the space, the default size of a space's dimensions is 1000, 
        this.demensions = demensions;
        size = new();
        points = new();
        for(int i = 0; i < demensions; i++){
            size.Add(1000); 
        }
    }

    public Space(int demensions, List<double> sizes, int numberOfPoints) {  //another constuctor allowing sizable spaces, and setting a number of randomly added points
        this.demensions = demensions;
        size = new();
        points = new();
        for(int i = 0; i < demensions; i++){
            size.Add(sizes[i]); 
        }
        for(int i = 0; i < numberOfPoints; i++){
            AddRandomPoint(); 
        }
    }

    public void AddRandomPoint() { //adds a random point to the space using the space's random number generator
        points.Add(new Point(demensions, size, RND));
    }

    public List<double> GetLocation(int index) { //gives location of the city at the index
        return points[index].location;
    }

    public double ReturnDistance(int indexA, int indexB) { //gives the euclidean distance between two cities in the space (highly useful)
        double sumA = 0;
        for(int i = 0; i < demensions; i++){
            sumA += Math.Pow((points[indexA].location[i]-points[indexB].location[i]), 2);
        }

        return Math.Sqrt(sumA);

    }

    public CityMap GetMap() {  //This generates a map (which is a seperate data structure) from the current space state
        CityMap CreatedMap = new();
        CreatedMap.numberOfCities = points.Count;
        CreatedMap.distanceMap = new double[points.Count][];
        for (int i = 0; i < points.Count; i++) CreatedMap.distanceMap[i] = new double[points.Count];

        for(int i = 0; i < points.Count; i++) { //gets the distance for all of the cities in the map.
            for(int j = 0; j < i; j++) {
                CreatedMap.distanceMap[i][j] = this.ReturnDistance(i, j);
                CreatedMap.distanceMap[j][i] = this.ReturnDistance(i, j);
            }
        }

        return CreatedMap;
    }

}