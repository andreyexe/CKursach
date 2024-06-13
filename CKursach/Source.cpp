#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//библиотеки линейной алгебры   mtl или eigen

using namespace cv;

const int squareSize = 5;
//const std::string imgPath = "C:/Users/olgab/OneDrive/Рабочий стол/mnist/0/img_1.jpg"; //28*28  0,6 сек
const std::string imgPath = "C:/Users/olgab/OneDrive/Рабочий стол/dataset/cars/img-0car-135.png"; // 182*112 19 сек
const std::string fileName = "test.txt";

void deleteArr(int** arr, int rows);
void print2dArr(int** arr, int rows, int cols);
void writeArrayToFile(const std::string& filename, std::string className, int** array, int rows);
void writeVectorToFile(const std::string& filename, std::string className, const std::vector<double>& vector, bool EOL = false);


void arrSums(const cv::Mat& arr, int** rows, int** cols)
{
	for (int i = 0; i < arr.rows; i++) {
		for (int j = 0; j < arr.cols; j++) {
			if (j == 0) {
				for (int k = 0; k <= j; k++) {
					rows[i][j] += static_cast<int>(arr.at<uchar>(i, k));
				}
			}
			else {
				rows[i][j] = rows[i][j - 1] + static_cast<int>(arr.at<uchar>(i, j));
			}
			if (i == 0) {
				for (int k = 0; k <= i; k++) {
					cols[i][j] += static_cast<int>(arr.at<uchar>(k, j));
				}
			}
			else {
				cols[i][j] = cols[i - 1][j] + static_cast<int>(arr.at<uchar>(i, j));
			}
		}
	}
}


int main(void) {
	setlocale(LC_ALL, "RU");
	double startTime = clock();

	cv::Mat image = cv::imread(imgPath, cv::IMREAD_GRAYSCALE); 

	if (image.empty()) {
		std::cout << "Не получилось загрузить изображение!" << std::endl;
		return -1;
	}

	int rows = image.rows;
	int cols = image.cols;

	std::cout << rows << " = rows" << std::endl << cols <<  " = cols" << std::endl;
	
	//<arrSums> создание и заполнение нулями массивов сумм по строкам и столбцам
	int** arrCollsSum;
	int** arrRowsSum;

	arrCollsSum = new int* [rows];
	for (int i = 0; i < rows; i++)
	{
		arrCollsSum[i] = new int[cols];
		for (int j = 0; j < cols; j++)
		{
			arrCollsSum[i][j] = 0;
		}
	}
	arrRowsSum = new int* [rows];
	for (int i = 0; i < rows; i++)
	{
		arrRowsSum[i] = new int[cols];
		for (int j = 0; j < cols; j++)
		{
			arrRowsSum[i][j] = 0;
		}
	}
	// </arrSums>

	arrSums(image, arrRowsSum, arrCollsSum);

	std::vector<double> arrMeanX;
	std::vector<double> arrMeanY;

	for (int i = 0; i < rows - squareSize; i++)
	{
		for (int j = 0; j < cols - squareSize; j++)
		{
			int isqs = i + squareSize;
			for (int k = i; k < isqs; k++)
			{
				if (j == 0)
				{
					arrMeanX.push_back((double)(arrRowsSum[k][j+1]) / (double)(squareSize));
					arrMeanY.push_back((double)(arrCollsSum[k][j+1]) / (double)(squareSize));
				}
				else {
					arrMeanX.push_back((double)(arrRowsSum[k][j + 1] - arrRowsSum[k][j - 1]) / (double)(squareSize));
					arrMeanY.push_back((double)(arrCollsSum[k][j + 1] - arrCollsSum[k][j - 1]) / (double)(squareSize));
				}
			}
			writeVectorToFile(fileName, "0", arrMeanX, false);
			writeVectorToFile(fileName, "0", arrMeanY, true);

			arrMeanX.clear();
			arrMeanY.clear();
		}
	}


	double endTime = clock();
	double resultTime = endTime - startTime;

	std::cout << "time = " << resultTime / 1000 << std::endl;

	deleteArr(arrCollsSum, rows);
	deleteArr(arrRowsSum, rows);

	waitKey(0);
	return(0);
}


void deleteArr(int** arr, int rows) {
	for (int i = 0; i < rows; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

void print2dArr(int** arr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void writeArrayToFile(const std::string& filename, std::string className, int** array, int rows, int colls, bool EOL ) {
	std::ofstream file;
	file.open(filename, std::ios_base::app);

	if (file.is_open()) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < colls; ++j) {
				file << array[i][j] << ';';
			}
		}
		if (EOL) {
			file << className << '\n';
		}
		file.close();
	}
	else {
		std::cerr << "Не удалось открыть файл для записи." << std::endl;
	}
}

void writeVectorToFile(const std::string& filename, std::string className, const std::vector<double>& vector, bool EOL) {
	std::ofstream file;
	file.open(filename, std::ios_base::app);

	if (file.is_open()) {
		for (double element : vector) {
			file << element << ';';
		}
		if (EOL) {
			file << className << '\n';
		}		file.close();
	}
	else {
		std::cerr << "Не удалось открыть файл для записи." << std::endl;
	}
}