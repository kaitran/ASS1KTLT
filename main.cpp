#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#define MAXINPUT 20000
using namespace std;

float data_input[MAXINPUT][2]= {};
float data_trn[(MAXINPUT*2/3)+2][2] = {};
float data_tst[((MAXINPUT/3)+2)][2] = {};
double bieudo[10] = {};
//Ham lay gia tri tu ham cin < assignment1.input.txt
bool set_input_data(float &num_iterations, float &learning_rate, float &start_a, float &start_b, float &num_folds, int &max_xy){
    string str, string_param;
    float x, y;
    max_xy = 0;
    getline(cin, str); //skip ---: read form file or console
    getline(cin, str); //skip ---: read form file or console
    getline(cin, str); //skip ---: read form file or console
    getline(cin, str); 	stringstream scin1(str);
    scin1 >> string_param; scin1 >> num_iterations; //get a parameter
    getline(cin, str); 	stringstream scin2(str);
    scin2 >> string_param; scin2 >> learning_rate;
    getline(cin, str); 	stringstream scin3(str);
    scin3 >> string_param; scin3 >> start_a;
    getline(cin, str); 	stringstream scin4(str);
    scin4 >> string_param; scin4 >> start_b;
    getline(cin, str); 	stringstream scin5(str);
    scin5 >> string_param; scin5 >> num_folds;
    getline(cin, str); //skip ---: read form file or console
    getline(cin, str); //skip ---: read form file or console
    getline(cin, str); //skip ---: read form file or console
    while(getline(cin, str)){
        stringstream scin6(str);
        scin6 >> data_input[max_xy][0];
        scin6 >> data_input[max_xy][1];
        if(data_input[max_xy][0] <= 0 || data_input[max_xy][1] <= 0){
            break;
        }
        max_xy++;
    }
    return true;
}
//Ham gradient vector
bool get_gradient_vector(double &grad_a, double &grad_b, float &a, float &b, int max_trn, int max_tst, int max_xy){
    for (int i = 0; i < max_trn; i++) {
        float xi = data_trn[i][0];
        float ti = data_trn[i][1];
        grad_a += (a * xi + b - ti)*xi;
        grad_b += (a * xi + b - ti);
    }
    double dolonAB = sqrt(pow(grad_a,2) + pow(grad_b,2));
    grad_a = grad_a / dolonAB;
    grad_b = grad_b / dolonAB;
    return true;
}
//Phuong trinh hoi quy y = ax+b
bool phuongtrinhhoiquy(float &start_a, float &start_b, int max_trn, int max_tst, int max_xy, float &num_iterations, float &learning_rate){
    float p_A = start_a;
    float p_B = start_b;
    double grad_a, grad_b;
    for (int i = 0; i < num_iterations; i++) {
        grad_a = grad_b = 0.0;
        get_gradient_vector(grad_a,grad_b,p_A,p_B, max_trn, max_tst, max_xy);
        p_A = p_A - learning_rate*grad_a;
        p_B = p_B - learning_rate*grad_b;
    }
    start_a = p_A;
    start_b = p_B;
    return true;
}
bool set_chart(float &start_a, float &start_b, double &e_RMSD, int max_trn, int max_tst, int max_xy){
    double e_avg = 0.0, xiMa = 0.0, v_Min = 0.0, v_Max = 0.0, v_Avg10 = 0.0;
    double p_A = start_a, p_B = start_b;
    double  D = max_tst;
    for (int i = 0; i < max_tst; i++) {
        float xi = data_tst[i][0];
        float ti = data_tst[i][1];
        e_RMSD += pow((p_A * xi + p_B - ti),2);
    }
    e_RMSD = sqrt(e_RMSD/max_tst);
    for (int i = 0; i < max_tst; i++) {
        float xi = data_tst[i][0];
        float ti = data_tst[i][1];
        e_avg += (p_A * xi + p_B - ti)*(1/D);
    }
    for (int i = 0; i < max_tst; i++) {
        float xi = data_tst[i][0];
        float ti = data_tst[i][1];
        xiMa += pow((p_A * xi + p_B - ti - e_avg),2)*(1/D);
    }
    xiMa = sqrt(xiMa);
    v_Min = -3 * xiMa;
    v_Max = 3 * xiMa;
    v_Avg10 = 6*xiMa/10;
    for (int i = 0; i < max_tst; i++) {
        double e_i = 0.0;
        float xi = data_tst[i][0];
        float ti = data_tst[i][1];
        e_i = (p_A * xi + p_B - ti);
        if((v_Min <= e_i) && (e_i <= v_Max)){
            if(e_i <= v_Min + v_Avg10*1){
                bieudo[0] += 1;
            }else if(e_i <= v_Min + v_Avg10*2){
                bieudo[1] += 1;
            }else if(e_i <= v_Min + v_Avg10*3){
                bieudo[2] += 1;
            }else if(e_i <= v_Min + v_Avg10*4){
                bieudo[3] += 1;
            }else if(e_i <= v_Min + v_Avg10*5){
                bieudo[4] += 1;
            }else if(e_i <= v_Min + v_Avg10*6){
                bieudo[5] += 1;
            }else if(e_i <= v_Min + v_Avg10*7){
                bieudo[6] += 1;
            }else if(e_i <= v_Min + v_Avg10*8){
                bieudo[7] += 1;
            }else if(e_i <= v_Min + v_Avg10*9){
                bieudo[8] += 1;
            }else{
                bieudo[9] += 1;
            }
        }
    }
    int total_id = 0;
    for (int i = 0; i < 10; i++) {
        total_id += bieudo[i];
    }
    for (int i = 0; i < 10; i++) {
        bieudo[i] = bieudo[i]/total_id;
    }
}
bool phanphoi_xy(int &id, int &max_trn, int &max_tst, int max_xy, int num_folds){
    if(id == 1){
        max_tst = max_xy/num_folds;
        max_trn = max_xy - max_tst;
        for (int i = 0; i < max_tst; i++) {
            data_tst[i][0] = data_input[i][0];
            data_tst[i][1] = data_input[i][1];
        }
        int temp_i = max_tst;
        for (int i = 0; i < max_trn; i++) {
            data_trn[i][0] = data_input[temp_i][0];
            data_trn[i][1] = data_input[temp_i][1];
            temp_i++;
        }
    }else if( id == num_folds){
        max_tst = max_xy - (num_folds - 1)*max_xy/num_folds;
        max_trn = max_xy - max_tst;
        int temp_i = max_trn;
        for (int i = 0; i < max_tst; i++) {
            data_tst[i][0] = data_input[temp_i][0];
            data_tst[i][1] = data_input[temp_i][1];
            temp_i++;
        }
        for (int i = 0; i < max_trn; i++) {
            data_trn[i][0] = data_input[i][0];
            data_trn[i][1] = data_input[i][1];
        }
    }else{
        max_tst = max_xy/num_folds;
        max_trn = max_xy - max_tst;
        int temp_i = max_tst*(id - 1);
        for (int i = 0; i < max_tst; i++) {
            data_tst[i][0] = data_input[temp_i][0];
            data_tst[i][1] = data_input[temp_i][1];
            temp_i++;
        }
        int temp_f = max_tst*(id -1);
        for (int i = 0; i < temp_f; i++) {
            data_trn[i][0] = data_input[i][0];
            data_trn[i][1] = data_input[i][1];
        }
        int temp_r = max_xy - (max_tst + max_tst*(id -1));
        for (int i = 0; i < temp_r; i++) {
            data_trn[temp_f][0] = data_input[temp_i][0];
            data_trn[temp_f][1] = data_input[temp_i][1];
            temp_i++;
            temp_f++;
        }
    }
}
int main(){
    string str, string_param;
    float num_iterations, learning_rate, start_a, start_b, num_folds;
    int max_xy = 0;
    float a = 0.0, b = 0.0;
    set_input_data(num_iterations, learning_rate, start_a, start_b, num_folds,max_xy);
    int max_trn = 0, max_tst = 0;

    cout << string(91, '-') << endl;
    cout << "Output of the validation " << endl;
    cout << string(91, '-') << endl;

    for (int i = 1; i <= num_folds; i++) {
        a = start_a ; b = start_b;
        phanphoi_xy(i,max_trn, max_tst, max_xy, num_folds);
        phuongtrinhhoiquy(a, b, max_trn, max_tst, max_xy, num_iterations, learning_rate);
        double e_RMSD = 0.0;
        set_chart(a, b, e_RMSD, max_trn, max_tst, max_xy);

            cout << right << fixed << setw(7) << setprecision(3) << a;
            cout << right << fixed << setw(7) << setprecision(3) << b;
            cout << right << fixed << setw(7) << setprecision(3) << e_RMSD;
            for (int i = 0; i < 10; i++) {
                cout << right << fixed << setw(7) << setprecision(3) << bieudo[i];
            }
            cout << endl;
    }
    return 0;
}