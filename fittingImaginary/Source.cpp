/*
���f�G�l���M�[�ŗL�l�̒T��
-> �܂������݂̂̃s�[�N��T���A�����̓t�B�b�e�B���O�ŒT��
*/

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "libfftw3-3.lib")
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<utility>
#include<string>
#include<algorithm>
#include<functional>
#include<complex>
#include<chrono>
#include<fftw3.h>
using namespace std;
using namespace std::chrono;
using Complex = complex<double>;
using vC = vector<Complex>;
using vvC = vector<vC>;
using vvvC = vector<vvC>;
inline fftw_complex* fftwcast(Complex* f){ return reinterpret_cast<fftw_complex*>(f); }

const double X_BEGIN = -5.0, X_END = 25.0; //�n�̗��[
const double L = X_END - X_BEGIN; //��ԕ�
const int N = 512; //��ԕ�����
const double DELTA_X = L / N;

const double T_END = 50; //�I������
const int TN = T_END * 10; //���ԕ�����
const double dt = T_END / TN; //���ԍ��ݕ�

const double S = 0.9; //�g���̕�
const double X = -0.5; //�����g���̌��_����̂���
const double X_OPT = 4.0; //���w�|�e���V������������ʒu
const double b = 1.0 / 3.0; //3���|�e���V�����̌W��

const double E_BEGIN_real = -1.2, E_END_real = 0.0;
const int EN_real = 500;
const double dE_real = (E_END_real - E_BEGIN_real) / EN_real;

const double E_BEGIN_imag = 0.0, E_END_imag = 0.1;
const int EN_imag = 400;
const double dE_imag = (E_END_imag - E_BEGIN_imag) / EN_imag;

double i2x(int i){
    return X_BEGIN + i * DELTA_X;
}
double i2k(int i){
    return 2 * M_PI * ((i < N / 2) ? i : i - N) / L;
}
double i2E(double begin, int index, double width){
    return begin + index * width;
}

double groundState(double x, double X){
    return pow(1 / (S*S * M_PI), 1.0 / 4.0) * exp(-(x - X) * (x - X) / (2 * S*S));
}
double firstExcited(double x, double X){
    return ((sqrt(2.0) * (x - X)) / S) * pow(1 / (S*S * M_PI), 1.0 / 4.0) * exp(-(x - X) * (x - X) / (2 * S*S));
}
double secondExcited(double x, double X){
    return sqrt(1 / (3 * S*S*S*S - 2 * S*S + 1)) * pow(1 / (S*S * M_PI), 1.0 / 4.0) * (2 * (x - X) * (x - X) - 1) * exp(-(x - X) * (x - X) / (2 * S*S));
}

//�������p�֐��̒�`
void init(vC &f){
    for (int i = 0; i < N; i++){
        f[i] = firstExcited(i2x(i), X);
    }
}

//�|�e���V�����֐��̒�`
double V(double x){
    return (1.0 / 2.0) * x*x - (b / 3.0) * x*x*x - 1.0 / (6 * b * b);
    //return (1.0 / 2.0) * x*x;
}

//���w�|�e���V�����̒�`
double V_opt(double x){
    return x > X_OPT ? x*x : 0.0;
}

//Simpson�ϕ�
double simpson(vC &f){
    double S_even, S_odd;

    S_even = S_odd = 0.0;

    for (int i = 1; i < (N / 2) - 1; i++){
        S_even += norm(f[2 * i]);
    }

    for (int i = 1; i < N / 2; i++){
        S_odd += norm(f[2 * i - 1]);
    }

    return DELTA_X * (norm(f[0]) + 2 * S_even + 4 * S_odd + norm(f[N - 1])) / 3.0;
}

//dt�������Ԕ��W������֐�
void timeEvolution(vC &f, fftw_plan plan_for, fftw_plan plan_back){
    //�|�e���V���������̌v�Z
    for (int j = 0; j < N; j++){
        f[j] *= polar(1.0 / N, -V(i2x(j)) * dt); // 1/N�͐��K�����q
    }

    //���w�|�e���V���������̌v�Z
    for (int j = 0; j < N; j++){
        f[j] *= exp(-V_opt(i2x(j)) * dt);
    }

    //f(x, t) -> g(k, t)
    fftw_execute(plan_for);

    //���Ԕ��W�̌v�Z
    for (int j = 0; j < N; j++){
        f[j] *= polar(1.0, -i2k(j) * i2k(j) * dt / 2.0);
    }

    //f(k, t+dt) -> f(x, t+dt)
    fftw_execute(plan_back);
}

