// g++ -std=c++2b main.cpp -o k `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <algorithm>
#include <utility> // для std::pair

#include <cmath>
#include <sstream>
#include <map>


using namespace std;

// Глобальная переменная для хранения указателя на GtkEntry
GtkWidget *global_entry = NULL;

// Объявление глобальной переменной для хранения предыдущего цвета обводки
static GdkRGBA previous_color;

// Добавлено объявление класса с указанием публичного доступа
class ErrorCalc {
private:
    // Статический метод для трясущегося эффекта
    //Доделать проблема в том что незаметнотряска
    static gboolean move_widget(gpointer data) {
        GtkWidget *widget = GTK_WIDGET(data);
        static int i = 0; // Используем статическую переменную для хранения состояния

        GtkAllocation allocation;
        gtk_widget_get_allocation(widget, &allocation);

        if (i % 2 == 0) {
            allocation.x += 10; // Сдвигаем вправо на 10 пикселей
        } else {
            allocation.x -= 10; // Сдвигаем влево на 10 пикселей
        }

        gtk_widget_set_allocation(widget, &allocation);
        gtk_widget_queue_draw(widget);

        i++;
        if (i >= 10) {
            i = 0; // Сбрасываем счетчик после 10 итераций
            return FALSE; // Завершаем таймер
        }
        return TRUE; // Продолжаем таймер
    }

    // Функция для сброса цвета обводки к предыдущему
    static gboolean reset_border_color(gpointer data) {
        GtkWidget *widget = GTK_WIDGET(data);
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "* { border-style: solid; border-width: 3px; border-color: rgba(0,0,0,0); transition: border-color 1s ease-in-out; }", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider); // освобождаем ресурсы
        return G_SOURCE_REMOVE;
    }

public:
    //  Функция вызываемая при арифметической ошибке
    void red_table() {
        if (global_entry == NULL) {
            g_print("Error: No GtkEntry provided for animation.\n");
            return;
        }

        GdkRGBA color;
        gdk_rgba_parse(&color, "red");

        // Создание анимации с использованием CSS
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "* { border-style: solid; border-width: 3px; border-color: red; transition: border-color 1s ease-in-out; }", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(global_entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider); // освобождаем ресурсы

        // Установка таймера на возврат к предыдущему цвету обводки через 2000 миллисекунд (две секунды)
        g_timeout_add(2000, (GSourceFunc)reset_border_color, global_entry);
        // Вызываем функцию перемещения через определенные интервалы времени
        g_timeout_add(300, move_widget, global_entry); // Вызываем статический метод
    }
};





bool if_coma(const std::string& expression){
    for (char i : expression){
        if (i == ','){
            return true;
        }
    }
    return false;
}

std::string expression_trimming(const std::string& expression) {
    std::string trimmed_expression = expression; // Создаем копию строки
    if (if_coma(trimmed_expression)) {
        for (int i = trimmed_expression.length() - 1; i >= 0; --i) {
            if (trimmed_expression[i] == '0' or trimmed_expression[i] == ',') {
                trimmed_expression.pop_back();
            } else {
                break;
            }
        }
        return trimmed_expression;
    }
    return trimmed_expression;
}



