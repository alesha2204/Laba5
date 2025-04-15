#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;
using namespace std::chrono;

struct SortStats {
    int iterations;
    int comparisons;
    int swaps;
    double time_ms;
};

SortStats insertionSort(vector<int>& arr) {
    SortStats stats = { 0, 0, 0, 0 };
    auto start = high_resolution_clock::now();

    for (int i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i;
        stats.iterations++;

        while (j > 0 && arr[j - 1] > key) {
            stats.comparisons++;
            arr[j] = arr[j - 1];
            j--;
            stats.swaps++;
        }
        stats.comparisons += (j > 0);
        arr[j] = key;
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

SortStats selectionSort(vector<int>& arr) {
    SortStats stats = { 0, 0, 0, 0 };
    auto start = high_resolution_clock::now();

    for (int i = 0; i < arr.size() - 1; ++i) {
        int min_idx = i;
        stats.iterations++;

        for (int j = i + 1; j < arr.size(); ++j) {
            stats.comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
            stats.swaps++;
        }
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

SortStats bubbleSort(vector<int>& arr) {
    SortStats stats = { 0, 0, 0, 0 };
    auto start = high_resolution_clock::now();

    for (int i = 0; i < arr.size() - 1; ++i) {
        stats.iterations++;
        bool swapped = false;

        for (int j = 0; j < arr.size() - i - 1; ++j) {
            stats.comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped) break;
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

void quickSortHelper(vector<int>& arr, int low, int high, SortStats& stats) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        stats.iterations++;

        for (int j = low; j <= high - 1; ++j) {
            stats.comparisons++;
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
                stats.swaps++;
            }
        }
        swap(arr[i + 1], arr[high]);
        stats.swaps++;
        int pi = i + 1;

        quickSortHelper(arr, low, pi - 1, stats);
        quickSortHelper(arr, pi + 1, high, stats);
    }
}

SortStats quickSort(vector<int>& arr) {
    SortStats stats = { 0, 0, 0, 0 };
    auto start = high_resolution_clock::now();

    quickSortHelper(arr, 0, arr.size() - 1, stats);

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

SortStats sortNegativesWithQuickSort(vector<int>& arr) {
    SortStats stats = { 0, 0, 0, 0 };
    auto start = high_resolution_clock::now();

    vector<int> negativeIndices;
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] < 0) {
            negativeIndices.push_back(i);
        }
    }

    if (!negativeIndices.empty()) {
        vector<int> negatives;
        for (auto idx : negativeIndices) {
            negatives.push_back(arr[idx]);
        }

        quickSortHelper(negatives, 0, negatives.size() - 1, stats);
        reverse(negatives.begin(), negatives.end());

        for (int i = 0; i < negativeIndices.size(); ++i) {
            arr[negativeIndices[i]] = negatives[i];
        }
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 2001 - 1000;
    }
    return arr;
}

void printResultsTable(const vector<int>& sizes, const vector<vector<SortStats>>& allStats) {
    const vector<string> sortNames = {
        "������ ����������", "������ �������", "������ �������", "�������"
    };

    for (int i = 0; i < sortNames.size(); ++i) {
        cout << "\n=== " << sortNames[i] << " ===" << endl;
        cout << left << setw(10) << "������" << setw(15) << "��������"
            << setw(15) << "���������" << setw(15) << "������������"
            << setw(15) << "����� (��)" << endl;

        for (int j = 0; j < sizes.size(); ++j) {
            cout << setw(10) << sizes[j]
                << setw(15) << allStats[i][j].iterations
                    << setw(15) << allStats[i][j].comparisons
                    << setw(15) << allStats[i][j].swaps
                    << setw(15) << fixed << setprecision(3) << allStats[i][j].time_ms << endl;
        }
    }
}

void printSingleArrayStats(const vector<SortStats>& stats) {
    const vector<string> sortNames = {
        "������ ����������", "������ �������", "������ �������", "�������"
    };

    cout << left << setw(20) << "\n����� ����������" << setw(15) << "��������"
        << setw(15) << "���������" << setw(15) << "������������"
        << setw(15) << "����� (��)" << endl;

    for (int i = 0; i < sortNames.size(); ++i) {
        cout << setw(20) << sortNames[i]
            << setw(15) << stats[i].iterations
                << setw(15) << stats[i].comparisons
                << setw(15) << stats[i].swaps
                << setw(15) << fixed << setprecision(3) << stats[i].time_ms << endl;
    }
}

vector<int> inputArray() {
    int size;
    cout << "������� ������ �������: ";
    while (!(cin >> size) || size <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "�������� ����, ������� ����� ������������� �����: ";
    }

    vector<int> arr(size);
    cout << "������� " << size << " ���������:\n";
    for (int i = 0; i < size; ++i) {
        while (!(cin >> arr[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����, ������� ����� �����: ";
        }
    }
    return arr;
}

void compareAllSorts() {
    vector<int> sizes = { 20, 500, 1000, 3000, 5000, 10000 };
    vector<vector<SortStats>> allStats(4);

    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size);

        vector<int> arr1 = arr;
        allStats[0].push_back(insertionSort(arr1));

        vector<int> arr2 = arr;
        allStats[1].push_back(selectionSort(arr2));

        vector<int> arr3 = arr;
        allStats[2].push_back(bubbleSort(arr3));

        vector<int> arr4 = arr;
        allStats[3].push_back(quickSort(arr4));
    }

    printResultsTable(sizes, allStats);
}

void sortUserArray() {
    vector<int> arr = inputArray();
    vector<SortStats> stats(4);

    vector<vector<int>> arrays(4, arr);

    stats[0] = insertionSort(arrays[0]);
    stats[1] = selectionSort(arrays[1]);
    stats[2] = bubbleSort(arrays[2]);
    stats[3] = quickSort(arrays[3]);


    printSingleArrayStats(stats);
}

void customTask() {
    cout << "������� 20 ���������: ";
    vector<int> customArr(20);
    for (int i = 0; i < 20; ++i) {
        while (!(cin >> customArr[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����, ������� ����� �����: ";
        }
    }

    cout << "\n�������� ������: ";
    for (int num : customArr) {
        cout << num << " ";
    }
    cout << endl;

    SortStats customStats = sortNegativesWithQuickSort(customArr);

    cout << "��������������� ������: ";
    for (int num : customArr) {
        cout << num << " ";
    }
    cout << endl;

    cout << "\n���������� ���������� ������� �����:" << endl;
    cout << "��������: " << customStats.iterations << endl;
    cout << "��������: " << customStats.comparisons << endl;
    cout << "������������: " << customStats.swaps << endl;
    cout << "�����: " << fixed << setprecision(3) << customStats.time_ms << " ��" << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    while (true) {
        cout << "����:\n"
            << "1. ��������� ���������� ����� �������� ������� �������� 20, 500, 1000, 3000, 5000, 10000 ���������\n"
            << "2. ��������� ���������� �������, ���������� �������������, ����� ��������\n"
            << "3. ���������� ������� ����� ������� �� 20 ���������\n"
            << "4. �����\n"
            << "��� �����: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����, ������� 1-4: ";
        }

        switch (choice) {
        case 1:
            compareAllSorts();
            break;
        case 2:
            sortUserArray();
            break;
        case 3:
            customTask();
            break;
        case 4:
            return 0;
        }
    }

    return 0;
}