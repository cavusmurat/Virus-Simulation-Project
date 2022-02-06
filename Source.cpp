#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
using namespace std;

const char health[] = { 'X','O','D' };
int matrixSize = 0;
int lifeCycle = 0;
int lifeCycleIndex = 0;
int infectedc = 0;
int normalc = 0;
int recoveredc = 0;
int deadc = 0;
string output2 = "";

class Route {
public:
    int x;
    int y;
};

class Entity {
public:
    string name = "";
    string gates = "";
    char health = ' ';
    int count = 0;
    vector<Route> route;
    bool immune = 0;

};

vector<vector<Entity>> entities; 

void printEntity(Entity entity) {
    cout << "*************************" << endl;
    cout << "name : " << entity.name << endl;
    cout << "gates : " << entity.gates << endl;
    cout << "health : " << entity.health << endl;
    cout << "count : " << entity.count << endl;
    cout << "immune : " << entity.immune << endl;
    cout << "route : ";
    for (int i = 0; i < entity.route.size(); ++i) {
        cout << entity.route[i].x << "x" << entity.route[i].y << " ";
    }
    cout << endl;
    cout << "*************************" << endl;
}

/*void printMatrix() {
    cout << "*********************************************************************************" << endl;
    cout << "Turn: " << lifeCycleIndex+1 << endl;
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            if(entities[i][j].name.empty()){
                cout << " entity ";
                //cout << "{" << "NAME:" << entities[i][j].name << "GATES:" << entities[i][j].gates << "HEALTH:" << entities[i][j].health << "IMMUNE:" << entities[i][j].immune << "COUNT:" << entities[i][j].count << "}";
            }
            else {
                cout << "{" << "   NAME:" << entities[i][j].name << "-GATES:" << entities[i][j].gates << "-HEALTH:" << entities[i][j].health << "-IMMUNE:" << entities[i][j].immune << "-COUNT:" << entities[i][j].count << "}";

            }
            //cout << entities[i][j].health ;
            //printEntity(entities[i][j]);
        }
        cout << endl;
    }
    cout << "*********************************************************************************" << endl;
}*/

string writeMatrix(string output) {
    cout << "Turn: " << lifeCycleIndex + 1 << endl;
    output = output + "Turn: " + to_string(lifeCycleIndex + 1) + "\n";
    cout << "*********************************************************************************" << endl;
    output = output + "------------" + "\n";
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            if (entities[i][j].name.empty()) {
                cout << " * ";
                output = output + " *";
                //cout << "{" << "NAME:" << entities[i][j].name << "GATES:" << entities[i][j].gates << "HEALTH:" << entities[i][j].health << "IMMUNE:" << entities[i][j].immune << "COUNT:" << entities[i][j].count << "}";
            }
            else {
                cout << " * ";
                output = output + " " + entities[i][j].health;
                //cout << "{" << "   NAME:" << entities[i][j].name << "-GATES:" << entities[i][j].gates << "-HEALTH:" << entities[i][j].health << "-IMMUNE:" << entities[i][j].immune << "-COUNT:" << entities[i][j].count << "}";

            }
            //cout << entities[i][j].health ;
            //printEntity(entities[i][j]);           
        }
        cout << endl;
        output = output + " \n";
    }
    cout << "*********************************************************************************" << endl;
    
    output = output + "------------" + "\n";
    return output;
}


int findChar(string gates, string key) {
    if (gates.find(key)<100) {
        return 1;
    }
    else {
        return 0;
    }
}
int moveEntity(int iCurr, int jCurr, int iNext, int jNext) {
    Entity holder;
    if (!entities[iNext][jNext].name.empty()) {
        holder = entities[iNext][jNext];
        entities[iNext][jNext] = entities[iCurr][jCurr];
        entities[iCurr][jCurr] = holder;
    }
    else {
        entities[iNext][jNext] = entities[iCurr][jCurr];
        entities[iCurr][jCurr] = holder;
    }
    return 0;
}

int spreading(int iEntity, int jEntity) {
    int rowStart = ((iEntity - 3) < 0) ? 0 : (iEntity - 3);
    int columnStart = ((jEntity - 3) < 0) ? 0 : (jEntity - 3);
    int row = ((iEntity + 3) >= entities.size()) ? entities.size() - 1 : (iEntity + 3);
    int column = ((jEntity + 3) >= entities.size()) ? entities.size() - 1 : (jEntity + 3);

    if ((entities[iEntity][jEntity].health == 'X' && entities[iEntity][jEntity].count > 3) ||
                entities[iEntity][jEntity].health == 'D') {
        int countA = 0, countB = 0;
        for (int i=rowStart; i <= row; ++i) {
            for (int j=columnStart; j <= column; ++j) {
                if (!entities[i][j].name.empty() && entities[i][j].health != 'D' && entities[i][j].health != 'X') {
                    if (findChar(entities[i][j].gates, "A") == 1 || findChar(entities[i][j].gates, "B") == 1) {
                        if (!entities[i][j].immune) {
                            entities[i][j].health = 'X';
                            entities[i][j].count = 0;
                            infectedc++;
                            normalc--;
                        }
                    }
                }
            }
        }
    }

    if (entities[iEntity][jEntity].health == 'D' && entities[iEntity][jEntity].count >= 5) {
        output2 = output2 + entities[iEntity][jEntity].name + " " + to_string(iEntity) + "x" + to_string(jEntity) + " " + "dead\n";
        entities[iEntity][jEntity].name = "";
        entities[iEntity][jEntity].count = 0;
    }


    if (entities[iEntity][jEntity].health == 'X' && entities[iEntity][jEntity].count > 14) {
        if ((findChar(entities[iEntity][jEntity].gates, "A") + findChar(entities[iEntity][jEntity].gates, "B") +
            findChar(entities[iEntity][jEntity].gates, "C") + findChar(entities[iEntity][jEntity].gates, "D")) >= 3) {
            entities[iEntity][jEntity].health = 'D';
            entities[iEntity][jEntity].count = 0;
            deadc++;
            infectedc--;
        }
        else {
            if (entities[iEntity][jEntity].count >= 30) {
                entities[iEntity][jEntity].health = 'O';
                entities[iEntity][jEntity].immune = 1;
                entities[iEntity][jEntity].count = 0;
                recoveredc++;
                normalc++;
            }
        }
    }
    return 0;
}

