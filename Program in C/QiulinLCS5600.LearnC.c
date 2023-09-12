#include "information.h" // Header
#include <stdio.h>

double mpg2kml(double mpg);
double mpg2lphm(double mpg);
double kml2mpg(double kpl);

int main(){
	printf("The course name is: ");
	courseName(); // In the header
	printf("The assignment title is: ");
	assignmentTitle(); // In the header
	printf("\nThe following are answers of Questions A, B and C:\n");
	
	double mpg = 1.0;
	double kpl = 1.0;
	double resultA = mpg2kml(mpg);
	double resultB = mpg2lphm(mpg);
	double resultC = kml2mpg(kpl);
	printf("If 1 mile per gallon, the A result is %f\n",resultA);
	printf("If 1 mile per gallon, the B result is %f\n",resultB);
	printf("If 1 kilometer per liter, the C result is %f\n",resultC);
	printf("\n");

	// Bad argument
	double errormpg = 0.0;
	double errorkpl = 0.0;
	double errA = mpg2kml(errormpg);
	double errB = mpg2lphm(errormpg);
	double errC =kml2mpg(errorkpl);
	printf("If 0 mile per gallon, the A result is %f\n",errA);
	printf("If 0 mile per gallon, the B result is %f\n",errB);
	printf("If 0 kilometer per liter, the C result is %f\n",errC);

	return 0;
}

double mpg2kml(double mpg){
	double gtl = 3.785411784;
	double mtk = 1.609344;
	double result;
	result = (mpg * mtk) / gtl;
	return result;
}

double mpg2lphm(double mpg){
	if(mpg == 0){
		return 0.0;
	}
	else{
		double gtl = 3.785411784;
        	double mtk = 1.609344;
        	double result;
        	result = (100 /  (mpg * mtk)) * gtl;
        	return result;
	}
}

double kml2mpg(double kpl){
	double ltg = 0.2641720523581;
	double ktm = 0.6213711922373;
	double result;
	result  = (kpl * ktm) / ltg;
	return result;
}
