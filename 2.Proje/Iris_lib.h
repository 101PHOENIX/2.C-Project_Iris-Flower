/*
@Iris_lib.h
@struct tanımlamalarımız.1.Projede kullandığımız ve bu projede de kullanacağımız fonksiyonların prototipleri.
@2.Proje
@28/12/2022
@Şerafettin Doruk SEZER/ sdoruksezer@gmail.com
*/

typedef struct {
   char Species2[50];
}test;

typedef enum Species {
    Iris_setosa = 1,
    Iris_versicolor,
    Iris_virginica
}kind;

typedef struct {
    int ID;
    float SepalLength;
    float SepalWidth;
    float PetalLength;
    float PetalWidth;
    kind Species;
}Iris;

#ifndef MATRIXLIB_C_IRIS_LIB_H
#define MATRIXLIB_C_IRIS_LIB_H

float mean(const float *vec, int size);

float covariance(float *vec1, float *vec2, int size);

float correlation(float *vec, float *vec2, int size);

float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2);

float **matrixTranspose(float **mat, int row, int col);

float *columnMeans(float **mat, int row, int col);

float **covarianceMatrix(float **mat, int row, int col);

float **returnMatrix(int row, int col);

void  printMatrix(float **dizi,int row, int col);

void printVector(float *dizi,int n);

void freeMatrix(float **mat, int row);

#endif //MATRIXLIB_C_IRIS_LIB_H
