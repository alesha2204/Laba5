#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <random>

using namespace std;
using namespace std::chrono;

struct SortStats {
    int iterations;
    int comparisons;
    int swaps;
    double time_ms;
};

struct SearchStats {
    int comparisons;
    double time_ms;
    bool found;
};

// Функции сортировки (остаются без изменений)
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

// Функции поиска
SearchStats linearSearch(const vector<int>& arr, int target, int& foundCount) {
    SearchStats stats = { 0, 0, false };
    foundCount = 0;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < arr.size(); ++i) {
        stats.comparisons++;
        if (arr[i] == target) {
            stats.found = true;
            foundCount++;
        }
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

SearchStats linearSearchWithBarrier(vector<int>& arr, int target, int& foundCount) {
    SearchStats stats = { 0, 0, false };
    foundCount = 0;
    auto start = high_resolution_clock::now();

    int last = arr.back();
    arr.push_back(target); // Добавляем барьер

    int i = 0;
    while (true) {
        stats.comparisons++;
        if (arr[i] == target) {
            if (i < arr.size() - 1) {
                foundCount++;
            }
            break;
        }
        i++;
    }

    // Проверяем остальные вхождения (если есть дубликаты)
    for (int j = i + 1; j < arr.size() - 1; ++j) {
        stats.comparisons++;
        if (arr[j] == target) {
            foundCount++;
        }
    }

    arr.back() = last; // Восстанавливаем исходный массив
    stats.found = (foundCount > 0);

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

SearchStats binarySearch(const vector<int>& arr, int target, int& foundCount) {
    SearchStats stats = {0, 0, false};
    foundCount = 0;
    auto start = high_resolution_clock::now();

    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        stats.comparisons++;

        if (arr[mid] == target) {
            stats.found = true;
            foundCount = 1;
            
            // Проверяем дубликаты слева
            int i = mid - 1;
            while (i >= left && arr[i] == target) {
                stats.comparisons++;
                foundCount++;
                i--;
            }
            
            // Проверяем дубликаты справа
            i = mid + 1;
            while (i <= right && arr[i] == target) {
                stats.comparisons++;
                foundCount++;
                i++;
            }
            break;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    auto end = high_resolution_clock::now();
    stats.time_ms = duration_cast<microseconds>(end - start).count() / 1000.0;
    return stats;
}

// Генерация массивов
vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-1000, 1000);

    for (int i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    return arr;
}

vector<int> generatePartiallySortedArray(int size, double sortedPercent) {
    vector<int> arr = generateRandomArray(size);
    int sortedSize = static_cast<int>(size * sortedPercent / 100.0);
    sort(arr.begin(), arr.begin() + sortedSize);
    return arr;
}

// Вспомогательные функции для вывода
void printSearchResultsTable(const vector<int>& sizes, const vector<SearchStats>& stats, const vector<int>& foundCounts) {
    cout << left << setw(10) << "Размер" << setw(20) << "Время (мс)"
        << setw(20) << "Сравнения" << setw(20) << "Найден"
        << setw(20) << "Кол-во вхождений" << endl;

    for (int i = 0; i < sizes.size(); ++i) {
        cout << setw(10) << sizes[i]
            << setw(20) << fixed << setprecision(6) << stats[i].time_ms
                << setw(20) << stats[i].comparisons
                << setw(20) << (stats[i].found ? "Да" : "Нет")
                << setw(20) << foundCounts[i] << endl;
    }
}

void printSearchComparisonTable(const vector<string>& types, const vector<SearchStats>& linearStats,
    const vector<int>& linearFoundCounts, const vector<SearchStats>& binaryStats = vector<SearchStats>(),
    const vector<int>& binaryFoundCounts = vector<int>()) {
    cout << left << setw(25) << "Тип массива" << setw(20) << "Метод поиска"
        << setw(15) << "Время (мс)" << setw(15) << "Сравнения"
        << setw(20) << "Кол-во вхождений" << endl;

    for (int i = 0; i < types.size(); ++i) {
        cout << setw(25) << types[i] << setw(20) << "Линейный"
            << setw(15) << fixed << setprecision(6) << linearStats[i].time_ms
            << setw(15) << linearStats[i].comparisons
            << setw(20) << linearFoundCounts[i] << endl;

        if (!binaryStats.empty() && i == 0) {
            cout << setw(25) << types[i] << setw(20) << "Бинарный"
                << setw(15) << fixed << setprecision(6) << binaryStats[i].time_ms
                << setw(15) << binaryStats[i].comparisons
                << setw(20) << binaryFoundCounts[i] << endl;
        }
    }
}

// Функции для новых пунктов меню
void compareLinearSearchPerformance() {
    vector<int> sizes = { 20, 500, 1000, 3000, 5000, 10000 };
    vector<SearchStats> stats;
    vector<int> foundCounts;

    int target;
    cout << "Введите элемент для поиска: ";
    cin >> target;

    cout << "\nСравнение времени линейного поиска для разных размеров массивов:\n";

    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size);
        int count;
        stats.push_back(linearSearch(arr, target, count));
        foundCounts.push_back(count);
    }

    printSearchResultsTable(sizes, stats, foundCounts);
}

