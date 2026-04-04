#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Structure to store outlier information
typedef struct {
    int day;
    int year;
    double temperature;
    double bound;
    int isBelow;  // 1 if below lower bound, 0 if above upper bound
} Outlier;

// Swapper function
void swap(double *a, double *b) {
    double fir = *a, sec = *b;
    *a = sec;
    *b = fir;
}

// Bubble sorter
void bubbleSort(double arr[], int size) {
    int control = 1;
    while (control != 0) {
        control = 0;
        for(int i = 0; i < size - 1; i++) {
            if(arr[i] > arr[i+1]) {
                swap(&arr[i], &arr[i+1]);
                control = 1;
            }
        }
    }
}

// Median Finder
double findMedian(double arr[], int start, int end) {
    int length = end - start + 1;
    int middle = start + length/2;
    
    if(length % 2 == 0) {
        return (arr[middle] + arr[middle - 1]) / 2;
    }
    else {
        return arr[middle];
    }
}

// Quartile Finder
void findQuartiles(double arr[], int size, double *Q1, double *Q2, double *Q3) {
    bubbleSort(arr, size);
    *Q2 = findMedian(arr, 0, size - 1);
    
    if(size % 2 == 0) {
        *Q1 = findMedian(arr, 0, size / 2 - 1);
        *Q3 = findMedian(arr, size/2, size - 1);
    }
    else {
        *Q1 = findMedian(arr, 0, size / 2 - 1);
        *Q3 = findMedian(arr, size/2 + 1, size - 1);
    }
}

// Mean Finder
double mean(double arr[], int size) {
    double total = 0;
    for (int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total / size;
}

// monthDay converts the days into teh seperate months so they can be easily read
void monthDay(int days, char *buffer) {
    if(days >= 1 && days <= 31) { sprintf(buffer, "Jan %d", days); }
    else if(days >= 32 && days <= 59) { sprintf(buffer, "Feb %d", days - 31); }
    else if(days >= 60 && days <= 90) { sprintf(buffer, "Mar %d", days - 59); }
    else if(days >= 91 && days <= 120) { sprintf(buffer, "Apr %d", days - 90); }
    else if(days >= 121 && days <= 151) { sprintf(buffer, "May %d", days - 120); }
    else if(days >= 152 && days <= 181) { sprintf(buffer, "Jun %d", days - 151); }
    else if(days >= 182 && days <= 212) { sprintf(buffer, "Jul %d", days - 181); }
    else if(days >= 213 && days <= 243) { sprintf(buffer, "Aug %d", days - 212); }
    else if(days >= 244 && days <= 273) { sprintf(buffer, "Sep %d", days - 243); }
    else if(days >= 274 && days <= 304) { sprintf(buffer, "Oct %d", days - 273); }
    else if(days >= 305 && days <= 334) { sprintf(buffer, "Nov %d", days - 304); }
    else if(days >= 335 && days <= 365) { sprintf(buffer, "Dec %d", days - 334); }
}

// cleandata takes the individualised points from parse and makes sure they are all formatted correctly no matter the style
double cleanData(char *str) {
    char cleaned[70];
    int count = 0;
    int hasDecimal = 0;
    
    for(int i = 0; str[i] != '\0' && count < 69; i++) {
        if(isdigit(str[i]) || str[i] == '-') {
            cleaned[count++] = str[i];
        }
        else if(str[i] == ',' && !hasDecimal) {
            cleaned[count++] = '.';
            hasDecimal = 1;
        }
        else if(str[i] == '.' && !hasDecimal) {
            cleaned[count++] = '.';
            hasDecimal = 1;
        }
    }
    
    cleaned[count] = '\0';
    if (count > 0) { 
        return atof(cleaned);
    }
    return -9999.0;
}

// parses a line from the CSV files, this is needed as it seperates each data point and makes sure it can be read universally
int parseLine(char *line, double *dayNum, double temps[], int maxTemps) {
    char *token;
    int count = 0;
    char separator;
    
    if(strchr(line, ';') != NULL) {
        separator = ';';
    } else {
        separator = ',';
    }
    
    token = strtok(line, &separator);
    if(token == NULL) return 0;
    *dayNum = cleanData(token);
    
    while((token = strtok(NULL, &separator)) != NULL && count < maxTemps) {
        temps[count++] = cleanData(token);
    }
    
    return count;
}

// the function to sort outliers by year, then by day in order to print then coheasively
void sortOutliers(Outlier outliers[], int count) {
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(outliers[j].year > outliers[j+1].year ||
               (outliers[j].year == outliers[j+1].year && outliers[j].day > outliers[j+1].day)) {
                Outlier temp = outliers[j];
                outliers[j] = outliers[j+1];
                outliers[j+1] = temp;
            }
        }
    }
}

