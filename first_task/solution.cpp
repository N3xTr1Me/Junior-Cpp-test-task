#include <iostream>
#include <string>
#include <vector>
#include <sstream> 
#include <algorithm>
#include <tuple>

// Рекурссивная реализация расширенного алгоритма Евклида
std::tuple<int, int, int> extended_gcd(const int& a, const int& b) {
    
    int x, y;
    // Выход из рекурсии
    if (b == 0) {
        x = 1;
        y = 0;
        return {a, x, y};
    }

    // Временные x и y, а также текущий делитель d.
    int x_temp, y_temp, d;
    std::tie(d, x_temp, y_temp) = extended_gcd(b, a % b);

    x = y_temp;
    y = x_temp - y_temp * (a / b);

    return { d, x, y };
}

// Нахождение обратного по модулю числа
inline int modular_inverse(const int& a, const int& m) {
    auto [g, x, y] = extended_gcd(a, m);

    if (g != 1) {
        // Обратный элемент существует только если НОД(a, m) == 1,
        // т.е. 'a' и 'm' - взаимно просты.
        return -1;
    }

    // Возвращаем результат в диапазоне [0, m-1]
    return (x % m + m) % m;
}

// Брут-форс решение через перебор.
void brute_force_solution(const std::vector<unsigned int>& prev_numbers) {

    // Обозначим границы перебора параметра модуля m
    // Прибавляем к максимальному элементу единицу, т.к. если элемент равен модулю деления
    // то результатом операции будет 0. Следовательно, значение модуля хотя бы на 1 больше
    // самого большого из предыдущих чисел последовательности.
    int m_lower_border = *std::max_element(prev_numbers.begin(), prev_numbers.end()) + 1;
    const int MAX_MODULUS = 65535;

    // Цикл перебора модуля m
    for (unsigned int m = m_lower_border; m <= MAX_MODULUS; ++m) {

        // В общем случае разность может быть отрицательной, что потребует деления по модулю,
        // который изменяется в цикле, поэтому мы не можем рассчитать разности вне цикла.
        int X0_X1_diff_raw = prev_numbers[0] - prev_numbers[1];
        // Сдвигаем по модулю и делим по нему же. Если число отрицательное, то эта операция переведет его
        // в положительные. Для положительных ничего не изменится, 
        // т.к. прибавленный модуль уйдет после деления по модулю на него же
        int X0_X1_diff = (X0_X1_diff_raw % m + m) % m;

        int X1_X2_diff_raw = prev_numbers[1] - prev_numbers[2];
        int X1_X2_diff = (X1_X2_diff_raw % m + m) % m;

        int X0_X2_diff_raw = prev_numbers[0] - prev_numbers[2];
        int X0_X2_diff = (X0_X2_diff_raw % m + m) % m;
        
        // Расчет параметров 'a' и 'c' по выведенным формулам.

        // Находим обратное для знаменателя в формуле параметра 'a'
        long long X0_X1_inv = modular_inverse( X0_X1_diff, m);
        // Если обратного не существует, то мы не смодем рассчитать параметры.
        if (X0_X1_inv == -1) continue;

        int a = (X1_X2_diff * X0_X1_inv) % m;

        // Для нахождения C подставим параметр 'a' в первое уравнение в системе и выразим 'c'.
        int c = ((prev_numbers[1] - a * prev_numbers[0]) % m + m) % m;

        // Наконец, проверка подстановкой в систему уравнений:
        if (prev_numbers[1] == (a * prev_numbers[0] + c) % m &&
            prev_numbers[2] == (a * prev_numbers[1] + c) % m &&
            prev_numbers[3] == (a * prev_numbers[2] + c) % m
        ) {
            std::cout << "[Параметры: a - " << a << ", c - " << c << ", m - " << m << "]";
            std::cout << " Следующее значение X: " << (a * prev_numbers[3] + c) % m << "\n";
        }

    }

}

// Функция - разветвление в зависимости от количества предоставленных чисел.
inline void predict(const std::vector<unsigned int>& prev_numbers) {

    // Если чисел меньше 4, то систему из 3 уравнений не построить.
    if (prev_numbers.size() < 4) {
        std::cout << "Недостаточно чисел для предсказания следующего в последовательности. Введено - " << prev_numbers.size() << ".\n";
        return;
    }

    // Если даны только 4 предыдущих числа, то можем решить только перебором
    if (prev_numbers.size() == 4) {
        brute_force_solution(prev_numbers);
        return;
    }

    // Если количество предыдущих числе более 5, то можно найти аналитическое решение, описанное:
    // https://security.stackexchange.com/questions/4268/cracking-a-linear-congruential-generator
    if (prev_numbers.size() >= 5) {
        // Аналитическое решение
        std::cout << "Аналитическое решение пока что не реализовано!" << std::endl;
        return;
    }

}

int main() {

    std::cout << "Введите предыдущие числа последовательности через пробел:\n";
    std::string inputs;
    // Ввод сразу одной строкой, которая потом будет разделена по пробелу.
    std::getline(std::cin, inputs);

    std::vector<unsigned int> previous_numbers;
    // Для разбиения используется istringstream.
    std::istringstream numbers_stream(inputs);

    unsigned int number;
    while (numbers_stream >> number) { previous_numbers.push_back(number); }

    predict(previous_numbers);

    return 0;
}
