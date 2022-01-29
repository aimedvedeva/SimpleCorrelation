#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int sign(double a){
  if (a < 0){
    return -1;
  }
  else if (a > 0){
    return 1;
  }
  return 0;
}

double getMedian(double *sample, int size){
  int index = 0;

  if (size % 2 == 0){
    index = size / 2;
    return sample[index];
  }
  index = size / 2;
  return sample[index] + sample[index + 1];
}

double **getCorrelationMatrix(double **samples, int samplesNum, int samplesSize, double *medians){
  double **correlationMatrix = new double*[samplesNum - 1];
  int colsShift = 1;

  for (int i = 0; i < samplesNum - 1; i++){
    correlationMatrix[i] = new double[samplesNum - colsShift];
    colsShift++;
  }
  
  colsShift = 1;

  for (int i = 0; i < samplesNum; i++){
    for (int j = i + 1, rIndex = 0; j < samplesSize, rIndex < samplesNum - colsShift; j++, rIndex++){
      correlationMatrix[i][rIndex] = 0;

      for (int k = 0; k < samplesSize; k++){
        correlationMatrix[i][rIndex] += sign(samples[i][k] - medians[i]) * sign(samples[j][k] - medians[j]);
      }
      correlationMatrix[i][rIndex] /= samplesSize;
    }
    colsShift++;
  }
  return correlationMatrix;
}

int getCorrelationPairsNum(double **correlationMatrix, int samplesNum, int samplesSize){
  int pairsNum = 0;
  int colsShift = 1;

  for (int i = 0; i < samplesNum; i++){
    for (int j = 0, index = i + 1; j < samplesNum - colsShift; j++, index++){
      if (abs(correlationMatrix[i][j]) == 1){
        pairsNum++;
      }
    }
    colsShift++;
  }

  return pairsNum;
}

int comparator(const void *v1, const void *v2){
  double a = *(double*)v1;
  double b = *(double*)v2;

  if (a > b){
    return 1;
  }
  else if (a > b){
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[]){
  string inputFileName;
  string outputFileName;

  ifstream inputFile;
  ofstream outputFile;
    
  if (argv[1] == NULL || argv[2] == NULL){
    cout << "Please, write input and output file names with their absolute pathes !" << endl;
    cin >> inputFileName;
    cin >> outputFileName;
  }
  else{
    inputFileName = argv[1];
    outputFileName = argv[2];
  }

  inputFile.open(inputFileName);
  outputFile.open(outputFileName);

  if (!inputFile.is_open()){
    cout << "InputFile can not be opened !" << endl;
    return 1;
  }
  if (!outputFile.is_open()){
    cout << "OutputFile can not be opened !" << endl;
    return 1;
  }
  
  int cols = 0;
  int rows = 0;
  inputFile >> rows >> cols;

  double **matrix = new double* [rows];
  for (int i = 0; i < rows; i++){
    matrix[i] = new double[cols];
  }

  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      inputFile >> matrix[i][j];
    }
  }

  for (int i = 0; i < rows; i++){
    qsort(matrix[i], cols, sizeof(double), comparator);
  }

  double *med = new double[rows];

  for (int i = 0; i < rows; i++){
    med[i] = getMedian(matrix[i], cols);
  }

  double **correlationMatrix = getCorrelationMatrix(matrix, rows, cols, med);

  int correlationPairsNum = getCorrelationPairsNum(correlationMatrix, rows, cols);

  outputFile << correlationPairsNum << endl;

  int colShift = 1;

  for (int i = 0; i < rows; i++){
    for (int j = 0, index = i + 1; j < rows - colShift; j++, index++){
      if (abs(correlationMatrix[i][j]) == 1){
        outputFile << correlationMatrix[i][j] << " " << i << " " << index << endl;
      }
    }
    colShift++;
  }

  for (int i = 0; i < rows - 1; i++){
    delete correlationMatrix[i];
  }
  delete correlationMatrix;

  for (int i = 0; i < rows; i++){
    delete matrix[i];
  }
  delete matrix;
  delete med;

  return 0;
}