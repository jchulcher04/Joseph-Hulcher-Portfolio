#include<bits/stdc++.h>
using namespace std;

/*
int isclear(long double x, long double y, long double z){
    int bad = 0;
    if(1 / (long double) 3 > x || x > 2 / (long double) 3) bad++;
    if(1 / (long double) 3 > y || y > 2 / (long double) 3) bad++;
    if(1 / (long double) 3 > z || z > 2 / (long double) 3) bad++;
    
    if(bad < 2) return 1;
    return 0;

}
*/

int main() {
    int L;
    int xnum, xden;
    int ynum, yden;
    int znum, zden;

    cin >> L;
    cin >> xnum; cin >>  xden;
    cin >> ynum; cin >> yden;
    cin >> znum; cin >> zden;

    /*
    long double x = (long double) xnum / xden;
    long double y = (long double) ynum / yden;
    long double z = (long double) znum / zden;

    for(int i = 0; i < L; i++){
        if(isclear(x, y, z)) {
            cout << 1;
            return 1;
        }

        x *= 3;
        while(x > 1) x -= 1;

        y*= 3;
        while(y > 1) y -= 1;

        z *= 3;
        while(z > 1) z-= 1;
    }
    */

    for(int i = 0; i < L; i++){
        int bad = 0;
        if(xnum * 3 >= xden * 2 || xnum * 3 <= xden) bad++;
        if(ynum * 3 >= yden * 2 || ynum * 3 <= yden) bad++;
        if(znum * 3 >= zden * 2 || znum * 3 <= zden) bad++;
        if(bad < 2) {
            cout << 0;
            return 0;
        }

        xnum = xnum * 3;
        while(xnum > xden) xnum = xnum - xden;
        ynum = ynum * 3;
        while(ynum > yden) ynum = ynum - yden;
        znum = znum * 3;
        while(znum > zden) znum = znum - zden;

    }

    cout << 1;
    return 0;

}