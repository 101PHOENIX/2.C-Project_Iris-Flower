/*
@Iris_main.c
@İlgili dosyanın içindeki verileri okuyup, bir dizi hesaplamalara yapıp,
@sonuçları bir text dosyasının içine yazıyor.
@2.Proje
@28/12/2022
@Şerafettin Doruk SEZER/ sdoruksezer@gmail.com
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Iris_lib.h"

int main(){
    // Dosyamızı okumak için fopen'ı kullanarak yolunu ve okuma işlevi olan "r" işlevini tanımlıyoruz.
    // Eğer dosyanın yolunda bir sıkıntı varsa if ile error kodumuzu yazdırıyoruz.
    FILE *file;
    file = fopen("C:\\Users\\Lenovo\\CLionProjects\\BLM19204\\2.Proje\\Iris.csv","r");
    if (file == NULL){
        perror("Unable to open the file.");
        exit(1);
    }

    // dosyamızın 1.satırında bizim alacağımız verinin sütun sayısını veriyor.
    // Bu sayıyı almak için fscanf komutunu kullanıyoruz.
    int n;
    fscanf(file,"%i",&n);

    // Iris struct yapımızın dinamik struct vektörünü oluşturuyoruz.
    Iris *IrisDataType;
    IrisDataType = malloc(n * sizeof(Iris));

    // Dosyanın içindeki veiler 2 satır sonra yazıldığı için,
    // Bizim ilk 2 satırı geçmemiz gerekiyor. Bunun için,
    // 1 tane dinamik dizi oluşturuyoruz.
    char *arr;
    arr = calloc(n, sizeof(char ));

    // Bu dosyada 2 satır olarak verilmiş ama diğer dosyalarda,
    // 3 yada 10 olabileceği için komut satırından,
    // kişinin ne kadar geçmek istediği değeri alıyoruz.
    int line_skip = 0;
    printf("How many lines do you want to skip:");
    scanf("%d",&line_skip);

    // Burada fgets komutu ile geçeceğimiz satırları
    // girdiğimiz satır sayısı kadar okuyoruz.
    for (int i = 0; i < line_skip; ++i) {
        fgets(arr,n * sizeof(Iris),file);
        printf("Titles:\n%s",arr);
    }
    //Species de bizden string olarak değil,
    // Belirli stringlere karşılık int değerler yazmamızı istemişti.
    // bunun için (test) adında typedef struct oluşturduk.
    // test typedef struct'ın dinamik struct'ını oluşturduk.
    test *arr_Species;
    arr_Species = calloc(n, sizeof(test));

    // read ve records adında iki tane int değer oluşturduk
    // Bunlar sayfanın sonuna kadar bizim dosyadaki
    // verileri okuyup oluşturduğumuz dinamik structlara atıyor.
    int read = 0;
    int records = 0;
    do {
        read = fscanf(file,
                      "%d, %f, %f, %f, %f,%s,",
                      &IrisDataType[records].ID,
                      &IrisDataType[records].SepalLength,
                      &IrisDataType[records].SepalWidth,
                      &IrisDataType[records].PetalLength,
                      &IrisDataType[records].PetalWidth,
                      arr_Species[records].Species2);
        // Burada read 6 da kontrol etmemizin sebebi 6 sütun gittikten sonra,
        // Satır sayısını yani records değerini artırmamız gerektiği için.
        if(read == 6 ){
            ++records;
        }
        // read 6 olmaması demek, sayfanın sonuna geldiği anlamındadır.
        // Burada hatanın ne olduğunu yazıp return 1 yapıyoruz bunu yapmamızın sebebi
        // Komut satırına bir sorun olduğunu göndermek.
        if(read != 6 && !feof(file)){
            printf("File format incorrect.\n");
            return 1;
        }
        // Okuma sırasında bir hata olduğunda kodumuzun patlamasına engel oluyor.
        if(ferror(file)){
            printf("Error reading file .\n");
            exit(1);
        }
        //sayfanın sonuna kadar okuyor.
    } while (!feof(file));

    /*
     * arr_Species dinamik struct'a atadığımız değerleri ilgili eşleşmeleri
     * (Iris_setosa = 1,Iris_versicolor = 2,Iris_virginica = 3) sağlayıp,
     * kind struct'a attıyoruz.
     * kind'dan ilk oluşturduğumuz dinamik struct'a IrisDataType'a atıyoruz.
     */
    enum Species kind;
    for (int i = 0; i < n; ++i) {
        printf("\n i = %d , \nValue of Species: %s" ,i,arr_Species[i].Species2);
        if(strcmp(arr_Species[i].Species2, "Iris-setosa") == 0){
            kind = Iris_setosa;
        } if(strcmp(arr_Species[i].Species2, "Iris-versicolor") == 0){
            kind = Iris_versicolor;
        } if (strcmp(arr_Species[i].Species2, "Iris-virginica") == 0){
            kind = Iris_virginica;
        }
        if( kind == Iris_setosa || kind == Iris_versicolor || kind == Iris_virginica ){
            IrisDataType[i].Species = kind;
        }
    }
    // Buradada açtığımız dosyayı kapatıyoruz.
    fclose(file);

    printf("***************ALL OF DATA*********************");

    // komut satırına bütün değerleri yazdırıyoruz.
    for (int i = 0; i < n; ++i) {
        printf("\n Value of ID: %.2d" ,IrisDataType[i].ID);
        printf("\n Value of SepalLength: %.2f" ,IrisDataType[i].SepalLength);
        printf("\n Value of SepalWidth: %.2f" ,IrisDataType[i].SepalWidth);
        printf("\n Value of PetalLength: %.2f" ,IrisDataType[i].PetalLength);
        printf("\n Value of PetalWidth: %.2f",IrisDataType[i].PetalWidth);
        printf("\n Value of Species: %u" ,IrisDataType[i].Species);
        printf("\n i = %d",i);
        printf("\n ***************************");
    }
    // özellik matrisimizi dinamik olarak oluşturuyoruz.
    printf("\n***************FEATURE MATRIX*****************\n");

    float **feature_matrix;
    int row = 0, col = 0;
    printf("enter the number of rows.");
    scanf("%d",&row);
    printf("enter the number of columns.");
    scanf("%d",&col);

    feature_matrix = returnMatrix(row,col);

   // printMatrix(feature_matrix,row,col);

    printf("\n After the process");

    // Hesaplamalarda kullanacağımız “SepalLength”, “SepalWidth”, “PetalLength” ve “PetalWidth”
    // dinamik dizilerini oluşturuyoruz.
    float *arr_sepal_length;
    arr_sepal_length = malloc(n * sizeof(float ));

    float *arr_sepal_width;
    arr_sepal_width = malloc(n* sizeof(float ));

    float *arr_petal_length;
    arr_petal_length = malloc(n* sizeof(float ));

    float *arr_petal_width;
    arr_petal_width = malloc(n* sizeof(float ));

    //oluşturduğumuz dinamik dizileri adlarına göre IrisDataType'ın ilgili parametreleriyle dolduruyoruz.
    // özellik matrisini de dolduruyoruz.
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(j == 0){
                feature_matrix[i][j] = IrisDataType[i].SepalLength;
                arr_sepal_length[i] = IrisDataType[i].SepalLength;

            } else if(j == 1){
                feature_matrix[i][j] = IrisDataType[i].SepalWidth;
                arr_sepal_width[i] = IrisDataType[i].SepalWidth;

            }else if(j == 2){
                feature_matrix[i][j] = IrisDataType[i].PetalLength;
                arr_petal_length[i] = IrisDataType[i].PetalLength;

            } else{
                feature_matrix[i][j] = IrisDataType[i].PetalWidth;
                arr_petal_width[i] = IrisDataType[i].PetalWidth;
            }
        }
    }
    // özellik matrisini ve doldurduğumuz dinamik dizileri komut satırına yazıyoruz.
    printMatrix(feature_matrix,row,col);
    int i;
    for ( i = 0; i < n; ++i) {
        printf("\n arr_sepal_length[%d]: %.2f", i,arr_sepal_length[i]);;
    }
    for ( i = 0; i < n; ++i) {
        printf("\n arr_sepal_width[%d]: %.2f", i,arr_sepal_width[i]);;
    }
    for ( i = 0; i < n; ++i) {
        printf("\n arr_petal_length[%d]: %.2f", i,arr_petal_length[i]);;
    }
    for ( i = 0; i < n; ++i) {
        printf("\n arr_petal_width[%d]: %.2f", i,arr_petal_width[i]);;
    }
    printf("\n *************************");

    // Bizden istenen “SepalLength”, “SepalWidth”, “PetalLength” ve “PetalWidth”
    // dinamik dizilerinin ortalamalarını hesaplıyoruz.
    printf("\n***************MEAN PART*****************\n");

    float SepalLength_mean;
    float SepalWidth_mean;
    float PetalLength_mean;
    float PetalWidth_mean;

    SepalLength_mean = mean(arr_sepal_length,n);

    SepalWidth_mean = mean(arr_sepal_width,n);

    PetalLength_mean = mean(arr_petal_length,n);

    PetalWidth_mean = mean(arr_petal_width,n);

    // Bulduğumuz ortalamaları komut satırına yazıyoruz.
    printf("\nValue of SepalLength_Mean: %.3f ", SepalLength_mean);
    printf("\nValue of SepalWidth_mean: %.3f ", SepalWidth_mean);
    printf("\nValue of PetalLength_mean: %.3f ", PetalLength_mean);
    printf("\nValue of PetalWidth_mean: %.3f ", PetalWidth_mean);

    printf("\n****************************************************\n");

    // “SepalLength”, “SepalWidth”, “PetalLength” ve “PetalWidth” değerlerinin varyans değerlerini buluyoruz.
    // varyansı bulmak için aynı dinamik matrisi 2 kere covariance fonksiyonuna atıyoruz.
    printf("\n***************VARIANCE PART*****************\n");

    float SepalLength_variance;
    float SepalWidth_variance;
    float PetalLength_variance;
    float PetalWidth_variance;

    SepalLength_variance = covariance(arr_sepal_length,arr_sepal_length,n);

    SepalWidth_variance = covariance(arr_sepal_width,arr_sepal_width,n);

    PetalLength_variance = covariance(arr_petal_length,arr_petal_length,n);

    PetalWidth_variance = covariance(arr_petal_width,arr_petal_width,n);

    // Bulduğumuz variance değerlerini komut satırına yazıyoruz.
    printf("\nValue of SepalLength_variance: %.18f ", SepalLength_variance);
    printf("\nValue of SepalWidth_variance: %.18f ", SepalWidth_variance);
    printf("\nValue of PetalLength_variance: %.18f ", PetalLength_variance);
    printf("\nValue of PetalWidth_variance: %.18f", PetalWidth_variance);

    printf("\n****************************************************\n");

    // Bizden 3 özellikli korelasyon hesaplamamızı istiyordu.
    // Bizde “SepalLength”, “SepalWidth”, “PetalLength” ve “PetalWidth” dinamik matrislerinden,
    // seçtiklerimizi correlation fonksiyonuna gönderip korelasyon değerlerini hesaplıyoruz.
    printf("\n***************CORRELATION PART*****************\n");

    float corr_SepalLength_SepalWidth;
    float corr_SepalWidth_PetalLength;
    float corr_PetalLength_PetalWidth;

    corr_SepalLength_SepalWidth = correlation(arr_sepal_length,arr_sepal_width,n);

    corr_SepalWidth_PetalLength = correlation(arr_sepal_width,arr_petal_length,n);

    corr_PetalLength_PetalWidth = correlation(arr_petal_length,arr_petal_width,n);

    // Bulduğumuz korelasyon değerlerini komut satırına yazıyoruz.
    printf("\nValue of corr_SepalLength_SepalWidth: %.18f ", corr_SepalLength_SepalWidth);
    printf("\nValue of corr_SepalWidth_PetalLength: %.18f ", corr_SepalWidth_PetalLength);
    printf("\nValue of corr_PetalLength_PetalWidth: %.18f ", corr_PetalLength_PetalWidth);

    printf("\n****************************************************\n");

    //Oluşturduğumuz özellik matrisinin kovaryans değerini hesaplıyoruz.
    printf("\n***************COVARIANCE MATRIX PART*****************\n");


    float **covarianceM;
    // covarianceMatrix fonskiyonunu çalıştırdık.
    covarianceM = covarianceMatrix(feature_matrix,row,col);
    // covarianceM matrisini yazdırdık.
    printf("\n Result:\n");
    printMatrix(covarianceM,col,col);


    printf("\n****************************************************\n");

    printf("\n***************WRITE TO TEXT FILE*****************\n");

