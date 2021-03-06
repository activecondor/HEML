#include "GD.h"

#include <NTL/ZZ.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace NTL;

long** GD::xyDataFromFile(string& path, long& factorDim, long& sampleDim, bool isfirst) {
	vector<vector<long>> xdata;
	vector<long> ydata;
	factorDim = 0; 		// dimension of x
	sampleDim = 0;	// number of samples
	ifstream openFile(path.data());
	if(openFile.is_open()) {
		string line;
		getline(openFile, line);
		for(long i = 0; i < line.length(); ++i) if(line[i] == ',' ) factorDim++;
		if(isfirst) {
			while(getline(openFile, line)){
				if(line.length() != 2 * factorDim + 1 ) {
					cout << "Error: data format" << endl;
					break;
				}
				if(line[0] == '0') {
					ydata.push_back(-1);
				} else if(line[0] == '1') {
					ydata.push_back(1);
				} else {
					cout << "Error: data value" << endl;
					break;
				}
				vector<long> vecline;
				for(long i = 2; i < 2 * factorDim + 1; i += 2) {
					if(line[i] == '0') {
						vecline.push_back(0);
					} else if(line[i] == '1') {
						vecline.push_back(1);
					} else{
						cout << "Error: data value" << endl;
						break;
					}
				}
				xdata.push_back(vecline);
				sampleDim++;
			}
			openFile.close();
		} else {
			while(getline(openFile, line)){
				if(line.length() != 2 * factorDim + 1 ) {
					cout << "Error: data format" << endl;
					break;
				}
				if(line[2 * factorDim] == '0') {
					ydata.push_back(-1);
				} else if(line[2 * factorDim] == '1') {
					ydata.push_back(1);
				} else {
					cout << "Error: data value" << endl;
					break;
				}
				vector<long> vecline;
				for(long i = 0; i < 2 * factorDim; i += 2) {
					if(line[i] == '0') {
						vecline.push_back(0);
					} else if(line[i] == '1') {
						vecline.push_back(1);
					} else{
						cout << "Error: data value" << endl;
						break;
					}
				}
				xdata.push_back(vecline);
				sampleDim++;
			}
			openFile.close();
		}
	} else {
		cout << "Error: cannot read file" << endl;
	}
	long** xyData = new long*[sampleDim];
	for(long j = 0; j < sampleDim; ++j){
		long* xyj = new long[factorDim + 1];
		xyj[0] = ydata[j];
		for(long i = 1; i < factorDim + 1; ++i){
			xyj[i] = ydata[j] * xdata[j][i-1];
		}
		xyData[j] = xyj;
	}
	factorDim++;
	return xyData;
}

long** GD::RandomxyDataLearn(long**& xyData, long& learnDim, long& sampleDim, long& factorDim) {
	long** res = new long*[learnDim];

	bool* notTaken = new bool[sampleDim];
	fill_n(notTaken, sampleDim, false);
	long r;
	for (long i = 0; i < sampleDim - learnDim; ++i) {
		do {
			r = RandomBnd(sampleDim);
		} while(notTaken[r]);
		notTaken[r] = true;
	}
	long idx = 0;
	for (long i = 0; i < sampleDim; ++i) {
		if(!notTaken[i]) {
			res[idx++] = xyData[i];
		}
	}
	delete[] notTaken;
	return res;
}

double GD::innerprod(double*& w, long*& xy, long& size){
	double res = 0.0;
	for(int i = 0; i < size; ++i) {
		res += w[i] * xy[i];
	}
	return res;
}

void GD::stepLGD(long**& xyData, double*& wData, long& factorDim, long& learnDim, double& gamma) {
	//TODO implement method
}

void GD::stepMLGD(long**& xyData, double*& wData, double*& vData, long& factorDim, long& learnDim, double& gamma, double& eta) {
	//TODO implement method
}

void GD::stepNLGD(long**& xyData, double*& wData, double*& vData, long& factorDim, long& learnDim, double& gamma, double& eta) {
	double* grad = new double[factorDim]();

	for(int j = 0; j < learnDim; ++j) {
		double ip = innerprod(wData, xyData[j], factorDim);
		double tmp = (ip > 15.0) ? 0 : (ip < -15.0) ? -1.0 : - 1. / (1. + exp(ip));
		if(ip > 6) {
			cout << "too big ip: " << ip << endl;
		} else if(ip < -6) {
			cout << "too small ip: " << ip << endl;
		}
		for(int i = 0; i < factorDim; ++i) {
			grad[i] += tmp * (double) xyData[j][i];
		}
	}

	// Nesterov steps
	for (int i = 0; i < factorDim; ++i) {
		double tmpv = wData[i] - gamma * grad[i];
		wData[i] = (1.0 - eta) * tmpv + eta * vData[i];
		vData[i] = tmpv;
	}
	delete[] grad;
}

void GD::check(long**& xyData, double*& wData, long& factorDim, long& sampleDim) {
	cout << "w:";
	for (long i = 0; i < factorDim; ++i) {
		cout << wData[i] << ",";
	}
	cout << endl;

	long num = 0;
	for(long j = 0; j < sampleDim; ++j){
		if(innerprod(wData, xyData[j], factorDim) > 0) num++;
	}
	cout << "Correctness: " << num << "/" << sampleDim << endl;

}

long* GD::calculateYtrueData(long**& xyData, long& sampleDim) {
	long* res = new long[sampleDim];
	for (long i = 0; i < sampleDim; ++i) {
		res[i] = xyData[i][0] == -1 ? 0 : 1;
	}
	return res;
}

double* GD::calculateYpredictData(long**& xyData, double*& wData, long& factorDim, long& sampleDim) {
	double* res = new double[sampleDim];
	for(long i = 0; i < sampleDim; ++i){
		res[i] = innerprod(wData, xyData[i], factorDim) * xyData[i][0] / 2. + 0.5;
	}
	return res;
}

double GD::calcuateAUC(long**& xyData, double*& wData, long& factorDim, long& sampleDim, long steps) {

	long* yTrueData = calculateYtrueData(xyData, sampleDim);
	double* yPredictData = calculateYpredictData(xyData, wData, factorDim, sampleDim);

	double* TPR = new double[steps + 1];
	double* FPR = new double[steps + 1];

	for (long i = 0; i < steps + 1; ++i) {
		double threshold =  (double)i / steps;

		long FP = 0;
		long TP = 0;
		long TN = 0;
		long FN = 0;

		for (long j = 0; j < sampleDim; ++j) {
			if(yTrueData[j] == 0) {
				if(yPredictData[j] > threshold) FP++;
				else TN++;
			} else {
				if(yPredictData[j] > threshold) TP++;
				else FN++;
			}
		}
		double TPFN = TP + FN;
		double FPTN = FP + TN;

		TPR[i] = TP / TPFN;
		FPR[i] = FP / FPTN;
	}
	double auc = 0.0;

	for (long i = 0; i < steps; ++i) {
		auc += (TPR[i] + TPR[i + 1]) * (FPR[i] - FPR[i + 1]) / 2.;
	}
	delete[] TPR;
	delete[] FPR;
	delete[] yTrueData;
	delete[] yPredictData;
	return auc;
}
