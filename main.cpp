// g++ main.cpp -o k `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>



using namespace std;

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




// Функция для проверки, является ли символ оператором
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Функция для определения приоритета оператора
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

// Функция для выполнения операции между двумя операндами
double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

// Функция для вычисления значения арифметического выражения в постфиксной форме
std::string calculate(const string& expression) {
    stack<double> values;
    stack<char> operators;

    for (int i = 0; i < expression.size(); ++i) {
        char c = expression[i];
        // Пропускаем пробелы
        if (c == ' ')
            continue;

        // Если символ - цифра, то помещаем его в стек значений
        if (isdigit(c)) {
            double num = 0;
            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    double fractionalPart = 0;
                    double divisor = 10;
                    ++i;
                    while (i < expression.size() && isdigit(expression[i])) {
                        fractionalPart += (expression[i] - '0') / divisor;
                        divisor *= 10;
                        ++i;
                    }
                    num += fractionalPart;
                } else {
                    num = num * 10 + (expression[i] - '0');
                    ++i;
                }
            }
            values.push(num);
            --i;
        }
        // Если символ - открывающая скобка, помещаем его в стек операторов
        else if (c == '(') {
            operators.push(c);
        }
        // Если символ - закрывающая скобка, выполняем операции до тех пор,
        // пока не встретим открывающую скобку
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.pop(); // Удаляем открывающую скобку из стека
        }
        // Если символ - оператор
        else if (isOperator(c)) {
            // Выполняем операции с более высоким или равным приоритетом
            // до тех пор, пока в стеке есть операторы с более высоким приоритетом
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(c);
        }
    }

    // Выполняем оставшиеся операции в стеках
    while (!operators.empty()) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(applyOperation(a, b, op));
    }

    // Возвращаем результат
    return expression_trimming(std::to_string(values.top()));
}




// Функция для вычисления факториала числа
unsigned long long factorial(int n) {
    if (n < 0) return 0; // Факториал отрицательного числа не определен
    unsigned long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
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
        // Вычисляем результат выражения
        std::string result = calculate(entry_string);
        // Преобразуем результат обратно в строку
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
        try {
            int n = std::stoi(entry_string);
            entry_string = std::to_string(factorial(n));
        } catch (...) {
            entry_string = "Ошибка";
        }
    } else if (std::string(text) == "%") {
        int last_operator;
        for (int i = entry_string.length() - 1; i != 0; --i){
            if (isOperator(entry_string[i])){
                last_operator = i;
                break;
            }
        }
        std::string begin_str = entry_string.substr(0, last_operator);
        double end_double = std::stod(entry_string.substr(last_operator + 1));
        char operat = entry_string[last_operator];
        std::cout<<begin_str<<end_double<<operat;
        std::string percent;
        int number_of_open_parentheses = 1;
        int start_expression = 0;
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
                        start_expression = i;
                        break;
                    }
                }
                std::cout<<std::endl<<start_expression<<std::endl;
                std::cout<<begin_str.substr(start_expression);
                percent = expression_trimming(std::to_string(std::stod(calculate(begin_str.substr(start_expression))) / 100 * end_double));
                break;
            case '*':
            case '/':
                double r = end_double / 100;
                percent = expression_trimming(std::to_string(r));
                break;
        }
        entry_string = begin_str + operat + percent;

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

    // Устанавливаем CSS для фона окна
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(cssProvider, "window {background: linear-gradient(to bottom right, #ff0000, #0000ff);}", -1, NULL);
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

    // Массив с названиями кнопок калькулятора
    const gchar *button_labels[] = {
        "(", ")", "!", "%",
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+",
        "C", "CE", "00", "000"
    };

    // Создаем кнопки и присоединяем к ним функцию button_clicked
    for (int i = 0; i < 24; ++i) {
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
