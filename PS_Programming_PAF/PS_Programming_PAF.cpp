#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

int monthToNumber(string m) {
    if (m == "Ene") return 1;
    if (m == "Feb") return 2;
    if (m == "Mar") return 3;
    if (m == "Abr") return 4;
    if (m == "May") return 5;
    if (m == "Jun") return 6;
    if (m == "Jul") return 7;
    if (m == "Ago") return 8;
    if (m == "Sep") return 9;
    if (m == "Oct") return 10;
    if (m == "Nov") return 11;
    if (m == "Dic") return 12;
    return 0;
}

bool compareDates(string mA, int dA, string mB, int dB) {
    int monthA = monthToNumber(mA);
    int monthB = monthToNumber(mB);
    if (monthA != monthB) return monthA < monthB;
    return dA <= dB;
}

void readOrders(string months[], int days[], string info[], int& count) {
    ifstream ordersFile("C:\\Users\\Reg00\\source\\repos\\PS_Programming_PAF\\x64\\Debug\\orders.txt");
    count = 0;
    string month, time, restOfLine;
    int day;

    while (ordersFile >> month >> day >> time) {
        getline(ordersFile, restOfLine);
        months[count] = month;
        days[count] = day;
        info[count] = month + " " + to_string(day) + " " + time + restOfLine;
        count++;
    }
    ordersFile.close();
}

void insertionSort(string months[], int days[], string info[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        string keyMonth = months[i];
        int keyDay = days[i];
        string keyInfo = info[i];

        int j = i - 1;
        while (j >= left && !compareDates(months[j], days[j], keyMonth, keyDay)) {
            months[j + 1] = months[j];
            days[j + 1] = days[j];
            info[j + 1] = info[j];
            j--;
        }
        months[j + 1] = keyMonth;
        days[j + 1] = keyDay;
        info[j + 1] = keyInfo;
    }
}

void merge(string months[], int days[], string info[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    string* leftMonths = new string[n1];
    string* rightMonths = new string[n2];
    int* leftDays = new int[n1];
    int* rightDays = new int[n2];
    string* leftInfo = new string[n1];
    string* rightInfo = new string[n2];

    for (int i = 0; i < n1; i++) {
        leftMonths[i] = months[l + i];
        leftDays[i] = days[l + i];
        leftInfo[i] = info[l + i];
    }
    for (int j = 0; j < n2; j++) {
        rightMonths[j] = months[m + 1 + j];
        rightDays[j] = days[m + 1 + j];
        rightInfo[j] = info[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (compareDates(leftMonths[i], leftDays[i], rightMonths[j], rightDays[j])) {
            months[k] = leftMonths[i];
            days[k] = leftDays[i];
            info[k] = leftInfo[i];
            i++;
        }
        else {
            months[k] = rightMonths[j];
            days[k] = rightDays[j];
            info[k] = rightInfo[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        months[k] = leftMonths[i];
        days[k] = leftDays[i];
        info[k] = leftInfo[i];
        i++; k++;
    }
    while (j < n2) {
        months[k] = rightMonths[j];
        days[k] = rightDays[j];
        info[k] = rightInfo[j];
        j++; k++;
    }

    delete[] leftMonths;
    delete[] rightMonths;
    delete[] leftDays;
    delete[] rightDays;
    delete[] leftInfo;
    delete[] rightInfo;
}

const int run = 100; 

void timsort(string months[], int days[], string info[], int n) {
    for (int i = 0; i < n; i += run) {
        int right = min(i + run - 1, n - 1);
        insertionSort(months, days, info, i, right);
    }

    for (int size = run; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right) {
                merge(months, days, info, left, mid, right);
            }
        }
    }
}

int main() {
    const int n = 10000;
    string months[n];
    int days[n];
    string info[n];
    int count;

    readOrders(months, days, info, count);
    timsort(months, days, info, count);

    string startMonth, endMonth;
    int startDay, endDay;

    cout << "Enter start (e.g. Mar 1): ";
    cin >> startMonth >> startDay;
    cout << "Enter end (e.g. Sep 7): ";
    cin >> endMonth >> endDay;
    cout << "\nRecords between " << startMonth << " " << startDay
        << " and " << endMonth << " " << endDay << ":\n";

    ofstream outFile("output.txt");
    for (int i = 0; i < count; i++) {
        if (compareDates(startMonth, startDay, months[i], days[i]) &&
            compareDates(months[i], days[i], endMonth, endDay)) {
            cout << info[i] << endl;
            outFile << info[i] << endl;
        }
    }
    outFile.close();

    cout << "\nFiltered info saved to new file: output.txt\n";
    return 0;
}
