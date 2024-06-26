// g++ -std=c++2b main.cpp -o k `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility> // для std::pair
#include <cmath>

#include <sstream>
#include <map>


using namespace std;

struct Type_list_entry_container {
    GtkWidget *grid_1, *grid_2, *entry;
};

// Глобальная переменная для хранения 3 указателя на GtkEntry
Type_list_entry_container *list_entry_container;

//Окно
GtkWidget *window;
//Основной grid
GtkWidget *main_grid;

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
        gtk_css_provider_load_from_data(provider, "entry { background-color: rgba(0,0,0,0.1);}", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider); // освобождаем ресурсы
        return G_SOURCE_REMOVE;
    }

public:
    //  Функция вызываемая при арифметической ошибке
    void red_table() {
        if (list_entry_container->entry == NULL) {
            g_print("Error: No GtkEntry provided for animation.\n");
            return;
        }
        // Создание анимации с использованием CSS
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "entry { background-color: red;}", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(list_entry_container->entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider); // освобождаем ресурсы

        // Установка таймера на возврат к предыдущему цвету обводки через 2000 миллисекунд (две секунды)
        g_timeout_add(2000, (GSourceFunc)reset_border_color, list_entry_container->entry);
        // Вызываем функцию перемещения через определенные интервалы времени
        g_timeout_add(300, move_widget, list_entry_container->entry); // Вызываем статический метод
    }
};






// Пользовательское исключение для деления на ноль
class DivisionByZeroException : public std::runtime_error {
public:
    DivisionByZeroException() : std::runtime_error("Error: Division by zero") {
        ErrorCalc err;
        err.red_table();
    }
};

// Пользовательское исключение для неравного количества скобок
class UnequalNumberOfParenthesesException : public std::runtime_error {
public:
    UnequalNumberOfParenthesesException() : std::runtime_error("Error: Unequal Number Of Parentheses") {
        ErrorCalc err;
        err.red_table();
    }
};



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
            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
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
                            throw DivisionByZeroException();
                        }
                        break;
                    case '^':
                        result = std::pow(result, num);
                        break;
                }
            }
        }
        return std::to_string(result);
    }

public:
    std::string removing_zeros(std::string expression){
        if (expression.find(',') != std::string::npos) {
            for (int i = expression.length() - 1; expression[i] == '0'; --i) expression.pop_back();
            if (expression[expression.length() - 1] = ',') expression.pop_back();
        }
        return expression == "" ? "0" : expression;
    }

    // Основной метод калькулятора
    std::string calc(const std::string &expression) {
        try{
            if (!equality_of_two_numbers(counting_parentheses(expression))) {
                throw UnequalNumberOfParenthesesException();
            }
            std::string expression_1 = expression;
            while (counting_parentheses(expression_1)[0] != 0) {
                std::vector<int> priority_brackets = searching_for_priority_brackets(expression_1, counting_parentheses(expression_1)[0]);
                expression_1 = expression_1.substr(0, priority_brackets[0]) +
                calculate_expression(tokenize(expression_1.substr(priority_brackets[0] + 1, priority_brackets[1] - priority_brackets[0] - 1))) +
                expression_1.substr(priority_brackets[1] + 1);
            }
            return removing_zeros(calculate_expression(tokenize(expression_1)));
        } catch (const std::exception &e) {
            std::cout<<e.what()<<std::endl;
            return expression;
        }
    }
};


// Подсчёт процентов
class Percent : public Calculate {
private:
    std::string entry_string;

    const std::string operators_4 = "+-/*";

    std::string computeResult(char priority_operator, const std::string& kl) {
        const std::string opres = "/*";
        std::string result;

        if (opres.find(priority_operator) != std::string::npos) {
            result = std::to_string(std::stod(kl) / 100);
        } else {
            std::string kl_1;

            if (entry_string.back() == ')') {
                int level = 1;
                short close_brackets = 0;

                for (int i = entry_string.size() - 2; i >= 0 && level > 0; --i) {
                    if (entry_string[i] == ')') {
                        level++;
                    } else if (entry_string[i] == '(') {
                        level--;
                        close_brackets = i;
                    }
                }

                if (entry_string[close_brackets - 1] == '-' && operators_4.find(entry_string[close_brackets - 2]) != std::string::npos)
                    close_brackets--;

                kl_1 = this->calc(entry_string.substr(close_brackets));
            } else {
                for (int i = entry_string.size() - 1; i >= 0; --i) {
                    if (operators_4.find(entry_string[i]) != std::string::npos) {
                        kl_1 = (entry_string[i] == '-' && operators_4.find(entry_string[i - 1]) != std::string::npos)
                        ? entry_string.substr(i)
                        : entry_string.substr(i + 1);
                        break;
                    }
                }
                if (kl_1.empty()) kl_1 = entry_string;
            }

            result = std::to_string(std::stod(kl_1) / 100 * std::stod(kl));
        }
        return removing_zeros(entry_string + priority_operator + result);
    }

public:
    Percent(const std::string& expression) : entry_string(expression) {}

