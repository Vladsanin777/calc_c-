// g++ main.cpp -o k `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <utility> // для std::pair

#include <cmath>
#include <sstream>
#include <map>


using namespace std;


// Глобальная переменная для хранения указателя на GtkEntry
GtkWidget *global_entry = NULL;


// Объявление глобальной переменной для хранения предыдущего цвета обводки
static GdkRGBA previous_color;



class ErrorCalc {
private:

    // Функция для сброса цвета обводки к предыдущему
    static gboolean reset_border_color() {
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "* { border-style: solid; border-width: 3px; border-color: rgba(0,0,0,0); transition: border-color 1s ease-in-out; }", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(global_entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

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

        // Установка таймера на возврат к предыдущему цвету обводки через 500 миллисекунд (полсекунды)
        g_timeout_add(2000, (GSourceFunc)reset_border_color, global_entry);
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

class Calculate{
private:
    //Метод подсчёта скобок
    std::vector<int> counting_parentheses (const std::string& expression){
        int bracket_1 = 0, bracket_2 = 0;
        for (char e : expression){
            switch (e){
                case '(':
                    bracket_1++;
                    break;
                case ')':
                    bracket_2++;
                    break;
            }
        }
        std::cout<<"counting_parentheses"<<std::endl;
        return std::vector<int>{bracket_1, bracket_2};
    }
    //Равность двух чисел
    bool equality_of_two_numbers (std::vector<int> counting_parentheses){
        std::cout<<"equality_of_two_numbers"<<std::endl;
        return counting_parentheses[0] == counting_parentheses[1];
    }

    // Метод для поиска приоритетных скобок
    std::vector <int> searching_for_priority_brackets (std::string expression, int number_of_brackets){
        std::cout<<expression<<"expression   "<<size(expression)<<std::endl;
        int number_last_open_brackets;
        for (int i = size(expression) - 1; i != -1; i--){
            std::cout<<expression[i]<<std::endl;
            if (expression[i] == '('){
                number_last_open_brackets = i;
                break;
            }
        }
        std::cout<<number_last_open_brackets<<std::endl;
        int number_priority_close_brackets = -1;
        for (int i = 1; number_last_open_brackets > number_priority_close_brackets; i++){
            std::cout<<expression[i]<<std::endl;
            if (expression[i] == ')'){
                std::cout<<i<<std::endl;
                number_priority_close_brackets = i;
            }
        }
        std::cout<<number_last_open_brackets<<number_priority_close_brackets<<std::endl;
        std::cout<<"searching_for_priority_brackets"<<std::endl;
        return {number_last_open_brackets, number_priority_close_brackets};
    }

    bool calc_3(std::string expression_2){
        for (int i = 1; i != size(expression_2); i++){
            if (expression_2[i] == '+' || expression_2[i] == '-' || expression_2[i] == '*' || expression_2[i] == '/') {
                std::cout<<"calc_3"<<std::endl;
                return true;
            }
        }
        std::cout<<"calc_3"<<std::endl;
        return false;
    }

    // Функция для разбиения строки на отдельные элементы (числа и операторы)
    std::vector<string> tokenize(const std::string expression) {
        std::cout<<expression<<std::endl;
        std::vector<string> tokens;
        std::string token;
        int minus = 0;
        for (int i = 0; i < expression.size(); i++) {
            if ((isdigit(expression[i]) || expression[i] == ',') || (minus == 0 && expression[i] == '-')) { // Если текущий символ - цифра или минус (для отрицательных чисел)
                minus++;
                token += expression[i];
            } else {
                minus = 0;
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(string(1, expression[i])); // Оператор
            }
        }
        if (!token.empty()) {
            tokens.push_back(token); // Последнее число в выражении
        }
        pprint_2(tokens);
        std::cout<<"tokenize"<<std::endl;
        return tokens;
    }

    std::string calculate_1(std::vector<string> tokens) {
        pprint_2(tokens);
        double result = stod(tokens[0]); // Первый токен всегда число
        char last_operator = '+'; // Начинаем с оператора сложения

        for (size_t i = 1; i < tokens.size(); ++i) {
            const string &token = tokens[i];
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                last_operator = token[0];
            } else {
                double num = stod(token);
                if (i > 1 && tokens[i - 2] == "-") {
                    num *= -1; // Если число идет после минуса, делаем его отрицательным
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
                            std::cout<<"calculate_1"<<std::endl;
                            return "Деление на ноль";
                        }
                        break;
                }
            }
        }
        std::cout<<"calculate_1"<<std::endl;
        return std::to_string(result);
    }

    // Метод для подсчёта простых выражений
    std::string calc_2 (std::vector <int> priority_brackets, std::string expression_1){
        std::cout<<expression_1<<std::endl;
        pprint_1(priority_brackets);
        std::string bracket = this->calculate_1(this->tokenize(expression_1.substr(priority_brackets[0] + 1, priority_brackets[1] - priority_brackets[0]-1)));
        std::cout<<"calc_2"<<std::endl;
        return expression_1.substr(0, priority_brackets[0]) + bracket + expression_1.substr(priority_brackets[1]+1);
    }
public:
    //Основной метод калькулятора
    std::string calc(std::string expression_1){
        if (!this->equality_of_two_numbers(this->counting_parentheses(expression_1))) return "Неравное количество скобок!";
        while (this->counting_parentheses(expression_1)[0] != 0){
            std::cout<<"Со скобками"<<std::endl;
            expression_1 = this->calc_2(this->searching_for_priority_brackets(expression_1, this->counting_parentheses(expression_1)[0]), expression_1);
        }
        std::cout<<"Без скобок"<<std::endl;
        return expression_trimming(this->calculate_1(this->tokenize(expression_1)));
    }
};



class Factorial : public Calculate {
private:
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
    // Вспомогательный метод для подсчёта факториалов
    std::string factorial_1(std::string entry_string){
        std::cout<<"factorial_1"<<std::endl;
        std::cout<<entry_string[size(entry_string)-1]<<std::endl;
        if (entry_string[size(entry_string)-1] == ')'){
            std::cout<<"Со скобками"<<std::endl;
            int level = 1;
            short close_brackets;
            for (int i = size(entry_string) - 2; level > 0; i--){
                if (entry_string[i] == ')'){
                    level++;
                }else if (entry_string[i] == '('){
                    level--;
                    close_brackets = i;
                }
            }
            std::string kl = this->factorial(std::stoi(this->calc(entry_string.substr(close_brackets))));
            return kl == "!" ? entry_string : entry_string.substr(0, close_brackets) + kl;
        }else{
            std::cout<<"Без скобок"<<std::endl;
            std::cout<<size(entry_string)-1<<std::endl;
            for (int i = size(entry_string)-1; i > 0; i--){
                std::cout<<"gh"<<std::endl;
                if (entry_string[i] == '+' || entry_string[i] == '*' || entry_string[i] == '/'){
                    return entry_string.substr(i+1);
                } else if (entry_string[i] == '-') {
                    if (entry_string[i-1] == '+' || entry_string[i-1] == '-'|| entry_string[i-1] == '*' || entry_string[i-1] == '/'){
                        ErrorCalc err;
                        err.red_table();
                        return entry_string;
                    }else{
                        std::cout<<"1"<<std::endl;
                        std::string kl = this->factorial(std::stoi(entry_string.substr(i+1)));
                        return kl == "!" ? entry_string : entry_string.substr(0, i + 1) + kl;
                    }
                }
            }
            std::cout<<"2"<<std::endl;
            std::string kl = this->factorial(std::stoi(entry_string));
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
        Factorial factor;
        entry_string = factor.factorial_1(entry_string);

    } else if (std::string(text) == "%") {
        int last_operator;
        for (int i = entry_string.length() - 1; i != 0; --i){
            if (isOperator(entry_string[i])){
                last_operator = i;
                break;
            }
        }
        if (last_operator != 0){
            std::string begin_str = entry_string.substr(0, last_operator);
            double end_double = std::stod(entry_string.substr(last_operator + 1));
            char operat = entry_string[last_operator];
            std::cout<<begin_str<<end_double<<operat;
            std::string percent;
            int number_of_open_parentheses = 1;
            int start_expression = 0;
            double result;
            switch (operat) {
                case '+':
                case '-':

                    for (int i = begin_str.length() - 1; i >= 0; --i){
                        switch (begin_str[i]){
                            case '(':
                                number_of_open_parentheses -= 1;
                                break;
                            case ')':
                                number_of_open_parentheses += 1;
                                break;
                        }
                        if (number_of_open_parentheses == 0){
                            start_expression = i + 1;
                            break;
                        }
                    }
                    Calculate cal;
                    result = std::stod(cal.calc(begin_str.substr(start_expression)));
                    percent = expression_trimming(std::to_string(result / 100 * end_double));
                    break;
                case '*':
                case '/':
                    double r = end_double / 100;
                    percent = expression_trimming(std::to_string(r));
                    break;
            }
            entry_string = begin_str + operat + percent;
        }else{
            double r = std::stod(entry_string) / 100;
            entry_string = expression_trimming(std::to_string(r));
        }

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

int main(int argc, char *argv[]) {
    // Инициализация GTK
    gtk_init(&argc, &argv);





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

    // Создаем Grid для размещения виджетов
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создаем GtkEntry для отображения ввода
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1); // Выравнивание по правому краю
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // Не редактируемый
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Установка глобальной переменной global_entry
    global_entry = entry;

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

    // Устанавливаем обработчик закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Показываем все виджеты
    gtk_widget_show_all(window);

    // Запускаем цикл обработки событий GTK
    gtk_main();

    return 0;
}
