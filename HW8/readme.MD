Запуск утилиты: ./bayan [options].

Параметры:
  - --help - вывод help-сообщения
  - --algorithm - алгоритм хеширования: Dummy, SHA3, MD5
  - --targets - пути для сканирования, разделяются пробелом, могут быть указаны в двойных ковычках
  - --blackList - пути для исключия из сканирования, разделяются пробелом, могут быть указаны в двойных ковычках
  - --nonRecursive - использовать нерекурсивное сканирование
  - --size - минимальный размер файла для сканирования
  - --mask - список расширений имен файлов, для которых производить сканирование
  - --blockSize - размер блока, который ситается при расчете хэш-значения
  - --threads - максимальное количество используемых потоков при сканировании