void compareSearchWithDifferentArrayTypes() {
    const int size = 10000;
    vector<string> types = {
        "Отсортированный", "Обратный порядок", "25% отсортирован",
        "50% отсортирован", "75% отсортирован"
    };

    int target;
    cout << "Введите элемент для поиска: ";
    cin >> target;

    vector<int> sortedArr = generateRandomArray(size);
    sort(sortedArr.begin(), sortedArr.end());

    vector<int> reverseSortedArr = sortedArr;
    reverse(reverseSortedArr.begin(), reverseSortedArr.end());

    vector<int> partiallySorted25 = generatePartiallySortedArray(size, 25);
    vector<int> partiallySorted50 = generatePartiallySortedArray(size, 50);
    vector<int> partiallySorted75 = generatePartiallySortedArray(size, 75);

    vector<SearchStats> linearStats;
    vector<int> linearFoundCounts;
    vector<SearchStats> binaryStats;
    vector<int> binaryFoundCounts;

    // Линейный поиск
    int count;
    linearStats.push_back(linearSearch(sortedArr, target, count));
    linearFoundCounts.push_back(count);

    linearStats.push_back(linearSearch(reverseSortedArr, target, count));
    linearFoundCounts.push_back(count);

    linearStats.push_back(linearSearch(partiallySorted25, target, count));
    linearFoundCounts.push_back(count);

    linearStats.push_back(linearSearch(partiallySorted50, target, count));
    linearFoundCounts.push_back(count);

    linearStats.push_back(linearSearch(partiallySorted75, target, count));
    linearFoundCounts.push_back(count);

    // Бинарный поиск (только для отсортированного массива)
    binaryStats.push_back(binarySearch(sortedArr, target, count));
    binaryFoundCounts.push_back(count);

    cout << "\nСравнение поиска в массивах с разной степенью упорядоченности (размер = "
        << size << "):\n";
    printSearchComparisonTable(types, linearStats, linearFoundCounts, binaryStats, binaryFoundCounts);
}