//���f�G�l���M�[�s�[�N�̃C���f�b�N�X�����߂�֐�
void getPeaks(vector<pair<double, int>> &peak, vector<double> &res){
    //�����l�������畉�ɕς�����Ƃ���̒l�ƃC���f�b�N�X
    for (int i = 1; i < EN_real; i++){
        if (res[i - 1] < res[i] && res[i] > res[i + 1]){
            peak.push_back(make_pair(res[i], i));
        }
    }

    //�s�[�N�l�̑傫�����Ƀ\�[�g
    sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.first > j.first; });
    if (peak.empty()){
        cout << "no peak" << endl;
        exit(1);
    }
    else{
        double E_th = peak[0].first / 10; //�������l
        //�������l�ȉ��̗v�f���폜
        peak.erase(remove_if(peak.begin(), peak.end(), [E_th](pair<double, int> pair) {return pair.first < E_th; }), peak.end());

        //�����̏��������Ƀ\�[�g
        sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.second < j.second; });

        //����ꂽ�s�[�N�l��\��
        cout << "---- real ver. ----" << endl << endl;

        cout << "threshold value : " << E_th << endl << endl;
        cout << "Re" << "\t" << "peak value" << endl;
        for (auto pair : peak){
            double Re = i2E(E_BEGIN_real, pair.second, dE_real);
            printf("%.3lf\t%.3lf\n", Re, pair.first);
        }
    }
}

int main(){
    auto start = system_clock::now();
    vC f(N);
    vvC C(EN_real, vC(N));

    //������Fourier�ϊ�
    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);

    //�t����
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    printf("---- calculation parameters ----\n");
    printf("system size : \t [%.1f, %.1f]\n", X_BEGIN, X_END);
    printf("partition number : N = %d\n", N);
    printf("calculation time : T = %.0f\n", T_END);
    printf("time step : dt = %.1f\n", dt);
    printf("energy width (real) : [%.2f, %.2f] N = %d\n", E_BEGIN_real, E_END_real, EN_real);
    printf("energy width (imag) : [%.2f, %.2f] N = %d\n", E_BEGIN_imag, E_END_imag, EN_imag);
    printf("--------------------------------\n");

    init(f); //��������f(x,0)�̐ݒ�

    ofstream ofs;
    char filename[50];

    for (int i = 0; i <= TN; i++){
        sprintf(filename, "./output/timeEvo/output%03d.txt", i);
        ofs.open(filename);
        if (!ofs){
            cerr << "file open error!" << endl;
            exit(1);
        }

        for (int j = 0; j < N; j++){
            ofs << i2x(j) << "\t" << norm(f[j]) << "\t" << V(i2x(j)) << endl;
        }

        ofs.close();

        //�����݂̂ŐU��
        for (int j = 0; j < EN_real; j++){
            for (int k = 0; k < N; k++){
                C[j][k] += f[k] * polar(dt, i2E(E_BEGIN_real, j, dE_real) * (i * dt));
            }
        }

        //���Ԕ��W
        timeEvolution(f, plan_for, plan_back);
    }

    for (int i = 0; i < EN_real; i++){
        for (int j = 0; j < N; j++){
            C[i][j] /= T_END;
        }
    }

    string str = "./output/energy_T_" + to_string((int)T_END) + ".txt";
    ofs.open(str);
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    vector<double> res(EN_real);
    ofs << scientific;
    for (int i = 0; i < EN_real; i++){
            res[i] = simpson(C[i]);
            ofs << i2E(E_BEGIN_real, i, dE_real) << "\t";
            ofs << res[i] << endl;
    }

    ofs.close();

    vector<pair<double, int>> peak;

    getPeaks(peak, res); //�ŗL�l�̃s�[�N�̒T��

    int peakNum = peak.size(); //����ꂽ�s�[�N��

    init(f);

    vvvC imag(EN_imag, vvC(peakNum, vC(N)));

    for (int i = 0; i <= TN; i++){
        //�����݂̂ŐU��
        for (int j = 0; j < EN_imag; j++){
            for (int k = 0; k < peakNum; k++){
                for (int l = 0; l < N; l++){
                    imag[j][k][l] += f[l] * polar(dt, i2E(E_BEGIN_real, peak[k].second, dE_real) * (i * dt)) * exp(i2E(E_BEGIN_imag, j, dE_imag) * (i * dt));
                }
            }
        }

        //���Ԕ��W
        timeEvolution(f, plan_for, plan_back);
    }

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peakNum; j++){
            for (int k = 0; k < N; k++){
                imag[i][j][k] *= exp(-i2E(E_BEGIN_imag, i, dE_imag) * T_END) / T_END;
            }
        }
    }

    vector<vector<double>> res_imag(EN_imag, vector<double>(peakNum));
    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peakNum; j++){
            res_imag[i][j] = simpson(imag[i][j]);
        }
    }

    ofs.open("./output/imaginary.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_imag; i++){
        ofs << i2E(E_BEGIN_imag, i, dE_imag) << "\t";
        for (int j = 0; j < peakNum; j++){
            ofs << res_imag[i][j] << "\t";
        }
        ofs << endl;
    }

    //�p�����[�^�ƌ��ʂ̃t�@�C����������(gnuplot�p)
    FILE *fp = fopen("params.txt", "w");
    if (fp == NULL){
        cerr << "file open error!" << endl;
        exit(1);
    }

    fprintf(fp, "T = %.0f\n", T_END);
    fprintf(fp, "N = %d\n", N);
    fprintf(fp, "peakNum = %d\n", peakNum);

    for (int i = 0; i < peakNum; i++){
        fprintf(fp, "E%d = %lf\n", i, i2E(E_BEGIN_real, peak[i].second, dE_real));
    }

    fclose(fp);

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);

    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    printf("\nexecution time : %dsec (%dmin)\n\n", sec, min);

    return 0;
}