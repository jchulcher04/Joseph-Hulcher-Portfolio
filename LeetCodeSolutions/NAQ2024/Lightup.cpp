#include<bits/stdc++.h>
using namespace std;

int main() {
    int size;
    cin >> size;

    char grid[30][30];
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            char hold;
            cin >> grid[i][j];
        }
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){

            if(grid[i][j] == '.'){
                bool fail = 1;
                for(int k = i -1; k >= 0; k--){
                    if(grid[k][j] == '?') {
                        fail = 0;
                        break;
                    }
                    if(grid[k][j] != '.') break;
                }
                for(int k = i +1; k < size; k++){
                    if(grid[k][j] == '?') {
                        fail = 0;
                        break;
                    }
                    if(grid[k][j] != '.') break;
                }
                for(int k = j -1; k >= 0; k--){
                    if(grid[i][k] == '?') {
                        fail = 0;
                        break;
                    }
                    if(grid[i][k] != '.') break;
                }
                for(int k = j +1; k < size; k++){
                    if(grid[i][k] == '?') {
                        fail = 0;
                        break;
                    }
                    if(grid[i][k] != '.') break;
                }
                if(fail) {
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == '?'){
                bool fail = 0;
                for(int k = i -1; k >= 0; k--){
                    if(grid[k][j] == '?') {
                        fail = 1;
                        break;
                    }
                    if(grid[k][j] != '.') break;
                }
                for(int k = i +1; k < size; k++){
                    if(grid[k][j] == '?') {
                        fail = 1;
                        break;
                    }
                    if(grid[k][j] != '.') break;
                }
                for(int k = j -1; k >= 0; k--){
                    if(grid[i][k] == '?') {
                        fail = 1;
                        break;
                    }
                    if(grid[i][k] != '.') break;
                }
                for(int k = j +1; k < size; k++){
                    if(grid[i][k] == '?') {
                        fail = 1;
                        break;
                    }
                    if(grid[i][k] != '.') break;
                }
                if(fail) {
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == 'X') {

            }

            else if(grid[i][j] == '0'){
                int count = 0;
                if(j > 0 && grid[i][j-1] == '?') count++;
                if(j < size - 1 && grid[i][j+1] == '?') count++;
                if(i > 0 && grid[i-1][j] == '?') count++;
                if(i < size -1 && grid[i+1][j] == '?') count++;
                if(count != 0){
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == '1'){
                int count = 0;
                if(j > 0 && grid[i][j-1] == '?') count++;
                if(j < size - 1 && grid[i][j+1] == '?') count++;
                if(i > 0 && grid[i-1][j] == '?') count++;
                if(i < size -1 && grid[i+1][j] == '?') count++;
                if(count != 1){
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == '2'){
                int count = 0;
                if(j > 0 && grid[i][j-1] == '?') count++;
                if(j < size - 1 && grid[i][j+1] == '?') count++;
                if(i > 0 && grid[i-1][j] == '?') count++;
                if(i < size -1 && grid[i+1][j] == '?') count++;
                if(count != 2){
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == '3'){
                int count = 0;
                if(j > 0 && grid[i][j-1] == '?') count++;
                if(j < size - 1 && grid[i][j+1] == '?') count++;
                if(i > 0 && grid[i-1][j] == '?') count++;
                if(i < size -1 && grid[i+1][j] == '?') count++;
                if(count != 3){
                    cout << 0;
                    return 0;
                }
            }

            else if(grid[i][j] == '4'){
                int count = 0;
                if(j > 0 && grid[i][j-1] == '?') count++;
                if(j < size - 1 && grid[i][j+1] == '?') count++;
                if(i > 0 && grid[i-1][j] == '?') count++;
                if(i < size -1 && grid[i+1][j] == '?') count++;
                if(count != 4){
                    cout << 0;
                    return 0;
                }
            }

        }
    }

    cout << 1;
    return 0;
}