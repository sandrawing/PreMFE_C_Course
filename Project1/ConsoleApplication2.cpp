// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

double calculateCallIntrinsicValue(double spot, double strike) {
	double iv;
	double iv1 = spot - strike;
	iv = (iv1 < 0 ? 0:iv1);
	return iv;
}

double calculatePutIntrinsicValue(double spot, double strike) {
	double iv;
	double iv1 = strike - spot;
	iv = (iv1 < 0 ? 0 : iv1);
	return iv;
}

int main() {
	double spot, strike;
	cout<<"What is the sopt price?"<<endl;
	cin >> spot;
	cout << "How about option's strike price?" << endl;
	cin >> strike;
	double Call_intrinvalue = calculateCallIntrinsicValue(spot, strike);
	double Put_intrinvalue = calculatePutIntrinsicValue(spot, strike);
	cout << "The Call option's intrinsic value of given spot and strike price is" << Call_intrinvalue << endl;
	cout << "The Put option's intrinsic value of given spot and strike price is" << Put_intrinvalue << endl;
	cout << "Next, we will run a loop to test my functions; ENJOY!!" << endl;
	double sp1 = 85;
	double st1 = 100;
	while (sp1 <= 115) {
		double Call_intrinvalue = calculateCallIntrinsicValue(sp1, st1);
		double Put_intrinvalue = calculatePutIntrinsicValue(sp1, st1);
		cout << "The Call option's intrinsic value of given spot and strike price is" << Call_intrinvalue << endl;
		cout << "The Put option's intrinsic value of given spot and strike price is" << Put_intrinvalue << endl;
		sp1 ++;
	}
	double st2 = 90;
	double sp2 = 105;
	do {
		double Call_intrinvalue = calculateCallIntrinsicValue(sp2, st2);
		double Put_intrinvalue = calculatePutIntrinsicValue(sp2, st2);
		cout << "The Call option's intrinsic value of given spot and strike price is" << Call_intrinvalue << endl;
		cout << "The Put option's intrinsic value of given spot and strike price is" << Put_intrinvalue << endl;
		st2++;
	} while (st2 <= 100);
	cout << "After ending of test, let's play a game!!" << endl;
	int a;
	cout << "Do you like 0 or 1?" << endl;
	cin >> a;
	if (a == 0) {
		double sp3;
		cout << "Wow, you like 0!Then tell your spot price for the underlying asset of your option" << endl;
		cin >> sp3;
		double Put = calculatePutIntrinsicValue(sp3, 100);
		cout << "The Put option's intrinsic value of given spot and strike price is" << Put_intrinvalue << endl;
	}
	else if (a == 1) {
		double st3;
		cout << "Inteseting!Then tell your strike price of your option" << endl;
		cin >> st3;
		double Put = calculatePutIntrinsicValue(63.5, st3);
		cout << "The Put option's intrinsic value of given spot and strike price is" << Put_intrinvalue << endl;
	}
	else cout << "Soooory, I do not think of this situation, ERROR!!" << endl;
	cout << "Thank you for your participation! CU in next assignment!!Xixi" << endl;
	return 0;
}