void pprint_2(std::vector<std::string> nestedVector ){
    // Выводим вложенный вектор в консоль
    std::cout << "Nested Vector:_2" << std::endl;
    for (std::string i  : nestedVector) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void pprint_1(std::vector<int> nestedVector ){
    // Выводим вложенный вектор в консоль
    std::cout << "Nested Vector:_1" << std::endl;
    for (int i  : nestedVector) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void pprint(std::vector <std::vector<int>> nestedVector ){
    // Выводим вложенный вектор в консоль
    std::cout << "Nested Vector:" << std::endl;
    for (const auto& innerVector : nestedVector) {
        for (int element : innerVector) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

class Calculate {
private:
    // Метод подсчёта скобок
    std::vector<int> counting_parentheses(const std::string &expression) {
        int bracket_1 = std::count(expression.begin(), expression.end(), '(');
        int bracket_2 = std::count(expression.begin(), expression.end(), ')');
        std::cout << "counting_parentheses" << std::endl;
        return std::vector<int>{bracket_1, bracket_2};
    }

    // Метод для поиска приоритетных скобок
    std::vector<int> searching_for_priority_brackets(std::string expression, int number_of_brackets) {
        int number_last_open_brackets = expression.find_last_of('(');
        int number_priority_close_brackets = expression.find(')', number_last_open_brackets);
        std::cout << "searching_for_priority_brackets" << std::endl;
        return {number_last_open_brackets, number_priority_close_brackets};
    }

    // Равность двух чисел
    bool equality_of_two_numbers(std::vector<int> counting_parentheses) {
        std::cout << "equality_of_two_numbers" << std::endl;
        return counting_parentheses[0] == counting_parentheses[1];
    }

    // Проверка наличия операций в выражении
    bool has_operations(const std::string &expression) {
        return std::any_of(expression.begin(), expression.end(), [](char c) {
            return c == '+' || c == '-' || c == '*' || c == '/';
        });
    }

    // Разбиение строки на отдельные элементы (числа и операторы)
    std::vector<std::string> tokenize(const std::string &expression) {
        std::vector<std::string> tokens;
        std::string token;
        for (char c : expression) {
            if (isdigit(c) || c == ',' || (token.empty() && c == '-')) {
                token += c;
            } else {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(std::string(1, c)); // Оператор
            }
        }
        if (!token.empty()) {
            tokens.push_back(token); // Последнее число в выражении
        }
        return tokens;
    }

    // Выполнение операций над числами
    std::string calculate_expression(const std::vector<std::string> &tokens) {
        double result = std::stod(tokens[0]);
        char last_operator = '+';
        for (size_t i = 1; i < tokens.size(); ++i) {
            const std::string &token = tokens[i];
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                last_operator = token[0];
            } else {
                double num = std::stod(token);
                if (i > 1 && tokens[i - 2] == "-") {
                    num *= -1;
                }
                switch (last_operator) {
                    case '+':
                        result += num;
                        break;
                    case '-':
                        result -= num;
                        break;
                    case '*':
                        result *= num;
                        break;
                    case '/':
                        if (num != 0) {
                            result /= num;
                        } else {
                            return "Деление на ноль";
                        }
                        break;
                }
            }
        }
        return std::to_string(result);
    }

public:
    // Основной метод калькулятора
    std::string calc(const std::string &expression) {
        if (!equality_of_two_numbers(counting_parentheses(expression))) {
            return "Неравное количество скобок!";
        }
        std::string expression_1 = expression;
        while (counting_parentheses(expression_1)[0] != 0) {
            std::vector<int> priority_brackets = searching_for_priority_brackets(expression_1, counting_parentheses(expression_1)[0]);
            expression_1 = expression_1.substr(0, priority_brackets[0]) +
            calculate_expression(tokenize(expression_1.substr(priority_brackets[0] + 1, priority_brackets[1] - priority_brackets[0] - 1))) +
            expression_1.substr(priority_brackets[1] + 1);
        }
        return calculate_expression(tokenize(expression_1));
    }
};


// Подсчёт процентов
class Percent : public Calculate {
private:
    std::string entry_string;
public:
    Percent(std::string expression) : entry_string(expression) {}
    std::string percent_1() {
        std::string result;

        char priority_operator = '*';
        size_t num_priority_operator = entry_string.size() - 1;
        std::string kl, kl_1;

        const std::string operators = "+-/*";

        // Check if the expression ends with a closing bracket
        if (entry_string.back() == ')') {
            int level = 1;
            size_t close_brackets = 0;

            // Find the corresponding opening bracket
            for (int i = entry_string.size() - 2; level > 0; i--) {
                if (entry_string[i] == ')') {
                    level++;
                } else if (entry_string[i] == '(') {
                    level--;
                    close_brackets = i;
                }
            }

            // Adjust close_brackets if the previous character is '-' and an operator
            if (entry_string[close_brackets - 1] == '-' && operators.find(entry_string[close_brackets - 2]) != std::string::npos) {
                close_brackets--;
            }

            kl = this->calc(entry_string.substr(close_brackets));
            num_priority_operator = close_brackets - 1;
            priority_operator = entry_string[num_priority_operator];
        } else {
            // Find the last operator in the expression
            for (int i = entry_string.size() - 1; i > 0; i--) {
                if (operators.find(entry_string[i]) != std::string::npos) {
                    if (entry_string[i] == '-' && operators.find(entry_string[i - 1]) != std::string::npos) {
                        kl = entry_string.substr(i);
                        num_priority_operator = i - 1;
                    } else {
                        kl = entry_string.substr(i + 1);
                        num_priority_operator = i;
                    }
                    priority_operator = entry_string[num_priority_operator];
                    break;
                }
            }
            // If the expression is a number, return the percentage value
            if (num_priority_operator == 0) {
                return std::to_string(std::stod(entry_string) / 100);
            }
        }

        // Calculate kl_1 and result based on priority_operator
        kl_1 = (entry_string[num_priority_operator - 1] == ')') ? this->calc(entry_string.substr(0, num_priority_operator)) : entry_string.substr(0, num_priority_operator);
        double kl_val = std::stod(kl);
        double kl_1_val = std::stod(kl_1);

        if (priority_operator == '*' || priority_operator == '/') {
            result = std::to_string(kl_val / 100);
        } else {
            result = std::to_string(kl_val * kl_1_val / 100);
        }

        // Construct and return the final expression
        return kl_1 + priority_operator + result;
    }
};




class Factorial : public Calculate {
private:
    std::string entry_string;

    // Функция для вычисления факториала числа
    std::string factorial(int n) {
        if (n < 0) {
            ErrorCalc err;
            err.red_table();
            return "!"; // Факториал отрицательного числа не определен
        }
        unsigned long long result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return std::to_string(result);
    }

public:
    Factorial(std::string expression) : entry_string(std::move(expression)) {}

    // Вспомогательный метод для подсчёта факториалов
    std::string factorial_1() {
        if (entry_string.back() == ')') {
            int level = 1;
            size_t close_brackets = entry_string.find_last_of('(');
            if (close_brackets == std::string::npos) {
                return entry_string; // Вернуть исходную строку, если нет открывающей скобки
            }
            std::string kl = factorial(std::stoi(calc(entry_string.substr(close_brackets))));
            return kl == "!" ? entry_string : entry_string.substr(0, close_brackets) + kl;
        } else {
            size_t pos = entry_string.size() - 1;
            while (pos > 0) {
                if (entry_string[pos] == '+' || entry_string[pos] == '*' || entry_string[pos] == '/') {
                    return entry_string.substr(pos + 1);
                } else if (entry_string[pos] == '-') {
                    if (pos == 0 || entry_string[pos - 1] == '+' || entry_string[pos - 1] == '-' || entry_string[pos - 1] == '*' || entry_string[pos - 1] == '/') {
                        ErrorCalc err;
                        err.red_table();
                        return entry_string;
                    } else {
                        std::string kl = factorial(std::stoi(entry_string.substr(pos + 1)));
                        return kl == "!" ? entry_string : entry_string.substr(0, pos + 1) + kl;
                    }
                }
                --pos;
            }
            std::string kl = factorial(std::stoi(entry_string));
            return kl == "!" ? entry_string : kl;
        }
    }
};



bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}



// Функция, вызываемая при нажатии кнопок
static void button_clicked(GtkWidget *widget, gpointer data) {
    // Получаем текст с кнопки
    const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
    // Получаем указатель на данные (это указатель на GtkEntry)
    GtkWidget *entry = (GtkWidget *)data;
    // Получаем текущий текст из GtkEntry
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    // Преобразуем текст в строку
    std::string entry_string(entry_text);

    // Проверяем, была ли нажата кнопка "="
    if (std::string(text) == "=") {
        Calculate cal;
        std::string result = cal.calc(entry_string);
        std::cout<<"kl"<<endl;
        entry_string = result;
    } else if (std::string(text) == "C") {
        // Стираем один символ, если нажата кнопка "C"
        if (!entry_string.empty())
            entry_string.pop_back();
    } else if (std::string(text) == "CE") {
        // Стираем всё выражение, если нажата кнопка "CE"
        entry_string = "";
    } else if (std::string(text) == "!") {
        // Вычисляем факториал, если нажата кнопка "!"
        Factorial factor(entry_string);
        entry_string = factor.factorial_1();
    } else if (std::string(text) == "%") {
        Percent per(entry_string);
        entry_string = per.percent_1();
    } else {
        // Добавляем текст с кнопки к текущему тексту
        entry_string += text;
    }
    // Устанавливаем новый текст в GtkEntry
    gtk_entry_set_text(GTK_ENTRY(entry), entry_string.c_str());
}

// Функция для создания кастомных кнопок с прозрачным фоном
GtkWidget *create_custom_button(const gchar *label) {
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_widget_set_opacity(button, 0.7); // Устанавливаем прозрачность кнопки
    return button;
}





void on_window_realize(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);

    // Получаем размеры окна и устанавливаем ширину entry
    int window_width;
    gtk_window_get_size(GTK_WINDOW(widget), &window_width, NULL);
    int entry_width = window_width - 10; // Ширина окна минус 10px
    gtk_widget_set_size_request(entry, entry_width, -1);
}


class StartUI{
    GtkWidget* window(){
        // Создаем окно
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Простой калькулятор");
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);
        gtk_widget_set_size_request(window, 200, 200);

        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        //Рандомайзер для фона окна
        vector<std::string> fons_calor = {"#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);
        //Генерируем cssзапрос
        std::string css_1 = "window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}";

        const gchar* css_data = css_1.c_str();

        // Устанавливаем CSS для фона окна
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        return window;
    }

    GtkWidget* entry(){
        // Создаем GtkEntry для отображения ввода
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1); // Выравнивание по правому краю
        gtk_entry_set_text(GTK_ENTRY(entry), "");
        gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // Не редактируемый
        gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);
    }

    GtkWidget* keybord(){
        // Создаем Grid для размещения виджетов
        GtkWidget *grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

    }

    GtkWidget* buttons(){
        // Создаем Grid для размещения виджетов
        GtkWidget *grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

        // Массив с названиями кнопок калькулятора
        const gchar *button_labels[] = {
            "(", ")", "!", "%",
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", ".", "=", "+",
            "C", "CE", "00", "000",
            "sin", "cos", "ctg", "tg"
        };

        // Создаем кнопки и присоединяем к ним функцию button_clicked
        for (int i = 0; i < 28; ++i) {
            GtkWidget *button = create_custom_button(button_labels[i]);
            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
            gtk_grid_attach(GTK_GRID(grid), button, i % 4, i / 4 + 1, 1, 1);
        }
    }

    void start (){
        GtkWidget *window = this->window();
        GtkWidget *entry = this->entry();
        GtkWidget *keybord = this->keybord();


        //--------


        // Выравнивание текста по центру


        // Сигнал realize для окна, чтобы установить размеры entry после отображения окна
        g_signal_connect(window, "realize", G_CALLBACK(on_window_realize), entry);

        //---------

        // Установка глобальной переменной global_entry
        global_entry = entry;


    }
};

int main(int argc, char *argv[]) {
    // Инициализация GTK
    gtk_init(&argc, &argv);

    StartUI start;
    start.start()





    // Устанавливаем обработчик закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Показываем все виджеты
    gtk_widget_show_all(window);

    // Запускаем цикл обработки событий GTK
    gtk_main();

    return 0;
}
