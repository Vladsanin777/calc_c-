// g++ main.cpp -o k `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Функция для вычисления результата выражения
double calculate(const std::string &expression) {
    // Простейший калькулятор, который просто выполняет вычисление строки
    try {
        return std::stod(expression);
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 0.0; // Возвращаем 0 в случае ошибки
    }
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
        double result = calculate(entry_string);
        // Преобразуем результат обратно в строку
        entry_string = std::to_string(result);
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
    } else {
        // Добавляем текст с кнопки к текущему тексту
        entry_string += text;
    }

    // Устанавливаем новый текст в GtkEntry
    gtk_entry_set_text(GTK_ENTRY(entry), entry_string.c_str());
}

int main(int argc, char *argv[]) {
    // Инициализация GTK
    gtk_init(&argc, &argv);

    // Создаем окно
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Простой калькулятор");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 200, 200);

    // Устанавливаем CSS для фона окна с измененным направлением градиента
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
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+",
        "C", "CE", "!"
    };

    // Создаем кнопки и присоединяем к ним функцию button_clicked
    for (int i = 0; i < 19; ++i) {
        GtkWidget *button = gtk_button_new_with_label(button_labels[i]);
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

