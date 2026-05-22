cd ~/lab08

cat > README.md << 'EOF'
# Лабораторная работа №8: Docker

**Студент:** qNetayS  
**Дата выполнения:** 2026-05-22  
**Окружение:** Ubuntu 18.04, Docker, GitHub Actions

---

## Цель работы

Изучение основ Docker: создание Dockerfile, сборка образов, запуск контейнеров, монтирование томов, автоматизация сборки с GitHub Actions.

---

## Ход выполнения работы

### 1. Создание репозитория на GitHub

```shell
# Создаём публичный репозиторий lab08
gh repo create lab08 --public --clone

# Переходим в директорию репозитория
cd lab08
```
Результат: Репозиторий https://github.com/qNetayS/lab08 успешно создан.

### 2. Создание программы demo.cpp
Программа читает строки из стандартного ввода и сохраняет их в файл лога.
```
cat > demo.cpp << 'EOF'
#include <iostream>
#include <fstream>
#include <cstdlib>

int main() {
    const char* log_path = std::getenv("LOG_PATH");
    std::ofstream log(log_path ? log_path : "/home/logs/log.txt");
    std::string line;
    
    std::cout << "Enter text (Ctrl+D to finish):" << std::endl;
    while (std::getline(std::cin, line)) {
        std::cout << "You entered: " << line << std::endl;
        if (log.is_open()) {
            log << line << std::endl;
        }
    }
    
    std::cout << "Log saved" << std::endl;
    return 0;
}
EOF
```
### 3.Создание CMakeLists.txt
```
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.10)
project(demo)

set(CMAKE_CXX_STANDARD 11)
add_executable(demo demo.cpp)

install(TARGETS demo DESTINATION bin)
EOF
```
### 4.Создание Dockerfile
```
cat > Dockerfile << 'EOF'
FROM ubuntu:18.04

# Обновление и установка необходимых пакетов
RUN apt update
RUN apt install -yy gcc g++ cmake

# Копирование исходного кода в контейнер
COPY . /print
WORKDIR /print

# Сборка проекта с помощью CMake
RUN cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_install
RUN cmake --build _build
RUN cmake --build _build --target install

# Настройка переменной окружения для пути к логам
ENV LOG_PATH /home/logs/log.txt

# Создание тома для хранения логов
VOLUME /home/logs

# Установка рабочей директории и точки входа
WORKDIR /print/_install/bin
ENTRYPOINT ./demo
EOF
```

# 5. Сборка Docker-a
```
docker build -t logger .
```

# 6. Запуск контейнера
```
# Создание директории для логов на хост-системе
mkdir -p logs

# Запуск контейнера с монтированием тома
docker run -it -v "$(pwd)/logs/:/home/logs/" logger
```

# 7.Настройка Github Actions 
Создаем workflow для автоматической сборки Docker-образа при каждом  push
```
mkdir -p .github/workflows

cat > .github/workflows/docker.yml << 'EOF'
name: Docker Build

on:
  push:
    branches: [ main, master ]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Build Docker image
      run: docker build -t logger .

    - name: Run container
      run: |
        mkdir -p logs
        echo -e "test1\ntest2\ntest3" | docker run -i -v "$(pwd)/logs:/home/logs/" logger
        cat logs/log.txt
EOF
```
# 9. Отправка на Github
```
# Добавление всех файлов
git add .

# Проверка статуса
git status

# Создание коммита
git commit -m "Lab08: Docker with GitHub Actions"

# Переименование ветки (если нужно)
git branch -M main

# Отправка на GitHub
git push -u origin main
```