void compareLinearSearchWithBarrier() {
    vector<int> sizes = { 20, 500, 1000, 3000, 5000, 10000 };
    vector<SearchStats> regularStats;
    vector<SearchStats> barrierStats;
    vector<int> regularFoundCounts;
    vector<int> barrierFoundCounts;

    int target;
    cout << "Введите элемент для поиска: ";
    cin >> target;

    cout << "\nСравнение линейного поиска с барьером и без:\n";
    cout << left << setw(10) << "Размер" << setw(25) << "Обычный (время мс)"
        << setw(25) << "С барьером (время мс)" << setw(20) << "Разница (мс)"
        << setw(20) << "Найдено (обычный)" << setw(20) << "Найдено (барьер)" << endl;

    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size);
        vector<int> arrCopy = arr;

        int countRegular, countBarrier;
        SearchStats regular = linearSearch(arr, target, countRegular);
        SearchStats barrier = linearSearchWithBarrier(arrCopy, target, countBarrier);

        regularStats.push_back(regular);
        barrierStats.push_back(barrier);
        regularFoundCounts.push_back(countRegular);
        barrierFoundCounts.push_back(countBarrier);

        cout << setw(10) << size
            << setw(25) << fixed << setprecision(6) << regular.time_ms
            << setw(25) << fixed << setprecision(6) << barrier.time_ms
            << setw(20) << fixed << setprecision(6) << (regular.time_ms - barrier.time_ms)
            << setw(20) << countRegular
            << setw(20) << countBarrier << endl;
    }
}
// Остальные функции (без изменений)
void printResultsTable(const vector<int>& sizes, const vector<vector<SortStats>>& allStats) {
    const vector<string> sortNames = {
        "Прямым включением", "Прямым выбором", "Прямым обменом", "Быстрая"
    };

    for (int i = 0; i < sortNames.size(); ++i) {
        cout << "\n=== " << sortNames[i] << " ===" << endl;
        cout << left << setw(10) << "Размер" << setw(15) << "Итерации"
            << setw(15) << "Сравнения" << setw(15) << "Перестановки"
            << setw(15) << "Время (мс)" << endl;

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
        "Прямым включением", "Прямым выбором", "Прямым обменом", "Быстрая"
    };

    cout << left << setw(20) << "\nМетод сортировки" << setw(15) << "Итерации"
        << setw(15) << "Сравнения" << setw(15) << "Перестановки"
        << setw(15) << "Время (мс)" << endl;

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
    cout << "Введите размер массива: ";
    while (!(cin >> size) || size <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод, введите целое положительное число: ";
    }

    vector<int> arr(size);
    cout << "Введите " << size << " элементов:\n";
    for (int i = 0; i < size; ++i) {
        while (!(cin >> arr[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод, введите целое число: ";
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
    cout << "Введите 20 элементов: ";
    vector<int> customArr(20);
    for (int i = 0; i < 20; ++i) {
        while (!(cin >> customArr[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод, введите целое число: ";
        }
    }

    cout << "\nИсходный массив: ";
    for (int num : customArr) {
        cout << num << " ";
    }
    cout << endl;

    SortStats customStats = sortNegativesWithQuickSort(customArr);

    cout << "Отсортированный массив: ";
    for (int num : customArr) {
        cout << num << " ";
    }
    cout << endl;

    cout << "\nСтатистика сортировки методом Хоара:" << endl;
    cout << "Итерации: " << customStats.iterations << endl;
    cout << "Сравнения: " << customStats.comparisons << endl;
    cout << "Перестановки: " << customStats.swaps << endl;
    cout << "Время: " << fixed << setprecision(3) << customStats.time_ms << " мс" << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    while (true) {
        cout << "\nМеню:\n"
            << "1. Выполнить сортировку всеми методами массива размером 20, 500, 1000, 3000, 5000, 10000 элементов\n"
            << "2. Выполнить сортировку массива, введенного пользователем, всеми методами\n"
            << "3. Сортировка методом Хоара массива из 20 элементов\n"
            << "4. Сравнить время выполнения линейного поиска\n"
            << "5. Исследовать влияние упорядоченности массива на поиск\n"
            << "6. Исследовать эффективность поиска с барьером\n"
            << "7. Выход\n"
            << "Ваш выбор: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод, введите 1-7: ";
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
            compareLinearSearchPerformance();
            break;
        case 5:
            compareSearchWithDifferentArrayTypes();
            break;
        case 6:
            compareLinearSearchWithBarrier();
            break;
        case 7:
            return 0;
        }
    }

    return 0;
}