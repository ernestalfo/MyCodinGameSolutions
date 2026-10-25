#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;
#define DEBSTR cerr
#define Qsols 100000

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


//option 1
//int SolsMAX[Qsols];
int getIDX_opt1(int Quri);
int getMax_opt1(int Nuri);
int getSections(int N, int org_idx, int &endN);
//option 2
int getMax_opt2 (int Quri);
int getIDX_opt2(int Quri);

int main()
{
    //option 1
    //memset(SolsMAX, -1, sizeof(SolsMAX));
    //SolsMAX[0] = 1;
    //SolsMAX[1] = SolsMAX[2] = 2; //2, 3, 4 ==>>> 1 , 2, 2
    //
    int n;
    cin >> n; cin.ignore();

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    DEBSTR << n << endl;
    int IDX =  getIDX_opt2(n);
    int MAX;
    if (IDX == 1)
        MAX = getMax_opt2(n);
    else {
        int max2_a = getMax_opt2(IDX);
        int max2_b = getMax_opt2(n - IDX + 1);
        MAX =  max2_a + max2_b - 1;
    }

    cout << MAX << " " << IDX;

    

}
//-----------------------------------------------------------------------------------------------------------
int getSections(int N, int org_idx, int &endN)
{
    endN = N - org_idx + 1;
    if (N - endN >= 2) {
        return org_idx;
    }
    else
        return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------
/*
int getMax_opt1(int Nuri)
{
    int solIDX = Nuri - 2;
    //if (solIDX < Nsols) {
    if(solIDX >= 0) {
        if (SolsMAX[solIDX] != -1){
            return SolsMAX[solIDX];
        }
        else {
            int Na, Nb;
            int OptA, OptB, OptR;
            Na = getSections(Nuri, Nuri /2.0 + 0.5,Nb);
            OptA = getMax_opt1(Na);
            OptB = getMax_opt1(Nb);
            OptR = OptA + OptB;
            if (OptA > 0 && OptB > 0)
                OptR--;
            //Recording into the solutions
            if (OptR > 0) {
                solIDX = Nuri - 2;
                //if (solIDX > Nsols) {
                if (solIDX < 100000)
                    if (SolsMAX[solIDX] != -1){
                        SolsMAX[solIDX] = Nuri;
                        //4Nsols++;
                    }
            }
            return OptR;
        }
    }//  if(solIDX >= 0)
    else
        return 0;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------
//Finding IDX option 1
/*
int getIDX_opt1(int Quri)
{
    int Opt = Quri / 2.0 + 0.5;
    int Middle = log2(Quri);
    //Middle = pow(2, Middle - 1);
    Middle = pow(2, Middle - 1) + 1;

    int Uidx = -1, OptMAX = 0;
    //int  Na_final, Nb_final;

    if (Quri == 2) {
        OptMAX = 1;
        Uidx = 1;
    }
    else if (Quri == 3){
        OptMAX = 2;
        Uidx = 1;
    }
    else {
        for (int i = 1; i <= Middle; i++) {
            int Na, Nb;
            int optA, optB, newOPT;
            Na = getSections(Quri, i, Nb);
            optA = getMax_opt1(Na);
            optB = getMax_opt1(Nb);
            newOPT = optA + optB;
            if (optA > 0 && optB > 0)
                newOPT--;
            if (newOPT == Opt) {
                OptMAX = newOPT;
                Uidx = i;
                //Na_final = Na;
                //Nb_final = Nb;
                break;
            }
            else if (newOPT > OptMAX) {
                OptMAX = newOPT;
                Uidx = i;
                //Na_final = Na;
                //Nb_final = Nb;
            }
        }//for
    }
    return Uidx;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------
//Finding IDX option 2
int getIDX_opt2(int Quri)
{
    //int preMAX[9] = {1, 2, 2, 3, 3, 4, 4, 5, 5}; //2 - 10
    int preIDX[9] = {1, 1, 1, 1, 1, 3, 1, 1, 1}; //2 - 10

    //int Max = 0, Idx = -1;
    int Idx = -1;
    if (Quri >= 2) {
        if (Quri < 11) {
            //int i = N - 2;
            //Max = preMAX[Quri -2];
            Idx = preIDX[Quri -2];
        }
        else {
            //Finding limits:
            int p = log2(Quri);
            int n_pot = pow(2, p);
            int L_min = (n_pot + 1) -pow(2, p - 3);
            int L_max = n_pot + 2;

            if (L_max < Quri) { //Try to lok in the next limits:
                int tmp_p = p, tmp_npot = n_pot, tmp_L_min = L_min, tmp_L_max = L_max;
                p++;
                n_pot = pow(2, p);
                L_min = (n_pot + 1) -pow(2, p - 3);
                L_max = n_pot + 2;
                if (Quri < L_min) { //retrive values:
                    p = tmp_p;
                    n_pot = tmp_npot;
                    L_min = tmp_L_min;
                    L_max = tmp_L_max;
                }
            }
            //
            int y = Quri - n_pot;
            int p_2 = p;
            int base = 2;
            if (y == 0) {
                p_2 = --p;
                base = pow(2, p_2);
            }
            else {
                p_2 = log2(y);
                base = pow(2, p_2);
                if (y < base + 1) {
                    if (p_2)
                        p_2--;
                    base = pow(2, p_2);
                }
            }
            //
            if (Quri >= L_min && Quri <= L_max) {
                Idx = 1;
                //Max = N - base;
            }
            else {
                int p_3 = --p_2;
                if (y > base + pow(2, p_3) + 1)
                //if (y > base + pow(2, p_2))
                    Idx = y;
                else
                    Idx = base + 1;
            }
        }
        //cout << Max << " " << Idx << endl;
        return Idx;
    }//if(N >= 2)
    else
        return -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------
int getMax_opt2 (int Quri)
{
    int preMAX[9] = {1, 2, 2, 3, 3, 4, 4, 5, 5}; //2 - 10
    if (Quri < 11) {
        if (Quri >= 2)
            return preMAX[Quri - 2];
        return 0;
    }
    //
    int p = log2(Quri);
    int n_pot = pow(2, p);
    int n_pot2 = pow (2, p - 1);
    int L_min = (n_pot + 1);
    int L_max = L_min + n_pot2;
    if (Quri >= L_min && Quri <= L_max)
        return n_pot2 + 1;

    int result =  Quri - n_pot;
    if (result == 0)
        return Quri - n_pot2;

    return result;
}