    std::string percent_1() {

        if (entry_string.back() == ')') {
            int level = 1;
            short close_brackets = 0;

            for (int i = entry_string.size() - 2; i >= 0 && level > 0; --i) {
                if (entry_string[i] == ')') {
                    level++;
                } else if (entry_string[i] == '(') {
                    level--;
                    close_brackets = i;
                }
            }

            if (entry_string[close_brackets - 1] == '-' && operators_4.find(entry_string[close_brackets - 2]) != std::string::npos)
                close_brackets--;

            std::string kl = this->calc(entry_string.substr(close_brackets));
            char priority_operator = entry_string[close_brackets - 1];
            entry_string = entry_string.substr(0, close_brackets - 1);

            return computeResult(priority_operator, kl);
        } else {
            for (int i = entry_string.size() - 1; i > 0; --i) {
                if (operators_4.find(entry_string[i]) != std::string::npos) {
                    std::string kl = (entry_string[i] == '-' && operators_4.find(entry_string[i - 1]) != std::string::npos)
                    ? entry_string.substr(i)
                    : entry_string.substr(i + 1);
                    char priority_operator = entry_string[i];

                    entry_string = entry_string.substr(0, i);

                    return computeResult(priority_operator, kl);
                }
            }
            return removing_zeros(std::to_string(std::stod(entry_string) / 100));
        }
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


class StartUI {
private:

    // Обработчик сигнала изменения размера окна
    static void on_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer data) {
        GtkCssProvider *provider = GTK_CSS_PROVIDER(data);

        const char *css = (allocation->height > 660) ? "* {font-size: 35px;}" : "* {font-size: 20px;}";
        gtk_css_provider_load_from_data(provider, css, -1, NULL);

        GtkStyleContext *context = gtk_widget_get_style_context(widget);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }


    static void button_clicked(GtkWidget *widget, gpointer data) {
        // Получаем текст с кнопки
        const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
        // Получаем указатель на данные (это указатель на GtkEntry)
        GtkWidget *entry = (GtkWidget *)data;
        // Получаем текущий текст из GtkEntry
        const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
        // Преобразуем текст в строку
        std::string entry_string(entry_text);
        if (entry_string == "0" && std::string(",+*/").find(text) == std::string::npos) entry_string.clear();

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
            entry_string = "0";
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

    void window_s() {


        // Создаем окно
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Калькулятор");
        gtk_widget_set_size_request(window, 150, 350);

        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Рандомайзер для фона окна
        std::vector<std::string> fons_calor = {"#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);

        // Генерируем CSS-запрос
        std::string css_1 = "window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}";

        const gchar* css_data = css_1.c_str();

        // Устанавливаем CSS для фона окна
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    void main_grid_s() {
        main_grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), main_grid);
        gtk_container_set_border_width(GTK_CONTAINER(main_grid), 10);
        gtk_widget_set_hexpand(main_grid, TRUE);
        gtk_widget_set_vexpand(main_grid, TRUE);
    }

    void entry() {
        GtkWidget *grid_1 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), grid_1, 0, 0, 4, 1);
        gtk_widget_set_hexpand(grid_1, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(grid_1, TRUE); // Не растягивать по вертикали


        // Создаем CSS-провайдер
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "grid { border-bottom: 10px solid rgba(0, 0, 0, 0); }", -1, NULL);

        // Применяем CSS-провайдер к GtkGrid
        GtkStyleContext *context = gtk_widget_get_style_context(grid_1);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


        GtkWidget *grid_2 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(grid_1), grid_2, 0, 0, 1, 1);
        gtk_widget_set_hexpand(grid_2, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(grid_2, TRUE); // Не растягивать по вертикали

        // Создаем GtkEntry для отображения ввода
        GtkWidget* entry = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1); // Выравнивание по правому краю
        gtk_entry_set_text(GTK_ENTRY(entry), "0");
        gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // Не редактируемый
        gtk_grid_attach(GTK_GRID(grid_2), entry, 0, 0, 1, 1);
        gtk_widget_set_hexpand(entry, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(entry, TRUE); // Растяжение по вертикали

        provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "entry { background-color: rgba(0,0,0,0.1);}", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        list_entry_container = new Type_list_entry_container{grid_1, grid_2, entry};
    }

    GtkWidget *create_custom_button(const gchar *label) {
        GtkWidget *button = gtk_button_new_with_label(label);
        gtk_widget_set_hexpand(button, TRUE); // Кнопки растягиваются по горизонтали
        gtk_widget_set_vexpand(button, TRUE); // Кнопки растягиваются по вертикали
        gtk_widget_set_name(button, "custom-button");

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,R"(
        button {
            background: rgba(0,0,0,0.3);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none; /* Убирает возможные тени */
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        button:focus {
            background: rgba(0,0,0,0.3); /* Убирает изменение цвета при фокусировке */
        }
        )",
                                        -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(button), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


        return button;
    }

    void keybord() {
        const gchar *button_labels[] = {
            "(", ")", "!", "%",
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", ",", "=", "+",
            "C", "CE", "00", "^",
            "sin", "cos", "tan", "ctan"
        };

        for (int i = 0; i < 28; ++i) {
            GtkWidget* button = create_custom_button(button_labels[i]);
            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), list_entry_container->entry);
            gtk_grid_attach(GTK_GRID(main_grid), button, i % 4, i / 4 + 1, 1, 1);
        }
    }

public:
    void start() {
        window_s();
        main_grid_s();
        entry();
        keybord();

        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        // Создание CSS провайдера
        GtkCssProvider *provider = gtk_css_provider_new();

        // Подключение обработчика сигнала изменения размера окна
        g_signal_connect(window, "size-allocate", G_CALLBACK(on_size_allocate), provider);


        gtk_widget_show_all(window);

        gtk_main();
    }
};

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    StartUI start;
    start.start();

    return 0;
}
