/*
@Iris_lib.c
@1.Projede kullandığımız ve bu projede de kullanacağımız fonksiyon gövdeleri.
@2.Proje
@28/12/2022
@Şerafettin Doruk SEZER/ sdoruksezer@gmail.com
*/

#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "Iris_lib.h"

float mean(const float *vec, int size) {
    //  bir boyutlu bir vektörü ve boyutunu alıp, matrisin ortalamasını döndürüyor.
    float mean;
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += vec[i];
    }
    mean = sum / (float) (size);
    return mean;
}

float covariance(float *vec1, float *vec2, int size) {
    // iki tane bir boyutlu vektörleri ve uzunluklarını alıyoruz.
    // her bir elemanından vektörün ortalamasını çıkarıp, çarpımlarını vektörün uzunluğuna bölüyoruz.
    // bulduğumuz sonucu döndürüyoruz.
    float sum1 = 0;
    float sum2 = 0;
    float sum3 = 0;
    float cov;

    for (int i = 0; i < size; ++i) {
        sum1 += (float) (vec1[i] - mean(vec1, size));
        sum2 += (float) (vec2[i] - mean(vec2, size));
        sum3 += sum1 * sum2;
        sum1 = 0;
        sum2 = 0;
    }
    printf("\n sum1_mean: %f",mean(vec1, size));
    printf("\n sum2_mean: %f",mean(vec2, size));
    printf("\n sum1: %.18f ", sum1);
    printf("\nsum2: %.18f", sum2);
    printf("\nsum3: %.18f", sum3);
    printf("\nsize: %d", size - 1);


    cov = (float) ((float) (sum3) / (float) (size - 1));
   // cov = powf((float) (sum1 * sum2) / (float) (size),2);

    return cov;
}

float correlation(float *vec, float *vec2, int size) {
/*
     * Korelasyonu bulmak için ilk önce kovaryansta yaptığımız işlemin aynısını yapıyoruz. Buradan cov(x,y)'yi buluyoruz.
     * Cov_x = cov(x,x) dediğimiz kovaryansa sadece dizi1_x vektörünün aynısını atayarak buluyoruz.
     * Cov_y = cov(y,y) dediğimiz kovaryansa sadece dizi1_y vektörünün aynısını atayarak buluyoruz.
     * Corr(x,y) = cov(x,y)/[cov(x,x) * cov(y,y)] formulünü uygulayıp korelasyonu döndürüyoruz.
     */
    float cov; // Cov(x,y)
    float cov_x; // Cov(x,x)
    float cov_y; // Cov(y,y)
    float corr = 0; // Corr(x,y)

    cov = covariance(vec, vec2, size);
    printf(" cov: %.18f ", cov);

    cov_x = covariance(vec, vec, size);
    printf(" cov_x: %.18f ", cov_x);

    cov_y = covariance(vec2, vec2, size);
    printf(" cov_y: %.18f ", cov_y);

    // paydanın sıfır olup olmadığını kontrol ediyoruz
    // payda 0 ise ekrana printf'de ki yazıyı yazıp correlation değerini 0 yapıyor.
    if (cov_x * cov_y != 0) {
        corr = (float) (cov / (sqrtf(cov_x) * sqrtf(cov_y)));
    } else {
        printf(" hatali bolme, carpim = 0");
    }
    return corr;
}

//                             matrixMultiplication(Xc,Xc_T,row,col,col,row)
//                                    Xc              Xc_T     3        2           2        3
float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2){
    /*
     * Öncelikle 2 matrisi çarpmak için 1.'nin sütun sayısıyla 2.'nin satır sayısının,
     * aynı olup olmadığını kontrol ediyoruz.
     * 1.'nin = **mat1 değişkeninin sütunundan 2.'nin = **mat2 satırından giderek çarpıp,
     * bulduğumuz sonucu ekrana yazıyoruz, sonra bulduğumuz sonucu döndürüyoruz.
     */

    float **matrixM = (float **) calloc(col1,sizeof(float *));

    for (int m = 0; m < col1; ++m) {
        matrixM[m] = (float *) calloc(row2,sizeof(float));
    }
    printf("\n **********Multiplication Atama:***************\n");
    printMatrix(matrixM,col1,row2); // 3 3
    printf("\n *************************");
    printf("\nrow1 = %d \n col2 = %d" ,col1,row2);
    printf("\n *************************\n");

    float sum = 0;

    if(col1 == row2){

        for (int i = 0; i < col1; ++i) { // 2
            for (int j = 0; j < row2; ++j) { // 2

                for (int k = 0; k < col2; ++k) { // 3

                    //        01              10
                    sum += mat1[k][j] * mat2[i][k]; // mat1 sütunundan mat2 satırından giderek çarpıyor.
                    //     Xc           Xc_t
                    matrixM[i][j] = sum;
                }
                sum = 0;
            }
        }
    } else{
        printf("these two matrices cannot be multiplied.\n");
    }
    printMatrix(matrixM,col1,row2);

    return matrixM;
}

float **matrixTranspose(float **mat, int row, int col) {
    // iki boyutlu bir vektörü , satır sayısını ve sütun sayısını alıp,
    // vektörün tarnspozesini döndürüyor.
    float **matrix_T = returnMatrix(col,row);

    for (int i = 0; i < col; ++i) { // 3
        for (int j = 0; j < row; ++j) { // 2
            matrix_T[i][j] = mat[j][i];
        }
    }
    return matrix_T;
}