void linesIntoWords(string& line, vector<string>& words, char splitter) {
    words.clear();
    if (line[line.size() - 1] != splitter) {
        line.push_back(splitter);
    }
    string temp = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] != splitter) {
            temp.push_back(line[i]);
        }
        else {
            words.push_back(temp);
            temp = "";
        }
    }
}



void setEntity(vector<string> words) {
    Entity entity;
    entity.name = words[1];
    entity.gates = words[2];
    vector<string> coordinates;
    linesIntoWords(words[3], coordinates, 'x');
    if (words.size() == 5) {
        entity.health = 'X';
        infectedc++;
    }
    else {
        entity.health = 'O';
        normalc++;
    }
    entities[stoi(coordinates[0])][stoi(coordinates[1])] = entity;
}

void setEntityRoutes(vector<string> words) {
    vector<string> coordinates;
    int turnNum = stoi(words[1]);
    int x, y;
    for (int index = 1; index < matrixSize-2; ++index) {
        linesIntoWords(words[1 + index], coordinates, 'x');
        x = stoi(coordinates[0]);
        y = stoi(coordinates[1]);
        Route route = { x, y };
        

        bool breakFlag = false;
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                if (to_string(index).compare(entities[i][j].name) == 0) {
                    entities[i][j].route.push_back(route);
                    breakFlag = true;
                    //printEntity(entities[i][j]);
                    break;
                }
            }
            if (breakFlag) {
                break;
            }
        }

        //entity.route.push_back(route);

    }
}




int main() {

    vector<string> words;
    ifstream readingFile;
    string line;
    readingFile.open("INPUT.txt");
    string output = "";
    while (getline(readingFile, line)) {
        linesIntoWords(line, words, ' ');
        if (words[0].compare("size") == 0) {
            matrixSize = stoi(words[1]);
            matrixSize += 1;
            vector<Entity> tempVector;
            Entity entity;
            for (int j = 0; j < matrixSize; ++j) {
                tempVector.push_back(entity);
            }
            for (int i = 0; i < matrixSize; ++i) {
                entities.push_back(tempVector);
            }
        }
        else if (words[0].compare("turn_count") == 0) {
            lifeCycle = stoi(words[1]);
        }
        else if (words[0].compare("entity") == 0) {
            setEntity(words);
            //entityNum++;
        }
        else if (words[0].compare("turn") == 0) {
            setEntityRoutes(words);
        }
    }
    readingFile.close();
    //printMatrix();


    for (; lifeCycleIndex < lifeCycle; ++lifeCycleIndex) {
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                ++entities[i][j].count;
            }
        }
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                if (!entities[i][j].name.empty()) {
                    switch (entities[i][j].health) {
                    case 'X':
                        moveEntity(i, j, entities[i][j].route[lifeCycleIndex].x, entities[i][j].route[lifeCycleIndex].y);
                        spreading(i, j);
                        break;
                    case 'O':
                        moveEntity(i, j, entities[i][j].route[lifeCycleIndex].x, entities[i][j].route[lifeCycleIndex].y);
                        break;
                    case 'D':
                        spreading(i, j);
                        break;
                    default:
                        cout << "Hatali varlik bulundu!" << endl;
                        break;
                    }
                }
            }
        }
        output = writeMatrix(output);
        
    }

    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            if (!entities[i][j].name.empty()) {
                if (entities[i][j].health = 'O') {
                    output2 = output2 + entities[i][j].name + " " + to_string(i) + "x" + to_string(j) + " " + "normal\n";
                }
                else {
                    output2 = output2 + entities[i][j].name + " " + to_string(i) + "x" + to_string(j) + " " + "infected\n";
                }

            }
        }
    }

    ofstream turnsFile;
    turnsFile.open("TURNS.txt");
    turnsFile << output << endl;
    turnsFile.close();

    ofstream writingFile;
    writingFile.open("OUTPUT.txt");
    writingFile << "Normal    :" << " " << normalc << endl;
    writingFile << "Infected  :" << " " << infectedc << endl;
    writingFile << "Dead      :" << " " << deadc << endl;
    writingFile << "Recovered :" << " " << recoveredc << endl;
    writingFile << output2 << endl;
    writingFile.close();
    return 0;
    
   
}
