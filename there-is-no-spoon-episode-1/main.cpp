#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Don't let the machines win. You are humanity's last hope...
 **/

 struct node
 {
    bool occupied = false;
    //int n_x = -1;
    //int n_y = -1;
    int r_x = -1;
    //int r_y = -1;
    int b_y = -1;
    //int b_x = -1;
 };

 void printNode(node &myNode);

int main()
{
    int width; // the number of cells on the X axis
    cin >> width; cin.ignore();
    int height; // the number of cells on the Y axis
    cin >> height; cin.ignore();

    //bool **nodegrid = new bool*[height];
    //bool *nodegrid = new bool[width * height];
    //memset(nodegrid, false, sizeof(bool));     //Initializing in false
    node *nodegrid = new node[width * height];
    int *yFinder = new int[width]; 
    int *xFinder = new int[height];
    memset(yFinder, -1, sizeof(int)*width);
    memset(xFinder, -1, sizeof(int)*height);

    int q =  height * width;
    cerr << "Q_matrix: "<< q << endl;

    for(int i = 0; i < q; i++)
     printNode(nodegrid[i]);

    for(int i = 0; i < width; i++)
     cerr << yFinder[i] << endl;
    cerr << endl;
    for(int i = 0; i < height; i++)
        cerr << xFinder[i] << endl;


    for(int i = 0; i < height; i++) {
        string line;
        getline(cin, line); // width characters, each either 0 or .j 
        cerr << "--------------------------" << endl;
        for(int j = 0; j < width; j++){
            if(line.c_str()[j] == '0'){
                nodegrid[i*width + j].occupied = true;

                cerr << "node(" << j << "," << i << ")" << endl;

                if(yFinder[j] != -1){
                    cerr << "(" << j << "," << yFinder[j] << ").b_y = " << i << endl;
                    nodegrid[yFinder[j]*width + j].b_y = i;
                }
                yFinder[j] = i;
                
                if(xFinder[i] != -1){
                    cerr << "(" << xFinder[i] << "," << i*height << ").r_x = " << j << endl;
                    nodegrid[i*width + xFinder[i]].r_x = j;
                }
                xFinder[i] = j;  
            }// if(line.c_str()[j] == '0')
            cerr << " |" << endl;
        }//for cols
    }//for rows
    //Display Code
    /*
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(nodegrid[i*height + j].occupied){
                cout << j << " " << i << " ";
                if(nodegrid[i*height + j].r_x != -1)
                    cout << nodegrid[i*height + j].r_x  << " " << i << " ";
                else
                    cout << "-1 -1 ";
                //
                if(nodegrid[i*height + j].b_y != -1)
                    cout << j << " " << nodegrid[i*height + j].b_y << " ";
                else
                    cout << "-1 -1";
                //
                cout << endl;
            }//is there a node?
        }//for cols
    }//for rows
    */
    ///*
    for(int i = 0; i < q; i++)
    {
        int curr_x = i % width;
        int curr_y = i / width;
        if(nodegrid[i].occupied){
            cerr << "found!" << endl;
            cout << curr_x << " " << curr_y << " ";
            if(nodegrid[i].r_x != -1)
                cout << nodegrid[i].r_x << " " << curr_y << " ";
            else
                cout << "-1 -1 ";
            //
            if(nodegrid[i].b_y != -1)
                cout << curr_x << " " << nodegrid[i].b_y << " ";
            else
                cout << "-1 -1 ";
            //
            cout << endl;
        }
    }
       // */
    
    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // Three coordinates: a node, its right neighbor, its bottom neighbor
    //cout << "0 0 1 0 0 1" << endl;
}

void printNode(node &myNode)
{
    cerr << "r_x: " << myNode.r_x << endl;
    cerr << "b_y: " << myNode.b_y << endl;
    cerr << endl;
}