float *columnMeans(float **mat, int row, int col){
// iki boyutlu bir vektörü , satır sayısını ve sütun sayısımı alıp,
// sütunlarının ortalamasını tutan bir boyutlu bir vektör döndürüyor.
    float *colM = (float *) calloc(col,sizeof(float));
    //  float *colM = (float *) malloc(row*sizeof (float));

    printf("\n *******Atama matrisi:*****\n");
    printVector(colM,col);
    //  printVector(colM,row);

    printf("\n ********************\n");


    float sum = 0;

    for (int i = 0; i < col; ++i) { // 5
        for (int j = 0; j < row; ++j) { // 3
            //  sum += mat[i][j];
            sum += mat[j][i];
        }
        colM[i] = (float)(sum / (float)(row));
        sum = 0;
    }

    return colM;

}

float **covarianceMatrix(float **mat, int row, int col){
// ilk önce işlem yapacağımız Xc matrisini dinamik olarak oluşturup ekrana yazdık.
    float **Xc = returnMatrix(row,col);
    printf("Xc matrisi:\n");
    printMatrix(Xc,row,col);
    printf("***************************\n");

//Xc_means dinamik matrisini oluşturup ekrana yazdık.
    float *Xc_means = (float *) calloc(col,sizeof(float));
    printf("Xc_means matrisi:\n");
    printVector(Xc_means,col);
    printf("\n***************************\n");

// Oluşturduğumuz Xc_means matrisine parametre olarak aldığımız  **mat matrisinin sütun ortalamalarını atayıp,
// ekrana yazdık.
    Xc_means = columnMeans(mat,row,col);
    printf("\n after the add (mat columMeans) to Xc_means matrisi:\n");
    printVector(Xc_means,col);
    printf("\n***************************\n");

//parametre olarak aldığımız matrisinin her bir elemanından Xc_means yani her bir sütun ortalaması değerlerini çıkarıp,
//oluşturduğumuz Xc matrisine atadık.
    for (int i = 0; i < col; ++i) {  // 2    col = 3
        for (int j = 0; j < row; ++j) { // 3  row = 2
            Xc[j][i] = (float)(mat[j][i] - Xc_means[i]);
        }
    }
    // Xc matrisinin işlemden sonraki değerini yazdık.
    printf("after the process to Xc matrisi:\n");
    printMatrix(Xc,row,col);
    printf("\n***************************\n");

    // Xc matrisinin transpozunu aldık, ekrana yazarken satır ve sütun değerlerini tam tersi girdik.
    //                                     2    3
    float **Xc_T = matrixTranspose(Xc,row,col);
    printf("Xc_T matrisi:\n");
    printMatrix(Xc_T,col,row); // 3   2
    printf("\n***************************");

    // resultMatris adında yeni bir dinamik vektör oluşturduk, ve ekrana yazdık.
    float **resultMatris = returnMatrix(row,row);
    printf("resultAtama matrisi:\n");
    printMatrix(resultMatris,row,row);
    printf("\n******************************\n");

    /*
     * Xc matrisi ve Xc'nin transpozu Xc_T matrisini çarpıp, resultMatris'e atadık.
     * Burada girdiğimiz sonuçlara ters sonuç çıkmasının sebebi:
     * biz 3 satır 2 sütun girdiğimizde matrixMultiplication fonksiyonu bunu  3x2 * 2X3 olarak değil.
     *  2x3 * 3x2 olarak girdiğimiz değerin soluna atayarak yapıyor.
     */
    //    resultMatris = matrixMultiplication(Xc_T,Xc,col,row,row,col) ;
    resultMatris = matrixMultiplication(Xc,Xc_T,row,col,col,row) ;

    // resultMatris sonucunu ekrana yazdırdık.
    printf("after the Multiplication process:\n");
    printMatrix(resultMatris,col,col);
    printf("\n***************************\n");

    // Parametre olarak aldığımız matrisin satır sayısına böleceğimiz için
    // satır sayısını ekrana yazdırdık
    printf("row: %d\n", row);
    printf("\n***************************\n");

    // resultMatris'in her bir elemanını satır sayısına böldük.
    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < col; ++j) {
            resultMatris[i][j] /= (float)(row);
        }
    }
    // Bölümden sonraki bulduğumuz matrisi ekrana yazdırıp, döndürdük.
    // sütun sayılarını girmemizin nedeni oluşan matris sütun sayısı kadardır.
    printf("after the (n) divide process:\n");
    printMatrix(resultMatris,col,col);
    printf("\n***************************\n");

    return resultMatris;
}

float **returnMatrix(int row, int col){
    // Girdiğimiz satır ve sütun değerlerini alıp iki boyutlu bir vektör oluşturup içine random sayılar atıp.
    // iki boyutlu bir vektör döndürüyor.
    float **dizi2 = (float **) malloc(sizeof(float *)*row);

    for (int i = 0; i < row; ++i) {

        dizi2[i] = (float *) malloc(sizeof(float)*col);

        for (int j = 0; j < col; ++j) {

            dizi2[i][j]  = (float)(rand() % 10);
          //  printf("%.2f\n ", *(*(dizi2+i)+j));
        }
    }
    return dizi2;
}

void printMatrix(float **dizi2,int row, int col) {
    // iki boyutlu bir vektör, satır sayısını ve sütun sayısını alıp, ekrana yazdırıyor.
    int i;
    for (i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("\n dizi[%d][%d]: %.2f", i, j, dizi2[i][j]);;
        }
        printf("\n");
    }
}

void freeMatrix(float **mat, int row) {
    // iki boyutlu bir vektör ve satır sayısını alıp,
    // ayırdığımız alanı sisteme geri iade ediyor.
    printf("\n free matrix");
    for (int i = 0; i < row; ++i) {
        free(mat[i]);
    }
    free(mat);

}

void printVector(float *dizi1,int size){
    // bir boyutlu bir vektör ve boyutunu alıp, ekrana yazdırıyor.
    int i;
    for ( i = 0; i < size; ++i) {
        printf("\n dizi[%d]: %.2f", i,dizi1[i]);;
    }
}