// Yazacağımız IrisStatistic.txt text dosyasını oluşturduk ve okumada yaptığımız fopen'ı,
// kullanarak bu sefer yazma modunda kullanıyoruz.
    FILE *file_w;
    file_w = fopen("C:\\Users\\Lenovo\\CLionProjects\\BLM19204\\2.Proje\\IrisStatistic.txt","w");
    if (file_w == NULL){
       printf("file is empty.");
    }
    // Bu kısımda hesapladığımız bütün değerleri IrisStatistic.txt text dosyasına yazıyoruz.
    fprintf(file_w,"SepalLength_Mean: %.3f,\n"
                   "SepalWidth_mean: %.3f,\n"
                   "PetalLength_mean: %.3f,\n"
                   "PetalWidth_mean: %.3f,\n"
                   "SepalLength_variance: %.18f,\n"
                   "SepalWidth_variance: %.18f,\n"
                   "PetalLength_variance: %.18f,\n"
                   "PetalWidth_variance: %.18f,\n"
                   "corr_SepalLength_SepalWidth: %.18f,\n"
                   "corr_SepalWidth_PetalLength: %.18f,\n"
                   "corr_PetalLength_PetalWidth: %.18f\n",
                   SepalLength_mean,
                   SepalWidth_mean,
                   PetalLength_mean,
                   PetalWidth_mean,
                   SepalLength_variance,
                   SepalWidth_variance,
                   PetalLength_variance,
                   PetalWidth_variance,
                   corr_SepalLength_SepalWidth,
                   corr_SepalWidth_PetalLength,
                   corr_PetalLength_PetalWidth);

    // covarianceM dinamik matrisi text dosyasına yazmak için 2 kere for döngüsü kullanıyoruz.
    for (i = 0; i < col; ++i) {
        for (int j = 0; j < col; ++j) {
            fprintf(file_w,"covarianceM: %.2f\n",covarianceM[i][j]);
            printf("\n dizi[%d][%d]: %.2f", i, j, covarianceM[i][j]);;
        }
        printf("\n");
    }
    // Buradada açtığımız dosyayı kapatıyoruz.
    fclose(file_w);

    //Sistemden Aldığımız bütün alanları sisteme geri iade ediyoruz.
    freeMatrix(covarianceM,col);
    free(arr_sepal_length);
    printf("\nspl");
    free(arr_sepal_width);
    printf("\nswt");
    free(arr_petal_length);
    printf("\nplt");
    free(arr_petal_width);
    printf("\npwt");
    free(IrisDataType);
    printf("\nIDT");
    free(arr);
    printf("\n ARR");
    free(arr_Species);
    printf("\n ARR_SPECIES");
    freeMatrix(feature_matrix,row);
    printf("\n free matrix2");
    return 0;
}