// Structure to hold lake data
typedef struct {
    char name[20];
    char filename[20];
    double summerAvg;
} Lake;

int main() {
    Lake lakes[6] = {
        {"Superior", "Superior.csv", 0},
        {"Michigan", "Michigan.csv", 0},
        {"Huron", "Huron.csv", 0},
        {"Erie", "Erie.csv", 0},
        {"Ontario", "Ontario.csv", 0},
        {"St. Clair", "StClair.csv", 0}
    };
    
    int days = 365;
    int years = 31;
    
    double allSummerAvgs[6];
    char allLakeNames[6][20];
    
    printf("GREAT LAKES WATER TEMPERATURE ANALYSIS\n");
    printf("Data Period: 1995-2025 (31 years)\n");

    //Trys to open each lake file and notifys if they cannot be
    for(int lakeIdx = 0; lakeIdx < 6; lakeIdx++) {
        FILE *data = fopen(lakes[lakeIdx].filename, "r");
        if(data == NULL) {
            printf("\nERROR: Cannot open %s\n", lakes[lakeIdx].filename);
            continue;
        }
        
        printf("\n\n========== %s ==========\n", lakes[lakeIdx].name);
        
        double avg_temp[days][years];
        double day_num[days];
        double tempor[years];
        char line[5000];
        
        // Initialize array with -9999
        for(int i = 0; i < days; i++) {
            for(int j = 0; j < years; j++) {
                avg_temp[i][j] = -9999;
            }
        }
        
        // Read all data
        int day = 0;
        while(fgets(line, sizeof(line), data) != NULL && day < days) {
            double temps[years];
            int numTemps = parseLine(line, &day_num[day], temps, years);
            
            for(int i = 0; i < numTemps && i < years; i++) {
                avg_temp[day][i] = temps[i];
            }
            day++;
        }
        fclose(data);
        
        //QUARTILES BY DAY, finds and prints the first 10 quartiles
        printf("\n--- QUARTILES BY DAY (First 10 days shown) ---\n");
        printf("Day      Date         Q1     Median   Q3\n");
        
        for(int j = 0; j < days && j < 10; j++) {
            int count = 0;
            for(int i = 0; i < years; i++) {
                if(avg_temp[j][i] != -9999) {
                    tempor[count++] = avg_temp[j][i];
                }
            }
            
            if(count > 0) {
                double Q1, Q2, Q3;
                findQuartiles(tempor, count, &Q1, &Q2, &Q3);
                char dateBuf[20];
                monthDay((int)day_num[j], dateBuf);
                printf("Day %3.0f   %-10s %6.2f  %6.2f  %6.2f\n", day_num[j], dateBuf, Q1, Q2, Q3);
            }
        }
        
        //OUTLIERS DETECTION
        printf("\n--- OUTLIERS (Sorted by Year, then by Day) ---\n");
        
        Outlier outliers[10000];
        int outlierCount = 0;
        
        //runs through every temp for each day, ignores any invalid numbers
        for(int j = 0; j < days; j++) {
            int count = 0;
            for(int i = 0; i < years; i++) {
                if(avg_temp[j][i] != -9999) {
                    tempor[count++] = avg_temp[j][i];
                }
            }
            
            if(count > 0) {
                double Q1, Q2, Q3;
                findQuartiles(tempor, count, &Q1, &Q2, &Q3);
                
                double IQR = Q3 - Q1;
                double LB = Q1 - 1.5 * IQR;
                double UB = Q3 + 1.5 * IQR;
                
                for(int i = 0; i < years; i++) {
                    if(avg_temp[j][i] != -9999) {
						//checks for lower bound
                        if(avg_temp[j][i] < LB) {
                            outliers[outlierCount].day = (int)day_num[j];
                            outliers[outlierCount].year = 1995 + i;
                            outliers[outlierCount].temperature = avg_temp[j][i];
                            outliers[outlierCount].bound = LB;
                            outliers[outlierCount].isBelow = 1;
                            outlierCount++;
                        }
                        //checks for higher bound
                        else if(avg_temp[j][i] > UB) {
                            outliers[outlierCount].day = (int)day_num[j];
                            outliers[outlierCount].year = 1995 + i;
                            outliers[outlierCount].temperature = avg_temp[j][i];
                            outliers[outlierCount].bound = UB;
                            outliers[outlierCount].isBelow = 0;
                            outlierCount++;
                        }
                    }
                }
            }
        }
        
        //Sort and print outliers by year, then by day
        sortOutliers(outliers, outlierCount);
        
        if(outlierCount == 0) {
            printf("  No outliers detected for this lake.\n");
        } else {
            int currentYear = -1;
            for(int i = 0; i < outlierCount; i++) {
                if(outliers[i].year != currentYear) {
                    currentYear = outliers[i].year;
                    printf("\n  Year %d:\n", currentYear);
                }
                char dateBuf[20];
                monthDay(outliers[i].day, dateBuf);
                printf("    Day %3d (%10s): %.2f C ", outliers[i].day, dateBuf, outliers[i].temperature);
                if(outliers[i].isBelow) {
                    printf("[BELOW %.2f]\n", outliers[i].bound);
                } else {
                    printf("[ABOVE %.2f]\n", outliers[i].bound);
                }
            }
            printf("\n  Total outliers found: %d\n", outlierCount);
        }
        
        //WARMEST AND COLDEST DAYS, finds and prints thedays with the highest and lowest everage temperature 
        printf("\n--- WARMEST & COLDEST DAYS (30-year average) ---\n");
        
        double dailyAvgs[days];
        for(int j = 0; j < days; j++) {
            int count = 0;
            for(int i = 0; i < years; i++) {
                if(avg_temp[j][i] != -9999) {
                    tempor[count++] = avg_temp[j][i];
                }
            }
            dailyAvgs[j] = (count > 0) ? mean(tempor, count) : -9999;
        }
        
        int coldestDay = 0, warmestDay = 0;
        for(int j = 1; j < days; j++) {
            if(dailyAvgs[j] > -9999) {
                if(dailyAvgs[j] < dailyAvgs[coldestDay]) coldestDay = j;
                if(dailyAvgs[j] > dailyAvgs[warmestDay]) warmestDay = j;
            }
        }
        
        char coldestDate[20], warmestDate[20];
        monthDay(coldestDay + 1, coldestDate);
        monthDay(warmestDay + 1, warmestDate);
        
        printf("  Coldest: Day %d (%s) - Avg: %.2f C\n", coldestDay + 1, coldestDate, dailyAvgs[coldestDay]);
        printf("  Warmest: Day %d (%s) - Avg: %.2f C\n", warmestDay + 1, warmestDate, dailyAvgs[warmestDay]);
        
        //SUMMER AVERAGE, prints the average temperature from the days between june 21 and september 22
        printf("\n--- SUMMER AVERAGE (Days 172-265: June 21 - September 22) ---\n");
        
        double summerTotal = 0;
        int summerCount = 0;
        for(int j = 171; j < 265; j++) {
            for(int i = 0; i < years; i++) {
                if(avg_temp[j][i] != -9999) {
                    summerTotal += avg_temp[j][i];
                    summerCount++;
                }
            }
        }
        
        lakes[lakeIdx].summerAvg = (summerCount > 0) ? summerTotal / summerCount : 0;
        strcpy(allLakeNames[lakeIdx], lakes[lakeIdx].name);
        allSummerAvgs[lakeIdx] = lakes[lakeIdx].summerAvg;
        
        printf("  Summer average: %.2f C\n", lakes[lakeIdx].summerAvg);
    }
    
    // RANK LAKES BY SUMMER AVERAGE, the lakes are then ranked by their average temperature in the summer
    printf("\n\n================================================================\n");
    printf("LAKES RANKED BY SUMMER AVERAGE (Warmest to Coldest)\n");
    printf("================================================================\n");
    
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5 - i; j++) {
            if(allSummerAvgs[j] < allSummerAvgs[j + 1]) {
                double tempAvg = allSummerAvgs[j];
                allSummerAvgs[j] = allSummerAvgs[j + 1];
                allSummerAvgs[j + 1] = tempAvg;
                
                char tempName[20];
                strcpy(tempName, allLakeNames[j]);
                strcpy(allLakeNames[j], allLakeNames[j + 1]);
                strcpy(allLakeNames[j + 1], tempName);
            }
        }
    }
    
    for(int i = 0; i < 6; i++) {
        printf("%d. %-12s: %.2f C\n", i + 1, allLakeNames[i], allSummerAvgs[i]);
    }
    
    printf("PROGRAM COMPLETED SUCCESSFULLY\n");    
    return 0;
}
