using namespace std;
#include <fstream>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <unordered_map>

bool stringCompare(string a, string b) //for comparing strings without checking for case
{
    int length = a.size();

    if (b.size() != length) return false;

    for (unsigned int i = 0; i < length; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

struct atom{ //atom holds info, and pointer to next atom. In the case of a list, it is a special atom with the data "(", in which it has an additional special pinter to the first atom of the list
    string data;
    atom* nextAtom = nullptr;
    atom* specialPointer = nullptr;
};

unordered_map<string, atom*> dataMap; //hold the data of the set function
unordered_map<string, atom*> functionMap; // holds all declared functions


void parseFile(string file, atom* startingAtom){ //this turns source code into the internal data structure
    ifstream myfile;
    myfile.open(file);
    if(!myfile) { //checks that file is properly opened
        cout << "Failed to open file :/\n";
        exit(1);
    }

    stack<atom*> listLayers;
    atom* currentAtom = startingAtom;

    string line;
    if(myfile.is_open()){ //parsing code
        while( getline (myfile, line)){

            string data;
            stringstream lineStream;

            int i = 0;
            while(line[i] != '\0'){ //inserts spaces around important caracters
                if(line[i] == '(' || line[i] == ')' || line[i] == ';'){
                    line.insert(i, " ");
                    i +=2;
                    line.insert(i, " ");
                }
                i++;
            }

            lineStream << line;

            


            while(lineStream >> data){  // creates the data structure that will be run

                
                if(data == "("){ //if beginning of list, go down one level and start working
                    currentAtom->data = "(";

                    listLayers.push(currentAtom);

                    atom* listBegin = new atom;
                    currentAtom->specialPointer = listBegin;
                    currentAtom = listBegin;
                } else if (data == ")"){ // if end of list go back up one level
                    currentAtom->data = "";

                    currentAtom = listLayers.top();
                    listLayers.pop();

                    atom* nextAtom = new atom;
                    currentAtom->nextAtom = nextAtom;
                    currentAtom = currentAtom->nextAtom;
                    
                } else if (data == ";") { // this is so we can add comments in lines
                    break;
                } else if (data == "\\*") { // this for demarking everything after as comments in a file
                    currentAtom->data = "";
                    return; 
                } else {
                    if(data == "nil") data = ""; //so we have optional nil
                    currentAtom->data = data;
                    

                    atom* nextAtom = new atom;
                    currentAtom->nextAtom = nextAtom;
                    currentAtom = nextAtom;
                }
            }           
        }
    }
    currentAtom->data = "";

}

atom* interpreter(atom* mainCode, unordered_map<string, atom*> &localVariables); //the interpreter runs one list. the core of the runner

atom* linkResolver(atom* codeLine, unordered_map<string, atom*> &localVariables){ //returns the link to the refered atom from a name
    if(dataMap.find(codeLine->data) != dataMap.end()) { //check if its a referene to a variable
        return dataMap[codeLine->data];
    } else if(localVariables.find(codeLine->data) != localVariables.end()){
        return localVariables[codeLine->data];
    }
    return codeLine;
}

atom* requestData(atom* codeLine, unordered_map<string, atom*> &localVariables){ // checker if list when i want an atom, if list then the list is evaluated first
    codeLine = linkResolver(codeLine, localVariables);
    if(codeLine->data == "("){ //check if start of list
        return interpreter(codeLine->specialPointer, localVariables);
    } 
    return codeLine;
}

atom* moveFoward(atom* codeline, int timesToMoveFoward){ //moves to next atom this number of times, kinda pointless in retrospect because the syntax does not make it better...
    for(int i = 0; i < timesToMoveFoward; i++){
        codeline = codeline->nextAtom;
    } 
    return codeline;
}

atom* interpreter(atom* mainCode, unordered_map<string, atom*> &localVariables){ //the interpreter
    atom* currentAtom = mainCode; //line of code

    atom* toReturn = new atom;
    toReturn->data = ""; //default return
    
    while(currentAtom->data != ""){ //heart of the runner

        if (stringCompare(currentAtom->data, "print")) { // crashes if at end of list, also "invisible" function, meaning this works: (print toPrint function variables) DOES NOT RETURN
            currentAtom = currentAtom->nextAtom;
            cout << requestData(currentAtom, localVariables)->data << "\n";
            toReturn = requestData(currentAtom, localVariables);
        } 

        else if (stringCompare(currentAtom->data, "quote")){
            currentAtom = moveFoward(currentAtom, 1); // determine wether to resolve links for quoting
            if(currentAtom->data == "!"){
                currentAtom = moveFoward(currentAtom, 1);
            } else {
                currentAtom = linkResolver(currentAtom, localVariables);
            }

            if(currentAtom->data != "("){ // add space to quote, breaking links
                toReturn->data = " " + currentAtom->data;
                return toReturn;
            }

            string sentance; // if its a list, print all the items of the list, seperated by a " "
            atom* inside = currentAtom->specialPointer;
            while(inside->data != ""){
                sentance = sentance + " " + inside->data;
                inside = moveFoward(inside, 1);
            }
            toReturn->data = sentance;
            return toReturn;
        }

        else if (stringCompare(currentAtom->data, "eval")){
            return requestData(moveFoward(currentAtom, 1), localVariables);
        }

        else if (stringCompare(currentAtom->data, "if")) { // if next atom is true, then second atom returned, otherwise thrid is. RETURNS
            currentAtom = currentAtom->nextAtom;
            if(requestData(currentAtom, localVariables)->data == ""){ //if false
                currentAtom = moveFoward(currentAtom, 2); // skip first expression
                return requestData(currentAtom, localVariables); //second expression is evaluated
            } else {
                currentAtom = moveFoward(currentAtom, 1);; //this time the second expression is evaluated
                toReturn = requestData(currentAtom, localVariables);
                currentAtom = moveFoward(currentAtom, 1);
                return toReturn;
            }
        } 
        else if (stringCompare(currentAtom->data, "cond")) { // takes pairs of arguments, when one of the first is true, the second is RETURNED
            currentAtom = currentAtom->nextAtom;
            while(1){
                if(requestData(currentAtom, localVariables)->data == ""){ //if false
                    currentAtom = moveFoward(currentAtom, 2); // skip first expression
                    
                } else {
                    currentAtom = moveFoward(currentAtom, 1);
                    return requestData(currentAtom, localVariables); // next expression is evaluated
                }
            }
        } 

        else if(stringCompare(currentAtom->data, "set")){ //setter sets the next atom's data to be the name of the link to the third atom RETURNS
            currentAtom = moveFoward(currentAtom, 1); 
            dataMap[currentAtom->data] = linkResolver(moveFoward(currentAtom, 1), localVariables); 
            toReturn->data = currentAtom->data;
            return toReturn;
        } 

        else if(dataMap.find(currentAtom->data) != dataMap.end()) { //returns the setter's data if a links is found RETURNS
            //cout << "link activated\n";
            return dataMap[currentAtom->data];
        } 

        else if(stringCompare(currentAtom->data, "define")){ //define function, sets the name to be a link back to the funciton code, which is the s-expresion RETURNS name of function
            currentAtom = moveFoward(currentAtom, 1); //move to name
            toReturn->data = currentAtom->data; //return is name of function
            functionMap[currentAtom->data] = moveFoward(currentAtom, 1); //set name link in map to variables
            currentAtom = moveFoward(currentAtom, 2); //move to fucntion body so it will move on after
            return toReturn;
        }

        else if(functionMap.find(currentAtom->data) != functionMap.end()){ // WORKS!
            atom* variables = functionMap[currentAtom->data]->specialPointer; // get link to variables
            atom* functionBody = functionMap[currentAtom->data]->nextAtom; //get link to function body
            unordered_map<string, atom*> functionArgumentMap;
            

            currentAtom = moveFoward(currentAtom, 1);
            
            while(variables->data != ""){
                if(currentAtom->data == "!"){
                    currentAtom = moveFoward(currentAtom, 1);
                    functionArgumentMap[variables->data] = linkResolver(currentAtom, localVariables);
                } else {
                    functionArgumentMap[variables->data] = requestData(currentAtom, localVariables);
                }
                variables = moveFoward(variables, 1);
                currentAtom = moveFoward(currentAtom, 1); 
            }
            
            toReturn = interpreter(functionBody->specialPointer, functionArgumentMap);
            return toReturn; // kind of want to figure out why this doesn't work but later
        }

        else if(localVariables.find(currentAtom->data) != localVariables.end()){ //same as data but with  local variables, DOESN'T RETURN?
            toReturn = localVariables[currentAtom->data];
        }

        else if(currentAtom->data == "("){ //DOESN't RETURN default to evaluate found list, quite useful
            toReturn = (interpreter(currentAtom->specialPointer, localVariables));
        } 

        else if(stringCompare(currentAtom->data, "cons")) { // copies first expression, creates a list with first copy second, with next pointer on end list being ender // NOT WORKING YETs
            atom* frontCopy = new atom;
            atom* startOfList = new atom;
            atom* ender = new atom;

            currentAtom = moveFoward(currentAtom, 1);
            frontCopy->data = requestData(currentAtom, localVariables)->data;

            currentAtom = moveFoward(currentAtom, 1);
            frontCopy->nextAtom = linkResolver(currentAtom, localVariables)->specialPointer;

            /*
            if(stringCompare(frontCopy->nextAtom->data, "cdr") || stringCompare(frontCopy->nextAtom->data, "cons")){
                frontCopy->nextAtom = interpreter(frontCopy->nextAtom, localVariables);
            }*/

            ender->data = "";

            startOfList->data = "(";
            startOfList->specialPointer = frontCopy;
            startOfList->nextAtom = ender;

            return startOfList;
        }

        else if (stringCompare(currentAtom->data, "car")){ //the thing about these lists is the orginal rest of the list is just fucking taken, no copies made only passing pointerss
            currentAtom = moveFoward(currentAtom, 1); //current fix is to just evualate if its cdr, not if anything else but that would be broken for lists

            atom* extra = linkResolver(currentAtom, localVariables);

            atom* copy = new atom;
            

            copy->data = extra->specialPointer->data;
            copy->nextAtom = extra->specialPointer->nextAtom;
            copy->specialPointer = extra->specialPointer->specialPointer;

            

            if(stringCompare(extra->specialPointer->data, "cdr") || stringCompare(extra->specialPointer->data, "cons")){
                copy = interpreter(copy, localVariables)->specialPointer;
            }
            
            return copy;
        }

        else if (stringCompare(currentAtom->data, "cdr")) { // IDK IF THIS WORKS WELL BUT WE BE TRYING
            atom* newList = new atom;
            newList->data = "(";

            currentAtom = moveFoward(currentAtom, 1);
            atom* check = linkResolver(currentAtom, localVariables);
            

            if(stringCompare(check->specialPointer->data, "cdr") || stringCompare(check->specialPointer->data, "cons")){
                newList->specialPointer = interpreter(check->specialPointer, localVariables)->specialPointer->nextAtom;
            } else {
                newList->specialPointer = check->specialPointer->nextAtom;
            }
            

            return newList;

        }

        else if (currentAtom->data == "+"){ // +implementation, takes two arguments
            currentAtom = moveFoward(currentAtom, 1);
            int firstTerm = stoi(requestData(currentAtom, localVariables)->data);
            currentAtom = moveFoward(currentAtom, 1);
            int secondTerm = stoi(requestData(currentAtom, localVariables)->data);
            toReturn->data = to_string(firstTerm + secondTerm);
            return toReturn;
        } 

        else if (currentAtom->data == "-"){ // +implementation, takes two arguments
            currentAtom = moveFoward(currentAtom, 1);
            int firstTerm = stoi(requestData(currentAtom, localVariables)->data);
            currentAtom = moveFoward(currentAtom, 1);
            int secondTerm = stoi(requestData(currentAtom, localVariables)->data);
            toReturn->data = to_string(firstTerm - secondTerm);
            return toReturn;
        } 

        else if (currentAtom->data == "*"){ // +implementation, takes two arguments
            currentAtom = moveFoward(currentAtom, 1);
            int firstTerm = stoi(requestData(currentAtom, localVariables)->data);
            currentAtom = moveFoward(currentAtom, 1);
            int secondTerm = stoi(requestData(currentAtom, localVariables)->data);
            toReturn->data = to_string(firstTerm * secondTerm);
            return toReturn;
        } 

        else if (currentAtom->data == "/"){ // +implementation, takes two arguments
            currentAtom = moveFoward(currentAtom, 1);
            int firstTerm = stoi(requestData(currentAtom, localVariables)->data);
            currentAtom = moveFoward(currentAtom, 1);
            int secondTerm = stoi(requestData(currentAtom, localVariables)->data);
            toReturn->data = to_string(firstTerm / secondTerm);
            return toReturn;
        } 

        else if(currentAtom->data == "="){ //compares two arguments, compares the value of the two next resolved expressions
            string data1;
            string data2;
            toReturn = new atom;

            currentAtom = moveFoward(currentAtom, 1);
            data1 = requestData(currentAtom, localVariables)->data;

            currentAtom = moveFoward(currentAtom, 1);
            data2 = requestData(currentAtom, localVariables)->data;

            if(data1 == data2) toReturn->data = "T";
            else toReturn->data = "";

            return toReturn;
        }

         

        else if(currentAtom->data == "<") {
            string data1;
            string data2;
            toReturn = new atom;

            currentAtom = moveFoward(currentAtom, 1);
            data1 = requestData(currentAtom, localVariables)->data;

            currentAtom = moveFoward(currentAtom, 1);
            data2 = requestData(currentAtom, localVariables)->data;

            if(stoi(data1) < stoi(data2)) toReturn->data = "T";
            else toReturn->data = "";
            return toReturn;
        }

        else if(currentAtom->data == ">") {
            string data1;
            string data2;
            toReturn = new atom;

            currentAtom = moveFoward(currentAtom, 1);
            data1 = requestData(currentAtom, localVariables)->data;

            currentAtom = moveFoward(currentAtom, 1);
            data2 = requestData(currentAtom, localVariables)->data;

            if(stoi(data1) > stoi(data2)) toReturn->data = "T";
            else toReturn->data = "";
            return toReturn;
        }

        else if (stringCompare(currentAtom->data, "and?")) { // if neither of the next two evaluated terms are null, it is true
            currentAtom = moveFoward(currentAtom, 1);
            string firstTerm = requestData(currentAtom, localVariables)->data;
            currentAtom = moveFoward(currentAtom, 1);
            string secondTerm = requestData(currentAtom, localVariables)->data;
            if(firstTerm == "" || secondTerm == ""){
                toReturn->data = "";
            } else {
                toReturn->data = "T";
            }
            return toReturn;

        }

        else if (stringCompare(currentAtom->data, "or?")) { // is true if either of the next two terms are not nil
            currentAtom = moveFoward(currentAtom, 1);
            string firstTerm = requestData(currentAtom, localVariables)->data;
            currentAtom = moveFoward(currentAtom, 1);
            string secondTerm = requestData(currentAtom, localVariables)->data;
            if(firstTerm == "" && secondTerm == ""){
                toReturn->data = "";
            } else {
                toReturn->data = "T";
            }
            return toReturn;

        }

        else if (stringCompare(currentAtom->data, "nil?")) { //true if the next evauated exrpession is nil EVALUATES
            currentAtom = moveFoward(currentAtom, 1);
            if(requestData(currentAtom, localVariables)->data == "") toReturn->data = "T";
            else toReturn->data = "";
            return toReturn;
        }

        else if (stringCompare(currentAtom->data, "number?")){ // checks if next element just is a number NO EVALUATION
            toReturn->data = "T";
            currentAtom = moveFoward(currentAtom, 1);
            int i = 0;
            while(currentAtom->data[i] != '\0'){
                if(!isdigit(currentAtom->data[i])) {toReturn->data = ""; break; }
                i++;
            }
            return toReturn;
        }

        else if (stringCompare(currentAtom->data, "list?")){ // checks if next element just is a list NO EVALUATION
            currentAtom = moveFoward(currentAtom, 1);
            if(linkResolver(currentAtom, localVariables)->data == "(") toReturn->data = "T";
            else toReturn->data = ""; 
            return toReturn;
        }

        else if (stringCompare(currentAtom->data, "symbol?")){ // checks if next element just is a symbol NO EVALUATION
            toReturn->data = "";
            currentAtom = moveFoward(currentAtom, 1);
            if(dataMap.find(currentAtom->data) != dataMap.end()) { //check if its a referene to a variable
                toReturn->data = "T";
            } else if(localVariables.find(currentAtom->data) != localVariables.end()){
                toReturn->data = "T";
            } else if(functionMap.find(currentAtom->data) != functionMap.end()){
                toReturn->data = "T";
            }

            return toReturn;
        }

        else {
            toReturn->data = currentAtom->data;
        }   

        currentAtom = moveFoward(currentAtom, 1); //move to next atom after code is executed
    }

    return toReturn;
    }

void readDataStructure(atom* currentAtom) { //used to print out the program as a data structure to see if the file is parsed correctly
    while(currentAtom != nullptr){ //sees top list of program
    cout << currentAtom->data << " ";
        if(currentAtom->specialPointer != nullptr) {
            cout << " ";
            readDataStructure(currentAtom->specialPointer);
        }
        currentAtom = currentAtom->nextAtom;
    }
    cout << " ) ";
}

  

int main(int argc, char **argv){
    
    if(argc != 2 && argc != 3) { //checks for right number of arguments
        cout << "wrong number of arguments";
        exit(1);
    }

    atom* firstAtom = new atom; //for the main program
    atom* secondAtom = new atom; //in theory for treating whole program like a function
    atom* currentAtom;

    parseFile(argv[1], firstAtom); //turns file into runnable data structure
    

    
    if(argc == 3) { //debug mode info
        currentAtom = firstAtom;
        readDataStructure(currentAtom); // if second argument of 1 is included, print the program
        
    }
    

    /*
    while(currentAtom != nullptr){ //sees top list of program
        cout << currentAtom->data << " | ";
        currentAtom = currentAtom->nextAtom;
    }
    */
    currentAtom = firstAtom;
    string finalOut = (interpreter(currentAtom, dataMap))->data; //runs the program
    cout << "\n";

    if(argc == 3) cout << "Final Return: " << finalOut << "\n" ; // debug mode info
    
}


