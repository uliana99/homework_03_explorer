#include <explorer.hpp>

//  Декларируем функции для проверки состояния
void get_name(const path& _path);
void get_size(const path& _path);
void get_data(const path& _path);
bool print_info(const path& _path);


int main(int argc, char const *argv[]) {
    try {
        scheduler_t scheduler;

        std::map<const path, std::future<bool> > results;

        for (int i = 1; i < argc; i++) {

            const path filePath = argv[i];

            //  Конструируем задание для future
            auto task = std::packaged_task<bool()> { std::bind(print_info, filePath) };
            //  Получаем future
            auto result = task.get_future();
            //  Добавляем задание в планировщик
            scheduler.add( task );
            //  Добавляем future в контейнер 
            results.emplace(filePath, std::move(result));
        }
        //  Запускаем потоки
        scheduler.run();

        //  Проходим по контейнеру и проверяем состояние future для переданных файлов
        for(auto & filePath : results) {
            auto& result = filePath.second;
            auto is_ok = result.get();
            if (! is_ok) {
                std::cerr << "Wrong adress: " << filePath.first << std::endl;
            }
        }


    }  catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

//  Реализуем функции для проверки состояния
bool print_info(const path& _path) {
    //  Проверяем файл на существование
    if(!exists(_path) )
        return false;

    //  Получаем данные о файле
    get_name(_path);
    get_size(_path);
    get_data(_path);

    return true;
}

void get_name(const path& _path) {
    std::ofstream out( "log.txt", std::ios_base::app);
    out << "NAME:\t" << _path.filename() << std::endl;
    out.close();
}

void get_size(const path& _path) {
    std::ofstream out( "log.txt", std::ios_base::app);
    out << "SIZE:\t" << ( ( (double)(file_size(_path) ) ) / 1000 ) << std::endl;
    out.close();
}

void get_data(const path& _path) {
    std::time_t cftime = last_write_time(_path);
    std::ofstream out( "log.txt", std::ios_base::app);
    out << "DATE:\t" << asctime(localtime(&cftime)) << std::endl;
    out.close();
}
